/**
  ******************************************************************************
  * @file    stf7som_sd.h
  * @author  yldzmuhammed
  * @version V1.0
  * @date    13-06-2020
  * @brief   This file contains the common defines and functions prototypes for
  *          the stf7som_sd.c driver.
  ******************************************************************************
  * @attention
  *
  * experiment platform: YiLDiZ IoT STF7-SoM Board BSP Library
  * web-site: https://www.yildiziot.com
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STF7SOM_SD_H
#define __STF7SOM_SD_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stf7som.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STF7SOM
  * @{
  */
    
/** @addtogroup STF7SOM_SD
  * @{
  */    

/** @defgroup STF7SOM_SD_Exported_Types STF7SOM_SD Exported Types
  * @{
  */

/** 
  * @brief SD Card information structure 
  */
#define BSP_SD_CardInfo HAL_SD_CardInfoTypeDef
/**
  * @}
  */
   
/** 
  * @brief  SD status structure definition  
  */     
#define MSD_OK                              ((uint8_t)0x00)
#define MSD_ERROR                           ((uint8_t)0x01)
#define MSD_ERROR_SD_NOT_PRESENT            ((uint8_t)0x02)

/** 
  * @brief  SD transfer state definition  
  */     
#define SD_TRANSFER_OK                      ((uint8_t)0x00)
#define SD_TRANSFER_BUSY                    ((uint8_t)0x01)
   
/** @defgroup STF7SOM_SD_Exported_Constants STF7SOM_SD Exported Constants
  * @{
  */ 
#define SD_PRESENT               		        ((uint8_t)0x01)
#define SD_NOT_PRESENT           		        ((uint8_t)0x00)

#define SD_DATATIMEOUT           		        ((uint32_t)100000000)

#define SD_D0_GPIO_Port                     GPIOC
#define SD_D1_GPIO_Port                     GPIOC
#define SD_D2_GPIO_Port                     GPIOC
#define SD_D3_GPIO_Port                     GPIOC
#define SD_CK_GPIO_Port                     GPIOC
#define SD_CMD_GPIO_Port                    GPIOD
#define SD_DETECT_GPIO_PORT                 GPIOH

#define SD_CK_Pin                           GPIO_PIN_12
#define SD_D0_Pin                           GPIO_PIN_8
#define SD_D1_Pin                           GPIO_PIN_9
#define SD_D2_Pin                           GPIO_PIN_10
#define SD_D3_Pin                           GPIO_PIN_11
#define SD_CMD_Pin                          GPIO_PIN_2
#define SD_DETECT_PIN                       GPIO_PIN_7

#define SD_DETECT_EXTI_IRQn				          EXTI9_5_IRQn

#define BSP_SDMMC_IRQHandler			          SDMMC1_IRQHandler   
#define SD_DetectIRQHandler()			          HAL_GPIO_EXTI_IRQHandler(SD_DETECT_PIN)

/* DMA definitions for SD DMA transfer */
#define SD_DMAx_Tx_STREAM         		      DMA2_Stream6
#define SD_DMAx_Rx_STREAM         		      DMA2_Stream3

#define SD_DMAx_Tx_CHANNEL        		      DMA_CHANNEL_4
#define SD_DMAx_Rx_CHANNEL        		      DMA_CHANNEL_4

#define SD_DMAx_Tx_IRQn           		      DMA2_Stream6_IRQn
#define SD_DMAx_Rx_IRQn           		      DMA2_Stream3_IRQn

#define BSP_SDMMC_DMA_Tx_IRQHandler 	      DMA2_Stream6_IRQHandler
#define BSP_SDMMC_DMA_Rx_IRQHandler 	      DMA2_Stream3_IRQHandler  
 
#define __DMAx_TxRx_CLK_ENABLE    		      __HAL_RCC_DMA2_CLK_ENABLE

/**
  * @}
  */
  
/** @defgroup STF7SOM_SD_Exported_Macro STF7SOM_SD Exported Macro
  * @{
  */ 
/**
  * @}
  */
   
/** @addtogroup STF7SOM_SD_Exported_Functions
  * @{
  */

uint8_t BSP_SD_Init(void);
uint8_t BSP_SD_DeInit(void);
uint8_t BSP_SD_ITConfig(void);
uint8_t BSP_SD_IsDetected(void);
uint8_t BSP_SD_GetCardState(void);

void    BSP_SD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo);

uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr);
uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks);
uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks);
uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout);
uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout);

/* These functions can be modified in case the current settings (e.g. DMA stream)
   need to be changed for specific application needs */
void    BSP_SD_AbortCallback(void);
void    BSP_SD_ReadCpltCallback(void);
void    BSP_SD_WriteCpltCallback(void);
void    BSP_SD_MspInit(SD_HandleTypeDef *hsd, void *Params);
void    BSP_SD_MspDeInit(SD_HandleTypeDef *hsd, void *Params);
void    BSP_SD_Detect_MspInit(SD_HandleTypeDef *hsd, void *Params);
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

#endif /* __STF7SOM_SD_H */

/************************ (C) COPYRIGHT YiLDiZ IoT END OF FILE ************************/
