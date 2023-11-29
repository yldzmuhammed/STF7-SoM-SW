/**
  ******************************************************************************
  * @file    quectel_m66.v
  * @author  YıLDıZ IoT Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the quectel_m66.c driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 YıLDıZ IoT.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by YıLDıZ IoT under BSD 3-Clause license,
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
#include "bishydex.h"
#include "ringbuffer.h"
#include "quectel_m66.h"

/* Private typedef -----------------------------------------------------------*/
#define BUFFER_LENGTH											1024
#define DOWNLOADFILE_SIZE									1025

#define IPSTACK_STATE_INITIAL							"IP INITIAL"
#define IPSTACK_STATE_START								"IP START"
#define IPSTACK_STATE_CONFIG							"IP CONFIG"
#define IPSTACK_STATE_IND									"IP IND"
#define IPSTACK_STATE_GPRSACT							"IP GPRSACT"
#define IPSTACK_STATE_STATUS							"IP STATUS"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile bool 				httpsredirect;

static char 				_buffer[BUFFER_LENGTH];
static char 				_buffer2[256];
static char 				_command[32];
static char 				_readBuffer[255];
static char 				_firmwareVersion[20];

static int8_t 				_lastError;

static uint8_t				uartChar;
static uint8_t				UARTBuffer[BUFFER_LENGTH];

static uint32_t 			sslLength;



static const char 			*_INET_PREFIX = "I";
static const char 			*_AT 					= "AT";
static const char 			*_OK 					= "OK";
static const char				*_DEACTOK			= "DEACT OK";
//static const char 			*_useragent		= "PP";
static const char 			*_SSL_PREFIX 	= "SSL";
//static const char 			*_ERROR			= "ERROR";
static const char 			*_CONNECT 		= "CONNECT";
static const char 			*_CME_ERROR		= "CME ERROR: ";

static eTlsEncryption		_encryption;

static UART_HandleTypeDef 	huart1;
static xRingBufferStruct	xRBStruct;

static watchdogcallbacksignature watchdogcallback;
/* Private function prototypes -----------------------------------------------*/
static void callWatchdog(void);

static bool activateSsl(void);
static bool checkResult(void);
static bool useEncryption(void);

static bool readReply(uint16_t timeout, uint8_t lines);
static bool sendAndWaitFor(const char *command, const char *reply, uint16_t timeout);
static bool sendAndWaitForReply(const char *command, uint16_t timeout, uint8_t lines);
static bool sendAndCheckReply(const char *command, const char *reply, uint16_t timeout);
//static bool sendAndWaitForMultilineReply(const char *command, uint8_t lines, uint16_t timeout);

/* Private user code ---------------------------------------------------------*/

static void callWatchdog(void)
{
  if(watchdogcallback != NULL)
  {
    (watchdogcallback)();
  }
}
//

/**
  * @brief  Activate SSL
  * @retval true Succesfull actiaveted, false Activation failed.
  */
static bool activateSsl(void)
{
  if(!useEncryption())
  {
    _encryption = eTE_Tls12; //Set to Tls12 if no other encryption is specified
  }

  sprintf(_command, "AT+QSSLCFG=\"sslversion\",1,%i", (uint8_t)_encryption);
  if(!sendAndCheckReply(_command, _OK, 10000))     // Set TLS
  {
    QT_ERROR("Failed to set TLS version");
    return false;
  }
  if(!sendAndCheckReply("AT+QSSLCFG=\"ciphersuite\",1,\"0xFFFF\"", _OK, 10000))   // Allow all
  {
    QT_ERROR("Failed to set cipher suites");
    return false;
  }
  if(!sendAndCheckReply("AT+QSSLCFG=\"seclevel\",1,0", _OK, 10000))
  {
    QT_ERROR("Failed to set security level");
    return false;
  }
  return true;
}
//

/**
  * @brief  Check result
  * @retval true Succesfull actiaveted, false Activation failed.
  */
static bool checkResult(void)
{
  HAL_UART_Receive_IT(&huart1, &uartChar, 1);
  // CheckResult returns one of these:
  // true    OK
  // false   Unknown result
  // false  CME Error
  //
  char *token = strstr(_buffer, _OK);
  if(token)
  {
    //QT_TRACE("*OK - %s", _buffer);
    _lastError = 0;
    return true;
  }
  token = strstr(_buffer, _CME_ERROR);
  if(!token)
  {
    //QT_TRACE("*NO CME ERROR: %s", _buffer);
    _lastError = -1;
    return false;
  }
  //QT_TRACE("*CME ERROR: %s", _buffer);
  token = strtok(_buffer, _CME_ERROR);
  _lastError = atoi(token);
  return false;
}
//

/**
  * @brief  Is SSL active
  * @retval true Active, false Inactive
  */
static bool useEncryption(void)
{
  return _encryption != eTE_None;
}
//

/**
  * @brief  Read GSM Module replay
  * @param	timeout Wait timeout
  * @param	lines Waited line number
  * @retval true Succesfull, false Failed
  */
static bool readReply(uint16_t timeout, uint8_t lines)
{
  HAL_UART_Receive_IT(&huart1, &uartChar, 1);
	
  uint16_t index = 0;
  uint16_t linesFound = 0;

  while(timeout--)
  {
    if(index > 254)
    {
      break;
    }

    while(xRingBuffer.available(&xRBStruct) > 0)
    {
			/* start atomic operation */
//			portENTER_CRITICAL();
			
      char c = xRingBuffer.getc(&xRBStruct);
      if(c == '\r')
      {
        continue;
      }

      if(c == '\n' && index == 0)
      {
        continue;    // Ignore first \n.
      }

      _buffer[index++] = c;
      if(c == '\n')
      {
        linesFound++;
      }
			
			/* end of atomic operation */
//			portEXIT_CRITICAL();
			
      if(linesFound >= lines)
      {
        break;
      }
    }

    if(linesFound >= lines)
    {
      break;
    }

    if(timeout == 0)
    {
      _buffer[index] = 0;
      QT_COM_TRACE(" <- (Timeout) %s", _buffer);
      return false;
    }
		
    osDelay(17);
		if(timeout > 16)
			timeout -= 17;
		else
			timeout = 0;
  }
  _buffer[index] = 0;
  QT_COM_TRACE(" <-  %s", _buffer);

  return true;
}
//

