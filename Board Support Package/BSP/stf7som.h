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

#ifndef __STF7SoM_H
#define __STF7SoM_H
#ifdef __cplusplus
	extern "C"{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "stm32f7xx_hal.h"

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* Public user function ------------------------------------------------------*/

/* Public define -------------------------------------------------------------*/
#define VREF_CONSTANT                   (float)(3.300f)
    
/* GPIO Pin ------------------------------------------------------------------*/
#define USER_BTTN_Pin                   GPIO_PIN_0
#define VBAT_MEASURE_Pin                GPIO_PIN_3
#define CHARGE_STATUS_Pin               GPIO_PIN_11

#define WiFi_IRQ_Pin                    GPIO_PIN_9
#define WiFi_PWE_Pin                    GPIO_PIN_15

#define WiFi_SCK_Pin                    GPIO_PIN_3
#define WiFi_MOSI_Pin                   GPIO_PIN_3
#define WiFi_MISO_Pin                   GPIO_PIN_2

#define WiFi_TX_Pin                     GPIO_PIN_9
#define WiFi_RX_Pin                     GPIO_PIN_10
#define WiFi_CTS_Pin                    GPIO_PIN_11
#define WiFi_RTS_Pin                    GPIO_PIN_12

#define RS485_DE_Pin                    GPIO_PIN_1
#define RS485_RX_Pin                    GPIO_PIN_11
#define RS485_TX_Pin                    GPIO_PIN_10

#define GSM_RI_Pin                      GPIO_PIN_9
#define GSM_RST_Pin                     GPIO_PIN_6
#define GSM_PWE_Pin                     GPIO_PIN_11

#define GSM_RX_Pin                      GPIO_PIN_7
#define GSM_DCD_Pin                     GPIO_PIN_7
#define GSM_DTR_Pin                     GPIO_PIN_0
#define GSM_CTS_Pin                     GPIO_PIN_13
#define GSM_RTS_Pin                     GPIO_PIN_12
#define GSM_TX_Pin                      GPIO_PIN_14


#define DCMI_SDA_Pin                    GPIO_PIN_7
#define DCMI_SCL_Pin                    GPIO_PIN_8

#define EXP_SCL_Pin                     GPIO_PIN_8
#define EXP_SDA_Pin                     GPIO_PIN_8

#define LTDC_SDA_Pin                    GPIO_PIN_5
#define LTDC_SCL_Pin                    GPIO_PIN_4

#define DCMI_SCK_Pin                    GPIO_PIN_5
#define DCMI_MOSI_Pin                   GPIO_PIN_5
#define DCMI_MISO_Pin                   GPIO_PIN_4


/* GPIO Port -----------------------------------------------------------------*/
#define USER_BTTN_GPIO_Port             GPIOA
#define VBAT_MEASURE_GPIO_Port          GPIOA
#define CHARGE_STATUS_GPIO_Port         GPIOI

#define WiFi_IRQ_GPIO_Port              GPIOG
#define WiFi_PWE_GPIO_Port              GPIOA

#define WiFi_SCK_GPIO_Port              GPIOD
#define WiFi_MISO_GPIO_Port             GPIOI
#define WiFi_MOSI_GPIO_Port             GPIOI

#define WiFi_RX_GPIO_Port               GPIOA
#define WiFi_TX_GPIO_Port               GPIOA
#define WiFi_CTS_GPIO_Port              GPIOA
#define WiFi_RTS_GPIO_Port              GPIOA

#define RS485_DE_GPIO_Port              GPIOB
#define RS485_RX_GPIO_Port              GPIOB
#define RS485_TX_GPIO_Port              GPIOB

#define GSM_RI_GPIO_Port                GPIOJ
#define GSM_PWE_GPIO_Port               GPIOH
#define GSM_RST_GPIO_Port               GPIOJ

#define GSM_TX_GPIO_Port                GPIOG
#define GSM_RX_GPIO_Port                GPIOC
#define GSM_CTS_GPIO_Port               GPIOG
#define GSM_DCD_GPIO_Port               GPIOG
#define GSM_DTR_GPIO_Port               GPIOK
#define GSM_RTS_GPIO_Port               GPIOG

#define EXP_SCL_GPIO_Port               GPIOA
#define EXP_SDA_GPIO_Port               GPIOH

#define LTDC_SCL_GPIO_Port              GPIOH
#define LTDC_SDA_GPIO_Port              GPIOH

#define DCMI_RST_GPIO_Port              GPIOI
#define DCMI_PWRE_GPIO_Port             GPIOI
#define DCMI_LIGHT_GPIO_Port            GPIOB

#define DCMI_SDA_GPIO_Port              GPIOB
#define DCMI_SCL_GPIO_Port              GPIOB

#define DCMI_SCK_GPIO_Port              GPIOA
#define DCMI_MOSI_GPIO_Port             GPIOB
#define DCMI_MISO_GPIO_Port             GPIOB
/* Public macro --------------------------------------------------------------*/
#define BSP_PB_GetStatus()              HAL_GPIO_ReadPin(USER_BTTN_GPIO_Port, USER_BTTN_Pin)


/**
  * @brief BSP Delay Function
  * @param delay Time for delay in millisecond
  * @retval None
  */
void BSP_Delay(uint32_t delay);

/**
  * @brief Return with user button state
  * @param None
  * @retval true When button pushed
	* @retval false When button released
  */
bool BSP_PB_Get(void);

/**
  * @brief Initialize User Push Button
  * @param None
  * @retval None
  */
void BSP_PB_Init(void);

/**
  * @brief Enable GPIO Clock
  * @param None
  * @retval None
  */
void BSP_GPIOCLK_Enable(void);

/**
  * @brief Init battery check pins
  * @param None
  * @retval None
  */
void BSP_BattCheck_Init(void);

/**
  * @brief  Switch in system clock.
  * @retval None
  */
void SwitchSystemClock(void);

/**
  * @brief  Measure ADC Channel and return the value with float
  * @retval Battery voltage value
  */
float BSP_Get_BatteryVoltageF(void);

/**
  * @brief  Measure ADC Channel and return the value with uint16
  * @retval Battery voltage value
  */
uint16_t BSP_Get_BatteryVoltageUI16(void);

/**
  * @brief  Initializes Camera low level.
  */
void CAMERA_IO_Init(void);

/**
  * @brief  Camera writes single data.
  * @param  Addr: I2C address
  * @param  Reg: Register address 
  * @param  Value: Data to be written
  */
void CAMERA_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);

/**
  * @brief  Camera reads single data.
  * @param  Addr: I2C address
  * @param  Reg: Register address 
  * @retval Read data
  */
uint8_t CAMERA_IO_Read(uint8_t Addr, uint8_t Reg);

/**
  * @brief  Camera delay 
  * @param  Delay: Delay in ms
  */
void CAMERA_Delay(uint32_t Delay);



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
