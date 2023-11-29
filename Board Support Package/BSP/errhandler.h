
#ifndef __ERRORHANDLER_H
#define __ERRORHANDLER_H
#ifdef __cplusplus
	extern "C"{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

#include "st_errno.h"

#define Error_Handler()	

void ErrorHandler(int32_t errno);


#ifdef __cplusplus
	}
#endif
#endif /* Bish-y Clef */

/************************ (C) COPYRIGHT YiLDiZ IoT END OF FILE ************************/