static bool sendBufferWaitOK(uint8_t *command, uint32_t length, const char *reply, uint16_t timeout)
{
  uint16_t index = 0;

  QT_COM_TRACE(" -> %s", command);

  xRingBuffer.resetBuffer(&xRBStruct);

  HAL_UART_Transmit(&huart1, (uint8_t *)command, length, 1000);
  HAL_UART_Receive_IT(&huart1, &uartChar, 1);

  while(timeout--)
  {
    if(index > 254)
      break;
    
    while(xRingBuffer.available(&xRBStruct) > 0)
    {
			/* start atomic operation */
//			portENTER_CRITICAL();
			
      char c = xRingBuffer.getc(&xRBStruct);
      if(c == '\r')
        continue;
			
			// Ignore first "\n"
      if(c == '\n' && index == 0)
        continue;    

      _buffer[index++] = c;
			
			/* end of atomic operation */
//			portEXIT_CRITICAL();
    }

    if(strstr(_buffer, reply))
    {
      QT_COM_TRACE("Match found");
      break;
    }

    if(timeout == 0)
    {
      _buffer[index] = '\0';
      QT_COM_TRACE(" <- (Timeout) %s", _buffer);
      return false;
    }
		
    osDelay(17);
		if(timeout > 16)
			timeout -= 17;
		else
			timeout = 0;
  }
  _buffer[index] = 0;
  QT_COM_TRACE(" <-  %s", _buffer);

  return true;
}
//

static bool sendAndWaitFor(const char *command, const char *reply, uint16_t timeout)
{
	uint16_t index = 0;
  QT_COM_TRACE(" -> %s", command);
	
  xRingBuffer.resetBuffer(&xRBStruct);

	osDelay(10);
	
  HAL_UART_Transmit(&huart1, (uint8_t *)command, strlen(command), timeout);
  HAL_UART_Transmit(&huart1, (uint8_t *)"\r\n", sizeof("\r\n"), timeout);
	memset(_buffer2, 0, sizeof(_buffer2));
  HAL_UART_Receive_IT(&huart1, &uartChar, 1);

  while(timeout--)
  {
    if(index > 254)
    {
      break;
    }

    while(xRingBuffer.available(&xRBStruct) > 0)
    {
			/* start atomic operation */
//			portENTER_CRITICAL();
			
      char c = xRingBuffer.getc(&xRBStruct);
      if(c == '\r')
        continue;
			
			// Ignore first "\n"
      if(c == '\n' && index == 0)
        continue;

      _buffer[index++] = c;
			
			/* end of atomic operation */
//			portEXIT_CRITICAL();
    }

    if(strstr(_buffer, reply))
    {
      QT_COM_TRACE("Match found");
      break;
    }

    if(timeout == 0)
    {
      _buffer[index] = '\0';
      QT_COM_TRACE(" <- (Timeout) %s", _buffer);
      return false;
    }
		
    osDelay(17);
		if(timeout > 16)
			timeout -= 17;
		else
			timeout = 0;
  }
  _buffer[index] = 0;
  QT_COM_TRACE(" <-  %s", _buffer);

  return true;
}
//

static bool sendAndWaitFor2(const char *command, const char *reply, uint16_t timeout)
{
	uint16_t index2 = 0;
  QT_COM_TRACE(" -> %s", command);
	
  xRingBuffer.resetBuffer(&xRBStruct);
	memset(_buffer2, 0, sizeof(_buffer2));

	osDelay(10);
	
  HAL_UART_Transmit(&huart1, (uint8_t *)command, strlen(command), timeout);
  HAL_UART_Transmit(&huart1, (uint8_t *)"\r\n", sizeof("\r\n"), timeout);
  HAL_UART_Receive_IT(&huart1, &uartChar, 1);

  while(timeout--)
  {
    if(index2 > 254)
      break;

    while(xRingBuffer.available(&xRBStruct) > 0)
    {
			/* start atomic operation */
//			portENTER_CRITICAL();
			
      char c = xRingBuffer.getc(&xRBStruct);
      if(c == '\r')
      {
        continue;
      }

      if(c == '\n' && index2 == 0)
      {
        continue;    // Ignore first "\n"
      }

      _buffer2[index2++] = c;
			
			/* end of atomic operation */
//			portEXIT_CRITICAL();
    }

    if(strstr(_buffer2, reply))
    {
      QT_COM_TRACE("Match found");
      break;
    }

    if(timeout == 0)
    {
      _buffer2[index2] = '\0';
      QT_COM_TRACE(" <- (Timeout) %s", _buffer2);
      return false;
    }
		
    osDelay(17);
		if(timeout > 16)
			timeout -= 17;
		else
			timeout = 0;
  }
	
  _buffer2[index2] = 0;
  QT_COM_TRACE(" <-  %s", _buffer2);

  return true;
}
//

static bool sendAndWaitForReply(const char *command, uint16_t timeout, uint8_t lines)
{
  xRingBuffer.resetBuffer(&xRBStruct);
  QT_COM_TRACE(" -> %s", command);

  HAL_UART_Receive_IT(&huart1, &uartChar, 1);
	
  HAL_UART_Transmit(&huart1, (uint8_t *)command, strlen(command), 1000);
  HAL_UART_Transmit(&huart1, (uint8_t *)"\r\n", sizeof("\r\n"), 1000);

  return readReply(timeout, lines);
}
//

static bool sendAndCheckReply(const char *command, const char *reply, uint16_t timeout)
{
  HAL_UART_Receive_IT(&huart1, &uartChar, timeout);
	
  sendAndWaitForReply(command, timeout, 1);

  /** check for searching string */
  if(strstr(_buffer, reply) == NULL)
    return false;
  else
    return true;
}
//

#if 0
static bool sendAndWaitForMultilineReply(const char *command, uint8_t lines, uint16_t timeout)
{
  HAL_UART_Receive_IT(&huart1, &uartChar, 1);
	
  return sendAndWaitForReply(command, timeout, lines);
}
#endif
//

/* Callback function ---------------------------------------------------------*/
/**
  * @brief  This function handles USART 1 interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart1);
	
  HAL_UART_Receive_IT(&huart1, &uartChar, 1);
}
//

/**
  * @brief  Rx Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  xRingBuffer.putc(&xRBStruct, uartChar);
}
//

/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
__weak void UART_MspInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  __HAL_RCC_USART1_CLK_ENABLE();

  __HAL_RCC_GPIOA_CLK_ENABLE();

  /**USART1 GPIO Configuration
  PA10     ------> USART1_RX
  PA9     ------> USART1_TX
  */
  GPIO_InitStruct.Pull 		= GPIO_NOPULL;
  GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;
  GPIO_InitStruct.Alternate 	= GPIO_AF7_USART1;
  GPIO_InitStruct.Pin 		= GPIO_PIN_9 | GPIO_PIN_10;
  GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  huart1.Instance 										= USART1;
  huart1.Init.BaudRate 								= 115200;
  huart1.Init.Mode 										= UART_MODE_TX_RX;
  huart1.Init.StopBits 								= UART_STOPBITS_1;
  huart1.Init.Parity 									= UART_PARITY_NONE;
  huart1.Init.WordLength 							= UART_WORDLENGTH_8B;
  huart1.Init.HwFlowCtl 							= UART_HWCONTROL_NONE;
  huart1.Init.OverSampling 						= UART_OVERSAMPLING_16;
  huart1.AdvancedInit.AdvFeatureInit 	= UART_ADVFEATURE_NO_INIT;
  huart1.Init.OneBitSampling 					= UART_ONE_BIT_SAMPLE_DISABLE;

  HAL_UART_Receive_IT(&huart1, &uartChar, 1);
	
