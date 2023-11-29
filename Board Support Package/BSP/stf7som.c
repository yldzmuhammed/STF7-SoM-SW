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

/* Includes ------------------------------------------------------------------*/
#include "sdk_config.h"

#include "stf7som.h"
#include "errhandler.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if ENABLE_VBATTMON == 1
#define ANALOG_SAMPLE_COUNT                     (16)
static __IO uint16_t                            ADCRawValue;
static __IO float                               Vbatt_Voltage;
static DMA_HandleTypeDef                        hdma_adc;
static ADC_HandleTypeDef                        AdcHandle;
#endif
//

#if ENABLE_CAMERAI2C == 1
static I2C_HandleTypeDef                        hi2cSTF7SOM;
#endif
//

/* Private function prototypes -----------------------------------------------*/
static void SystemClockHSI_Config(void);
static void SystemClockHSE_Config(void);
/* Private user code ---------------------------------------------------------*/
/* Private user function -----------------------------------------------------*/

/**
    * @brief  Switch the PLL source from HSI to HSE , and select the PLL as SYSCLK
  *         source.
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE )
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
static void SystemClockHSE_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    /* -1- Select HSI as system clock source to allow modification of the PLL configuration */
    RCC_ClkInitStruct.ClockType       = RCC_CLOCKTYPE_SYSCLK;
    RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_HSI;
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    /* -2- Enable HSE  Oscillator, select it as PLL source and finally activate the PLL */
    RCC_OscInitStruct.HSEState        = RCC_HSE_ON;
    RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;

    RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_OFF;
//  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;

    RCC_OscInitStruct.PLL.PLLQ        = 9;
    RCC_OscInitStruct.PLL.PLLM        = 25;
    RCC_OscInitStruct.PLL.PLLN        = 432;
    RCC_OscInitStruct.PLL.PLLP        = RCC_PLLP_DIV2;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    /* -Activate the Over Drive feature to reach 216 MHz as system clock frequency */
    if(HAL_PWREx_EnableOverDrive() != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    /* -3- Select the PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
    RCC_ClkInitStruct.APB1CLKDivider  = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider  = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.AHBCLKDivider   = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.ClockType       = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    /* -4- Optional: Disable HSI Oscillator (if the HSI is no more needed by the application)*/
    RCC_OscInitStruct.HSIState        = RCC_HSI_OFF;
    RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_NONE;
    RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSI;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }
}
//

/**
  * @brief  Switch the PLL source from HSE  to HSI, and select the PLL as SYSCLK
  *         source.
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            PLLM                           = 25
  *            PLLN                           = 432
  *            PLLP                           = 2
  *            PLLQ                           = 9
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
static void SystemClockHSI_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct    = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct    = {0};

    /* -1- Select HSE  as system clock source to allow modification of the PLL configuration */
    RCC_ClkInitStruct.ClockType             = RCC_CLOCKTYPE_SYSCLK;
    RCC_ClkInitStruct.SYSCLKSource          = RCC_SYSCLKSOURCE_HSE;
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    /* -2- Enable HSI Oscillator, select it as PLL source and finally activate the PLL */
    RCC_OscInitStruct.PLL.PLLQ              = 9;
    RCC_OscInitStruct.HSICalibrationValue   = 16;
    RCC_OscInitStruct.PLL.PLLM              = 16;
    RCC_OscInitStruct.PLL.PLLN              = 432;
    RCC_OscInitStruct.HSIState              = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState          = RCC_PLL_OFF;
//  RCC_OscInitStruct.PLL.PLLState          = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLP              = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLSource         = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.OscillatorType        = RCC_OSCILLATORTYPE_HSI;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    /* - Activate the Over Drive feature to reach 216 MHz as system clock frequency */
    if(HAL_PWREx_EnableOverDrive() != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    /* -3- Select the PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
    RCC_ClkInitStruct.APB1CLKDivider        = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider        = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.AHBCLKDivider         = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.SYSCLKSource          = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.ClockType             = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    /* -4- Optional: Disable HSE  Oscillator (if the HSE  is no more needed by the application) */
    RCC_OscInitStruct.HSEState              = RCC_HSE_OFF;
    RCC_OscInitStruct.PLL.PLLState          = RCC_PLL_NONE;
    RCC_OscInitStruct.OscillatorType        = RCC_OSCILLATORTYPE_HSE;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }
}
//

