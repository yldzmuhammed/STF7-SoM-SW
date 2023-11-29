/**
  ******************************************************************************
  * @file    stf7som_nor.h
  * @author  yldzmuhammed
  * @version V1.0
  * @date    13-06-2020
  * @brief   This file contains the common defines and functions prototypes for
  *          the stf7som_nor.c driver.
  ******************************************************************************
  * @attention
  *
  * experiment platform: YiLDiZ IoT STF7-SoM Board BSP Library
  * web-site: https://www.yildiziot.com
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STF7SoM_NOR_H
#define __STF7SoM_NOR_H

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
    
/** @addtogroup STF7SoM_NOR
  * @{
  */    

/** @defgroup STF7SoM_NOR_Exported_Types NOR Exported Types
  * @{
  */
/**
  * @}
  */ 
    
/** 
  * @brief  NOR status structure definition  
  */     
#define   NOR_STATUS_OK                                 ((uint8_t)0x00)
#define   NOR_STATUS_ERROR                              ((uint8_t)0x01)

/** @defgroup STF7SoM_NOR_Exported_Constants NOR Exported Constants
  * @{
  */
#define NOR_DEVICE_ADDR                                 ((uint32_t)0x60000000)  
  
//#define NOR_MEMORY_WIDTH                                FMC_NORSRAM_MEM_BUS_WIDTH_8
#define NOR_MEMORY_WIDTH                                FMC_NORSRAM_MEM_BUS_WIDTH_16

#define NOR_BURSTACCESS                                 FMC_BURST_ACCESS_MODE_DISABLE  
//#define NOR_BURSTACCESS                                 FMC_BURST_ACCESS_MODE_ENABLE
  
#define NOR_WRITEBURST                                  FMC_WRITE_BURST_DISABLE  
//#define NOR_WRITEBURST                                  FMC_WRITE_BURST_ENABLE
 
#define CONTINUOUSCLOCK_FEATURE                         FMC_CONTINUOUS_CLOCK_SYNC_ONLY 
//#define CONTINUOUSCLOCK_FEATURE                         FMC_CONTINUOUS_CLOCK_SYNC_ASYNC

/* NOR operations Timeout definitions */
#define BLOCKERASE_TIMEOUT                              ((uint32_t)0x00A00000)  /* NOR block erase timeout */
#define CHIPERASE_TIMEOUT                               ((uint32_t)0x30000000)  /* NOR chip erase timeout  */ 
#define PROGRAM_TIMEOUT                                 ((uint32_t)0x00004400)  /* NOR program timeout     */ 

/* NOR Ready/Busy signal GPIO definitions */
#define NOR_READY_STATE                                 GPIO_PIN_SET
#define NOR_BUSY_STATE                                  GPIO_PIN_RESET 
#define NOR_READY_BUSY_PIN                              GPIO_PIN_6 
#define NOR_READY_BUSY_GPIO                             GPIOD

/* Paralel NOR Flash Device Information */
#define NOR_MANUFACTURER_ID                             (0x0089)
#define NOR_DEVICE_CODE_1                               (0x227E)
#define NOR_DEVICE_CODE_2                               (0x2221)
#define NOR_DEVICE_CODE_3                               (0x2201)

/**
  * @}
  */ 
    
/** @defgroup STF7SoM_NOR_Exported_Macro NOR Exported Macro
  * @{
  */ 
/**
  * @}
  */ 
    
/** @defgroup STF7SoM_NOR_Exported_Functions NOR Exported Functions
  * @{
  */
void    BSP_NOR_ReturnToReadMode(void);

/* These functions can be modified in case the current settings (e.g. DMA stream)
   need to be changed for specific application needs */
void    BSP_NOR_MspInit(NOR_HandleTypeDef  *hnor, void *Params);
void    BSP_NOR_MspDeInit(NOR_HandleTypeDef  *hnor, void *Params);

uint8_t BSP_NOR_Init(void);
uint8_t BSP_NOR_DeInit(void);
uint8_t BSP_NOR_Erase_Chip(void);

uint8_t BSP_NOR_Read_ID(NOR_IDTypeDef *pNOR_ID);
uint8_t BSP_NOR_Erase_Block(uint32_t BlockAddress);
uint8_t BSP_NOR_ReadData(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize);
uint8_t BSP_NOR_WriteData(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize);
uint8_t BSP_NOR_ReadBuffer(uint32_t uwStartAddress, uint16_t* pData, uint32_t uwDataSize);
uint8_t BSP_NOR_WriteBuffer(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize);

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

#endif /* __STF7SoM_NOR_H */

/************************ (C) COPYRIGHT YiLDiZ IoT END OF FILE ************************/
