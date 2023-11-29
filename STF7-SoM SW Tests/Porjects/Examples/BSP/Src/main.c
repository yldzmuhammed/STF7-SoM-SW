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
#include "stf7som_sd.h"
#include "stf7som_lcd.h"
#include "stf7som_ts.h"
#include "stf7som_nor.h"
#include "stf7som_fmc.h"
#include "bsp_examples.h"
#include "yildiziot_logo.h"
#include "stf7som_communication.h"
/* Private typedef -----------------------------------------------------------*/
typedef struct
{
	void		(*DemoFunc)(void);
	uint8_t		DemoName[50];
	uint32_t 	DemoIndex;
}BSP_DemoTypedef;

/* Private define ------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypedef))
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t DemoIndex = 0;
__IO uint8_t NbLoop = 1;
__IO uint32_t PressCount = 0;	/* Counter for Sel Joystick pressed*/

BSP_DemoTypedef  BSP_examples[]=
{
  {Touchscreen_demo, 	"TOUCHSCREEN", 	0}, 
  {LCD_demo,			"LCD", 			0},
  {SD_demo, 			"SD", 			0},
  {NOR_demo, 			"NOR", 			0},
  {SDRAM_demo, 			"SDRAM", 		0},
  {Log_demo, 			"LCD LOG", 		0},
};

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Display_DemoDescription(void);
/* Private user code ---------------------------------------------------------*/

extern uint32_t ActiveLayer;
extern LTDC_HandleTypeDef hLtdcHandler;
extern DMA2D_HandleTypeDef hDma2dHandler;

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
  
  /* Initialize ADC Channel */
//  BSP_BattCheck_Init();
  
  /*##-1- Initialize the LCD #################################################*/
  /* Initialize the LCD */
  BSP_LCD_Init();

  BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS);

  Display_DemoDescription();

  /* Wait For User inputs */
  /* Infinite loop */
  while (1)
  {
    if(BSP_PB_Get() == GPIO_PIN_SET)
    {
      while (BSP_PB_Get() == GPIO_PIN_SET);
      
      BSP_examples[DemoIndex++].DemoFunc();
      
      if(DemoIndex >= COUNT_OF_EXAMPLE(BSP_examples))
      {
        NbLoop++;
        DemoIndex = 0;
      }
      Display_DemoDescription();
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

/**
  * @brief  Converts a line to an ARGB8888 pixel format.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Output color
  * @param  xSize: Buffer width
  * @param  ColorMode: Input color mode
  * @retval None
  */
static void LL_ConvertLineToARGB8888(void *pSrc, void *pDst, uint32_t xSize, uint32_t ColorMode)
{
    /* Configure the DMA2D Mode, Color Mode and output offset */
    hDma2dHandler.Init.Mode         = DMA2D_M2M_PFC;
    hDma2dHandler.Init.ColorMode    = DMA2D_ARGB8888;
    hDma2dHandler.Init.OutputOffset = 0;

    /* Foreground Configuration */
    hDma2dHandler.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    hDma2dHandler.LayerCfg[1].InputAlpha = 0xFF;
    hDma2dHandler.LayerCfg[1].InputColorMode = ColorMode;
    hDma2dHandler.LayerCfg[1].InputOffset = 0;

    hDma2dHandler.Instance = DMA2D;

    /* DMA2D Initialization */
    if(HAL_DMA2D_Init(&hDma2dHandler) == HAL_OK)
    {
        if(HAL_DMA2D_ConfigLayer(&hDma2dHandler, 1) == HAL_OK)
        {
            if(HAL_DMA2D_Start(&hDma2dHandler, (uint32_t)pSrc, (uint32_t)pDst, xSize, 1) == HAL_OK)
            {
                /* Polling For DMA transfer */
                HAL_DMA2D_PollForTransfer(&hDma2dHandler, 10);
            }
        }
    }
}
//

static void LCD_DrawPicture(uint32_t Xpos, uint32_t Ypos, uint8_t *pic)
{
    uint32_t index = 0, width = 47, height = 136, bit_pixel = DMA2D_INPUT_RGB565;
    uint32_t address;
    uint32_t input_color_mode = 0;

	
//    /* Set the address */
//    address = hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress + (((BSP_LCD_GetXSize() * Ypos) + Xpos) * (4));

//    /* Convert picture to ARGB8888 pixel format */
//    for(index = 0; index < height; index++)
//    {
//        /* Pixel format conversion */
//        LL_ConvertLineToARGB8888((uint32_t *)pic, (uint32_t *)address, width, input_color_mode);

//        /* Increment the source and destination buffers */
//        address += (BSP_LCD_GetXSize() * 4);
//        pic -= width * (bit_pixel / 8);
//    }
	
	
	LTDC_LayerCfgTypeDef      pLayerCfg;
	/* Windowing configuration */ 
	/* In this case all the active display area is used to display a picture then :
	 Horizontal start = horizontal synchronization + Horizontal back porch = 43 
	 Vertical start   = vertical synchronization + vertical back porch     = 12
	 Horizontal stop = Horizontal start + window width -1 = 43 + 480 -1 
	 Vertical stop   = Vertical start + window height -1  = 12 + 272 -1      */
	pLayerCfg.WindowX0 = Xpos;
	pLayerCfg.WindowX1 = Xpos + width;
	pLayerCfg.WindowY0 = Ypos;
	pLayerCfg.WindowY1 = Ypos + height;

	/* Pixel Format configuration */ 
	pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;

	/* Start Address configuration : frame buffer is located in FLASH memory */
	pLayerCfg.FBStartAdress = (uint32_t)&pic;

	/* Alpha constant (255 totally opaque) */
	pLayerCfg.Alpha = 255;

	/* Default Color configuration (configure A,R,G,B component values) */
	/* default color == all transparent pixel                           */
	pLayerCfg.Alpha0 = 0; /* completely transparent */
	pLayerCfg.Backcolor.Blue = 0;
	pLayerCfg.Backcolor.Green = 0;
	pLayerCfg.Backcolor.Red = 0;

	/* Configure blending factors */
	pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
	pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;

	/* Configure the number of lines and number of pixels per line */
	pLayerCfg.ImageWidth  = width;
	pLayerCfg.ImageHeight = height;

	/* Configure the Layer 1 */
	if(HAL_LTDC_ConfigLayer(&hLtdcHandler, &pLayerCfg, LTDC_LAYER_1) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler(); 
	} 
}
//

/**
  * @brief  Display main demo messages
  * @param  None
  * @retval None
  */
static void Display_DemoDescription(void)
{
  char desc[50];
  
  BSP_LCD_DisplayOn();
  
  /* Set LCD Foreground Layer  */
  BSP_LCD_SelectLayer(1);

  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  
  /* Clear the LCD */ 
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);  

  /* Display LCD messages */
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"STM32756G BSP", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"Drivers examples", CENTER_MODE);
  
  /* Draw Bitmap */
//  BSP_LCD_DrawBitmap((BSP_LCD_GetXSize() - 80)/2, 65, (uint8_t *)yildiziot_logo);
  LCD_DrawPicture(50, 40, (uint8_t *)yildiziot_logo);
  
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 20, (uint8_t *)"Copyright (c) STMicroelectronics 2016", CENTER_MODE);
  
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize()/2 + 150, BSP_LCD_GetXSize(), 60);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + 190, (uint8_t *)"Press Tamper push-button to start :", CENTER_MODE);
  sprintf(desc,"%s example", BSP_examples[DemoIndex].DemoName);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 160, (uint8_t *)desc, CENTER_MODE);   
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