/* Public user function ------------------------------------------------------*/
#if ENABLE_USERBUTTON == 1
/**
  * @brief Return with user button state
  * @param None
  * @retval true When button pushed
	* @retval false When button released
  */
bool BSP_PB_Get(void)
{
    if(HAL_GPIO_ReadPin(USER_BTTN_GPIO_Port, USER_BTTN_Pin) == GPIO_PIN_SET)
    {
        HAL_Delay(100);
        return (HAL_GPIO_ReadPin(USER_BTTN_GPIO_Port, USER_BTTN_Pin) == GPIO_PIN_SET) ? true : false;
    }
    else
    {
        return false;
    }
}
//

/**
  * @brief Initialize User Push Button
  * @param None
  * @retval None
  */
void BSP_PB_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Pin       = USER_BTTN_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(USER_BTTN_GPIO_Port, &GPIO_InitStruct);
    /* Enable and set Button EXTI Interrupt to the lowest priority */
//  HAL_NVIC_SetPriority(EXTI0_IRQn, 0x0F, 0x00);
//  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}
//
#endif
//

void SwitchSystemClock(void)
{
    if(__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_HSI)
    {
        /* PLL source is HSI oscillator */
        /* Set SYSCLK frequency to 216 MHz, coming from the PLL which is clocked by HSE  */
        SystemClockHSE_Config();
    }
    else if(__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_HSI)
    {
        /* PLL source is HSI oscillator */
        /* Set SYSCLK frequency to 216 MHz, coming from the PLL which is clocked by HSE  */
        SystemClockHSE_Config();
    }
    else if(__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_HSE)
    {
        /* PLL source is HSE  oscillator */
        /* Set SYSCLK frequency to 216 MHz, coming from the PLL which is clocked by HSI */
        SystemClockHSI_Config();
    }
    else if(__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_HSE)
    {
        /* PLL source is HSI oscillator */
        /* Set SYSCLK frequency to 216 MHz, coming from the PLL which is clocked by HSE  */
        SystemClockHSE_Config();
    }
}
//

/**
  * @brief Enable GPIO Clock
  * @param None
  * @retval None
  */
void BSP_GPIOCLK_Enable(void)
{
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    __HAL_RCC_GPIOK_CLK_ENABLE();
}
//

/**
  * @brief BSP Delay Function
  * @param delay Time for delay in millisecond
  * @retval None
  */
void BSP_Delay(uint32_t delay)
{
    HAL_Delay(delay);
}
//

#if ENABLE_VBATTMON == 1
/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  AdcHandle : AdcHandle handle
  * @note   This example shows a simple way to report end of conversion, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
{
    Vbatt_Voltage = (float)ADCRawValue * 3.3f / 4096.0f;
}
//

/**
* @brief  This function handles DMA interrupt request.
* @param  None
* @retval None
*/
void DMA2_Stream0_IRQHandler(void)
{
    HAL_DMA_IRQHandler(AdcHandle.DMA_Handle);
}
//

