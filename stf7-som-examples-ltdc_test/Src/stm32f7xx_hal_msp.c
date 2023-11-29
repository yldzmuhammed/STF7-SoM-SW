/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : stm32f7xx_hal_msp.c
  * Description        : This file provides code for the MSP Initialization 
  *                      and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */
 
/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  /* System interrupt init*/

  /* Peripheral interrupt init */
  /* RCC_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(RCC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(RCC_IRQn);

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}
#if 0
/* USER CODE END 0 */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                    /**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  /* System interrupt init*/

  /* Peripheral interrupt init */
  /* RCC_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(RCC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(RCC_IRQn);

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/**
* @brief ADC MSP Initialization
* This function configures the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hadc->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA3     ------> ADC1_IN3
    */
    GPIO_InitStruct.Pin = VBATT_SENSE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(VBATT_SENSE_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }

}

/**
* @brief ADC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA3     ------> ADC1_IN3
    */
    HAL_GPIO_DeInit(VBATT_SENSE_GPIO_Port, VBATT_SENSE_Pin);

  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }

}

/**
* @brief DMA2D MSP Initialization
* This function configures the hardware resources used in this example
* @param hdma2d: DMA2D handle pointer
* @retval None
*/
void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef* hdma2d)
{
  if(hdma2d->Instance==DMA2D)
  {
  /* USER CODE BEGIN DMA2D_MspInit 0 */

  /* USER CODE END DMA2D_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_DMA2D_CLK_ENABLE();
  /* USER CODE BEGIN DMA2D_MspInit 1 */

  /* USER CODE END DMA2D_MspInit 1 */
  }

}

/**
* @brief DMA2D MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hdma2d: DMA2D handle pointer
* @retval None
*/
void HAL_DMA2D_MspDeInit(DMA2D_HandleTypeDef* hdma2d)
{
  if(hdma2d->Instance==DMA2D)
  {
  /* USER CODE BEGIN DMA2D_MspDeInit 0 */

  /* USER CODE END DMA2D_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_DMA2D_CLK_DISABLE();
  /* USER CODE BEGIN DMA2D_MspDeInit 1 */

  /* USER CODE END DMA2D_MspDeInit 1 */
  }

}

/**
* @brief LTDC MSP Initialization
* This function configures the hardware resources used in this example
* @param hltdc: LTDC handle pointer
* @retval None
*/
void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hltdc->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspInit 0 */

  /* USER CODE END LTDC_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_LTDC_CLK_ENABLE();

    __HAL_RCC_GPIOJ_CLK_ENABLE();
    __HAL_RCC_GPIOK_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    /**LTDC GPIO Configuration
    PJ13     ------> LTDC_B1
    PJ12     ------> LTDC_B0
    PK7     ------> LTDC_DE
    PK6     ------> LTDC_B7
    PK5     ------> LTDC_B6
    PJ14     ------> LTDC_B2
    PI10     ------> LTDC_HSYNC
    PK4     ------> LTDC_B5
    PK3     ------> LTDC_B4
    PJ15     ------> LTDC_B3
    PH15     ------> LTDC_G4
    PI1     ------> LTDC_G6
    PH13     ------> LTDC_G2
    PI0     ------> LTDC_G5
    PK2     ------> LTDC_G7
    PI13     ------> LTDC_VSYNC
    PI15     ------> LTDC_R0
    PI14     ------> LTDC_CLK
    PJ8     ------> LTDC_G1
    PJ10     ------> LTDC_G3
    PJ7     ------> LTDC_G0
    PG6     ------> LTDC_R7
    PJ4     ------> LTDC_R5
    PJ5     ------> LTDC_R6
    PJ3     ------> LTDC_R4
    PH9     ------> LTDC_R3
    PJ0     ------> LTDC_R1
    PJ1     ------> LTDC_R2
    */
    GPIO_InitStruct.Pin = LTDC_B1_Pin|LTDC_B0_Pin|LTDC_B2_Pin|LTDC_B3_Pin
                          |LTDC_G1_Pin|LTDC_G3_Pin|LTDC_G0_Pin|LTDC_R5_Pin
                          |LTDC_R6_Pin|LTDC_R4_Pin|LTDC_R1_Pin|LTDC_R2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LTDC_DE_Pin|LTDC_B7_Pin|LTDC_B6_Pin|LTDC_B5_Pin
                          |LTDC_B4_Pin|LTDC_G7_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LTDC_HSYNC_Pin|LTDC_G6_Pin|LTDC_G5_Pin|LTDC_VSYNC_Pin
                          |LTDC_R0_Pin|LTDC_CLK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LTDC_G4_Pin|LTDC_G2_Pin|LTDC_R3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LTDC_R7_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(LTDC_R7_GPIO_Port, &GPIO_InitStruct);

    /* LTDC interrupt Init */
    HAL_NVIC_SetPriority(LTDC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(LTDC_IRQn);
    HAL_NVIC_SetPriority(LTDC_ER_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(LTDC_ER_IRQn);
  /* USER CODE BEGIN LTDC_MspInit 1 */

  /* USER CODE END LTDC_MspInit 1 */
  }

}