//  huart1.RxISR												= &HAL_UART_RxCpltCallback;
  if(HAL_UART_Init(&huart1) != HAL_OK)
  {
//		Error_Handler();
  }
	
  /* USART1 interrupt Init */
  HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
}
//

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
__weak void UART_MspDeInit(void)
{
  /* Peripheral clock disable */
  __HAL_RCC_USART1_CLK_DISABLE();

  /**USART1 GPIO Configuration
  PA10     ------> USART1_RX
  PA9     ------> USART1_TX
  */
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

  HAL_UART_DeInit(&huart1);

  /* USART1 interrupt DeInit */
  HAL_NVIC_DisableIRQ(USART1_IRQn);
}
//

__weak void BSP_GSMPowerPins_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /*Configure GPIO pins : GSM_RST_Pin VBAT_MEASCTRL_Pin GSM_PWREN_Pin GSM_PWRG_Pin */
  GPIO_InitStruct.Pin = GSM_RST_Pin|GSM_PWREN_Pin|GSM_PWRG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
//




/* Public user code ----------------------------------------------------------*/
void GSM_DeInitialize(void)
{
	GSM_PowerOn(false);
	
	UART_MspDeInit();
}
//

int8_t GSM_getLastError(void)
{
  return _lastError;
}
//


eGSMErrorCode GSM_Initialize(void)
{
  UART_MspInit();
  BSP_GSMPowerPins_Init();

  /** init ring buffer */
  xRingBuffer.init(&xRBStruct, UARTBuffer, BUFFER_LENGTH);

  QT_DEBUG("Powering off module");
  GSM_PowerOn(false);

  QT_DEBUG("Powering on module");
  if(GSM_PowerOn(true) == false)
  {
    return eGSMErrCode_NoModule;
  }

	sendAndCheckReply("AT+IPR=115200", _OK, 1000);
	sendAndCheckReply("AT&W", _OK, 1000);
	
  uint32_t timeout = 5000;
  QT_DEBUG("Waiting for module initialization");
	while(timeout)
	{
    if(readReply(500, 2) && strstr(_buffer, "Call Ready"))
    {
      QT_DEBUG("Call Ready.");
			break;
    }
		
    osDelay(500);

    timeout -= 500;
	}
	
	timeout = 5000;
	while(timeout)
	{
    if(readReply(500, 1) && strstr(_buffer, "SMS Ready"))
    {
      QT_DEBUG("SMS Ready.");
			break;
    }
		
    osDelay(500);

    timeout -= 500;
	}

  if(timeout <= 0)
    QT_DEBUG("Failed waiting for phonebook initialization");    // Non critical error

  // Disable echo
  sendAndCheckReply("ATE0", _OK, 1000);

  // Set verbose error messages
  sendAndCheckReply("AT+CMEE=2", _OK, 1000);

  QT_DEBUG("Checking SIM card");
  if(!GSM_getSimPresent())
  {
    QT_ERROR("No SIM card detected");
    return eGSMErrCode_NoSIMCard;
  }

  // Wait for network registration
  timeout = 60000;
  eNwkRegState state;
  QT_DEBUG("Waiting for network registration");
  while(timeout > 0)
  {
    state = GSM_getGSMRegistration();
    switch(state)
    {
      case eNRS_NotRegistered:
        QT_DEBUG("Not registered");
        break;
      case eNRS_Registered:
        QT_DEBUG("Registered");
        break;
      case eNRS_Searching:
        QT_DEBUG("Searching");
        break;
      case eNRS_Denied:
        QT_DEBUG("Denied");
        break;
      case eNRS_Unknown:
        QT_DEBUG("Unknown");
        break;
      case eNRS_Roaming:
        QT_DEBUG("Roaming");
        break;
    }
    if(state == eNRS_Registered || state == eNRS_Roaming)
    {
      break;
    }
		
    osDelay(500);

    timeout -= 500;
  }

  if(timeout <= 0)
  {
    QT_ERROR("Network registration failed");
    return eGSMErrCode_NotRegGSM;
  }

#if 0
	if(sendAndWaitForReply("ATI", 1000, 5))
	{
			/* response is:
			 * Quectel
			 * UG96
			 * Revision: UG96LNAR02A06E1G
			 *
			 * OK */
			const char linefeed[] = "\n";
			char *token = strtok(_buffer, linefeed);
			if(token == NULL || strcmp(token, "Quectel_Ltd") != 0)
			{
					QT_ERROR("Not a Quectel module");
					return eGSMErrCode_NotMatchModel;
			}

			token = strtok(NULL, linefeed);

			if(token == NULL)
			{
					QT_ERROR("Parse error");
					return eGSMErrCode_UNKNOWNERROR;
			}

			token = strtok(NULL, linefeed);
			if(strlen(token) > 10)
			{
					strcpy(_firmwareVersion, token + 10);
			}
	}
#endif
  return eGSMErrCode_NoError;
}
//





/* Module Power Functions ----------------------------------------------------*/
bool GSM_getStatus(void)
{
  return (IS_GSM_PWR_ON() == GPIO_PIN_SET);
}
//

float GSM_getVoltage(void)
{
  if(sendAndWaitForReply("AT+CBC", 1000, 3))
  {
    const char delimiter[] = ",";
    char *token = strtok(_buffer, delimiter);
    if(token)
    {
      token = strtok(NULL, delimiter);
      token = strtok(NULL, delimiter);
      if(token)
      {
        char *ptr;
        uint16_t milliVolts = strtol(token, &ptr, 10);
        return milliVolts / 1000.0f;
      }
    }
  }
  return 0.0f;
}
//

bool GSM_PowerOn(bool state)
{
  QT_DEBUG("setPower: %i", state);
  if(state == true)
  {
    /** turn gsm regulator on */
    GSM_PWRKEY_HIGH();
    GSM_PWR_ON();

    /** let regulator settle */
    osDelay(2000);

    GSM_PWRKEY_LOW();

    osDelay(2000);

    GSM_PWRKEY_HIGH();

    osDelay(1000);

    QT_DEBUG("Open communications");
    int32_t timeout = 5000;
    while(timeout > 0)
    {
      xRingBuffer.resetBuffer(&xRBStruct);
      if(sendAndCheckReply(_AT, _OK, 500))
      {
        QT_COM_TRACE("GOT AT");
        break;
      }

      callWatchdog();

      osDelay(500);

      timeout -= 500;
    }

    if(timeout < 0)
    {
      QT_ERROR("Failed to initialize cellular module");
      return false;
    }
  }
  else
  {
    QT_DEBUG("Powering down module");

    /** turn gsm regulator off */
    GSM_PWRKEY_LOW();

    osDelay(1000);
		
    GSM_PWR_OFF();
		
    osDelay(1000);
    return true;
  }
  return true;
}
//




