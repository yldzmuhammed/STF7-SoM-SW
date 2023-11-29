/**
  ******************************************************************************
  * @file    BSP/Src/ts_calibration.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to calibrate the touchscreen.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stf7som_ts.h"
#include "stf7som_lcd.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Performs the TS calibration
  * @param  None
  * @retval None
  */
void Touchscreen_Calibration (void);

/**
  * @brief  Calibrate X position
  * @param  x : X position
  * @retval calibrated x
  */
uint16_t Calibration_GetX(uint16_t x);

/**
  * @brief  Calibrate Y position
  * @param  y : Y position
  * @retval calibrated y
  */
uint16_t Calibration_GetY(uint16_t y);

/**
  * @brief 	Check if the TS is calibrated
  * @param  None
  * @retval	calibration state (1 : calibrated / 0: no)
  */
uint8_t IsCalibrationDone(void);

/**
  * @}
  */ 
  
/**
  * @}
  */
    
/************************ (C) COPYRIGHT YiLDiZ IoT END OF FILE ************************/
