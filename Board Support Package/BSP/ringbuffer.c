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

int32_t RingBuffer_Getc(xRingBufferStruct *prb);
int32_t RingBuffer_IsEmpty(xRingBufferStruct *prb);
int32_t RingBuffer_Available(xRingBufferStruct *prb);
int32_t RingBuffer_ResetBuffer(xRingBufferStruct *prb);
int32_t RingBuffer_GetFreeSpace(xRingBufferStruct *prb);
int32_t RingBuffer_Putc(xRingBufferStruct *prb, uint8_t ch);
int32_t RingBuffer_Gets(xRingBufferStruct *prb, uint8_t *pbuffer, uint32_t len);
int32_t RingBuffer_Puts(xRingBufferStruct *prb, uint8_t *pbuffer, uint32_t len);
int32_t RingBuffer_Init(xRingBufferStruct *pRB, uint8_t *pBuffer, uint32_t lenght);

xRingBuffer_TypeDef xRingBuffer = {
	.init 			= RingBuffer_Init,
	.getc 			= RingBuffer_Getc,
	.gets 			= RingBuffer_Gets,
	.putc 			= RingBuffer_Putc,
	.puts 			= RingBuffer_Puts,
	.isEmpty 		= RingBuffer_IsEmpty,
	.available 		= RingBuffer_Available,
	.resetBuffer 	= RingBuffer_ResetBuffer,
	.getFreeSpace 	= RingBuffer_GetFreeSpace
};

int32_t RingBuffer_Init(xRingBufferStruct *pRB, uint8_t *pBuffer, uint32_t lenght)
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

int32_t RingBuffer_GetFreeSpace(xRingBufferStruct *prb)
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

int32_t RingBuffer_Putc(xRingBufferStruct *prb, uint8_t ch)
{
	if(prb == NULL)
		return -1;
	
	if(RingBuffer_GetFreeSpace(prb) > 0)
	{
		prb->pBuffer[prb->head] = ch;
		prb->head++;
		if(prb->head > prb->length)
			prb->head = 0;
		
		return 0;
	}
	else
		return 2;
}
//

int32_t RingBuffer_Puts(xRingBufferStruct *prb, uint8_t *pbuffer, uint32_t len)
{
	if(prb == NULL)				return -1;
	else if(pbuffer == NULL) 	return -1;
	else if(len == 0)			return 0;
	
	if(prb->head >= prb->length)
		prb->head = 0;
	
	uint32_t freeSpace = RingBuffer_GetFreeSpace(prb);
	if(freeSpace < len)
	{
		if(freeSpace == 0)
			return 0;
		
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
	
	return count;
}
//

int32_t RingBuffer_Getc(xRingBufferStruct *prb)
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
	return ch;
}
//

int32_t RingBuffer_Gets(xRingBufferStruct *prb, uint8_t *pbuffer, uint32_t len)
{
	if(prb == NULL)				return -1;
	else if(len == 0)			return 0;
	else if(pbuffer == NULL) 	return -1;
	
	if(prb->tail >= prb->length)
		prb->tail = 0;
	
	uint32_t freeSpace = RingBuffer_GetFreeSpace(prb);
	if(freeSpace < len)
	{
		if(freeSpace == 0)
			return 0;
		
		len = freeSpace;
	}
	
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
	
	return count;
}
//

int32_t RingBuffer_ResetBuffer(xRingBufferStruct *prb)
{
	if(prb == NULL)
		return -1;
	
	prb->tail = prb->head;
	
	return 0;
}
//

int32_t RingBuffer_IsEmpty(xRingBufferStruct *prb)
{
	if(prb == NULL)
		return -1;
	
	if(prb->head == prb->tail)
		return 1;
	else
		return 0;
}
//

int32_t RingBuffer_Available(xRingBufferStruct *prb)
{
	if(prb == NULL)
		return -1;
	
	if(prb->head != prb->tail)
		return 1;
	else
		return 0;
}
//