/* SIM Card Security Functions -----------------------------------------------*/
bool GSM_getSimPresent(void)
{
  // Reply is:
  // +QSIMSTAT: 0,1
  //
  // OK
  if(sendAndWaitFor("AT+CPIN?", "+CPIN: READY", 1000))
  {
		return true;
//    const char delimiter[] = ",";
//    char *token = strtok(_buffer, delimiter);
//    token = strtok(NULL, delimiter);
//    if(token)
//    {
//      return token[0] == 0x31;
//    }
  }
	else
		return false;
}
//




/* SIM Related Functions -----------------------------------------------------*/
uint8_t GSM_getIMEI(char *buffer)
{
  if(sendAndWaitForReply("AT+GSN", 1000, 1))
  {
    strncpy(buffer, _buffer, 15);
    buffer[15] = 0;
    return strlen(buffer);
  }
  return 0;
}
//

uint8_t GSM_getSIMCCID(char *buffer)
{
  // +QCCID: 898600220909A0206023
  //
  // OK
  if(sendAndWaitForReply("AT+CCID", 1000, 1))
	{
		if(strstr(_buffer, "+CCID: \""))
		{
			strncpy(buffer, &_buffer[8], 20);
			buffer[20] = 0;
			return strlen(buffer);
		}
	}
  return 0;
}
//

const char *GSM_getFirmwareVersion(void)
{
  return _firmwareVersion;
}
//




/* RTC Related Functions -----------------------------------------------------*/
bool GSM_getLocalTime(uint32_t *year, uint32_t *mount, uint32_t *day, uint32_t *hour, uint32_t *minute, uint32_t *second, int32_t *timeZone)
{
  /* check for wrong buffer access */
  if(year == NULL 		|| mount == NULL 	|| day == NULL 		||	\
      hour == NULL 		|| minute == NULL || second == NULL ||	\
      timeZone == NULL)
  {
    return false;
  }

	if(!sendAndWaitFor("AT+CTZU=3", _OK, 300))
	{
		QT_DEBUG("Sync time failed");
		return false;
	}
	
  if(!sendAndWaitFor("AT+CCLK?", _OK, 300))
  {
		QT_DEBUG("Getting time failed");
		return false;
  }
	
	int len = sscanf(_buffer, "+CCLK: \"%u/%u/%u,%u:%u:%u%d\"\nOK\n", year, mount, day, 
																																		hour, minute, second, timeZone);
  return (len != 0);
}
//




/* Network Querying And Setting Functions ------------------------------------*/
int8_t GSM_getRSSI(void)
{
  // Reply is:
  // +CSQ: 14,2
  //
  // OK
  if(sendAndWaitForReply("AT+CSQ", 1000, 3))
  {
    char *token = strtok(_buffer, " ");
    if(token)
    {
      token = strtok(NULL, ",");
      if(token)
      {
        char *ptr;
        int8_t rssi = strtol(token, &ptr, 10);
        if(rssi > -1 && rssi < 32)					/** acceptable range -113dB / -51dB */
        {
          rssi = (rssi * 2) - 113;
        }
        else if(rssi >= 31 && rssi <= 99)			/** perfect range rssi < -51dB */
        {
          rssi = -51;
        }
        else if(rssi == 99)									/** not detectable. too bad signal quality */
        {
          rssi = 0xFF;
        }

        QT_DEBUG("RSSI: %d", rssi);
        return rssi;
      }
    }
  }
  return 0;
}
//

uint8_t GSM_getOperatorName(char *buffer)
{
  // Reply is:
  // +COPS: 0,0,"Telenor SE",6
  //
  // OK
  if(sendAndWaitForReply("AT+COPS?", 1000, 3))
  {
    const char delimiter[] = ",";
    char *token = strtok(_buffer, delimiter);
    if(token)
    {
      token = strtok(NULL, delimiter);
      token = strtok(NULL, delimiter);
      if(token)
      {
        // Strip out the " characters
        uint8_t len = strlen(token);
        strncpy(buffer, token + 1, len - 7);
        buffer[len - 2] = 0;

        QT_DEBUG("Operator: %s", buffer);

        return strlen(buffer);
      }
    }
  }
  return 0;
}
//

eNwkRegState GSM_getGSMRegistration(void)
{
  if(sendAndWaitForReply("AT+CREG?", 1000, 3))
  {
    const char delimiter[] = ",";
    char *token = strtok(_buffer, delimiter);
    if(token)
    {
      token = strtok(NULL, delimiter);
      if(token)
      {
        return (eNwkRegState)(token[0] - 0x30);
      }
    }
  }
  return eNRS_Unknown;
}
//

eNwkRegState GSM_getGPRSRegistration(void)
{
  if(sendAndWaitForReply("AT+CGREG?", 1000, 3))
  {
    const char delimiter[] = ",";
    char *token = strtok(_buffer, delimiter);
    if(token)
    {
      token = strtok(NULL, delimiter);
      if(token)
      {
        return (eNwkRegState)(token[0] - 0x30);
      }
    }
  }
  return eNRS_Unknown;
}
//




/* GPRS Functions ------------------------------------------------------------*/
bool GSM_DisconnectGPRS(void)
{
  if(!sendAndCheckReply("AT+QIDEACT", _DEACTOK, IPDEACTIVATE_TIMEOUT))
  {
    QT_ERROR("Failed to deactivate PDP context");
    return false;
  }
  return true;
}
//

eIPState_TypeDef GSM_isGPRSConnected(void)
{
	if(!sendAndCheckReply("AT+QISTAT", "STATE: ", GSMSTANDARD_TIMEOUT))
	{
		QT_ERROR("Failed to check GPRS connection");
		return eIPState_UNKOWN;
	}
	
	if(strstr(_buffer, IPSTACK_STATE_INITIAL) == 0)
		return eIPState_INITIAL;
	else if(strstr(_buffer, IPSTACK_STATE_START) == 0)
		return eIPState_START;
	else if(strstr(_buffer, IPSTACK_STATE_CONFIG) == 0)
		return eIPState_CONFIG;
	else if(strstr(_buffer, IPSTACK_STATE_IND) == 0)
		return eIPState_IND;
	else if(strstr(_buffer, IPSTACK_STATE_GPRSACT) == 0)
		return eIPState_GPRSACT;
	else if(strstr(_buffer, IPSTACK_STATE_STATUS) == 0)
		return eIPState_STATUS;
	else
		return eIPState_UNKOWN;
}
//

bool GSM_ConnectGPRS(const char *apn, const char *userId, const char *password)
{
  if(!sendAndCheckReply("AT+QIFGCNT=0", _OK, GSMSTANDARD_TIMEOUT))
  {
    QT_ERROR("Failed to select context");
    return false;
  }

  // First set up PDP context
  if(!sendAndCheckReply("AT+QICSGP=1", _OK, GSMSTANDARD_TIMEOUT))
  {
    QT_ERROR("Failed to setup PDP context");
    return false;
  }

  sprintf(_buffer, "AT+QIREGAPP=\"%s\",\"%s\",\"%s\"", apn, userId, password);
  if(!sendAndCheckReply(_buffer, _OK, GSMSTANDARD_TIMEOUT))
  {
    QT_ERROR("Failed to disable MUXIP");
    return false;
  }


  callWatchdog();

  // Activate PDP context
  if(!sendAndCheckReply("AT+QIACT", _OK, SEC2mSEC(PDPACTIVATION_TIMEOUT)))
  {
    QT_ERROR("Failed to activate PDP context");
    return false;
  }
	
	if(!sendAndWaitFor("AT+QISTAT", "STATE: IP GPRSACT", GSMSTANDARD_TIMEOUT))
	{
		QT_ERROR("Failed to check PDP context activation");
		return false;
	}
	
  return true;
}
//




