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
#include "stf7som_camera.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FRAME_BUFFER_SIZE                 0x00800000
#define FRAME_BUFFER_BASE_ADDRESS         0xC0000000

#define LCD_LAYER_RGB565_SIZE             0x0012C000
#define LCD_LAYER_RGB888_SIZE             0x001C2000
#define LCD_LAYER_ARGB8888_SIZE           0x00258000


#define LCD_LAYER_0_FB_ADDRESS            (FRAME_BUFFER_BASE_ADDRESS)
#define LCD_LAYER_1_FB_ADDRESS            (LCD_LAYER_0_FB_ADDRESS   + LCD_LAYER_RGB565_SIZE)

#define CAMERA_FB_ADDRESS                 (LCD_LAYER_1_FB_ADDRESS   + LCD_LAYER_RGB565_SIZE)
#define CAMERA_FB_SIZE(x, y)              (x * y * 2)


#define CAMERA_FRAME_BUFFER               0xC0260000
#define LCD_FRAME_BUFFER                  0xC0130000
#define LCD_FRAME_BUFFER_LAYER1           0xC0000000
#define CONVERTED_FRAME_BUFFER            0xC0390000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t x_res;
uint32_t y_res;
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void LCD_LL_ConvertLineToARGB8888(void *pSrc, void *pDst);
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
    
    /* Initialize LCD */
    BSP_LCD_Init();

    /* select layer 0 */
    BSP_LCD_LayerRgb565Init(1, LCD_LAYER_1_FB_ADDRESS);

    /* Set Foreground Layer */
    BSP_LCD_SelectLayer(1);

    /* Clear the LCD Foreground layer */
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SetLayerVisible(1, DISABLE);

    /* Background Layer Initialization */
    BSP_LCD_LayerRgb565Init(0, LCD_LAYER_0_FB_ADDRESS);

    /* Enable the LCD */
    BSP_LCD_DisplayOn(); 

    /* Select the LCD Foreground layer */
    BSP_LCD_SelectLayer(0);
    BSP_LCD_Clear(LCD_COLOR_WHITE);

    x_res = 480;
    y_res = 272;
    BSP_LCD_SetLayerWindow(0, 100, 100, x_res, y_res);

    /* Initialize the Camera */
    BSP_CAMERA_Init(RESOLUTION_R480x272);

    /* Start the Camera Capture */
    BSP_CAMERA_ContinuousStart((uint8_t *)CAMERA_FB_ADDRESS);
    //  BSP_CAMERA_SnapshotStart((uint8_t *)CAMERA_FB_ADDRESS);
    
    /* Infinite loop */
    while (1)
    {
    }
}
//

/**
  * @brief  Line event callback.
  * @param  None
  * @retval None
  */
void BSP_CAMERA_LineEventCallback(void)
{
//  static uint32_t camera_datapointer, lcd_datapointer, line_number;
//  
//  if(line_number < y_res)
//  {
//    LCD_LL_ConvertLineToARGB8888((uint32_t *)(CAMERA_FB_ADDRESS + camera_datapointer), (uint32_t *)(LCD_LAYER_0_FB_ADDRESS + lcd_datapointer));
//    camera_datapointer = camera_datapointer + x_res * (sizeof(uint16_t)); 
//    lcd_datapointer = lcd_datapointer + x_res * (sizeof(uint16_t));
//    line_number++;
//  }
//  else
//  {
//    camera_datapointer = 0;
//    lcd_datapointer = 0;
//    line_number = 0;
//  }
}
//

void BSP_CAMERA_VsyncEventCallback(void)
{
  static DMA2D_HandleTypeDef hdma2d_eval;  
  
  /* Enable DMA2D clock */
  __HAL_RCC_DMA2D_CLK_ENABLE();
  
  /* Configure the DMA2D Mode, Color Mode and output offset */
  hdma2d_eval.Init.Mode         = DMA2D_M2M;
  hdma2d_eval.Init.ColorMode    = DMA2D_RGB565;
  hdma2d_eval.Init.OutputOffset = 0;     
  
  /* Foreground Configuration */
  hdma2d_eval.LayerCfg[1].InputOffset     = 0;
  hdma2d_eval.LayerCfg[1].InputAlpha      = 0xFF;
  hdma2d_eval.LayerCfg[1].InputColorMode  = DMA2D_INPUT_RGB565;
  hdma2d_eval.LayerCfg[1].AlphaMode       = DMA2D_NO_MODIF_ALPHA;
  
  hdma2d_eval.Instance = DMA2D;
  
  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d_eval) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1) == HAL_OK) 
    {
      if (HAL_DMA2D_Start(&hdma2d_eval, (uint32_t)CAMERA_FB_ADDRESS, (uint32_t)LCD_LAYER_0_FB_ADDRESS, x_res, y_res) == HAL_OK)
      {
        /* Polling For DMA transfer */  
        HAL_DMA2D_PollForTransfer(&hdma2d_eval, 10);
      }
    }
  }

}
//

/**
  * @brief  Converts a line to an ARGB8888 pixel format.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Output color  
  * @retval None
  */
static void LCD_LL_ConvertLineToARGB8888(void *pSrc, void *pDst)
{ 
  static DMA2D_HandleTypeDef hdma2d_eval;  
  
  /* Enable DMA2D clock */
  __HAL_RCC_DMA2D_CLK_ENABLE();
  
  /* Configure the DMA2D Mode, Color Mode and output offset */
  hdma2d_eval.Init.Mode         = DMA2D_M2M;
  hdma2d_eval.Init.ColorMode    = DMA2D_RGB565;
  hdma2d_eval.Init.OutputOffset = 0;     
  
  /* Foreground Configuration */
  hdma2d_eval.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d_eval.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d_eval.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
  hdma2d_eval.LayerCfg[1].InputOffset = 0;
  
  hdma2d_eval.Instance = DMA2D; 
  
  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d_eval) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1) == HAL_OK) 
    {
      if (HAL_DMA2D_Start(&hdma2d_eval, (uint32_t)pSrc, (uint32_t)pDst, x_res, 1) == HAL_OK)
      {
        /* Polling For DMA transfer */  
        HAL_DMA2D_PollForTransfer(&hdma2d_eval, 10);
      }
    }
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