void BSP_BattCheck_Init(void)
{
    ADC_ChannelConfTypeDef 	sConfig         = {0};
    GPIO_InitTypeDef        GPIO_InitStruct = {0};

    /* Peripheral clock enable */
    __HAL_RCC_ADC3_CLK_ENABLE();

    /* Enable DMA2 clock */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /** ADC1 GPIO Configuration PB0 ------> ADC1_IN8 */
    GPIO_InitStruct.Pull                    = GPIO_NOPULL;
    GPIO_InitStruct.Pin                     = VBAT_MEASURE_Pin;
    GPIO_InitStruct.Mode                    = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(VBAT_MEASURE_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Mode                    = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pin                     = CHARGE_STATUS_Pin;
    GPIO_InitStruct.Speed                   = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(CHARGE_STATUS_GPIO_Port, &GPIO_InitStruct);

    /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) */
    AdcHandle.Instance                      = ADC3;

    AdcHandle.Init.NbrOfConversion          = 1;
    AdcHandle.Init.ContinuousConvMode 		= ENABLE;
    AdcHandle.Init.DMAContinuousRequests 	= ENABLE;
    AdcHandle.Init.DiscontinuousConvMode 	= DISABLE;
    AdcHandle.Init.ScanConvMode             = ADC_SCAN_DISABLE;
    AdcHandle.Init.Resolution               = ADC_RESOLUTION_12B;
    AdcHandle.Init.ExternalTrigConv         = ADC_SOFTWARE_START;
    AdcHandle.Init.DataAlign                = ADC_DATAALIGN_RIGHT;
    AdcHandle.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;
    AdcHandle.Init.ClockPrescaler           = ADC_CLOCK_SYNC_PCLK_DIV4;
    AdcHandle.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;
    if(HAL_ADC_Init(&AdcHandle) != HAL_OK)
    {
        Error_Handler();
    }

    /* Set the parameters to be configured */
    hdma_adc.Instance                 = DMA2_Stream0;

    hdma_adc.Init.Mode                = DMA_CIRCULAR;
    hdma_adc.Init.Channel             = DMA_CHANNEL_2;
    hdma_adc.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_adc.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_adc.Init.Priority            = DMA_PRIORITY_HIGH;
    hdma_adc.Init.MemBurst            = DMA_MBURST_SINGLE;
    hdma_adc.Init.PeriphBurst         = DMA_PBURST_SINGLE;
    hdma_adc.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adc.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hdma_adc.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_adc.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_HALFFULL;
    HAL_DMA_Init(&hdma_adc);

    /* Associate the initialized DMA handle to the the ADC handle */
    __HAL_LINKDMA(&AdcHandle, DMA_Handle, hdma_adc);

    /* NVIC configuration for DMA transfer complete interrupt */
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);


    /** Select Battery Channel */
    sConfig.Rank                      = 1;
    sConfig.Offset                    = 0;
    sConfig.Channel                   = ADC_CHANNEL_3;
    sConfig.SamplingTime              = ADC_SAMPLETIME_3CYCLES;
    if(HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    if(HAL_ADC_Start_DMA(&AdcHandle, (uint32_t *)&ADCRawValue, 1) != HAL_OK)
    {
        /* Start Conversation Error */
        Error_Handler();
    }
}
//

float BSP_Get_BatteryVoltageF(void)
{
    return Vbatt_Voltage;
}
//

uint16_t BSP_Get_BatteryVoltageUI16(void)
{
    return (uint16_t)(Vbatt_Voltage * (float)100.0f);
}
//
#endif
//

