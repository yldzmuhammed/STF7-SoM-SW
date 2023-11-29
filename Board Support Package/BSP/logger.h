/******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under ST MYLIBERTY SOFTWARE LICENSE AGREEMENT (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/myliberty
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
  * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
******************************************************************************/
/*! \file
 *
 *  \author 
 *
 *  \brief Serial output log declaration file
 *
 */

/*!
 *
 * This driver provides a printf-like way to output log messages
 * via the UART interface. It makes use of the uart driver.
 *
 */

#ifndef LOGGER_H
#define LOGGER_H

/* Includes ------------------------------------------------------------------*/
#include <stddef.h>
#include "st_errno.h"
#include "stm32f7xx_hal.h"

/* Public typedef ------------------------------------------------------------*/
typedef enum
{
	eLOGPort_UART,
	eLOGPort_USB,
	eLOGPort_LCD,
	eLOGPort_ITM
} eLOGPort;
//

typedef enum
{
	eLOGType_Info,
	eLOGType_Debug,
	eLOGType_Error,
	eLOGType_TraceEnd,
	eLOGType_TracePart,
	eLOGType_TraceStart
	
}eLOGType;
//

/* Public define -------------------------------------------------------------*/
#define LOGGER_ON   									1
#define LOGGER_OFF  									0

#define USE_LOGGER 										LOGGER_ON

/* Public macro --------------------------------------------------------------*/
#if (USE_LOGGER == LOGGER_OFF && !defined(HAL_UART_MODULE_ENABLED))
  #define UART_HandleTypeDef 											void
#endif

/* Public variables ----------------------------------------------------------*/
/* Public function prototypes ------------------------------------------------*/
void logInit(eLOGPort logtype, void *pStream);
int platformLog(eLOGType logtype, const char *format, ...);

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char *, int);

/*!
 *****************************************************************************
 *  \brief  helper to convert hex data into formated string
 *
 *  \param[in] data : pointer to buffer to be dumped.
 *
 *  \param[in] dataLen : buffer length
 *
 *  \return hex formated string
 *
 *****************************************************************************
 */
char* hex2Str(unsigned char * data, size_t dataLen);

#endif /* LOGGER_H */

/************************ (C) COPYRIGHT Y?LD?Z IoT *****************************/

