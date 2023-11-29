/**
  ******************************************************************************
  * @file    quectel_m66.h
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
#ifndef __QUECTEL_M66_H
#define __QUECTEL_M66_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "ringbuffer.h"
#include "stm32f7xx_hal.h"

/* Public typedef ------------------------------------------------------------*/
typedef enum _eQuectelModule
{
	eUG96 = 0,
	eBG96,
	eM95,
	eM66,
} eQuectelModule;
//

typedef enum _eNwkRegState
{
    eNRS_NotRegistered = 0,
    eNRS_Registered,
    eNRS_Searching,
    eNRS_Denied,
    eNRS_Unknown,
    eNRS_Roaming
} eNwkRegState;
//

enum _eTlsEncryption
{
    eTE_None = 0,
    eTE_Ssl30,
    eTE_Tls10,
    eTE_Tls11,
    eTE_Tls12,
    eTE_All
};
typedef enum _eTlsEncryption eTlsEncryption;
//

typedef enum eGSMErrorCode_t
{
	eGSMErrCode_NoError,
	eGSMErrCode_NoModule,
	eGSMErrCode_NoSIMCard,
	eGSMErrCode_NotRegGSM,
	eGSMErrCode_NotRegGPRS,
	eGSMErrCode_NotMatchModel,
	
	eGSMErrCode_UNKNOWNERROR
} eGSMErrorCode;
//

typedef enum eRequestStatus_t
{
	eReqStat_RequestFailed,
	eReqStat_NoResponseRcv,
	eReqStat_Successfull
	
} eRequestStatus_TypeDef;
//

typedef enum eIPState_t
{
	eIPState_INITIAL,
	eIPState_START,
	eIPState_CONFIG,
	eIPState_IND,
	eIPState_GPRSACT,
	eIPState_STATUS,
	
	eIPState_UNKOWN
} eIPState_TypeDef;
//

struct _xGSMStruct
{
	char imei[12];
	char simCCID[32];
	char ipaddress[16];
	
	struct
	{
		char *Name;
		char *UserName;
		char *Password;
	}AccessPoint;
	
	int8_t rssi;
};
//

/** callback function signature */
typedef void (*watchdogcallbacksignature)(void);

/* Public define -------------------------------------------------------------*/
#define GSM_PWRKEY_GPIO_Port 			GPIOA
#define GSM_PWREN_GPIO_Port 			GPIOA

#define GSM_PWREN_Pin 						GPIO_PIN_3
#define GSM_PWRKEY_Pin 						GPIO_PIN_8



//#define M2M_QUECTEL_DEBUG
//#define M2M_QUECTEL_COM_DEBUG

#define NOT_A_FILE_HANDLE   			(-1)
#define FILE_HANDLE        				uint32_t

#define UNKNOWN_FILESIZE					(0)

#define PDPACTIVATION_TIMEOUT			(uint16_t)(60)
#define GSMSTANDARD_TIMEOUT				(uint16_t)(300)
#define IPDEACTIVATE_TIMEOUT			(uint16_t)(90000)
#define IPQUERYRESPONSE_TIMEOUT		(uint16_t)(10000)

/* Public macro --------------------------------------------------------------*/
#define SEC2mSEC(x)								(uint16_t)(x * 1000)

#ifdef M2M_QUECTEL_DEBUG
#define QT_INFO(...) 												printf(__VA_ARGS__);printf("\r\n")
#define QT_DEBUG(...) 											printf(__VA_ARGS__);printf("\r\n")
#define QT_ERROR(...)												printf(__VA_ARGS__);printf("\r\n")
#define QT_TRACE(...) 											printf(__VA_ARGS__);printf("\r\n")
#define QT_TRACE_END(...) 									printf(__VA_ARGS__);printf("\r\n")
#define QT_TRACE_PART(...) 									printf(__VA_ARGS__);printf("\r\n")
#define QT_TRACE_START(...) 								printf(__VA_ARGS__);printf("\r\n")
#else
#define QT_INFO(...)
#define QT_DEBUG(...)
#define QT_ERROR(...)
#define QT_TRACE(...)
#define QT_TRACE_END(...)
#define QT_TRACE_PART(...)
#define QT_TRACE_START(...)
#endif