/* SSL Functions -------------------------------------------------------------*/
void GSM_setEncryption(eTlsEncryption enc)
{
  _encryption = enc;
}
//




/* HTTP Interface Functions --------------------------------------------------*/
bool GSM_HTTP_GET(const char *url, const char *fileName)
{
  // (Uses PDP context 2)
  // -> AT+QHTTPCFG="contextid",2
  // <- OK
  // -> AT+QHTTPURL=23,80
  // <- CONNECT
  // -> http://www.sina.com.cn
  // <- OK
  // -> AT+QHTTPGET=80
  // <- OK
  // <- +QHTTPGET: 0,200,631871
  // -> AT+QHTTPREADFILE="RAM:1.bin",60,2
  // <- OK
  // <- +QHTTPREADFILE
  bool ssl = strstr(url, "https://") != NULL;

  if(!sendAndCheckReply("AT+QHTTPCFG=\"contextid\",1", _OK, 10000))
  {
    QT_ERROR("Failed to activate PDP context");
    return false;
  }

  if(ssl)
  {
    QT_TRACE("Enabling SSL support");
    if(!sendAndCheckReply("AT+QHTTPCFG=\"sslctxid\",1", _OK, 10000))
    {
      QT_ERROR("Failed to activate SSL context ID");
      return false;
    }
    if(!activateSsl())
    {
      return false;
    }
  }

  sprintf(_buffer, "AT+QHTTPURL=%i,30", strlen(url));
  if(!sendAndCheckReply(_buffer, _CONNECT, 2000))
  {
    QT_ERROR("Failed to activate URL");
    return false;
  }
  if(!sendAndCheckReply(url, _OK, 2000))
  {
    QT_ERROR("Failed to send URL");
    return false;
  }
  if(!sendAndCheckReply("AT+QHTTPGET=60", _OK, 30000))
  {
    QT_ERROR("Failed to send request");
    return false;
  }

  sprintf(_buffer, "AT+QHTTPDL=\"RAM:%s\",1024,30", fileName);
  if(!sendAndWaitForReply(_buffer, 60000, 3))
  {
    QT_ERROR("Failed to read response");
    return false;
  }

  return true;
}
//

eRequestStatus_TypeDef GSM_HTTP_POST(bool customHeader, const char *url, uint8_t *postData, char *response_buffer, uint32_t length, uint8_t timeout)
{
	uint32_t module_timeout = (timeout + 1) * 1000;
  // (Uses PDP context 2)
  // -> AT+QHTTPCFG="contextid",2
  // <- OK
  // -> AT+QHTTPURL=23,80
  // <- CONNECT
  // -> http://www.sina.com.cn
  // <- OK
  // -> AT+QHTTPPOST=18,50,10
	// <- CONNECT
	// -> Data...
  // <- OK
  // <- +QHTTPGET: 0,200,631871
  // -> AT+QHTTPREADFILE="RAM:1.bin",60,2
  // <- OK
  // <- +QHTTPREADFILE
  bool ssl = strstr(url, "https://") != NULL;

  if(ssl)
  {
    QT_TRACE("Enabling SSL support");
    if(!sendAndCheckReply("AT+QHTTPCFG=\"sslctxid\",1", _OK, 10000))
    {
      QT_ERROR("Failed to activate SSL context ID");
      return eReqStat_RequestFailed;
    }
    if(!activateSsl())
    {
      return eReqStat_RequestFailed;
    }
  }
	//

	if(customHeader == true)
	{
		if(!sendAndCheckReply("AT+QHTTPCFG=\"requestheader\",1", _OK, GSMSTANDARD_TIMEOUT))
		{
			QT_ERROR("Failed to activate PDP context");
			return eReqStat_RequestFailed;
		}
//		if(!sendAndCheckReply("AT+QHTTPCFG=\"responseheader\",1", _OK, GSMSTANDARD_TIMEOUT))
//		{
//			QT_ERROR("Failed to activate PDP context");
//			return eReqStat_RequestFailed;
//		}
	}
	//
	
	/* send http url in 2 second timeout and wait for CONNECT response */
  sprintf(_buffer, "AT+QHTTPURL=%u,2", strlen(url)/*, timeout*/);
  if(!sendAndCheckReply(_buffer, _CONNECT, 2000))
  {
    QT_ERROR("Failed to activate URL");
    return eReqStat_RequestFailed;
  }
	//
	
	/* send url */
  if(!sendAndCheckReply(url, _OK, 2000))
  {
    QT_ERROR("Failed to send URL");
    return eReqStat_RequestFailed;
  }
	//
	
	/* send post request with data */
	sprintf(_buffer, "AT+QHTTPPOST=%u,%d,%d", length, timeout, timeout);
  if(!sendAndCheckReply(_buffer, _CONNECT, SEC2mSEC(120)))
  {
    QT_ERROR("Failed to send request");
    return eReqStat_RequestFailed;
  }
	//
	
	/* send post data */
  QT_COM_TRACE(" -> %s", postData);
  if(!sendBufferWaitOK(postData, length, _OK, SEC2mSEC(120)))
  {
    QT_ERROR("Failed to send response");
    return eReqStat_RequestFailed;
  }
	//
	
//	portENTER_CRITICAL();
	if(response_buffer != NULL)
	{
		/* read response */
		sprintf(_buffer, "AT+QHTTPREAD=%d", timeout);
		if(!sendAndWaitFor2(_buffer, _OK, module_timeout))
		{
			QT_ERROR("Failed to read response");
			return eReqStat_NoResponseRcv;
		}
	//	portEXIT_CRITICAL();
		
		char *pStart = &_buffer2[8];
		char *pEnd = strstr(_buffer2, "OK");
		if(pEnd != NULL)
		{
			strncpy(response_buffer, pStart, ((pEnd - 1) - pStart));
			return eReqStat_Successfull;
		}
		else
			return eReqStat_NoResponseRcv;
	}
	
	return eReqStat_Successfull;
}
//

