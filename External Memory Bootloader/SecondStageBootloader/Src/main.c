/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
#include "stf7som.h"
#include "stf7som_sd.h"
#include "stf7som_lcd.h"
#include "stf7som_nor.h"
#include "stf7som_qspi.h"
#include "stf7som_bootloader.h"

#include "sd_diskio.h"
#include "ff_gen_drv.h"

#include "usbd_cdc.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

#include "otaa.h"
#include "port.h"

#include "cmsis_os.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static bool JumpAppFlag;
static bool ReStartFlag;
static uint32_t JumpAddress;
static osThreadId OTAAUSBPollerTaskHandle;
static osThreadId OTAAUARTPollerTaskHandle;
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void OTAAUSBPollerTaskHandler(void const * argument);
void OTAAUARTPollerTaskHandler(void const * argument);

/* Private user code ---------------------------------------------------------*/

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize GPIO Clock */
  BSP_GPIOCLK_Enable();
  
  /* Initialize User Button */
  BSP_PB_Init();
  
	BSP_QSPI_Init();
	
	BSP_NOR_Init();
  /* Configure SD Card */
//  BSP_SD_Init();
  
  /* add mutexes, ... */
  /* add semaphores, ... */
  /* start timers, add new ones, ... */
  /* add queues, ... */
  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(OTAAUSBPollerTask, OTAAUSBPollerTaskHandler, osPriorityNormal, 0, 512);
  OTAAUSBPollerTaskHandle = osThreadCreate(osThread(OTAAUSBPollerTask), NULL);

  /* definition and creation of defaultTask */
  osThreadDef(OTAAUARTPollerTask, OTAAUARTPollerTaskHandler, osPriorityNormal, 0, 128);
  OTAAUARTPollerTaskHandle = osThreadCreate(osThread(OTAAUARTPollerTask), NULL);

  /* add threads, ... */
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  while (1)
  {
  }
}
//

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability */
//  HAL_PWR_EnableBkUpAccess();
  
  /** Configure the main internal regulator output voltage */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /** Initializes the CPU, AHB and APB busses clocks */
  RCC_OscInitStruct.PLL.PLLQ        = 8;
  RCC_OscInitStruct.PLL.PLLM        = 12;
  RCC_OscInitStruct.PLL.PLLN        = 200;
  RCC_OscInitStruct.HSEState        = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLP        = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
  /** Activate the Over-Drive mode */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  
  /** Initializes the CPU, AHB and APB busses clocks */
  RCC_ClkInitStruct.APB1CLKDivider  = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider  = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.AHBCLKDivider   = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.ClockType       = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
  
  PeriphClkInitStruct.PLLSAIDivQ            = 1;
  PeriphClkInitStruct.PLLSAI.PLLSAIR        = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ        = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIN        = 192;
  PeriphClkInitStruct.PLLSAIDivR            = RCC_PLLSAIDIVR_2;
  PeriphClkInitStruct.PLLSAI.PLLSAIP        = RCC_PLLSAIP_DIV8;
  PeriphClkInitStruct.Clk48ClockSelection   = RCC_CLK48SOURCE_PLLSAIP;
  PeriphClkInitStruct.Sdmmc1ClockSelection  = RCC_SDMMC1CLKSOURCE_CLK48;
  PeriphClkInitStruct.PeriphClockSelection  = RCC_PERIPHCLK_SDMMC1 | RCC_CLK48SOURCE_PLLSAIP;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
  /** Enables the Clock Security System */
  HAL_RCC_EnableCSS();
}
//

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(WiFi_PWE_GPIO_Port, WiFi_PWE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, EXP_GPIO3_Pin|EXP_GPIO1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GSM_DCD_GPIO_Port, GSM_DCD_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOJ, GSM_RST_Pin|EXP_GPIO2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(EXP_GPIO0_GPIO_Port, EXP_GPIO0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, GSM_PWE_Pin|EXP_GPIO4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : GSM_TX_Pin GSM_CTS_Pin GSM_RTS_Pin */
  GPIO_InitStruct.Pin = GSM_TX_Pin|GSM_CTS_Pin|GSM_RTS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : DCMI_SCL_Pin DCMI_SDA_Pin */
  GPIO_InitStruct.Pin = DCMI_SCL_Pin|DCMI_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : DCMI_MOSI_Pin DCMI_MISO_Pin */
  GPIO_InitStruct.Pin = DCMI_MOSI_Pin|DCMI_MISO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : WiFi_PWE_Pin */
  GPIO_InitStruct.Pin = WiFi_PWE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(WiFi_PWE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DCMI_D7_Pin */
  GPIO_InitStruct.Pin = DCMI_D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
  HAL_GPIO_Init(DCMI_D7_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CAN_TX_Pin */
  GPIO_InitStruct.Pin = CAN_TX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
  HAL_GPIO_Init(CAN_TX_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ETH_TX_EN_Pin */
  GPIO_InitStruct.Pin = ETH_TX_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(ETH_TX_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : WiFi_RTS_Pin WiFi_CTS_Pin WiFi_RX_Pin WiFi_TX_Pin */
  GPIO_InitStruct.Pin = WiFi_RTS_Pin|WiFi_CTS_Pin|WiFi_RX_Pin|WiFi_TX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LTDC_DISP_Pin DCMI_PWRE_Pin DCMI_RST_Pin */
  GPIO_InitStruct.Pin = DCMI_PWRE_Pin|DCMI_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pins : DCMI_D5_Pin DCMI_VSYNC_Pin DCMI_D6_Pin */
  GPIO_InitStruct.Pin = DCMI_D5_Pin|DCMI_VSYNC_Pin|DCMI_D6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pin : DCMI_D2_Pin */
  GPIO_InitStruct.Pin = DCMI_D2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
  HAL_GPIO_Init(DCMI_D2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : WiFi_SCK_Pin */
  GPIO_InitStruct.Pin = WiFi_SCK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(WiFi_SCK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : WiFi_MOSI_Pin WiFi_MISO_Pin */
  GPIO_InitStruct.Pin = WiFi_MOSI_Pin|WiFi_MISO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pins : EXP_GPIO3_Pin EXP_GPIO1_Pin */
  GPIO_InitStruct.Pin = EXP_GPIO3_Pin|EXP_GPIO1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : WiFi_IRQ_Pin */
  GPIO_InitStruct.Pin = WiFi_IRQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(WiFi_IRQ_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CAN_RX_Pin */
  GPIO_InitStruct.Pin = CAN_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
  HAL_GPIO_Init(CAN_RX_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DCMI_D4_Pin DCMI_D3_Pin DCMI_D1_Pin */
  GPIO_InitStruct.Pin = DCMI_D4_Pin|DCMI_D3_Pin|DCMI_D1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pin : VBATT_CHSTAT_Pin */
  GPIO_InitStruct.Pin = VBATT_CHSTAT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(VBATT_CHSTAT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LTDC_TPRST_Pin GSM_DTR_Pin */
  GPIO_InitStruct.Pin = GSM_DTR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

  /*Configure GPIO pin : EXP_SCL_Pin */
  GPIO_InitStruct.Pin = EXP_SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
  HAL_GPIO_Init(EXP_SCL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LTDC_TPIRQ_Pin GSM_RI_Pin */
  GPIO_InitStruct.Pin = GSM_RI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

  /*Configure GPIO pin : GSM_RX_Pin */
  GPIO_InitStruct.Pin = GSM_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
  HAL_GPIO_Init(GSM_RX_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DCMI_D0_Pin */
  GPIO_InitStruct.Pin = DCMI_D0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
  HAL_GPIO_Init(DCMI_D0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : GSM_DCD_Pin */
  GPIO_InitStruct.Pin = GSM_DCD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GSM_DCD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : GSM_RST_Pin EXP_GPIO2_Pin */
  GPIO_InitStruct.Pin = GSM_RST_Pin|EXP_GPIO2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

  /*Configure GPIO pins : ETH_TXD1_Pin ETH_TXD0_Pin */
  GPIO_InitStruct.Pin = ETH_TXD1_Pin|ETH_TXD0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : EXP_GPIO0_Pin */
  GPIO_InitStruct.Pin = EXP_GPIO0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(EXP_GPIO0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ETH_MDC_Pin ETH_RXD0_Pin ETH_RXD1_Pin */
  GPIO_InitStruct.Pin = ETH_MDC_Pin|ETH_RXD0_Pin|ETH_RXD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : RMII_RXER_Pin */
  GPIO_InitStruct.Pin = RMII_RXER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RMII_RXER_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ETH_REF_CLK_Pin ETH_MDIO_Pin ETH_CRS_DV_Pin */
  GPIO_InitStruct.Pin = ETH_REF_CLK_Pin|ETH_MDIO_Pin|ETH_CRS_DV_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : DCMI_HSYNC_Pin DCMI_PIXCLK_Pin */
  GPIO_InitStruct.Pin = DCMI_HSYNC_Pin|DCMI_PIXCLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : GSM_PWE_Pin EXP_GPIO4_Pin */
  GPIO_InitStruct.Pin = GSM_PWE_Pin|EXP_GPIO4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pin : DCMI_SCLA5_Pin */
  GPIO_InitStruct.Pin = DCMI_SCLA5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(DCMI_SCLA5_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : EXP_SDA_Pin */
  GPIO_InitStruct.Pin = EXP_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
  HAL_GPIO_Init(EXP_SDA_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RS485_DE_Pin */
  GPIO_InitStruct.Pin = RS485_DE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RS485_DE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DCMI_LIGHT_Pin */
  GPIO_InitStruct.Pin = DCMI_LIGHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
  HAL_GPIO_Init(DCMI_LIGHT_GPIO_Port, &GPIO_InitStruct);
}
//

/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
static void OTAAUSBPollerTaskHandler(void const * argument)
{
	/* Init OTAA Stack */
	if(eOTAA_Init(eOTAA_USB) == eOTAA_ENOERR)
	{
		/* Enable OTAA stack */
		eOTAA_Enable();
	}
  
  /* Infinite loop */
  for(;;)
  {
	  /* Poll Stack */
		eOTAA_Poll();
		
		/* Check if there is a JumpApp request */
		if(JumpAppFlag)
		{
			JumpAppFlag = false;
			BSP_Bootloader_JumpToApplication(JumpAddress);
		}
		
		/* Check for reboot flag */
		if(ReStartFlag)
		{
			NVIC_SystemReset();
		}
  }
}
//

/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
static void OTAAUARTPollerTaskHandler(void const * argument)
{
	
  /* Infinite loop */
  for(;;)
  {
		osDelay(1);
  }
}
//

eOTAAErrorCode eOTAACmdReadID(uint8_t *buf, eOTAAMemoryType eMemory)
{
	eOTAAErrorCode eStatus = eOTAA_ENOERR;
	
	NOR_IDTypeDef xNORID = {0};
	xQSPIFlashID_TypeDef xQSPIID = {0};
	
	if(eMemory == eOTAA_InternalFlash)
	{
		uint32_t uid0 = HAL_GetUIDw0();
		uint32_t uid1 = HAL_GetUIDw1();
		uint32_t uid2 = HAL_GetUIDw2();
		
		for(int i = 0; i < 12; i++)
		{
			buf[i] = READ_REG(*((uint8_t *)UID_BASE + i));
		}
	}
	else if(eMemory == eOTAA_NORFlash)
	{
		if(BSP_NOR_Read_ID(&xNORID) != NOR_STATUS_OK)
		{
			eStatus = eOTAA_EIO;
		}
		else
		{
			memcpy(buf, &xNORID, sizeof(NOR_IDTypeDef));
		}
	}
	else if(eMemory == eOTAA_QSPIFlash)
	{
		if(BSP_QSPI_ReadID(&xQSPIID.JEDECID) 				== QSPI_OK && 
			 BSP_QSPI_ReadDeviceID(&xQSPIID.DeviceID) == QSPI_OK &&
			 BSP_QSPI_ReadUniqDevID(&xQSPIID.JEDECID) == QSPI_OK)
		{
			memcpy(buf, &xQSPIID, sizeof(xQSPIFlashID_TypeDef));
		}
		else
		{
			eStatus = eOTAA_EIO;
		}
			
	}
	else if(eMemory == eOTAA_EEPROM)
	{
		eStatus = eOTAA_EIO;
	}
	
	return eStatus;
}
//

eOTAAErrorCode eOTAACmdJumpApp(void)
{
	uint32_t SectorIndex = 0;
	
	eOTAAErrorCode eStatus = eOTAA_ENOERR;
	
	xMBR_TypeDef xMBR = {0};
	
	BSP_Bootloader_Get_BootTable(&xMBR);
	
	SectorIndex = xMBR.eSectorIndex;
	
	switch(xMBR.eSectorIndex)
	{
		case eSecIndx_FirstSector:
		case eSecIndx_SecondSector:
		{
			if(BSP_Bootloader_CheckForApplication(xMBR.Sectors[SectorIndex].address) == BL_OK)
			{
				if(BSP_Bootloader_VerifyChecksum(&xMBR.Sectors[SectorIndex]) == BL_OK)
				{
					JumpAddress = xMBR.Sectors[SectorIndex].address;
				}
				else
				{
					eStatus = eOTAA_EAPPCRC;
				}
			}
			else
			{
				eStatus = eOTAA_ENOAPP;
			}
			break;
		}
		default:
			eStatus = eOTAA_EFORMAT;
			break;
	}
	
	return eStatus;
}
//

eOTAAErrorCode eOTAACmdRestart(void)
{
	ReStartFlag = true;
	
	return eOTAA_ENOERR;
}
//

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}
//

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
