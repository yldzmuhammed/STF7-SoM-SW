/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GSM_TX_Pin GPIO_PIN_14
#define GSM_TX_GPIO_Port GPIOG
#define DCMI_SCL_Pin GPIO_PIN_8
#define DCMI_SCL_GPIO_Port GPIOB
#define DCMI_MOSI_Pin GPIO_PIN_5
#define DCMI_MOSI_GPIO_Port GPIOB
#define DCMI_MISO_Pin GPIO_PIN_4
#define DCMI_MISO_GPIO_Port GPIOB
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define WiFi_PWE_Pin GPIO_PIN_15
#define WiFi_PWE_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define DCMI_D7_Pin GPIO_PIN_6
#define DCMI_D7_GPIO_Port GPIOE
#define GSM_CTS_Pin GPIO_PIN_13
#define GSM_CTS_GPIO_Port GPIOG
#define CAN_TX_Pin GPIO_PIN_9
#define CAN_TX_GPIO_Port GPIOB
#define DCMI_SDA_Pin GPIO_PIN_7
#define DCMI_SDA_GPIO_Port GPIOB
#define QSP_NCS_Pin GPIO_PIN_6
#define QSP_NCS_GPIO_Port GPIOB
#define ETH_TX_EN_Pin GPIO_PIN_11
#define ETH_TX_EN_GPIO_Port GPIOG
#define WiFi_RTS_Pin GPIO_PIN_12
#define WiFi_RTS_GPIO_Port GPIOA
#define DCMI_D5_Pin GPIO_PIN_4
#define DCMI_D5_GPIO_Port GPIOI
#define GSM_RTS_Pin GPIO_PIN_12
#define GSM_RTS_GPIO_Port GPIOG
#define DCMI_D2_Pin GPIO_PIN_10
#define DCMI_D2_GPIO_Port GPIOG
#define WiFi_SCK_Pin GPIO_PIN_3
#define WiFi_SCK_GPIO_Port GPIOD
#define WiFi_MOSI_Pin GPIO_PIN_3
#define WiFi_MOSI_GPIO_Port GPIOI
#define WiFi_MISO_Pin GPIO_PIN_2
#define WiFi_MISO_GPIO_Port GPIOI
#define WiFi_CTS_Pin GPIO_PIN_11
#define WiFi_CTS_GPIO_Port GPIOA
#define EXP_GPIO3_Pin GPIO_PIN_13
#define EXP_GPIO3_GPIO_Port GPIOC
#define DCMI_VSYNC_Pin GPIO_PIN_5
#define DCMI_VSYNC_GPIO_Port GPIOI
#define DCMI_PWRE_Pin GPIO_PIN_7
#define DCMI_PWRE_GPIO_Port GPIOI
#define DCMI_D6_Pin GPIO_PIN_6
#define DCMI_D6_GPIO_Port GPIOI
#define WiFi_IRQ_Pin GPIO_PIN_9
#define WiFi_IRQ_GPIO_Port GPIOG
#define WiFi_RX_Pin GPIO_PIN_10
#define WiFi_RX_GPIO_Port GPIOA
#define LS_OSC1_Pin GPIO_PIN_14
#define LS_OSC1_GPIO_Port GPIOC
#define FMC_A1_Pin GPIO_PIN_1
#define FMC_A1_GPIO_Port GPIOF
#define DCMI_RST_Pin GPIO_PIN_12
#define DCMI_RST_GPIO_Port GPIOI
#define CAN_RX_Pin GPIO_PIN_9
#define CAN_RX_GPIO_Port GPIOI
#define DCMI_D4_Pin GPIO_PIN_14
#define DCMI_D4_GPIO_Port GPIOH
#define WiFi_TX_Pin GPIO_PIN_9
#define WiFi_TX_GPIO_Port GPIOA
#define LS_OSC2_Pin GPIO_PIN_15
#define LS_OSC2_GPIO_Port GPIOC
#define VBATT_CHSTAT_Pin GPIO_PIN_11
#define VBATT_CHSTAT_GPIO_Port GPIOI
#define EXP_SCL_Pin GPIO_PIN_8
#define EXP_SCL_GPIO_Port GPIOA
#define HS_OSC1_Pin GPIO_PIN_0
#define HS_OSC1_GPIO_Port GPIOH
#define GSM_DTR_Pin GPIO_PIN_0
#define GSM_DTR_GPIO_Port GPIOK
#define GSM_RX_Pin GPIO_PIN_7
#define GSM_RX_GPIO_Port GPIOC
#define HS_OSC2_Pin GPIO_PIN_1
#define HS_OSC2_GPIO_Port GPIOH
#define DCMI_D0_Pin GPIO_PIN_6
#define DCMI_D0_GPIO_Port GPIOC
#define GSM_RI_Pin GPIO_PIN_9
#define GSM_RI_GPIO_Port GPIOJ
#define GSM_DCD_Pin GPIO_PIN_7
#define GSM_DCD_GPIO_Port GPIOG
#define GSM_RST_Pin GPIO_PIN_6
#define GSM_RST_GPIO_Port GPIOJ
#define ETH_TXD1_Pin GPIO_PIN_13
#define ETH_TXD1_GPIO_Port GPIOB
#define EXP_GPIO0_Pin GPIO_PIN_10
#define EXP_GPIO0_GPIO_Port GPIOF
#define EXP_GPIO1_Pin GPIO_PIN_3
#define EXP_GPIO1_GPIO_Port GPIOC
#define ETH_TXD0_Pin GPIO_PIN_12
#define ETH_TXD0_GPIO_Port GPIOB
#define ETH_MDC_Pin GPIO_PIN_1
#define ETH_MDC_GPIO_Port GPIOC
#define RMII_RXER_Pin GPIO_PIN_2
#define RMII_RXER_GPIO_Port GPIOC
#define DCMI_D3_Pin GPIO_PIN_12
#define DCMI_D3_GPIO_Port GPIOH
#define ETH_REF_CLK_Pin GPIO_PIN_1
#define ETH_REF_CLK_GPIO_Port GPIOA
#define SYS_WAKUP_Pin GPIO_PIN_0
#define SYS_WAKUP_GPIO_Port GPIOA
#define DCMI_HSYNC_Pin GPIO_PIN_4
#define DCMI_HSYNC_GPIO_Port GPIOA
#define ETH_RXD0_Pin GPIO_PIN_4
#define ETH_RXD0_GPIO_Port GPIOC
#define GSM_PWE_Pin GPIO_PIN_11
#define GSM_PWE_GPIO_Port GPIOH
#define ETH_MDIO_Pin GPIO_PIN_2
#define ETH_MDIO_GPIO_Port GPIOA
#define DCMI_PIXCLK_Pin GPIO_PIN_6
#define DCMI_PIXCLK_GPIO_Port GPIOA
#define DCMI_SCLA5_Pin GPIO_PIN_5
#define DCMI_SCLA5_GPIO_Port GPIOA
#define EXP_GPIO2_Pin GPIO_PIN_2
#define EXP_GPIO2_GPIO_Port GPIOJ
#define EXP_GPIO4_Pin GPIO_PIN_6
#define EXP_GPIO4_GPIO_Port GPIOH
#define EXP_SDA_Pin GPIO_PIN_8
#define EXP_SDA_GPIO_Port GPIOH
#define DCMI_D1_Pin GPIO_PIN_10
#define DCMI_D1_GPIO_Port GPIOH
#define VBATT_SENSE_Pin GPIO_PIN_3
#define VBATT_SENSE_GPIO_Port GPIOA

#define ETH_RXD1_Pin GPIO_PIN_5
#define ETH_RXD1_GPIO_Port GPIOC
#define ETH_CRS_DV_Pin GPIO_PIN_7
#define ETH_CRS_DV_GPIO_Port GPIOA


#define DCMI_LIGHT_Pin GPIO_PIN_0
#define DCMI_LIGHT_GPIO_Port GPIOB


#define RS485_DE_GPIO_Port GPIOB
#define RS485_RX_GPIO_Port GPIOB
#define RS485_TX_GPIO_Port GPIOB

#define RS485_DE_Pin GPIO_PIN_1
#define RS485_RX_Pin GPIO_PIN_11
#define RS485_TX_Pin GPIO_PIN_10



#define USB_DM_GPIO_Port GPIOB
#define USB_DP_GPIO_Port GPIOB

#define USB_DM_Pin GPIO_PIN_14
#define USB_DP_Pin GPIO_PIN_15
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
