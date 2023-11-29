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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FMC_A20_Pin GPIO_PIN_4
#define FMC_A20_GPIO_Port GPIOE
#define FMC_A19_Pin GPIO_PIN_3
#define FMC_A19_GPIO_Port GPIOE
#define QSP_IO2_Pin GPIO_PIN_2
#define QSP_IO2_GPIO_Port GPIOE
#define GSM_TX_Pin GPIO_PIN_14
#define GSM_TX_GPIO_Port GPIOG
#define FMC_NBL1_Pin GPIO_PIN_1
#define FMC_NBL1_GPIO_Port GPIOE
#define FMC_NBL0_Pin GPIO_PIN_0
#define FMC_NBL0_GPIO_Port GPIOE
#define DCMI_SCL_Pin GPIO_PIN_8
#define DCMI_SCL_GPIO_Port GPIOB
#define DCMI_MOSI_Pin GPIO_PIN_5
#define DCMI_MOSI_GPIO_Port GPIOB
#define DCMI_MISO_Pin GPIO_PIN_4
#define DCMI_MISO_GPIO_Port GPIOB
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define FMC_NE_Pin GPIO_PIN_7
#define FMC_NE_GPIO_Port GPIOD
#define SD_CK_Pin GPIO_PIN_12
#define SD_CK_GPIO_Port GPIOC
#define WiFi_PWE_Pin GPIO_PIN_15
#define WiFi_PWE_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define FMC_A21_Pin GPIO_PIN_5
#define FMC_A21_GPIO_Port GPIOE
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
#define FMC_SDNCAS_Pin GPIO_PIN_15
#define FMC_SDNCAS_GPIO_Port GPIOG
#define ETH_TX_EN_Pin GPIO_PIN_11
#define ETH_TX_EN_GPIO_Port GPIOG
#define LTDC_B1_Pin GPIO_PIN_13
#define LTDC_B1_GPIO_Port GPIOJ
#define LTDC_B0_Pin GPIO_PIN_12
#define LTDC_B0_GPIO_Port GPIOJ
#define FMC_NWAIT_Pin GPIO_PIN_6
#define FMC_NWAIT_GPIO_Port GPIOD
#define FMC_D2_Pin GPIO_PIN_0
#define FMC_D2_GPIO_Port GPIOD
#define SD_D3_Pin GPIO_PIN_11
#define SD_D3_GPIO_Port GPIOC
#define SD_D2_Pin GPIO_PIN_10
#define SD_D2_GPIO_Port GPIOC
#define WiFi_RTS_Pin GPIO_PIN_12
#define WiFi_RTS_GPIO_Port GPIOA
#define LTDC_DISP_Pin GPIO_PIN_8
#define LTDC_DISP_GPIO_Port GPIOI
#define DCMI_D5_Pin GPIO_PIN_4
#define DCMI_D5_GPIO_Port GPIOI
#define LTDC_DE_Pin GPIO_PIN_7
#define LTDC_DE_GPIO_Port GPIOK
#define LTDC_B7_Pin GPIO_PIN_6
#define LTDC_B7_GPIO_Port GPIOK
#define LTDC_B6_Pin GPIO_PIN_5
#define LTDC_B6_GPIO_Port GPIOK
#define GSM_RTS_Pin GPIO_PIN_12
#define GSM_RTS_GPIO_Port GPIOG
#define DCMI_D2_Pin GPIO_PIN_10
#define DCMI_D2_GPIO_Port GPIOG
#define LTDC_B2_Pin GPIO_PIN_14
#define LTDC_B2_GPIO_Port GPIOJ
#define FMC_NWE_Pin GPIO_PIN_5
#define FMC_NWE_GPIO_Port GPIOD
#define WiFi_SCK_Pin GPIO_PIN_3
#define WiFi_SCK_GPIO_Port GPIOD
#define FMC_D3_Pin GPIO_PIN_1
#define FMC_D3_GPIO_Port GPIOD
#define WiFi_MOSI_Pin GPIO_PIN_3
#define WiFi_MOSI_GPIO_Port GPIOI
#define WiFi_MISO_Pin GPIO_PIN_2
#define WiFi_MISO_GPIO_Port GPIOI
#define WiFi_CTS_Pin GPIO_PIN_11
#define WiFi_CTS_GPIO_Port GPIOA
#define EXP_GPIO3_Pin GPIO_PIN_13
#define EXP_GPIO3_GPIO_Port GPIOC
#define FMC_A0_Pin GPIO_PIN_0
#define FMC_A0_GPIO_Port GPIOF
#define DCMI_VSYNC_Pin GPIO_PIN_5
#define DCMI_VSYNC_GPIO_Port GPIOI
#define DCMI_PWRE_Pin GPIO_PIN_7
#define DCMI_PWRE_GPIO_Port GPIOI
#define LTDC_HSYNC_Pin GPIO_PIN_10
#define LTDC_HSYNC_GPIO_Port GPIOI
#define DCMI_D6_Pin GPIO_PIN_6
#define DCMI_D6_GPIO_Port GPIOI
#define LTDC_B5_Pin GPIO_PIN_4
#define LTDC_B5_GPIO_Port GPIOK
#define LTDC_B4_Pin GPIO_PIN_3
#define LTDC_B4_GPIO_Port GPIOK
#define WiFi_IRQ_Pin GPIO_PIN_9
#define WiFi_IRQ_GPIO_Port GPIOG
#define LTDC_B3_Pin GPIO_PIN_15
#define LTDC_B3_GPIO_Port GPIOJ
#define FMC_NOE_Pin GPIO_PIN_4
#define FMC_NOE_GPIO_Port GPIOD
#define SD_CMD_Pin GPIO_PIN_2
#define SD_CMD_GPIO_Port GPIOD
#define LTDC_G4_Pin GPIO_PIN_15
#define LTDC_G4_GPIO_Port GPIOH
#define LTDC_G6_Pin GPIO_PIN_1
#define LTDC_G6_GPIO_Port GPIOI
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
#define LTDC_G2_Pin GPIO_PIN_13
#define LTDC_G2_GPIO_Port GPIOH
#define DCMI_D4_Pin GPIO_PIN_14
#define DCMI_D4_GPIO_Port GPIOH
#define LTDC_G5_Pin GPIO_PIN_0
#define LTDC_G5_GPIO_Port GPIOI
#define WiFi_TX_Pin GPIO_PIN_9
#define WiFi_TX_GPIO_Port GPIOA
#define LS_OSC2_Pin GPIO_PIN_15
#define LS_OSC2_GPIO_Port GPIOC
#define VBATT_CHSTAT_Pin GPIO_PIN_11
#define VBATT_CHSTAT_GPIO_Port GPIOI
#define LTDC_TPRST_Pin GPIO_PIN_1
#define LTDC_TPRST_GPIO_Port GPIOK
#define LTDC_G7_Pin GPIO_PIN_2
#define LTDC_G7_GPIO_Port GPIOK
#define SD_D1_Pin GPIO_PIN_9
#define SD_D1_GPIO_Port GPIOC
#define EXP_SCL_Pin GPIO_PIN_8
#define EXP_SCL_GPIO_Port GPIOA
#define HS_OSC1_Pin GPIO_PIN_0
#define HS_OSC1_GPIO_Port GPIOH
#define FMC_A2_Pin GPIO_PIN_2
#define FMC_A2_GPIO_Port GPIOF
#define LTDC_VSYNC_Pin GPIO_PIN_13
#define LTDC_VSYNC_GPIO_Port GPIOI
#define LTDC_R0_Pin GPIO_PIN_15
#define LTDC_R0_GPIO_Port GPIOI
#define LTDC_TPIRQ_Pin GPIO_PIN_11
#define LTDC_TPIRQ_GPIO_Port GPIOJ
#define GSM_DTR_Pin GPIO_PIN_0
#define GSM_DTR_GPIO_Port GPIOK
#define SD_D0_Pin GPIO_PIN_8
#define SD_D0_GPIO_Port GPIOC
#define GSM_RX_Pin GPIO_PIN_7
#define GSM_RX_GPIO_Port GPIOC
#define HS_OSC2_Pin GPIO_PIN_1
#define HS_OSC2_GPIO_Port GPIOH
#define FMC_A3_Pin GPIO_PIN_3
#define FMC_A3_GPIO_Port GPIOF
#define LTDC_CLK_Pin GPIO_PIN_14
#define LTDC_CLK_GPIO_Port GPIOI
#define LTDC_SCL_Pin GPIO_PIN_4
#define LTDC_SCL_GPIO_Port GPIOH
#define LTDC_G1_Pin GPIO_PIN_8
#define LTDC_G1_GPIO_Port GPIOJ
#define LTDC_G3_Pin GPIO_PIN_10
#define LTDC_G3_GPIO_Port GPIOJ
#define FMC_SDCLK_Pin GPIO_PIN_8
#define FMC_SDCLK_GPIO_Port GPIOG
#define DCMI_D0_Pin GPIO_PIN_6
#define DCMI_D0_GPIO_Port GPIOC
#define FMC_A4_Pin GPIO_PIN_4
#define FMC_A4_GPIO_Port GPIOF
#define LTDC_SDA_Pin GPIO_PIN_5
#define LTDC_SDA_GPIO_Port GPIOH
#define FMC_SDNE0_Pin GPIO_PIN_3
#define FMC_SDNE0_GPIO_Port GPIOH
#define LTDC_G0_Pin GPIO_PIN_7
#define LTDC_G0_GPIO_Port GPIOJ
#define GSM_RI_Pin GPIO_PIN_9
#define GSM_RI_GPIO_Port GPIOJ
#define GSM_DCD_Pin GPIO_PIN_7
#define GSM_DCD_GPIO_Port GPIOG
#define LTDC_R7_Pin GPIO_PIN_6
#define LTDC_R7_GPIO_Port GPIOG
#define LTDC_BL_Pin GPIO_PIN_7
#define LTDC_BL_GPIO_Port GPIOF
#define QSP_IO3_Pin GPIO_PIN_6
#define QSP_IO3_GPIO_Port GPIOF
#define FMC_A5_Pin GPIO_PIN_5
#define FMC_A5_GPIO_Port GPIOF
#define FMC_SDCKE0_Pin GPIO_PIN_2
#define FMC_SDCKE0_GPIO_Port GPIOH
#define GSM_RST_Pin GPIO_PIN_6
#define GSM_RST_GPIO_Port GPIOJ
#define FMC_D1_Pin GPIO_PIN_15
#define FMC_D1_GPIO_Port GPIOD
#define ETH_TXD1_Pin GPIO_PIN_13
#define ETH_TXD1_GPIO_Port GPIOB
#define FMC_D15_Pin GPIO_PIN_10
#define FMC_D15_GPIO_Port GPIOD
#define EXP_GPIO0_Pin GPIO_PIN_10
#define EXP_GPIO0_GPIO_Port GPIOF
#define QSP_IO1_Pin GPIO_PIN_9
#define QSP_IO1_GPIO_Port GPIOF
#define QSP_IO0_Pin GPIO_PIN_8
#define QSP_IO0_GPIO_Port GPIOF
#define EXP_GPIO1_Pin GPIO_PIN_3
#define EXP_GPIO1_GPIO_Port GPIOC
#define FMC_D0_Pin GPIO_PIN_14
#define FMC_D0_GPIO_Port GPIOD
#define ETH_TXD0_Pin GPIO_PIN_12
#define ETH_TXD0_GPIO_Port GPIOB
#define FMC_D14_Pin GPIO_PIN_9
#define FMC_D14_GPIO_Port GPIOD
#define FMC_D13_Pin GPIO_PIN_8
#define FMC_D13_GPIO_Port GPIOD
#define FMC_SDNWE_Pin GPIO_PIN_0
#define FMC_SDNWE_GPIO_Port GPIOC
#define ETH_MDC_Pin GPIO_PIN_1
#define ETH_MDC_GPIO_Port GPIOC
#define RMII_RXER_Pin GPIO_PIN_2
#define RMII_RXER_GPIO_Port GPIOC
#define QSP_CLK_Pin GPIO_PIN_2
#define QSP_CLK_GPIO_Port GPIOB
#define FMC_A6_Pin GPIO_PIN_12
#define FMC_A6_GPIO_Port GPIOF
#define FMC_A11_Pin GPIO_PIN_1
#define FMC_A11_GPIO_Port GPIOG
#define FMC_A9_Pin GPIO_PIN_15
#define FMC_A9_GPIO_Port GPIOF
#define LTDC_R5_Pin GPIO_PIN_4
#define LTDC_R5_GPIO_Port GPIOJ
#define FMC_A17_Pin GPIO_PIN_12
#define FMC_A17_GPIO_Port GPIOD
#define FMC_A18_Pin GPIO_PIN_13
#define FMC_A18_GPIO_Port GPIOD
#define FMC_A13_Pin GPIO_PIN_3
#define FMC_A13_GPIO_Port GPIOG
#define FMC_A12_Pin GPIO_PIN_2
#define FMC_A12_GPIO_Port GPIOG
#define LTDC_R6_Pin GPIO_PIN_5
#define LTDC_R6_GPIO_Port GPIOJ
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
#define FMC_A7_Pin GPIO_PIN_13
#define FMC_A7_GPIO_Port GPIOF
#define FMC_A10_Pin GPIO_PIN_0
#define FMC_A10_GPIO_Port GPIOG
#define LTDC_R4_Pin GPIO_PIN_3
#define LTDC_R4_GPIO_Port GPIOJ
#define FMC_D5_Pin GPIO_PIN_8
#define FMC_D5_GPIO_Port GPIOE
#define FMC_A16_Pin GPIO_PIN_11
#define FMC_A16_GPIO_Port GPIOD
#define FMC_A15_Pin GPIO_PIN_5
#define FMC_A15_GPIO_Port GPIOG
#define FMC_A14_Pin GPIO_PIN_4
#define FMC_A14_GPIO_Port GPIOG
#define SD_DETECT_Pin GPIO_PIN_7
#define SD_DETECT_GPIO_Port GPIOH
#define LTDC_R3_Pin GPIO_PIN_9
#define LTDC_R3_GPIO_Port GPIOH
#define GSM_PWE_Pin GPIO_PIN_11
#define GSM_PWE_GPIO_Port GPIOH
#define ETH_MDIO_Pin GPIO_PIN_2
#define ETH_MDIO_GPIO_Port GPIOA
#define DCMI_PIXCLK_Pin GPIO_PIN_6
#define DCMI_PIXCLK_GPIO_Port GPIOA
#define DCMI_SCLA5_Pin GPIO_PIN_5
#define DCMI_SCLA5_GPIO_Port GPIOA
#define ETH_RXD1_Pin GPIO_PIN_5
#define ETH_RXD1_GPIO_Port GPIOC
#define FMC_A8_Pin GPIO_PIN_14
#define FMC_A8_GPIO_Port GPIOF
#define EXP_GPIO2_Pin GPIO_PIN_2
#define EXP_GPIO2_GPIO_Port GPIOJ
#define FMC_SDNRAS_Pin GPIO_PIN_11
#define FMC_SDNRAS_GPIO_Port GPIOF
#define FMC_D6_Pin GPIO_PIN_9
#define FMC_D6_GPIO_Port GPIOE
#define FMC_D8_Pin GPIO_PIN_11
#define FMC_D8_GPIO_Port GPIOE
#define FMC_D11_Pin GPIO_PIN_14
#define FMC_D11_GPIO_Port GPIOE
#define RS485_TX_Pin GPIO_PIN_10
#define RS485_TX_GPIO_Port GPIOB
#define EXP_GPIO4_Pin GPIO_PIN_6
#define EXP_GPIO4_GPIO_Port GPIOH
#define EXP_SDA_Pin GPIO_PIN_8
#define EXP_SDA_GPIO_Port GPIOH
#define DCMI_D1_Pin GPIO_PIN_10
#define DCMI_D1_GPIO_Port GPIOH
#define VBATT_SENSE_Pin GPIO_PIN_3
#define VBATT_SENSE_GPIO_Port GPIOA
#define ETH_CRS_DV_Pin GPIO_PIN_7
#define ETH_CRS_DV_GPIO_Port GPIOA
#define RS485_DE_Pin GPIO_PIN_1
#define RS485_DE_GPIO_Port GPIOB
#define DCMI_LIGHT_Pin GPIO_PIN_0
#define DCMI_LIGHT_GPIO_Port GPIOB
#define LTDC_R1_Pin GPIO_PIN_0
#define LTDC_R1_GPIO_Port GPIOJ
#define LTDC_R2_Pin GPIO_PIN_1
#define LTDC_R2_GPIO_Port GPIOJ
#define FMC_D4_Pin GPIO_PIN_7
#define FMC_D4_GPIO_Port GPIOE
#define FMC_D7_Pin GPIO_PIN_10
#define FMC_D7_GPIO_Port GPIOE
#define FMC_D9_Pin GPIO_PIN_12
#define FMC_D9_GPIO_Port GPIOE
#define FMC_D12_Pin GPIO_PIN_15
#define FMC_D12_GPIO_Port GPIOE
#define FMC_D10_Pin GPIO_PIN_13
#define FMC_D10_GPIO_Port GPIOE
#define RS485_RX_Pin GPIO_PIN_11
#define RS485_RX_GPIO_Port GPIOB
#define USB_DM_Pin GPIO_PIN_14
#define USB_DM_GPIO_Port GPIOB
#define USB_DP_Pin GPIO_PIN_15
#define USB_DP_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