bool GSM_HTTP_GET_DownloadFile(const char *url, const char *filename, uint8_t *buffer, uint32_t size)
{
  /* check for null pointer */
  if(buffer == NULL)
    return false;

  // (Uses PDP context 2)
  // -> AT+QHTTPCFG="contextid",2
  // <- OK
  // -> AT+QHTTPURL=23,80
  // <- CONNECT
  // -> http://www.sina.com.cn
  // <- OK
  // -> AT+QHTTPGET=80
  // <- OK
  // <- +QHTTPGET: 0,200,631871
  // -> AT+QHTTPREADFILE="RAM:1.bin",60,2
  // <- OK
  // <- +QHTTPREADFILE
  bool ssl = strstr(url, "https://") != NULL;

  /* check for secure http connection */
  if(ssl)
  {
    QT_TRACE("Enabling SSL support");
    if(!sendAndCheckReply("AT+QHTTPCFG=\"sslctxid\",1", _OK, 10000))
    {
      QT_ERROR("Failed to activate SSL context ID");
      return false;
    }
    if(!activateSsl())
    {
      return false;
    }
  }
  //

  /* set url length */
  sprintf(_buffer, "AT+QHTTPURL=%d,30", strlen(url));
  if(!sendAndCheckReply(_buffer, _CONNECT, 2000))
  {
    QT_ERROR("Failed to activate URL");
    return false;
  }
  //

  /* send url */
  if(!sendAndCheckReply(url, _OK, 2000))
  {
    QT_ERROR("Failed to send URL");
    return false;
  }
  //

	uint32_t currentPart = 0;
	uint32_t totalPart = ((size / DOWNLOADFILE_SIZE) + ( ( (size % DOWNLOADFILE_SIZE) != 0) ? 1 : 0));
	
  /* get file in a loop */
  do
  {
    uint32_t fileSize;
    if(size > DOWNLOADFILE_SIZE)
    {
      size -= DOWNLOADFILE_SIZE;
      fileSize = DOWNLOADFILE_SIZE;
    }
    else
      fileSize = size;

		HAL_UART_Receive_IT(&huart1, &uartChar, 1);
		
    /* send get request with length */
    sprintf(_buffer, "AT+QHTTPGET=60,30,%d", fileSize);
    if(!sendAndWaitFor(_buffer, _OK, 30000))
    {
      QT_ERROR("Failed to send request");
      return false;
    }
    //

    QT_DEBUG("URL activated [%s] and request sended", url);

    /* download file to the module ram */
    sprintf(_buffer, "AT+QHTTPDL=\"RAM:%s\",%d,30", filename, DOWNLOADFILE_SIZE);
    if(!sendAndWaitForReply(_buffer, 60000, 3))
    {
      QT_ERROR("Failed to read response");
      return false;
    }
    //

    QT_DEBUG("%s downloaded from %s", filename, url);

    /* get file to the buffer */
    if(!GSM_downloadFile(filename, buffer, fileSize))
      return false;

    /* increse file buffer pointer */
    buffer += fileSize;
		
		/* HAL_Delay */
		HAL_Delay(1000);
		
		/* increase part index */
		currentPart++;
  }while(currentPart < totalPart);

  return true;
}
//




/* TCP Interface Functions ---------------------------------------------------*/
void GSM_stop(void)
{
  // AT+QICLOSE=1,10
  sprintf(_command, "+Q%sCLOSE", useEncryption() ? _SSL_PREFIX : _INET_PREFIX);
  sprintf(_buffer, "AT%s=1,10", _command);
  if(!sendAndCheckReply(_buffer, _OK, 10000))
  {
    QT_ERROR("Failed to close connection");
  }
  uint32_t timeout = HAL_GetTick() + 20000;
  while(HAL_GetTick() < timeout)
  {
    sprintf(_command, "AT+Q%sSTATE=1,1", useEncryption() ? _SSL_PREFIX : _INET_PREFIX);
    sendAndWaitForReply(_command, 1000, 3);
    if(_buffer[0] == 'O' && _buffer[1] == 'K')
    {
      QT_TRACE("Disconnected");
      return;
    }
    callWatchdog();
    HAL_Delay(500);
  }
}
//

void GSM_flush(void)
{
  while(xRingBuffer.available(&xRBStruct))
  {
    xRingBuffer.getc(&xRBStruct);
  }
}
//


int GSM_peek(void)
{
  // Not supported
  return 0;
}
//

int GSM_read(void)
{
  uint8_t buffer[2];
  GSM_readBuffer(buffer, 1);

  return buffer[0];
}
//

int GSM_available(void)
{
  if(useEncryption())
  {
    if(sslLength > 0)
    {
      return sslLength;
    }

    sprintf(_buffer, "AT+QSSLRECV=1,%i", sizeof(_buffer) - 36);
    if(sendAndWaitForReply(_buffer, 1000, 3))
    {
      char *recvToken = strstr(_buffer, "+QSSLRECV: ");
      recvToken += 11;

      /* Get length of data */
      char *lfToken = strstr(recvToken, "\n");
      uint32_t llen = lfToken - recvToken;

      char numberStr[llen];
      strncpy(numberStr, recvToken, llen);
      numberStr[llen] = '\0';

      sslLength = atoi(numberStr);

      if(sslLength > 0)
      {
        //set start of data to after the last new line
        char *data = lfToken + 1;

        //The check below is because sometimes a URC-message gets through and ruins everything because it includes 2 more lines that we don't expect
        //First cut off the message so we only check the first 30 characters

        //get first 30 characters
        char subbuff[31];
        memcpy(subbuff, _buffer, 30);
        subbuff[30] = '\0';

        //A bad message can at a maximum look like this: +QSSLURC: "recv",1<LF><LF>+QSSLRECV: [number]<LF>
        //but sometimes only part of it is included, example: v",1<LF><LF>+ QSSLRECV:[number]<LF>
        //i check for 2 new lines before +QSSLRECV because if there only is 1 then the data will still be read
        if(strstr(subbuff, "\n\n+QSSLRE") != NULL)
        {
          if(readReply(1000, 2))
          {
            data = _buffer;
          }
          else
          {
            QT_ERROR("Could not get data after URC-interrupt");

            sslLength = 0;
          }
        }

        memcpy(_readBuffer, data, sslLength);
      }
      QT_TRACE("available sslLength: %i", sslLength);

      return sslLength;
    }
  }
  else
  {
    sprintf(_buffer, "AT+QIRD=1,0");
    if(sendAndWaitForReply(_buffer, 1000, 3))
    {
      const char delimiter[] = ",";
      char *token = strtok(_buffer, delimiter);
      if(token)
      {
        token = strtok(NULL, delimiter);
        if(token)
        {
          token = strtok(NULL, delimiter);
          if(token)
          {
            char *ptr;
            uint16_t unread = strtol(token, &ptr, 10);
            QT_COM_TRACE("Available: %i", unread);
            return unread;
          }
        }
      }
    }
  }
  QT_COM_ERROR("Failed to read response");

  return 0;
}
//

