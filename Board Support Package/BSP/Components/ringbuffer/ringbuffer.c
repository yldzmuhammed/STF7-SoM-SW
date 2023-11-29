/**
  ******************************************************************************
  * @file    quectel_m66.v
  * @author  YiLDiZ IoT Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the quectel_m66.c driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 YiLDiZ IoT.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by YiLDiZ IoT under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ringbuffer.h"

static int32_t RingBuffer_Getc(xRingBufferStruct *prb);
static int32_t RingBuffer_GetUsed(xRingBufferStruct *prb);
static int32_t RingBuffer_Available(xRingBufferStruct *prb);
static int32_t RingBuffer_ResetBuffer(xRingBufferStruct *prb);
static int32_t RingBuffer_GetFreeSpace(xRingBufferStruct *prb);
static int32_t RingBuffer_Putc(xRingBufferStruct *prb, uint8_t ch);
static int32_t RingBuffer_Gets(xRingBufferStruct *prb, uint8_t *pbuffer, uint32_t len);
static int32_t RingBuffer_Puts(xRingBufferStruct *prb, uint8_t *pbuffer, uint32_t len);
static int32_t RingBuffer_Init(xRingBufferStruct *pRB, uint8_t *pBuffer, uint32_t lenght);

xRingBuffer_TypeDef xRingBuffer = {
	.init 				= RingBuffer_Init,
	.getc 				= RingBuffer_Getc,
	.gets 				= RingBuffer_Gets,
	.putc 				= RingBuffer_Putc,
	.puts 				= RingBuffer_Puts,
	.Used					= RingBuffer_GetUsed,
	.available 		= RingBuffer_Available,
	.resetBuffer 	= RingBuffer_ResetBuffer,
	.getFreeSpace	= RingBuffer_GetFreeSpace
};
//

/**
  * @brief  Initialize ring buffer struct.
  * @param  pRB 		Ring buffer struct pointer.
	* @param	pBuffer	Ring buffer's data buffer.
	* @param	length	Ring buffer capacity.
  * @retval -1 	if pointers null.
	*					0		if successfull.
  */
static int32_t RingBuffer_Init(xRingBufferStruct *pRB, uint8_t *pBuffer, uint32_t lenght)
{
	if(pRB == NULL)		return -1;
	if(pBuffer == NULL) return -1;
	
	/** reset buffer position */
	pRB->head = 0;
	pRB->tail = 0;
	
	pRB->length = lenght;
	pRB->pBuffer = pBuffer;
	
	return 0;
}
//

/**
  * @brief  Retuns with ring buffer's free space.
  * @param  pRB 		Ring buffer struct pointer.
  * @retval -1 	if pointers null.
	*					>=0 free space.
  */
static int32_t RingBuffer_GetFreeSpace(xRingBufferStruct *prb)
{
	int32_t freeSpace = 0;
	
	if(prb == NULL)
		return -1;
	else if(prb->tail == prb->head)
		freeSpace = prb->length;
	else if(prb->tail > prb->head)
		freeSpace = prb->tail - prb->head;
	else if(prb->head > prb->tail)
		freeSpace = prb->length - (prb->head - prb->tail);
	
	return freeSpace;
}
//

/**
  * @brief  Returns with ring buffer's used space.
  * @param  pRB 		Ring buffer struct pointer.
  * @retval -1 	if pointers null.
	*					>=0 used space.
  */
static int32_t RingBuffer_GetUsed(xRingBufferStruct *prb)
{
	if(prb == NULL)
		return -1;
	
	return prb->used;
}
//

/**
  * @brief  Put one byte to the ring buffer.
  * @param  pRB 		Ring buffer struct pointer.
	* @param	ch			one character.
  * @retval -1 	if pointers null.
	*					0 	if successfull.
	*					1		if there is no free space.
  */
static int32_t RingBuffer_Putc(xRingBufferStruct *prb, uint8_t ch)
{
	if(prb == NULL)
		return -1;
	
	if(RingBuffer_GetFreeSpace(prb) > 0)
	{
		prb->pBuffer[prb->head] = ch;
		prb->head++;
		if(prb->head >= prb->length)
			prb->head = 0;
		
		prb->used = prb->length - RingBuffer_GetFreeSpace(prb);
		
		return 0;
	}
	else
		return -2;
}
//