#if ENABLE_UART == 1
/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pull        = GPIO_NOPULL;
    GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_VERY_HIGH;
  
    if(huart->Instance == USART1)
    {
        /* Peripheral clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();

        /** USART1 GPIO Configuration    
          * PA12    ------> USART1_RTS
          * PA11    ------> USART1_CTS
          * PA10    ------> USART1_RX
          * PA9     ------> USART1_TX 
          */
        GPIO_InitStruct.Alternate   = GPIO_AF7_USART1;
        GPIO_InitStruct.Pin         = WiFi_RTS_Pin | WiFi_CTS_Pin | WiFi_RX_Pin | WiFi_TX_Pin;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /*Configure GPIO pin Output Level */
        HAL_GPIO_WritePin(WiFi_PWE_GPIO_Port, WiFi_PWE_Pin, GPIO_PIN_RESET);

        /*Configure GPIO pin : WiFi_PWE_Pin */
        GPIO_InitStruct.Pin         = WiFi_PWE_Pin;
        GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Mode        = GPIO_MODE_OUTPUT_PP;
        HAL_GPIO_Init(WiFi_PWE_GPIO_Port, &GPIO_InitStruct);

        /* USART1 interrupt Init */
        HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        
        /*Configure GPIO pin : WiFi_IRQ_Pin */
        GPIO_InitStruct.Pin         = WiFi_IRQ_Pin;
        GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Mode        = GPIO_MODE_IT_RISING;
        HAL_GPIO_Init(WiFi_IRQ_GPIO_Port, &GPIO_InitStruct);
    }
    else if(huart->Instance==USART3)
    {
        /* Peripheral clock enable */
        __HAL_RCC_USART3_CLK_ENABLE();

        /** USART3 GPIO Configuration    
          * PB10    ------> USART3_TX
          * PB11    ------> USART3_RX 
          */
        GPIO_InitStruct.Alternate   = GPIO_AF7_USART3;
        GPIO_InitStruct.Pin         = RS485_TX_Pin | RS485_RX_Pin;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /*Configure GPIO pin : RS485_DE_Pin */
        GPIO_InitStruct.Pin         = RS485_DE_Pin;
        GPIO_InitStruct.Mode        = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(RS485_DE_GPIO_Port, &GPIO_InitStruct);
    }
    else if(huart->Instance==USART6)
    {
        /* Peripheral clock enable */
        __HAL_RCC_USART6_CLK_ENABLE();

        /** USART6 GPIO Configuration    
          * PG14    ------> USART6_TX
          * PG13    ------> USART6_CTS
          * PG12    ------> USART6_RTS
          * PC7     ------> USART6_RX 
          */
        GPIO_InitStruct.Alternate   = GPIO_AF8_USART6;
        GPIO_InitStruct.Pin         = GSM_TX_Pin | GSM_CTS_Pin | GSM_RTS_Pin;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

        GPIO_InitStruct.Pin         = GSM_RX_Pin;
        GPIO_InitStruct.Alternate   = GPIO_AF8_USART6;
        HAL_GPIO_Init(GSM_RX_GPIO_Port, &GPIO_InitStruct);

        /*Configure GPIO pins : GSM_RI_Pin */
        GPIO_InitStruct.Pin         = GSM_RI_Pin;
        GPIO_InitStruct.Mode        = GPIO_MODE_INPUT;
        HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

        /*Configure GPIO pins : GSM_RST_Pin */
        GPIO_InitStruct.Pin         = GSM_RST_Pin;
        GPIO_InitStruct.Mode        = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GSM_RST_GPIO_Port, &GPIO_InitStruct);

        /*Configure GPIO pins : GSM_PWE_Pin */
        GPIO_InitStruct.Pin         = GSM_PWE_Pin;
        HAL_GPIO_Init(GSM_PWE_GPIO_Port, &GPIO_InitStruct);
    }
}
//

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
    if(huart->Instance==USART1)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();

        /** USART1 GPIO Configuration    
          * PA12    ------> USART1_RTS
          * PA11    ------> USART1_CTS
          * PA10    ------> USART1_RX
          * PA9     ------> USART1_TX 
          */
        HAL_GPIO_DeInit(GPIOA, WiFi_RTS_Pin | WiFi_CTS_Pin | WiFi_RX_Pin | WiFi_TX_Pin);
    }
    else if(huart->Instance==USART3)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART3_CLK_DISABLE();

        /** USART3 GPIO Configuration    
          * PB10     ------> USART3_TX
          * PB11     ------> USART3_RX 
          */
        HAL_GPIO_DeInit(GPIOB, RS485_TX_Pin | RS485_RX_Pin | RS485_DE_Pin);
    }
    else if(huart->Instance==USART6)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART6_CLK_DISABLE();

        /** USART6 GPIO Configuration    
          * PG14    ------> USART6_TX
          * PG13    ------> USART6_CTS
          * PG12    ------> USART6_RTS
          * PC7     ------> USART6_RX 
          */
        HAL_GPIO_DeInit(GPIOG, GSM_TX_Pin | GSM_CTS_Pin | GSM_RTS_Pin);

        HAL_GPIO_DeInit(GSM_RX_GPIO_Port, GSM_RX_Pin);

        HAL_GPIO_DeInit(GPIOH, GSM_PWE_Pin);
        HAL_GPIO_DeInit(GPIOJ, GSM_RI_Pin | GSM_RST_Pin);
    }
}
//
#endif
//