bool GSM_connected(void)
{
  // Response is:
  // +QISTATE: 1,"TCP","54.225.64.197",80,4097,5,1,1,0,"uart1"
  //
  // OK
  //
  //NOTE ON SSL:
  //UG96 has a bug where it returns instead of qsslstate on qsslstate requests
  //to mitigate this we have changed occurances of _command to QISTATE
  sprintf(_command, "Q%sSTATE", useEncryption() ? _SSL_PREFIX : _INET_PREFIX);
  sprintf(_buffer, "AT+%s=1,1", _command);
  if(sendAndWaitForReply(_buffer, 1000, 3) && strstr(_buffer, "QISTATE"))
  {
    char *tokenStart = strstr(_buffer, "QISTATE");
    tokenStart = &_buffer[tokenStart - _buffer];

    char *token = strtok(tokenStart, ",");
    token = strtok(NULL, ",");
    token = strtok(NULL, ",");
    token = strtok(NULL, ",");
    token = strtok(NULL, ",");
    token = strtok(NULL, ",");

    QT_COM_TRACE("Socket State: %s, Connected: %s", token, strcmp(token, "3") == 0 ? "true" : "false");

    return strcmp(token, "3") == 0;
  }
  return false;
}
//

size_t GSM_write(uint8_t value)
{
  return GSM_writeBuffer(&value, 1);
}
//

int GSM_readBuffer(uint8_t *buf, size_t size)
{
  if(size == 0)
  {
    return 0;
  }

  if(useEncryption())
  {
    uint32_t length = size > sslLength ? sslLength : size;
    QT_COM_TRACE("Data len: %i", length);

    memcpy(buf, _readBuffer, length);

    buf[length] = '\0';

    QT_COM_TRACE_START(" <- ");
    QT_COM_TRACE_ASCII(buf, length);
    QT_COM_TRACE_END("");

    sslLength -= length;
    QT_COM_TRACE("Remaining len: %i", sslLength);
    if(sslLength > 0)
    {
      QT_COM_TRACE("Move %i, %i", length, sslLength);
      memcpy(_readBuffer, _readBuffer + length, sslLength);
    }

    return length;
  }
  else
  {
    sprintf(_buffer, "AT+QIRD=1,%i", size);
    if(sendAndWaitForReply(_buffer, 1000, 1) && strstr(_buffer, "+QIRD:"))
    {
      // +QIRD: <len>
      // <data>
      //
      // OK
      char *token = strtok(_buffer, " ");
      token = strtok(NULL, "\n");
      char *ptr;
      uint16_t length = strtol(token, &ptr, 10);
      QT_COM_TRACE("Data len: %i", length);

      xRingBuffer.gets(&xRBStruct, buf, length);
      buf[length] = '\0';

      QT_COM_TRACE_START(" <- ");
      QT_COM_TRACE_ASCII(_buffer, size);
      QT_COM_TRACE_END("");

      return length;
    }
  }

  return 0;
}
//

int GSM_connectTo(const char *host, uint16_t port)
{
  if(useEncryption())
  {
    if(!activateSsl())
    {
      return false;
    }
  }

  sprintf(_buffer, "AT+QCFG=\"urc/port\",0,\"uart1\"");
  if(!sendAndCheckReply(_buffer, _OK, 1000))
  {
    QT_ERROR("Could not remove urc messages");
    return false;
  }

  // AT+QIOPEN=1,1,"TCP","220.180.239.201",8713,0,0
  sprintf(_command, "+Q%sOPEN", useEncryption() ? _SSL_PREFIX : _INET_PREFIX);
  if(useEncryption())
  {
    sprintf(_buffer, "AT%s=1,1,1,\"%s\",%i,0", _command, host, port);
  }
  else
  {
    sprintf(_buffer, "AT%s=1,1,\"TCP\",\"%s\",%i,0,0", _command, host, port);
  }

  if(!sendAndCheckReply(_buffer, _OK, 1000))
  {
    QT_ERROR("Connection failed");
    return false;
  }

  // Now we are waiting for connect
  // +QIOPEN: 1,0
  uint32_t expireTime = HAL_GetTick() + (30 * 1000);
  while(expireTime < HAL_GetTick())             // TODO: Timout
  {
    callWatchdog();
    if(readReply(500, 1) && strstr(_buffer, _command))
    {
      char *token = strtok(_buffer, " ");
      token = strtok(NULL, ",");

      if(*token != '1')
      {
        QT_ERROR("Connection failed, %i, '%c'", strlen(_buffer), token);
        return false;
      }
      QT_DEBUG("Connection open");
      return true;
    }
    if(HAL_GetTick() > expireTime)
    {
      QT_ERROR("Connection timeout");
      return false;
    }
  }

  QT_TRACE(_buffer);

  return false;
}
//

size_t GSM_writeBuffer(const uint8_t *buf, uint32_t size)
{
  // TODO: Max 1460 bytes can be sent in one +QISEND session
  // Add a loop
  sprintf(_command, "+Q%sSEND", useEncryption() ? _SSL_PREFIX : _INET_PREFIX);
  sprintf(_buffer, "AT%s=1,%i", _command, size);
  if(!sendAndWaitFor(_buffer, "> ", 5000))
  {
    QT_ERROR("%s handshake error, %s", _command, _buffer);
    return 0;
  }
  QT_COM_TRACE_START(" -> ");
  QT_COM_TRACE_BUFFER(buf, size);
  QT_COM_TRACE_END("");

  HAL_UART_Transmit(&huart1, (uint8_t *)buf, size, 10);
  if(readReply(5000, 1) && strstr(_buffer, "SEND OK"))
  {
    return size;
  }

  QT_ERROR("Send failed");

  return 0;
}
//

int GSM_connectSecure(const char *host, uint16_t port, eTlsEncryption encryption)
{
  _encryption = encryption;
  return GSM_connectTo(host, port);
}
//




/* FILE Interface Functions --------------------------------------------------*/
bool GSM_deleteFile(const char *fileName)
{
  // AT+QFDEL:"RAM:file.txt"
  // OK
  sprintf(_buffer, "AT+QFDEL=\"RAM:%s\"", fileName);
  if(!sendAndCheckReply(_buffer, _OK, 1000))
  {
    QT_ERROR("Timeout deleting file: %s", _buffer);
    return false;
  }

  return checkResult();
}
//

bool GSM_closeFile(FILE_HANDLE fileHandle)
{
  // AT+QFCLOSE=3000
  // OK
  sprintf(_buffer, "AT+QFCLOSE=%i", fileHandle);
  if(!sendAndCheckReply(_buffer, _OK, 1000))
  {
    QT_ERROR("Timeout closing file: %s", _buffer);
    return false;
  }

  return checkResult();
}
//

int32_t GSM_getFileList(char *listBuffer, uint8_t storage)
{
  // AT+QFLST:"RAM:file.txt"
  // +QFLST:"RAM:file.txt"",734"
  //
  // OK
  char str[6] = "";
  if(storage == 0)
  {
    strcpy(str, "");
  }
  else if(storage == 1)
  {
    strcpy(str, "RAM:");
  }

  sprintf(_buffer, "AT+QFLST=\"%s*\"", str);
  if(!sendAndWaitForReply(_buffer, 1000, 10))
  {
    QT_ERROR("Get file size error 1: %s", _buffer);
    return -1;
  }
  char *token = strtok(_buffer, "+QFLST: ");
  if(!token)
  {
    QT_ERROR("Get file size error: %s", _buffer);
    return -1;
  }
  token = strtok(NULL, ",");
  token = strtok(NULL, "\n");
  uint32_t result = atoi(token);
  return result;
}
//