#ifdef M2M_QUECTEL_COM_DEBUG
#define QT_COM_INFO(...) 										printf(__VA_ARGS__);printf("\r\n")
#define QT_COM_DEBUG(...) 									printf(__VA_ARGS__);printf("\r\n")
#define QT_COM_ERROR(...)										printf(__VA_ARGS__);printf("\r\n")
#define QT_COM_TRACE(...) 									printf(__VA_ARGS__);printf("\r\n")
#define QT_COM_TRACE_END(...) 							printf(__VA_ARGS__);printf("\r\n")
#define QT_COM_TRACE_PART(...) 							printf(__VA_ARGS__);printf("\r\n")
#define QT_COM_TRACE_START(...) 						printf(__VA_ARGS__);printf("\r\n")
#define QT_COM_TRACE_BUFFER(buffer, size)		//if (_logger != nullptr) _logger->tracePartHexDump(buffer, size)
#define QT_COM_TRACE_ASCII(buffer, size)		//if (_logger != nullptr) _logger->tracePartAsciiDump(buffer, size)
#else
#define QT_COM_INFO(...)
#define QT_COM_DEBUG(...)
#define QT_COM_ERROR(...)
#define QT_COM_TRACE(...)
#define QT_COM_TRACE_END(...)
#define QT_COM_TRACE_PART(...)
#define QT_COM_TRACE_START(...)
#define QT_COM_TRACE_ASCII(buffer, size)
#define QT_COM_TRACE_BUFFER(buffer, size)
#endif

/* Public variables ----------------------------------------------------------*/
/* Public function prototypes ------------------------------------------------*/

extern void Error_Handler(void);


void 					GSM_DeInitialize(void);

int8_t 				GSM_getLastError(void);
eGSMErrorCode	GSM_Initialize(void);

/* Module Power Functions ----------------------------------------------------*/
bool					GSM_getStatus(void);
float 				GSM_getVoltage(void);
bool 					GSM_PowerOn(bool state);

/* SIM Card Security Functions -----------------------------------------------*/
bool 					GSM_getSimPresent(void);


/* SIM Related Functions -----------------------------------------------------*/
uint8_t 			GSM_getIMEI(char* buffer);
uint8_t 			GSM_getSIMCCID(char* buffer);
const char		*GSM_getFirmwareVersion(void);


/* RTC Related Functions -----------------------------------------------------*/
bool 					GSM_getLocalTime(uint32_t *year, uint32_t *mount, uint32_t *day, uint32_t *hour, uint32_t *minute, uint32_t *second, int32_t *timeZone);


/* Network Querying And Setting Functions ------------------------------------*/
int8_t				GSM_getRSSI(void);
eNwkRegState 	GSM_getGSMRegistration(void);
eNwkRegState 	GSM_getGPRSRegistration(void);
uint8_t 			GSM_getOperatorName(char* buffer);

/* GPRS Functions ------------------------------------------------------------*/
bool 							GSM_DisconnectGPRS(void);
eIPState_TypeDef 	GSM_isGPRSConnected(void);
bool 							GSM_ConnectGPRS(const char* apn, const char* userid, const char* password);

/* SSL Functions -------------------------------------------------------------*/
void 					GSM_setEncryption(eTlsEncryption enc);

/* HTTP Interface Functions --------------------------------------------------*/
bool					GSM_HTTP_GET(const char* url, const char *fileName);
bool					GSM_HTTP_GET_DownloadFile(const char *url, const char *fileName, uint8_t * buffer, uint32_t size);

eRequestStatus_TypeDef	GSM_HTTP_POST(bool customHeader, const char *url, uint8_t *postData, char *response_buffer, uint32_t length, uint8_t timeout);

/* TCP Interface Functions ---------------------------------------------------*/
void 					GSM_stop(void);
void 					GSM_flush(void);

int32_t				GSM_peek(void);
int32_t 			GSM_read(void);
int32_t 			GSM_available(void);
bool 					GSM_connected(void);

uint32_t			GSM_write(uint8_t byte);
int32_t 			GSM_readBuffer(uint8_t *buf, uint32_t size);
int32_t 			GSM_connectTo(const char *host, uint16_t port);
uint32_t			GSM_writeBuffer(const uint8_t *buf, uint32_t size);
int32_t				GSM_connectSecure(const char *host, uint16_t port, eTlsEncryption encryption);

/* FILE Interface Functions --------------------------------------------------*/
bool 					GSM_deleteFile(const char* fileName);
bool 					GSM_closeFile(FILE_HANDLE fileHandle);
int32_t 			GSM_getFileSize(const char* fileName);
bool 					GSM_truncateFile(FILE_HANDLE fileHandle);
int32_t 			GSM_getFilePosition(FILE_HANDLE fileHandle);
int32_t 			GSM_getFileList(char *listBuffer, uint8_t storage);
FILE_HANDLE 	GSM_openFile(const char* fileName, bool overWrite);
bool 					GSM_seekFile(FILE_HANDLE fileHandle, uint32_t length);
bool 					GSM_readFile(FILE_HANDLE fileHandle, uint8_t* buffer, uint32_t length);
bool 					GSM_downloadFile(const char *fileName, uint8_t *buffer, uint32_t size);
bool 					GSM_uploadFile(const char* fileName, const uint8_t* buffer, uint32_t length);
bool 					GSM_writeFile(FILE_HANDLE fileHandle, const uint8_t* buffer, uint32_t length);

// Callbacks
void 					GSM_setWatchdogCallback(watchdogcallbacksignature wdCallback);

#ifdef __cplusplus
}
#endif

#endif

/************************ (C) COPYRIGHT YıLDıZ IoT *****************************/