/**
  * @brief  Put multiple bytes to the ring buffer.
  * @param  pRB 		Ring buffer struct pointer.
	* @param	pbuffer	Source buffer pointer.
	* @param	len			Source lenght.
  * @retval -1 	if pointers null.
	*					> 0 Number of bytes putted in to the buffer.
	*					-2	if there is no free space.
  */
static int32_t RingBuffer_Puts(xRingBufferStruct *prb, uint8_t *pbuffer, uint32_t len)
{
	if(prb == NULL)
		return -1;
	else if(pbuffer == NULL)
		return -1;
	else if(len == 0)
		return 0;
	
	if(prb->head >= prb->length)
		prb->head = 0;
	
	uint32_t freeSpace = RingBuffer_GetFreeSpace(prb);
	if(freeSpace == 0)
		return -2;
	
	if(freeSpace < len)
	{		
		len = freeSpace;
	}
	
	uint32_t count = 0;
	while(len--)
	{
		prb->pBuffer[prb->head] = *pbuffer;
		
		prb->head++;
		pbuffer++;
		count++;
		
		if(prb->head >= prb->length)
			prb->head = 0;
	}
	
	prb->used = prb->length - RingBuffer_GetFreeSpace(prb);
		
	return count;
}
//

/**
  * @brief  Returns with last byte in the buffer.
  * @param  pRB 		Ring buffer struct pointer.
  * @retval -1 	if pointers null.
	*					-2	if there is no data.
	*					>=0 last byte value in the buffer.
  */
static int32_t RingBuffer_Getc(xRingBufferStruct *prb)
{
	if(prb == NULL)
		return -1;
	
	uint8_t ch;
	if(RingBuffer_Available(prb) > 0)
	{
		ch = prb->pBuffer[prb->tail];
		prb->tail++;
		if(prb->tail > prb->length)
			prb->tail = 0;
	}
	else
	{
		return -2;
	}
	
	prb->used = prb->length - RingBuffer_GetFreeSpace(prb);
		
	return ch;
}
//

/**
  * @brief  Gets multiple bytes from ring buffer.
  * @param  pRB 		Ring buffer struct pointer.
	* @param	pbuffer	Destination buffer pointer.
	* @param	len			Destination lenght.
  * @retval -1 	if pointers null.
	*					>=0 number of copied data.
  */
static int32_t RingBuffer_Gets(xRingBufferStruct *prb, uint8_t *pbuffer, uint32_t len)
{
	if(prb == NULL)
		return -1;
	else if(len == 0)
		return 0;
	else if(pbuffer == NULL)
		return -1;
	
	if(prb->tail >= prb->length)
		prb->tail = 0;
	
	uint32_t count = 0;
	while((len > 0) && (RingBuffer_Available(prb) > 0))
	{
		*pbuffer = prb->pBuffer[prb->tail];
		prb->tail++;
		pbuffer++;
		count++;
		
		if(prb->tail >= prb->length)
			prb->tail = 0;
		
		len--;
	}
	
	prb->used = prb->length - RingBuffer_GetFreeSpace(prb);
	
	return count;
}
//

/**
  * @brief  Resets buffer's head and tail.
  * @param  pRB 		Ring buffer struct pointer.
  * @retval -1 	if pointers null.
	*					0		if successfull.
  */
static int32_t RingBuffer_ResetBuffer(xRingBufferStruct *prb)
{
	if(prb == NULL)
		return -1;
	
	prb->tail = prb->head = 0;
	
	prb->used = prb->length - RingBuffer_GetFreeSpace(prb);
		
	return 0;
}
//

/**
  * @brief  Checks for available data in the buffer.
  * @param  pRB 		Ring buffer struct pointer.
  * @retval -1 	if pointers null.
	*					0		if buffer is empty.
	*					1		if buffer is not empty.
  */
static int32_t RingBuffer_Available(xRingBufferStruct *prb)
{
	if(prb == NULL)
		return -1;
	
	if(prb->head != prb->tail)
		return 1;
	else
		return 0;
}
//