/**
* @brief LTDC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hltdc: LTDC handle pointer
* @retval None
*/
void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef* hltdc)
{
  if(hltdc->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspDeInit 0 */

  /* USER CODE END LTDC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LTDC_CLK_DISABLE();

    /**LTDC GPIO Configuration
    PJ13     ------> LTDC_B1
    PJ12     ------> LTDC_B0
    PK7     ------> LTDC_DE
    PK6     ------> LTDC_B7
    PK5     ------> LTDC_B6
    PJ14     ------> LTDC_B2
    PI10     ------> LTDC_HSYNC
    PK4     ------> LTDC_B5
    PK3     ------> LTDC_B4
    PJ15     ------> LTDC_B3
    PH15     ------> LTDC_G4
    PI1     ------> LTDC_G6
    PH13     ------> LTDC_G2
    PI0     ------> LTDC_G5
    PK2     ------> LTDC_G7
    PI13     ------> LTDC_VSYNC
    PI15     ------> LTDC_R0
    PI14     ------> LTDC_CLK
    PJ8     ------> LTDC_G1
    PJ10     ------> LTDC_G3
    PJ7     ------> LTDC_G0
    PG6     ------> LTDC_R7
    PJ4     ------> LTDC_R5
    PJ5     ------> LTDC_R6
    PJ3     ------> LTDC_R4
    PH9     ------> LTDC_R3
    PJ0     ------> LTDC_R1
    PJ1     ------> LTDC_R2
    */
    HAL_GPIO_DeInit(GPIOJ, LTDC_B1_Pin|LTDC_B0_Pin|LTDC_B2_Pin|LTDC_B3_Pin
                          |LTDC_G1_Pin|LTDC_G3_Pin|LTDC_G0_Pin|LTDC_R5_Pin
                          |LTDC_R6_Pin|LTDC_R4_Pin|LTDC_R1_Pin|LTDC_R2_Pin);

    HAL_GPIO_DeInit(GPIOK, LTDC_DE_Pin|LTDC_B7_Pin|LTDC_B6_Pin|LTDC_B5_Pin
                          |LTDC_B4_Pin|LTDC_G7_Pin);

    HAL_GPIO_DeInit(GPIOI, LTDC_HSYNC_Pin|LTDC_G6_Pin|LTDC_G5_Pin|LTDC_VSYNC_Pin
                          |LTDC_R0_Pin|LTDC_CLK_Pin);

    HAL_GPIO_DeInit(GPIOH, LTDC_G4_Pin|LTDC_G2_Pin|LTDC_R3_Pin);

    HAL_GPIO_DeInit(LTDC_R7_GPIO_Port, LTDC_R7_Pin);

    /* LTDC interrupt DeInit */
    HAL_NVIC_DisableIRQ(LTDC_IRQn);
    HAL_NVIC_DisableIRQ(LTDC_ER_IRQn);
  /* USER CODE BEGIN LTDC_MspDeInit 1 */

  /* USER CODE END LTDC_MspDeInit 1 */
  }

}

/**
* @brief TIM_Base MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM11)
  {
  /* USER CODE BEGIN TIM11_MspInit 0 */

  /* USER CODE END TIM11_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM11_CLK_ENABLE();
  /* USER CODE BEGIN TIM11_MspInit 1 */

  /* USER CODE END TIM11_MspInit 1 */
  }

}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(htim->Instance==TIM11)
  {
  /* USER CODE BEGIN TIM11_MspPostInit 0 */

  /* USER CODE END TIM11_MspPostInit 0 */

    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**TIM11 GPIO Configuration
    PF7     ------> TIM11_CH1
    */
    GPIO_InitStruct.Pin = LTDC_BL_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM11;
    HAL_GPIO_Init(LTDC_BL_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM11_MspPostInit 1 */

  /* USER CODE END TIM11_MspPostInit 1 */
  }

}
/**
* @brief TIM_Base MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM11)
  {
  /* USER CODE BEGIN TIM11_MspDeInit 0 */

  /* USER CODE END TIM11_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM11_CLK_DISABLE();
  /* USER CODE BEGIN TIM11_MspDeInit 1 */

  /* USER CODE END TIM11_MspDeInit 1 */
  }

}

static uint32_t FMC_Initialized = 0;

