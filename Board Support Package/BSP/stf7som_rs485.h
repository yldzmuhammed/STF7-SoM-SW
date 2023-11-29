/**
  ******************************************************************************
  * @file    stf7som_rs485.h
  * @author  yldzmuhammed
  * @version V1.0
  * @date    13-06-2020
  * @brief   This file contains the common defines and functions prototypes for
  *          the stf7som_rs485.c driver.
  ******************************************************************************
  * @attention
  *
  * experiment platform: YiLDiZ IoT STF7-SoM Board BSP Library
  * web-site: https://www.yildiziot.com
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STF7SoM_RS485_H
#define __STF7SoM_RS485_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STF7SoM
  * @{
  */
    
/** @addtogroup STF7SoM_RS485
  * @{
  */    

/** @defgroup STF7SoM_RS485_Exported_Types RS485 Exported Types
  * @{
  */
typedef enum
{
    eRS485Mode_Receiver,
    eRS485Mode_Tranciever
} eRS485Mode_TypeDef;
/**
  * @}
  */ 
    
/**
  * @}
  */ 
    
/** @defgroup STF7SoM_RS485_Exported_Macro RS485 Exported Macro
  * @{
  */
/**
  * @}
  */ 
    
/** @defgroup STF7SoM_RS485_Exported_Functions RS485 Exported Functions
  * @{
  */
uint8_t BSP_RS485_DeInit(void);
uint8_t BSP_RS485_Init(uint32_t baudrate);

void    BSP_RS485_ChangeMode(eRS485Mode_TypeDef emode);

void BSP_RS485_ReadData(uint8_t *pData, uint32_t *uwDataSize);
void BSP_RS485_WriteData(uint8_t *pData, uint32_t uwDataSize);

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif /* __STF7SoM_RS485_H */

/************************ (C) COPYRIGHT YiLDiZ IoT END OF FILE ************************/