/********************************* LINK CAMERA ********************************/
#if ENABLE_CAMERAI2C == 1
/**
* @brief I2C MSP Initialization
* This function configures the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
  
    GPIO_InitStruct.Pull        = GPIO_PULLUP;
    GPIO_InitStruct.Mode        = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_VERY_HIGH;
  
    if(hi2c->Instance==I2C1)
    {
        /** I2C1 GPIO Configuration    
          * PB8 ------> I2C1_SCL
          * PB7 ------> I2C1_SDA 
          */
        GPIO_InitStruct.Alternate   = GPIO_AF4_I2C1;
        GPIO_InitStruct.Pin         = DCMI_SCL_Pin | DCMI_SDA_Pin;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_I2C1_CLK_ENABLE();
    }
    else if(hi2c->Instance==I2C2)
    {
        /**I2C2 GPIO Configuration    
          * PH4 ------> I2C2_SCL
          * PH5 ------> I2C2_SDA 
          */
        GPIO_InitStruct.Pin         = LTDC_SCL_Pin | LTDC_SDA_Pin;
        HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_I2C2_CLK_ENABLE();
    }
    else if(hi2c->Instance==I2C3)
    {
        /** I2C3 GPIO Configuration    
          * PA8     ------> I2C3_SCL
          * PH8     ------> I2C3_SDA 
          */
        GPIO_InitStruct.Pin         = EXP_SCL_Pin;
        HAL_GPIO_Init(EXP_SCL_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin         = EXP_SDA_Pin;
        HAL_GPIO_Init(EXP_SDA_GPIO_Port, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_I2C3_CLK_ENABLE();
    }
}
//

/**
* @brief I2C MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();
  
    /**I2C1 GPIO Configuration    
    PB8     ------> I2C1_SCL
    PB7     ------> I2C1_SDA 
    */
    HAL_GPIO_DeInit(GPIOB, DCMI_SCL_Pin|DCMI_SDA_Pin);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
  else if(hi2c->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspDeInit 0 */

  /* USER CODE END I2C2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C2_CLK_DISABLE();
  
    /**I2C2 GPIO Configuration    
    PH4     ------> I2C2_SCL
    PH5     ------> I2C2_SDA 
    */
    HAL_GPIO_DeInit(GPIOH, LTDC_SCL_Pin|LTDC_SDA_Pin);

  /* USER CODE BEGIN I2C2_MspDeInit 1 */

  /* USER CODE END I2C2_MspDeInit 1 */
  }
  else if(hi2c->Instance==I2C3)
  {
  /* USER CODE BEGIN I2C3_MspDeInit 0 */

  /* USER CODE END I2C3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C3_CLK_DISABLE();
  
    /**I2C3 GPIO Configuration    
    PA8     ------> I2C3_SCL
    PH8     ------> I2C3_SDA 
    */
    HAL_GPIO_DeInit(EXP_SCL_GPIO_Port, EXP_SCL_Pin);

    HAL_GPIO_DeInit(EXP_SDA_GPIO_Port, EXP_SDA_Pin);

  /* USER CODE BEGIN I2C3_MspDeInit 1 */

  /* USER CODE END I2C3_MspDeInit 1 */
  }

}
//

/**
  * @brief  Manages error callback by re-initializing I2C.
  * @param  Addr: I2C Address
  */
static void I2Cx_Error(uint8_t Addr)
{
    /* De-initialize the I2C communication bus */
    HAL_I2C_DeInit(&hi2cSTF7SOM);

    /* Re-Initialize the I2C communication bus */
    CAMERA_IO_Init();
}
//

/**
  * @brief  Initializes Camera low level.
  */
void CAMERA_IO_Init(void) 
{
    if(HAL_I2C_GetState(&hi2cSTF7SOM) == HAL_I2C_STATE_RESET)
    {
        hi2cSTF7SOM.Instance              = I2C1;
        
        hi2cSTF7SOM.Init.OwnAddress1      = 0;
        hi2cSTF7SOM.Init.OwnAddress2      = 0;
        hi2cSTF7SOM.Init.Timing           = 0x00C0EAFF;
        hi2cSTF7SOM.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
        hi2cSTF7SOM.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
        hi2cSTF7SOM.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
        hi2cSTF7SOM.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
        hi2cSTF7SOM.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
        
        /* Init the I2C */
        HAL_I2C_Init(&hi2cSTF7SOM);
    }
}
//

/**
  * @brief  Camera writes single data.
  * @param  Addr: I2C address
  * @param  Reg: Register address 
  * @param  Value: Data to be written
  */
void CAMERA_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
    HAL_StatusTypeDef status = HAL_OK;

    status = HAL_I2C_Mem_Write(&hi2cSTF7SOM, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, 100); 

    /* Check the communication status */
    if(status != HAL_OK)
    {
        /* Execute user timeout callback */
        I2Cx_Error(Addr);
    }
}
//

/**
  * @brief  Camera reads single data.
  * @param  Addr: I2C address
  * @param  Reg: Register address 
  * @retval Read data
  */
uint8_t CAMERA_IO_Read(uint8_t Addr, uint8_t Reg)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint8_t Value = 0;

    status = HAL_I2C_Mem_Read(&hi2cSTF7SOM, Addr, Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, 1000);

    /* Check the communication status */
    if(status != HAL_OK)
    {
        /* Execute user timeout callback */
        I2Cx_Error(Addr);
    }
    return Value;   
}
//

/**
  * @brief  Camera delay 
  * @param  Delay: Delay in ms
  */
void CAMERA_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}
//
#endif
//


/************************ (C) COPYRIGHT YiLDiZ IoT END OF FILE ************************/
