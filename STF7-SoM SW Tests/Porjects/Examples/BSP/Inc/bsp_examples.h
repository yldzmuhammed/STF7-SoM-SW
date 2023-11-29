/**
  ******************************************************************************
  * @file    main.c
  * @author  yldzmuhammed
  * @version V1.0
  * @date    13-06-2020
  * @brief   This file contains the bsp example functions.
  ******************************************************************************
  * @attention
  *
  * experiment platform: YiLDiZ IoT STF7-SoM Board BSP Library
  * web-site: https://www.yildiziot.com
  ******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "stf7som.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SD Demo
  * @param  None
  * @retval None
  */
void SD_demo (void);

/**
  * @brief  LCD Log demo 
  * @param  None
  * @retval None
  */
void Log_demo(void);

/**
  * @brief  NOR Demo
  * @param  None
  * @retval None
  */
void NOR_demo (void);

/**
  * @brief  LCD demo
  * @param  None
  * @retval None
  */
void LCD_demo (void);

/**
  * @brief  SDRAM Demo
  * @param  None
  * @retval None
  */
void SDRAM_demo (void);

/**
  * @brief  Touchscreen Demo
  * @param  None
  * @retval None
  */
void Touchscreen_demo (void);

/**
  * @brief  Check for user input
  * @param  None
* @retval Input state (1 : active / 0 : Inactive)
  */
uint8_t CheckForUserInput(void);

/************************ (C) COPYRIGHT YiLDiZ IoT END OF FILE ************************/
