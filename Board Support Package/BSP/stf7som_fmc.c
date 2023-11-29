/**
  ******************************************************************************
  * @file    stf7som.h
  * @author  yldzmuhammed
  * @version V1.0
  * @date    13-06-2020
  * @brief   This file contains the common defines and functions prototypes for
  *          the stf7som_fmc.c driver.
  ******************************************************************************
  * @attention
  *
  * experiment platform: YiLDiZ IoT STF7-SoM Board BSP Library
  * web-site: https://www.yildiziot.com
  ******************************************************************************
*/


/************************ (C) COPYRIGHT YiLDiZ IoT END OF FILE ************************/

#include "stf7som_fmc.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32756G_EVAL
  * @{
  */ 
  
/** @defgroup STM32756G_EVAL_NOR STM32756G_EVAL NOR
  * @{
  */ 
  
/* Private typedef -----------------------------------------------------------*/

/** @defgroup STM32756G_EVAL_NOR_Private_Types_Definitions NOR Private Types Definitions
  * @{
  */ 
/**
  * @}
  */  
/* Private define ------------------------------------------------------------*/

/** @defgroup STM32756G_EVAL_NOR_Private_Defines NOR Private Defines
  * @{
  */

/**
  * @}
  */  
/* Private macro -------------------------------------------------------------*/

/** @defgroup STM32756G_EVAL_NOR_Private_Macros NOR Private Macros
  * @{
  */  
/**
  * @}
  */  
/* Private variables ---------------------------------------------------------*/

/** @defgroup STM32756G_EVAL_NOR_Private_Variables NOR Private Variables
  * @{
  */

/**
  * @}
  */ 

/* Private function prototypes -----------------------------------------------*/

/** @defgroup STM32756G_EVAL_NOR_Private_Functions_Prototypes NOR Private Functions Prototypes
  * @{
  */ 
/**
  * @}
  */ 
/* Private functions ---------------------------------------------------------*/
    
/** @defgroup STM32756G_EVAL_NOR_Private_Functions NOR Private Functions
  * @{
  */

/**
  * @brief  Initializes the FMC Common Data & Address Pins.
  * @param  None  
  * @retval None
  */
void BSP_FMC_Init_CommonPins(void)
{
  GPIO_InitTypeDef gpio_init_structure;
  
  /* Common GPIO configuration */
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  
  /* GPIOD configuration */
  gpio_init_structure.Pin       = FMC_D0_Pin  | FMC_D1_Pin  | FMC_D2_Pin | FMC_D3_Pin |
                                  FMC_D13_Pin | FMC_D14_Pin | FMC_D15_Pin;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* GPIOE configuration */  
  gpio_init_structure.Pin       = FMC_D4_Pin | FMC_D5_Pin  | FMC_D6_Pin  | FMC_D7_Pin  | FMC_D8_Pin |
                                  FMC_D9_Pin | FMC_D10_Pin | FMC_D11_Pin | FMC_D12_Pin;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
  
  /* GPIOF configuration */  
  gpio_init_structure.Pin       = FMC_A0_Pin | FMC_A1_Pin | FMC_A2_Pin | FMC_A3_Pin | FMC_A4_Pin |
                                  FMC_A5_Pin | FMC_A6_Pin | FMC_A7_Pin | FMC_A8_Pin | FMC_A9_Pin;
  HAL_GPIO_Init(GPIOF, &gpio_init_structure);
  
  /* GPIOG configuration */  
  gpio_init_structure.Pin       = FMC_A10_Pin | FMC_A11_Pin | FMC_A12_Pin | FMC_A14_Pin | FMC_A15_Pin;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure); 
}
//

/**
  * @brief  DeInitializes the FMC Common Data & Address Pins.
  * @param  None  
  * @retval None
  */
void BSP_FMC_DeInit_CommonPins(void)
{
  HAL_GPIO_DeInit(GPIOD,  FMC_D0_Pin  | FMC_D1_Pin | FMC_D2_Pin | FMC_D3_Pin  |
                          FMC_D13_Pin | FMC_D14_Pin| FMC_D15_Pin);

  HAL_GPIO_DeInit(GPIOE,  FMC_D4_Pin  | FMC_D5_Pin | FMC_D6_Pin | FMC_D7_Pin  | 
                          FMC_D8_Pin  | FMC_D9_Pin | FMC_D10_Pin| FMC_D11_Pin | 
                          FMC_D12_Pin);

  HAL_GPIO_DeInit(GPIOG,  FMC_A11_Pin | FMC_A12_Pin| FMC_A10_Pin | FMC_A15_Pin|
                          FMC_A14_Pin);

  HAL_GPIO_DeInit(GPIOF,  FMC_A0_Pin  | FMC_A1_Pin | FMC_A2_Pin  | FMC_A3_Pin |
                          FMC_A4_Pin  | FMC_A5_Pin | FMC_A6_Pin  | FMC_A7_Pin |
                          FMC_A8_Pin  | FMC_A9_Pin);
}
//



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
