/**
  ******************************************************************************
  * @file    stf7som.h
  * @author  yldzmuhammed
  * @version V1.0
  * @date    13-06-2020
  * @brief   This file contains the common defines and functions prototypes for
  *          the stf7som.c driver.
  ******************************************************************************
  * @attention
  *
  * experiment platform: YiLDiZ IoT STF7-SoM Board BSP Library
  * web-site: https://www.yildiziot.com
  ******************************************************************************
*/

#ifndef __STF7SoM_FMC_H
#define __STF7SoM_FMC_H
#ifdef __cplusplus
	extern "C"{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
/** @addtogroup BSP
  * @{
  */

/** @addtogroup STF7SoM
  * @{
  */
    
/** @addtogroup STF7SoM_FMC
  * @{
  */    

/** @defgroup STF7SoM_FMC_Exported_Types FMC Exported Types
  * @{
  */
/**
  * @}
  */ 
    
/** 
  * @brief  FMC Pins  
  */
/** @defgroup STF7SoM_FMC_Exported_Constants FMC Pins
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* Public user function ------------------------------------------------------*/
/* Public macro --------------------------------------------------------------*/
/* Public define -------------------------------------------------------------*/
/* GPIO Port */
#define FMC_NBL0_GPIO_Port    GPIOE
#define FMC_NBL1_GPIO_Port    GPIOE

#define FMC_SDNWE_GPIO_Port   GPIOC
#define FMC_SDCLK_GPIO_Port   GPIOG
#define FMC_SDNE0_GPIO_Port   GPIOH
#define FMC_SDCKE0_GPIO_Port  GPIOH
#define FMC_SDNCAS_GPIO_Port  GPIOG
#define FMC_SDNRAS_GPIO_Port  GPIOF

#define FMC_NE_GPIO_Port      GPIOD
#define FMC_NOE_GPIO_Port     GPIOD
#define FMC_NWE_GPIO_Port     GPIOD
#define FMC_NWAIT_GPIO_Port   GPIOD

#define FMC_D0_GPIO_Port      GPIOD
#define FMC_D1_GPIO_Port      GPIOD
#define FMC_D2_GPIO_Port      GPIOD
#define FMC_D3_GPIO_Port      GPIOD
#define FMC_D4_GPIO_Port      GPIOE
#define FMC_D5_GPIO_Port      GPIOE
#define FMC_D6_GPIO_Port      GPIOE
#define FMC_D7_GPIO_Port      GPIOE
#define FMC_D8_GPIO_Port      GPIOE
#define FMC_D9_GPIO_Port      GPIOE
#define FMC_D10_GPIO_Port     GPIOE
#define FMC_D11_GPIO_Port     GPIOE
#define FMC_D12_GPIO_Port     GPIOE
#define FMC_D13_GPIO_Port     GPIOD
#define FMC_D14_GPIO_Port     GPIOD
#define FMC_D15_GPIO_Port     GPIOD

#define FMC_A0_GPIO_Port      GPIOF
#define FMC_A1_GPIO_Port      GPIOF
#define FMC_A2_GPIO_Port      GPIOF
#define FMC_A3_GPIO_Port      GPIOF
#define FMC_A4_GPIO_Port      GPIOF
#define FMC_A5_GPIO_Port      GPIOF
#define FMC_A6_GPIO_Port      GPIOF
#define FMC_A7_GPIO_Port      GPIOF
#define FMC_A8_GPIO_Port      GPIOF
#define FMC_A9_GPIO_Port      GPIOF
#define FMC_A10_GPIO_Port     GPIOG
#define FMC_A11_GPIO_Port     GPIOG
#define FMC_A12_GPIO_Port     GPIOG
#define FMC_A13_GPIO_Port     GPIOG
#define FMC_A14_GPIO_Port     GPIOG
#define FMC_A15_GPIO_Port     GPIOG
#define FMC_A16_GPIO_Port     GPIOD
#define FMC_A17_GPIO_Port     GPIOD
#define FMC_A18_GPIO_Port     GPIOD
#define FMC_A19_GPIO_Port     GPIOE
#define FMC_A20_GPIO_Port     GPIOE
#define FMC_A21_GPIO_Port     GPIOE

/* GPIO Pin */
#define FMC_NBL0_Pin          GPIO_PIN_0
#define FMC_NBL1_Pin          GPIO_PIN_1

#define FMC_SDNWE_Pin         GPIO_PIN_0
#define FMC_SDCLK_Pin         GPIO_PIN_8
#define FMC_SDNE0_Pin         GPIO_PIN_3
#define FMC_SDCKE0_Pin        GPIO_PIN_2
#define FMC_SDNRAS_Pin        GPIO_PIN_11
#define FMC_SDNCAS_Pin        GPIO_PIN_15

#define FMC_NE_Pin            GPIO_PIN_7
#define FMC_NOE_Pin           GPIO_PIN_4
#define FMC_NWE_Pin           GPIO_PIN_5
#define FMC_NWAIT_Pin         GPIO_PIN_6

#define FMC_D0_Pin            GPIO_PIN_14
#define FMC_D1_Pin            GPIO_PIN_15
#define FMC_D2_Pin            GPIO_PIN_0
#define FMC_D3_Pin            GPIO_PIN_1
#define FMC_D4_Pin            GPIO_PIN_7
#define FMC_D5_Pin            GPIO_PIN_8
#define FMC_D6_Pin            GPIO_PIN_9
#define FMC_D7_Pin            GPIO_PIN_10
#define FMC_D8_Pin            GPIO_PIN_11
#define FMC_D9_Pin            GPIO_PIN_12
#define FMC_D10_Pin           GPIO_PIN_13
#define FMC_D11_Pin           GPIO_PIN_14
#define FMC_D12_Pin           GPIO_PIN_15
#define FMC_D13_Pin           GPIO_PIN_8
#define FMC_D14_Pin           GPIO_PIN_9
#define FMC_D15_Pin           GPIO_PIN_10

#define FMC_A0_Pin            GPIO_PIN_0
#define FMC_A1_Pin            GPIO_PIN_1
#define FMC_A2_Pin            GPIO_PIN_2
#define FMC_A3_Pin            GPIO_PIN_3
#define FMC_A4_Pin            GPIO_PIN_4
#define FMC_A5_Pin            GPIO_PIN_5
#define FMC_A6_Pin            GPIO_PIN_12
#define FMC_A7_Pin            GPIO_PIN_13
#define FMC_A8_Pin            GPIO_PIN_14
#define FMC_A9_Pin            GPIO_PIN_15
#define FMC_A10_Pin           GPIO_PIN_0
#define FMC_A11_Pin           GPIO_PIN_1
#define FMC_A12_Pin           GPIO_PIN_2
#define FMC_A13_Pin           GPIO_PIN_3
#define FMC_A14_Pin           GPIO_PIN_4
#define FMC_A15_Pin           GPIO_PIN_5
#define FMC_A16_Pin           GPIO_PIN_11
#define FMC_A17_Pin           GPIO_PIN_12
#define FMC_A18_Pin           GPIO_PIN_13
#define FMC_A19_Pin           GPIO_PIN_3
#define FMC_A20_Pin           GPIO_PIN_4
#define FMC_A21_Pin           GPIO_PIN_5

void BSP_FMC_Init_CommonPins(void);
void BSP_FMC_DeInit_CommonPins(void);

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
#endif /* STF7-SoM */

/************************ (C) COPYRIGHT YiLDiZ IoT END OF FILE ************************/
