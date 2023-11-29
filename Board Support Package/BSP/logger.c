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
/*
 *      PROJECT:   
 *      $Revision: $
 *      LANGUAGE:  ANSI C
 */

/*! \file
 *
 *  \author 
 *
 *  \brief Debug log output utility implementation.
 *
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "logger.h"
#include "platform.h"

#ifdef PLATFORM_LOGGER_MODULE_ENABLED
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#if (USE_LOGGER == LOGGER_ON)
#define MAX_HEX_STR						   4
#define MAX_HEX_STR_LENGTH				 128
#define MAX_LINE_COUNT					  19
#define MAX_CHAR_COUNT					  47
#endif

#define USART_TIMEOUT					1000
#define LOG_BUFFER_SIZE					 256

/* Private macro -------------------------------------------------------------*/
#if (USE_LOGGER == LOGGER_ON)
uint32_t rowIndex;
uint32_t columnIndex;

char string[MAX_LINE_COUNT + 1][MAX_CHAR_COUNT + 1];

char hexStr[MAX_HEX_STR][MAX_HEX_STR_LENGTH];
uint8_t hexStrIdx = 0;
#elif (USE_LOGGER == LOGGER_OFF && !defined(HAL_UART_MODULE_ENABLED))
  #define UART_HandleTypeDef void
#endif


/* Private variables ---------------------------------------------------------*/
eLOGPort 				eLogPort = eLOGPort_LCD;
UART_HandleTypeDef		*pLogUsart;


/* Private function prototypes -----------------------------------------------*/
static void logITMTx(uint8_t *data, uint16_t dataLen);
static uint8_t logUsartTx(uint8_t *data, uint16_t dataLen);

/* Private user code ---------------------------------------------------------*/
static void logITMTx(uint8_t *data, uint16_t dataLen)
{
    while (dataLen != 0)
    {
        ITM_SendChar(*data);
		
        data++;
        dataLen--;
    }
}
//

/**
  * @brief  This function Transmit data via USART
	* @param	data : data to be transmitted
	* @param	dataLen : length of data to be transmitted
  * @retval ERR_INVALID_HANDLE : in case the SPI HW is not initalized yet
  * @retval others : HAL status
  */
static uint8_t logUsartTx(uint8_t *data, uint16_t dataLen)
{
	if(pLogUsart == 0)
		return ERR_INVALID_HANDLE;
  
	return HAL_UART_Transmit(pLogUsart, data, dataLen, USART_TIMEOUT);
}
//

static void shiftRow(void)
{
	for(uint32_t i = 0; i < MAX_LINE_COUNT - 1; i++)
	{
		memcpy(string[i], string[i +  1], MAX_CHAR_COUNT);
	}
	
	memset(string[MAX_LINE_COUNT - 1], 0, MAX_CHAR_COUNT);
}
//

static void increment_rowIndex(void)
{
	if(rowIndex < MAX_LINE_COUNT - 1)
	{
		rowIndex++;
	}
	else
	{
		shiftRow();
	}
	columnIndex = 0;
}
//

static void increment_columnIndex(void)
{
	columnIndex++;
	if(columnIndex >= MAX_CHAR_COUNT)
	{
		columnIndex = 0;
		increment_rowIndex();
	}
}
//

static void putch(char ch)
{
	if(ch != '\r' && ch != '\n')
	{
		/** copy character to the lcd string buffer */
		string[rowIndex][columnIndex] = ch;
		
		/** increment character index */
		increment_columnIndex();
	}
	else
	{
		increment_rowIndex();
	}
}
//

static void logLCD(char *ch, eLOGType logType)
{
	char *pCh = ch;
	while(*pCh !=  '\0')
	{
		putch(*pCh);
		pCh++;
	}
	
//	if(logType == eLOGType_Info)
//		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
//	else if(logType == eLOGType_Debug)
//		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
//	else if(logType == eLOGType_Error)
//		BSP_LCD_SetTextColor(LCD_COLOR_RED);
//	else if(logType == eLOGType_TraceEnd)
//		BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
//	else if(logType == eLOGType_TracePart)
//		BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
//	else if(logType == eLOGType_TraceStart)
//		BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
//	else
//		BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
	
	uint32_t counter = 0;
	while(counter < rowIndex)
	{
		BSP_LCD_ClearStringLine(counter);
		BSP_LCD_DisplayStringAtLine(counter, (uint8_t *)string[counter]);
		counter++;
	}
}
//

/* Public user code ----------------------------------------------------------*/
int platformLog(eLOGType logtype, const char *format, ...)
{
	int cnt = 0;
    va_list argptr;
	char timestamp[64];
    char buf[LOG_BUFFER_SIZE];
	
	/** get arguments */
    va_start(argptr, format);
	
	/** convert agrs to string */
    cnt = vsnprintf(buf, LOG_BUFFER_SIZE, format, argptr);
	
	/** end of args */
    va_end(argptr);  
    
	/** get timestamp */
	sprintf(timestamp, "[%08d] ", HAL_GetTick());
	
	/** which log type */
	switch(eLogPort)
	{
		case eLOGPort_UART:
		{
			logUsartTx((uint8_t*)buf, strlen((const char *)buf));
			break;
		}
		case eLOGPort_USB:
		{
			break;
		}
		case eLOGPort_LCD:
		{
//			logLCD(timestamp, eLOGType_Info);
			logLCD(buf, logtype);
			
//			if(logtype != eLOGType_TraceStart && logtype != eLOGType_TraceEnd)
//				logLCD("\n", logtype);
			break;
		}
		case eLOGPort_ITM:
		{
			/* print ITM port */
			logITMTx((uint8_t*)buf, strlen(buf));
			
			/* WA to avoid ITM overflow */
			HAL_Delay((cnt + 9) / 10);
			break;
		}
		default:
			break;
	}
	
    return cnt;
}
//

void _Error_Handler(char * file, int line)
{
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
}
//

/**
  * @brief  This function initalize the UART handle.
	* @param	husart : already initalized handle to USART HW
  * @retval none :
  */
void logInit(eLOGPort logtype, void *pStream)
{
    eLogPort = logtype;
	if(logtype == eLOGPort_UART )
		pLogUsart = (UART_HandleTypeDef *)pStream;
}
//

char* hex2Str(unsigned char * data, size_t dataLen)
{
  #if (USE_LOGGER == LOGGER_ON)
  {
    unsigned char * pin = data;
    const char * hex = "0123456789ABCDEF";
    char * pout = hexStr[hexStrIdx];
    uint8_t i = 0;
    uint8_t idx = hexStrIdx;
    if(dataLen == 0)
    {
      pout[0] = 0;     
    } 
    else     
    {
      for(; i < dataLen - 1; ++i)
      {
          *pout++ = hex[(*pin>>4)&0xF];
          *pout++ = hex[(*pin++)&0xF];
      }
      *pout++ = hex[(*pin>>4)&0xF];
      *pout++ = hex[(*pin)&0xF];
      *pout = 0;
    }    
    
    hexStrIdx++;
    hexStrIdx %= MAX_HEX_STR;
    
    return hexStr[idx];
  }
  #else
  {
    return NULL;
  }
  #endif /* #if USE_LOGGER == LOGGER_ON */
}
//
#endif
