/**
  ******************************************************************************
  * @file    platform.h
  * @author  YiLDiZ IoT Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the platform.c driver.
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
#ifndef __RINGBUFFER_H
#define __RINGBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Public typedef ------------------------------------------------------------*/
typedef struct
{
	uint8_t *pBuffer;
	
	uint32_t head;
	uint32_t tail;
	uint32_t length;
} xRingBufferStruct;

typedef int32_t (*pfRBGetc)			(xRingBufferStruct *prb);
typedef int32_t (*pfRBIsEmpty)		(xRingBufferStruct *prb);
typedef int32_t (*pfRBAvailable)	(xRingBufferStruct *prb);
typedef int32_t (*pfRBResetBuffer)	(xRingBufferStruct *prb);
typedef int32_t (*pfRBGetFreeSpace)	(xRingBufferStruct *prb);
typedef int32_t (*pfRBPutc)			(xRingBufferStruct *prb, uint8_t ch);
typedef int32_t (*pfRBPuts)			(xRingBufferStruct *prb, uint8_t *pbuffer, uint32_t len);
typedef int32_t (*pfRBGets)			(xRingBufferStruct *prb, uint8_t *pbuffer, uint32_t len);
typedef int32_t (*pfRBInit)			(xRingBufferStruct *pRB, uint8_t *pBuffer, uint32_t len);
typedef struct
{
	pfRBInit 			init;
	pfRBGetc 			getc;
	pfRBGets 			gets;
	pfRBPutc 			putc;
	pfRBPuts 			puts;
	pfRBIsEmpty			isEmpty;
	pfRBAvailable		available;
	pfRBResetBuffer		resetBuffer;
	pfRBGetFreeSpace 	getFreeSpace;
}xRingBuffer_TypeDef;

/* Public define -------------------------------------------------------------*/
/* Public macro --------------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
extern xRingBuffer_TypeDef xRingBuffer;
/* Public function prototypes ------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

/************************ (C) COPYRIGHT YiLDiZ IoT *****************************/