static void HAL_FMC_MspInit(void){
  /* USER CODE BEGIN FMC_MspInit 0 */

  /* USER CODE END FMC_MspInit 0 */
  GPIO_InitTypeDef GPIO_InitStruct ={0};
  if (FMC_Initialized) {
    return;
  }
  FMC_Initialized = 1;

  /* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_ENABLE();

  /** FMC GPIO Configuration
  PE4   ------> FMC_A20
  PE3   ------> FMC_A19
  PE1   ------> FMC_NBL1
  PE0   ------> FMC_NBL0
  PD7   ------> FMC_NE1
  PE5   ------> FMC_A21
  PG15   ------> FMC_SDNCAS
  PD6   ------> FMC_NWAIT
  PD0   ------> FMC_D2
  PD5   ------> FMC_NWE
  PD1   ------> FMC_D3
  PF0   ------> FMC_A0
  PD4   ------> FMC_NOE
  PF1   ------> FMC_A1
  PF2   ------> FMC_A2
  PF3   ------> FMC_A3
  PG8   ------> FMC_SDCLK
  PF4   ------> FMC_A4
  PH3   ------> FMC_SDNE0
  PF5   ------> FMC_A5
  PH2   ------> FMC_SDCKE0
  PD15   ------> FMC_D1
  PD10   ------> FMC_D15
  PD14   ------> FMC_D0
  PD9   ------> FMC_D14
  PD8   ------> FMC_D13
  PC0   ------> FMC_SDNWE
  PF12   ------> FMC_A6
  PG1   ------> FMC_A11
  PF15   ------> FMC_A9
  PD12   ------> FMC_A17
  PD13   ------> FMC_A18
  PG3   ------> FMC_A13
  PG2   ------> FMC_A12
  PF13   ------> FMC_A7
  PG0   ------> FMC_A10
  PE8   ------> FMC_D5
  PD11   ------> FMC_A16
  PG5   ------> FMC_BA1
  PG5   ------> FMC_A15
  PG4   ------> FMC_BA0
  PG4   ------> FMC_A14
  PF14   ------> FMC_A8
  PF11   ------> FMC_SDNRAS
  PE9   ------> FMC_D6
  PE11   ------> FMC_D8
  PE14   ------> FMC_D11
  PE7   ------> FMC_D4
  PE10   ------> FMC_D7
  PE12   ------> FMC_D9
  PE15   ------> FMC_D12
  PE13   ------> FMC_D10
  */
  GPIO_InitStruct.Pin = FMC_A20_Pin|FMC_A19_Pin|FMC_NBL1_Pin|FMC_NBL0_Pin
                          |FMC_A21_Pin|FMC_D5_Pin|FMC_D6_Pin|FMC_D8_Pin
                          |FMC_D11_Pin|FMC_D4_Pin|FMC_D7_Pin|FMC_D9_Pin
                          |FMC_D12_Pin|FMC_D10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = FMC_NE_Pin|FMC_NWAIT_Pin|FMC_D2_Pin|FMC_NWE_Pin
                          |FMC_D3_Pin|FMC_NOE_Pin|FMC_D1_Pin|FMC_D15_Pin
                          |FMC_D0_Pin|FMC_D14_Pin|FMC_D13_Pin|FMC_A17_Pin
                          |FMC_A18_Pin|FMC_A16_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = FMC_SDNCAS_Pin|FMC_SDCLK_Pin|FMC_A11_Pin|FMC_A13_Pin
                          |FMC_A12_Pin|FMC_A10_Pin|FMC_A15_Pin|FMC_A14_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = FMC_A0_Pin|FMC_A1_Pin|FMC_A2_Pin|FMC_A3_Pin
                          |FMC_A4_Pin|FMC_A5_Pin|FMC_A6_Pin|FMC_A9_Pin
                          |FMC_A7_Pin|FMC_A8_Pin|FMC_SDNRAS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = FMC_SDNE0_Pin|FMC_SDCKE0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = FMC_SDNWE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(FMC_SDNWE_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN FMC_MspInit 1 */

  /* USER CODE END FMC_MspInit 1 */
}

void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef* hsdram){
  /* USER CODE BEGIN SDRAM_MspInit 0 */

  /* USER CODE END SDRAM_MspInit 0 */
  HAL_FMC_MspInit();
  /* USER CODE BEGIN SDRAM_MspInit 1 */

  /* USER CODE END SDRAM_MspInit 1 */
}

static uint32_t FMC_DeInitialized = 0;