int32_t GSM_getFileSize(const char *fileName)
{
  // AT+QFLST:"RAM:file.txt"
  // +QFLST:"RAM:file.txt"",734"
  //
  // OK
  sprintf(_buffer, "AT+QFLST=\"RAM:%s\"", fileName);
  if(!sendAndWaitForReply(_buffer, 1000, 2))
  {
    QT_ERROR("Get file size error 1: %s", _buffer);
    return -1;
  }
  char *token = strtok(_buffer, "+QFLST: ");
  if(!token)
  {
    QT_ERROR("Get file size error: %s", _buffer);
    return -1;
  }
  token = strtok(NULL, ",");
  token = strtok(NULL, "\n");
  uint32_t result = atoi(token);

  QT_DEBUG("%s size %u byte(s)", fileName, result);

//    return (result - strlen(fileName) + 1);
  return (result);
}
//

bool GSM_truncateFile(FILE_HANDLE fileHandle)
{
  // AT+QFTUCAT=3000
  // OK
  sprintf(_buffer, "AT+QFTUCAT=%i", fileHandle);
  if(!sendAndCheckReply(_buffer, _OK, 1000))
  {
    QT_ERROR("Timeout deleting file: %s", _buffer);
    return false;
  }
  return checkResult();
}
//

int32_t GSM_getFilePosition(FILE_HANDLE fileHandle)
{
  // AT+QFPOSITION=3000
  // +QFPOSITION: 123
  //
  // OK
  sprintf(_buffer, "AT+QFPOSITION=%i", fileHandle);
  if(!sendAndWaitForReply(_buffer, 1000, 3))
  {
    QT_ERROR("File position error: %s", _buffer);
    return -1;
  }

  char *token = strtok(_buffer, "+QFPOSITION: ");
  if(!token)
  {
    QT_ERROR("Get position error: %s", _buffer);
    return -1;
  }

  uint32_t result = atoi(token);
  return result;
}
//

FILE_HANDLE GSM_openFile(const char *fileName, bool overWrite)
{
  // AT+QFOPEN="RAM:file.ext",0
  // +QFOPEN:3000
  //
  // OK
  sprintf(_buffer, "AT+QFOPEN=\"RAM:%s\",%i", fileName, overWrite ? 1 : 0);
  if(!sendAndWaitForReply(_buffer, 1000, 3))
  {
    QT_ERROR("Timeout opening file");
    return (FILE_HANDLE)NOT_A_FILE_HANDLE;
  }
  char *token = strtok(_buffer, "+QFOPEN: ");
  if(token)
  {
    uint32_t result = atoi(token);
    return result;
  }
  return (FILE_HANDLE)NOT_A_FILE_HANDLE;
}
//

bool GSM_seekFile(FILE_HANDLE fileHandle, uint32_t length)
{
  // AT+QFSEEK=3000,0,0
  // OK
  sprintf(_buffer, "AT+QFSEEK=%i,%u,0", fileHandle, length);
  if(!sendAndCheckReply(_buffer, _OK, 1000))
  {
    QT_ERROR("Seek error: %s", _buffer);
    return false;
  }
  return checkResult();
}
//

bool GSM_readFile(FILE_HANDLE fileHandle, uint8_t *buffer, uint32_t length)
{
  // AT+QFREAD=3000,10
  // CONNECT
  // Read data
  //
  // OK
  sprintf(_buffer, "AT+QFREAD=%i,%u", fileHandle, length);
  if(!sendAndCheckReply(_buffer, _CONNECT, 1000))
  {
    QT_ERROR("Timeout for read command");
    return false;
  }

  int32_t timeout = 10000;
  while(timeout >= 0 && length > 0)
  {
    if(xRingBuffer.available(&xRBStruct))
    {
      *buffer = xRingBuffer.getc(&xRBStruct);
      buffer++;
      length--;
    }
    timeout--;
    HAL_Delay(1);
  }

  if(!readReply(1000, 1))
  {
    QT_ERROR("No reply after read");
    return false;
  }
  return checkResult();
}
//

// TODO: Not tested
bool GSM_downloadFile(const char *fileName, uint8_t *buffer, uint32_t size)
{
  // AT+QFDWL="RAM:test.txt"
  // CONNECT file length
  // <read data>
  // +QFDWL: 10,613e

  /* check for correct file and size */
  uint32_t fileSize = GSM_getFileSize(fileName);
  if(size != 0 && fileSize != size)
  {
    return false;
  }

  sprintf(_buffer, "AT+QFDWL=\"RAM:%s\"", fileName);
  if(!sendAndWaitFor(_buffer, "+QFDWL: ", 20000))
  {
    QT_ERROR("No response to download command");
    return false;
  }

  QT_DEBUG("%s downloaded from module. Parsing now.", fileName);

  memcpy(buffer, &_buffer[8], fileSize);

  /* flush gsm buffer */
  readReply(500, 5);

  return true;
}
//

// TODO: Not tested
bool GSM_uploadFile(const char *fileName, const uint8_t *buffer, uint32_t length)
{
  // AT+QFUPL="RAM:test1.txt",10
  // CONNECT
  // <data>
  // +QFUPL:300,B34A
  sprintf(_buffer, "AT+QFUPL=\"RAM:%s\",%u", fileName, length);
  if(!sendAndWaitForReply(_buffer, 1000, 2))
  {
    QT_ERROR("No response to upload command");
    return false;
  }

  if(!strstr(_buffer, _CONNECT))
  {
    QT_ERROR(_buffer);
    return false;
  }

  for(uint32_t i = 0; i < length; i++)
  {
    HAL_UART_Transmit(&huart1, (uint8_t *)(buffer + i), 1, 10);
  }

  if(!readReply(1000, 2))
  {
    QT_ERROR("No reponse after upload");
  }

  return checkResult();
}
//

bool GSM_writeFile(FILE_HANDLE fileHandle, const uint8_t *buffer, uint32_t length)
{
  // AT+QFWRITE=3000,10
  // CONNECT
  // write 10 bytes
  // +QFWRITE(10,10)
  sprintf(_buffer, "AT+QFWRITE=%i,%u", fileHandle, length);
  if(sendAndCheckReply(_buffer, _CONNECT, 1000))
  {
    for(uint32_t i = 0; i < length; i++)
    {
      HAL_UART_Transmit(&huart1, (uint8_t *)(buffer + i), 1, 10);
    }
    if(!readReply(1000, 3))
    {
      QT_ERROR("No reply after write");
      return false;
    }
    return (strstr(_buffer, "+QFWRITE:") != NULL);
  }
  return false;
}
//

/* Callback Function ---------------------------------------------------------*/
void GSM_setWatchdogCallback(watchdogcallbacksignature wdCallback)
{
  watchdogcallback = wdCallback;
}
//




/************************ (C) COPYRIGHT YıLDıZ IoT *****************************/
