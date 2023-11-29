/**
  ******************************************************************************
  * @file    main.c
  * @author  yldzmuhammed
  * @version V1.0
  * @date    13-06-2020
  * @brief   This file contains the main.c file.
  ******************************************************************************
  * @attention
  *
  * experiment platform: YiLDiZ IoT STF7-SoM Board BSP Library
  * web-site: https://www.yildiziot.com
  ******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include "stf7som.h"
#include "lcd_log.h"
#include "stf7som_lcd.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t i;
static uint8_t index;
static uint8_t devArray[256];

static HAL_StatusTypeDef result;

static I2C_HandleTypeDef hi2c1;
static RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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

    /* Init push button */
    BSP_PB_Init();

    /* Initialize all configured peripherals */
    /* Enable GPIO Ports Clock */
    BSP_GPIOCLK_Enable();

    PeriphClkInitStruct.PeriphClockSelection    = RCC_PERIPHCLK_I2C1;
    PeriphClkInitStruct.I2c1ClockSelection      = RCC_I2C1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    
    BSP_LCD_Init();
    
    BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
    BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS + (BSP_LCD_GetXSize() * BSP_LCD_GetYSize() * 4));
    
    BSP_LCD_DisplayOn();
    
    BSP_LCD_SelectLayer(0);
    
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    
    BSP_LCD_SelectLayer(1);
    
    BSP_LCD_Clear(LCD_COLOR_TRANSPARENT);
    
    BSP_LCD_SetTransparency(0, 0xFF);
    BSP_LCD_SetTransparency(1, 0x00);
    
    BSP_LCD_SelectLayer(0);
    
    /* Init the LCD Log module */
    LCD_LOG_Init();
    
    LCD_LOG_SetHeader((uint8_t *)"STF7-SoM I2C Scanner Example");
    LCD_LOG_SetFooter((uint8_t *)"YiLDiZ IoT");
    
    hi2c1.Instance              = I2C1;
    
    hi2c1.Init.OwnAddress1      = 0;
    hi2c1.Init.OwnAddress2      = 0;
    hi2c1.Init.Timing           = 0x00C0EAFF;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    hi2c1.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
    
    /** Configure Analogue filter */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        Error_Handler();
    }
    
    /** Configure Digital filter */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
    {
        Error_Handler();
    }
    
    /* Infinite loop */
    while (1)
    {
        LCD_UsrLog("I2C Scanner started. Scanning I2C bus...\n");
        
        for (i = 1; i < 128; i++)
        {
            /** the HAL wants a left aligned i2c address
              * &hi2c1 is the handle
              * (uint16_t)(i<<1) is the i2c address left aligned
              * retries 2
              * timeout 2
              */
            result = HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(i << 1), 2, 2);
            if (result == HAL_OK)
            {
                devArray[index++] = i;
            }
            HAL_Delay(1);
        }
        if(index != 0)
        {
            LCD_UsrLog("Found %d device(s) on the bus. Device address: \n", index);
            for(i = 0; i < index; i++)
            {
                LCD_UsrLog("0x%02X\t", devArray[i]);
            }
            LCD_UsrLog("\n");
        }
        else
        {
            LCD_ErrLog("No device found on the i2c bus!\n");
        }
        index = 0;
        HAL_Delay(3000);
        LCD_LOG_ClearTextZone();
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
  HAL_PWR_EnableBkUpAccess();
  
  /** Configure the main internal regulator output voltage */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /** Initializes the CPU, AHB and APB busses clocks */
  RCC_OscInitStruct.PLL.PLLQ        = 8;
  RCC_OscInitStruct.PLL.PLLM        = 12;
  RCC_OscInitStruct.PLL.PLLN        = 216;
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
  RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider   = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider  = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider  = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.ClockType       = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  
  PeriphClkInitStruct.PLLI2SDivQ            = 1;
  PeriphClkInitStruct.PLLI2S.PLLI2SR        = 2;
  PeriphClkInitStruct.PLLI2S.PLLI2SQ        = 2;
  PeriphClkInitStruct.PLLI2S.PLLI2SN        = 50;
  PeriphClkInitStruct.PLLI2S.PLLI2SP        = RCC_PLLP_DIV2;
  PeriphClkInitStruct.PeriphClockSelection  = RCC_PERIPHCLK_PLLI2S;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}
//

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    __breakpoint(0);
  /* User can add his own implementation to report the HAL error return state */
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
//

/************************ (C) COPYRIGHT YiLDiZ IoT END OF FILE ************************/