static void HAL_FMC_MspDeInit(void){
  /* USER CODE BEGIN FMC_MspDeInit 0 */

  /* USER CODE END FMC_MspDeInit 0 */
  if (FMC_DeInitialized) {
    return;
  }
  FMC_DeInitialized = 1;
  /* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_DISABLE();

  /** FMC GPIO Configuration
  PE4   ------> FMC_A20
  PE3   ------> FMC_A19
  PE1   ------> FMC_NBL1
  PE0   ------> FMC_NBL0
  PD7   ------> FMC_NE1
  PE5   ------> FMC_A21
  PG15   ------> FMC_SDNCAS
  PD6   ------> FMC_NWAIT
  PD0   ------> FMC_D2
  PD5   ------> FMC_NWE
  PD1   ------> FMC_D3
  PF0   ------> FMC_A0
  PD4   ------> FMC_NOE
  PF1   ------> FMC_A1
  PF2   ------> FMC_A2
  PF3   ------> FMC_A3
  PG8   ------> FMC_SDCLK
  PF4   ------> FMC_A4
  PH3   ------> FMC_SDNE0
  PF5   ------> FMC_A5
  PH2   ------> FMC_SDCKE0
  PD15   ------> FMC_D1
  PD10   ------> FMC_D15
  PD14   ------> FMC_D0
  PD9   ------> FMC_D14
  PD8   ------> FMC_D13
  PC0   ------> FMC_SDNWE
  PF12   ------> FMC_A6
  PG1   ------> FMC_A11
  PF15   ------> FMC_A9
  PD12   ------> FMC_A17
  PD13   ------> FMC_A18
  PG3   ------> FMC_A13
  PG2   ------> FMC_A12
  PF13   ------> FMC_A7
  PG0   ------> FMC_A10
  PE8   ------> FMC_D5
  PD11   ------> FMC_A16
  PG5   ------> FMC_BA1
  PG5   ------> FMC_A15
  PG4   ------> FMC_BA0
  PG4   ------> FMC_A14
  PF14   ------> FMC_A8
  PF11   ------> FMC_SDNRAS
  PE9   ------> FMC_D6
  PE11   ------> FMC_D8
  PE14   ------> FMC_D11
  PE7   ------> FMC_D4
  PE10   ------> FMC_D7
  PE12   ------> FMC_D9
  PE15   ------> FMC_D12
  PE13   ------> FMC_D10
  */
  HAL_GPIO_DeInit(GPIOE, FMC_A20_Pin|FMC_A19_Pin|FMC_NBL1_Pin|FMC_NBL0_Pin
                          |FMC_A21_Pin|FMC_D5_Pin|FMC_D6_Pin|FMC_D8_Pin
                          |FMC_D11_Pin|FMC_D4_Pin|FMC_D7_Pin|FMC_D9_Pin
                          |FMC_D12_Pin|FMC_D10_Pin);

  HAL_GPIO_DeInit(GPIOD, FMC_NE_Pin|FMC_NWAIT_Pin|FMC_D2_Pin|FMC_NWE_Pin
                          |FMC_D3_Pin|FMC_NOE_Pin|FMC_D1_Pin|FMC_D15_Pin
                          |FMC_D0_Pin|FMC_D14_Pin|FMC_D13_Pin|FMC_A17_Pin
                          |FMC_A18_Pin|FMC_A16_Pin);

  HAL_GPIO_DeInit(GPIOG, FMC_SDNCAS_Pin|FMC_SDCLK_Pin|FMC_A11_Pin|FMC_A13_Pin
                          |FMC_A12_Pin|FMC_A10_Pin|FMC_A15_Pin|FMC_A14_Pin);

  HAL_GPIO_DeInit(GPIOF, FMC_A0_Pin|FMC_A1_Pin|FMC_A2_Pin|FMC_A3_Pin
                          |FMC_A4_Pin|FMC_A5_Pin|FMC_A6_Pin|FMC_A9_Pin
                          |FMC_A7_Pin|FMC_A8_Pin|FMC_SDNRAS_Pin);

  HAL_GPIO_DeInit(GPIOH, FMC_SDNE0_Pin|FMC_SDCKE0_Pin);

  HAL_GPIO_DeInit(FMC_SDNWE_GPIO_Port, FMC_SDNWE_Pin);

  /* USER CODE BEGIN FMC_MspDeInit 1 */

  /* USER CODE END FMC_MspDeInit 1 */
}

void HAL_SDRAM_MspDeInit(SDRAM_HandleTypeDef* hsdram){
  /* USER CODE BEGIN SDRAM_MspDeInit 0 */

  /* USER CODE END SDRAM_MspDeInit 0 */
  HAL_FMC_MspDeInit();
  /* USER CODE BEGIN SDRAM_MspDeInit 1 */

  /* USER CODE END SDRAM_MspDeInit 1 */
}

/* USER CODE BEGIN 1 */
#endif
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
