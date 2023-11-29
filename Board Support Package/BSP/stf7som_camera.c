/**
  ******************************************************************************
  * @file    stf7som_camera.h
  * @author  yldzmuhammed
  * @version V1.0
  * @date    14-06-2020
  * @brief   This file contains the common defines and functions prototypes for
  *          the stf7som_camera.c driver.
  ******************************************************************************
  * @attention
  *
  * experiment platform: YiLDiZ IoT STF7-SoM Board BSP Library
  * web-site: https://www.yildiziot.com
  ******************************************************************************
*/

/* File Info: ------------------------------------------------------------------
                                   User NOTES
1. How to use this driver:
--------------------------
   - This driver is used to drive the camera.
   - The OV2640 component driver MUST be included with this driver.          

2. Driver description:
---------------------
  + Initialization steps:
     o Initialize the camera using the BSP_CAMERA_Init() function.
     o Start the camera capture/snapshot using the CAMERA_Start() function.
     o Suspend, resume or stop the camera capture using the following functions:
      - BSP_CAMERA_Suspend()
      - BSP_CAMERA_Resume()
      - BSP_CAMERA_Stop()
      
  + Options
     o Increase or decrease on the fly the brightness and/or contrast
       using the following function:
       - BSP_CAMERA_ContrastBrightnessConfig 
     o Add a special effect on the fly using the following functions:
       - BSP_CAMERA_BlackWhiteConfig()
       - BSP_CAMERA_ColorEffectConfig()  
      
------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stf7som_camera.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STF7SOM
  * @{
  */
    
/** @defgroup STF7SOM STF7SOM CAMERA
  * @{
  */ 

/** @defgroup STF7SOM_Private_Variables STF7SOM CAMERA Private Variables
  * @{
  */ 
static DCMI_HandleTypeDef   hdcmiSTF7SoM;
static DMA_HandleTypeDef    hdmaSTF7SoM;
       CAMERA_DrvTypeDef   *camera_drv;
static uint32_t             current_resolution;
/**
  * @}
  */ 
  
/** @defgroup STF7SOM_Private_FunctionPrototypes STF7SOM CAMERA Private FunctionPrototypes
  * @{
  */
static uint32_t GetSize(uint32_t resolution);
/**
  * @}
  */ 
  
/** @defgroup STF7SOM_Private_Functions STF7SOM CAMERA Private Functions
  * @{
  */

/**
  * @brief  Initializes the camera.
  * @param  Resolution: Camera Resolution 
  * @retval Camera status
  */
uint8_t BSP_CAMERA_Init(uint32_t Resolution)
{ 
  DCMI_HandleTypeDef *phdcmi;
  
  uint8_t ret = CAMERA_ERROR;
  
  /* Get the DCMI handle structure */
  phdcmi = &hdcmiSTF7SoM;
  
  /*** Configures the DCMI to interface with the camera module ***/
  /* DCMI configuration */
  phdcmi->Instance              = DCMI;
  phdcmi->Init.CaptureRate      = DCMI_CR_ALL_FRAME;  
  phdcmi->Init.HSPolarity       = DCMI_HSPOLARITY_LOW;
  phdcmi->Init.VSPolarity       = DCMI_VSPOLARITY_LOW;
  phdcmi->Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
  phdcmi->Init.SynchroMode      = DCMI_SYNCHRO_HARDWARE;
  phdcmi->Init.PCKPolarity      = DCMI_PCKPOLARITY_RISING;

  /* DCMI Initialization */
  HAL_DCMI_Init(phdcmi);
  
  if(ov2640_ReadID(CAMERA_I2C_ADDRESS) == OV2640_ID)
  { 
    /* Initialize the camera driver structure */
    camera_drv = &ov2640_drv;     
    
    /* Camera Init */   
    camera_drv->Init(CAMERA_I2C_ADDRESS, Resolution);
    
    /* Return CAMERA_OK status */
    ret = CAMERA_OK;
  } 
  
  current_resolution = Resolution;
  
  return ret;
}
//

/**
  * @brief  Stop the CAMERA capture 
  * @retval Camera status
  */
uint8_t BSP_CAMERA_Stop(void) 
{
  DCMI_HandleTypeDef *phdcmi;
  
  uint8_t ret = CAMERA_ERROR;
  
  /* Get the DCMI handle structure */
  phdcmi = &hdcmiSTF7SoM;
  
  if(HAL_DCMI_Stop(phdcmi) == HAL_OK)
  {
    ret = CAMERA_OK;
  }
  
  return ret;
}
//

/**
  * @brief  Starts the camera capture in snapshot mode.
  * @param  buff: pointer to the camera output buffer
  */
void BSP_CAMERA_SnapshotStart(uint8_t *buff)
{ 
  /* Start the camera capture */
  HAL_DCMI_Start_DMA(&hdcmiSTF7SoM, DCMI_MODE_SNAPSHOT, (uint32_t)buff, GetSize(current_resolution));  
}
//

/**
  * @brief  Starts the camera capture in continuous mode.
  * @param  buff: pointer to the camera output buffer
  */
void BSP_CAMERA_ContinuousStart(uint8_t *buff)
{ 
  /* Start the camera capture */
  HAL_DCMI_Start_DMA(&hdcmiSTF7SoM, DCMI_MODE_CONTINUOUS, (uint32_t)buff, GetSize(current_resolution));  
}
//

/**
  * @brief Resume the CAMERA capture
  */
void BSP_CAMERA_Resume(void) 
{
  /* Start the Camera Capture */
  HAL_DCMI_Resume(&hdcmiSTF7SoM);
}
//

/**
  * @brief Suspend the CAMERA capture 
  */
void BSP_CAMERA_Suspend(void) 
{
  /* Suspend the Camera Capture */
  HAL_DCMI_Suspend(&hdcmiSTF7SoM);
}
//

/**
  * @brief  Configures the camera white balance.
  * @param  Mode: black_white mode
  *          This parameter can be one of the following values:
  *            @arg  CAMERA_BLACK_WHITE_BW
  *            @arg  CAMERA_BLACK_WHITE_NEGATIVE
  *            @arg  CAMERA_BLACK_WHITE_BW_NEGATIVE
  *            @arg  CAMERA_BLACK_WHITE_NORMAL       
  */
void BSP_CAMERA_BlackWhiteConfig(uint32_t Mode)
{
  if(camera_drv->Config != NULL)
  {
    camera_drv->Config(CAMERA_I2C_ADDRESS, CAMERA_BLACK_WHITE, Mode, 0);
  }  
}
//

/**
  * @brief  Configures the camera color effect.
  * @param  Effect: Color effect
  *          This parameter can be one of the following values:
  *            @arg  CAMERA_COLOR_EFFECT_ANTIQUE               
  *            @arg  CAMERA_COLOR_EFFECT_BLUE        
  *            @arg  CAMERA_COLOR_EFFECT_GREEN    
  *            @arg  CAMERA_COLOR_EFFECT_RED        
  */
void BSP_CAMERA_ColorEffectConfig(uint32_t Effect)
{
  if(camera_drv->Config != NULL)
  {
    camera_drv->Config(CAMERA_I2C_ADDRESS, CAMERA_COLOR_EFFECT, Effect, 0);
  }  
}
//

/**
  * @brief  Configures the camera contrast and brightness.
  * @param  contrast_level: Contrast level
  *          This parameter can be one of the following values:
  *            @arg  CAMERA_CONTRAST_LEVEL4: for contrast +2
  *            @arg  CAMERA_CONTRAST_LEVEL3: for contrast +1
  *            @arg  CAMERA_CONTRAST_LEVEL2: for contrast  0
  *            @arg  CAMERA_CONTRAST_LEVEL1: for contrast -1
  *            @arg  CAMERA_CONTRAST_LEVEL0: for contrast -2
  * @param  brightness_level: Contrast level
  *          This parameter can be one of the following values:
  *            @arg  CAMERA_BRIGHTNESS_LEVEL4: for brightness +2
  *            @arg  CAMERA_BRIGHTNESS_LEVEL3: for brightness +1
  *            @arg  CAMERA_BRIGHTNESS_LEVEL2: for brightness  0
  *            @arg  CAMERA_BRIGHTNESS_LEVEL1: for brightness -1
  *            @arg  CAMERA_BRIGHTNESS_LEVEL0: for brightness -2    
  */
void BSP_CAMERA_ContrastBrightnessConfig(uint32_t contrast_level, uint32_t brightness_level)
{
  if(camera_drv->Config != NULL)
  {
    camera_drv->Config(CAMERA_I2C_ADDRESS, CAMERA_CONTRAST_BRIGHTNESS, contrast_level, brightness_level);
  }  
}
//

/**
  * @brief  Get the capture size.
  * @param  resolution: the current resolution.
  * @retval capture size.
  */
static uint32_t GetSize(uint32_t resolution)
{ 
  uint32_t size = 0;
  
  /* Get capture size */
  switch (resolution)
  {
  case CAMERA_R160x120:
    {
      size =  0x2580;
    }
    break;    
  case CAMERA_R320x240:
    {
      size =  0x9600;
    }
    break;
  case CAMERA_R480x272:
    {
      size =  0xFF00;
    }
    break;
  case CAMERA_R640x480:
    {
      size =  0x25800;
    }    
    break;
  default:
    {
      break;
    }
  }
  
  return size;
}
//

/* Middleware Init Functions -------------------------------------------------*/
/**
* @brief DCMI MSP Initialization
* This function configures the hardware resources used in this example
* @param hdcmi: DCMI handle pointer
* @retval None
*/
void HAL_DCMI_MspInit(DCMI_HandleTypeDef* hdcmi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    GPIO_InitStruct.Pull        = GPIO_PULLUP;
    GPIO_InitStruct.Alternate   = GPIO_AF13_DCMI;
    GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_VERY_HIGH;
        
    if(hdcmi->Instance==DCMI)
    {
        /* Peripheral clock enable */
        __HAL_RCC_DCMI_CLK_ENABLE();

        /* Enable DMA2 clock */
        __HAL_RCC_DMA2_CLK_ENABLE(); 

        /** DCMI GPIO Configuration
          * PC6     ------> DCMI_D0
          * PH10    ------> DCMI_D1
          * PG10    ------> DCMI_D2
          * PH12    ------> DCMI_D3
          * PH14    ------> DCMI_D4
          * PI4     ------> DCMI_D5
          * PI6     ------> DCMI_D6
          * PE6     ------> DCMI_D7
          * PA4     ------> DCMI_HSYNC
          * PI5     ------> DCMI_VSYNC
          * PA6     ------> DCMI_PIXCLK
          */
        GPIO_InitStruct.Pin         = DCMI_D0_Pin;
        HAL_GPIO_Init(DCMI_D0_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin         = DCMI_D2_Pin;
        HAL_GPIO_Init(DCMI_D2_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin         = DCMI_D4_Pin|DCMI_D3_Pin|DCMI_D1_Pin;
        HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

        GPIO_InitStruct.Pin         = DCMI_D5_Pin|DCMI_VSYNC_Pin|DCMI_D6_Pin;
        HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

        GPIO_InitStruct.Pin         = DCMI_D7_Pin;
        HAL_GPIO_Init(DCMI_D7_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin         = DCMI_HSYNC_Pin|DCMI_PIXCLK_Pin;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        /*Configure GPIO pins : LTDC_DISP_Pin DCMI_PWRE_Pin DCMI_RST_Pin */
        GPIO_InitStruct.Pull  = GPIO_NOPULL;
        GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Pin   = DCMI_PWRE_Pin | DCMI_RST_Pin;
        HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

        /*Configure GPIO pin Output Level */
        HAL_GPIO_WritePin(GPIOI, DCMI_PWRE_Pin, GPIO_PIN_RESET);

        HAL_Delay(100);

        /*Configure GPIO pin Output Level */
        HAL_GPIO_WritePin(GPIOI, DCMI_PWRE_Pin | DCMI_RST_Pin, GPIO_PIN_SET);

        HAL_Delay(100);

        /*Configure GPIO pin Output Level */
        HAL_GPIO_WritePin(GPIOI, DCMI_RST_Pin, GPIO_PIN_RESET);

        HAL_Delay(100);

        /*Configure GPIO pin Output Level */
        HAL_GPIO_WritePin(GPIOI, DCMI_RST_Pin, GPIO_PIN_SET);

        HAL_Delay(100);

        /*** Configure the DMA ***/
        /* Set the parameters to be configured */
        hdmaSTF7SoM.Init.Mode                = DMA_CIRCULAR;
        hdmaSTF7SoM.Init.Channel             = DMA_CHANNEL_1;
        hdmaSTF7SoM.Init.MemInc              = DMA_MINC_ENABLE;
        hdmaSTF7SoM.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdmaSTF7SoM.Init.Priority            = DMA_PRIORITY_HIGH;
        hdmaSTF7SoM.Init.MemBurst            = DMA_MBURST_SINGLE;
        hdmaSTF7SoM.Init.PeriphBurst         = DMA_PBURST_SINGLE;
        hdmaSTF7SoM.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
        hdmaSTF7SoM.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        hdmaSTF7SoM.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        hdmaSTF7SoM.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdmaSTF7SoM.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;

        hdmaSTF7SoM.Instance = DMA2_Stream1;

        /* Associate the initialized DMA handle to the DCMI handle */
        __HAL_LINKDMA(hdcmi, DMA_Handle, hdmaSTF7SoM);

        /*** Configure the NVIC for DCMI and DMA ***/
        /* NVIC configuration for DCMI transfer complete interrupt */
        HAL_NVIC_SetPriority(DCMI_IRQn, 0x0F, 0);
        HAL_NVIC_EnableIRQ(DCMI_IRQn);  

        /* NVIC configuration for DMA2D transfer complete interrupt */
        HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0x0F, 0);
        HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);

        /* Configure the DMA stream */
        HAL_DMA_Init(hdcmi->DMA_Handle);  
    }
}
//

/**
* @brief DCMI MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hdcmi: DCMI handle pointer
* @retval None
*/
void HAL_DCMI_MspDeInit(DCMI_HandleTypeDef* hdcmi)
{
    if(hdcmi->Instance==DCMI)
    {
        /* Peripheral clock disable */
        __HAL_RCC_DCMI_CLK_DISABLE();

        /** DCMI GPIO Configuration    
          * PE6     ------> DCMI_D7
          * PI4     ------> DCMI_D5
          * PG10     ------> DCMI_D2
          * PI5     ------> DCMI_VSYNC
          * PI6     ------> DCMI_D6
          * PH14     ------> DCMI_D4
          * PC6     ------> DCMI_D0
          * PH12     ------> DCMI_D3
          * PA4     ------> DCMI_HSYNC
          * PA6     ------> DCMI_PIXCLK
          * PH10     ------> DCMI_D1 
          */
        HAL_GPIO_DeInit(DCMI_D7_GPIO_Port, DCMI_D7_Pin);
        HAL_GPIO_DeInit(DCMI_D2_GPIO_Port, DCMI_D2_Pin);
        HAL_GPIO_DeInit(DCMI_D0_GPIO_Port, DCMI_D0_Pin);
        HAL_GPIO_DeInit(GPIOA, DCMI_HSYNC_Pin   | DCMI_PIXCLK_Pin);
        HAL_GPIO_DeInit(GPIOH, DCMI_D4_Pin      | DCMI_D3_Pin    | DCMI_D1_Pin);
        HAL_GPIO_DeInit(GPIOI, DCMI_D5_Pin      | DCMI_VSYNC_Pin | DCMI_D6_Pin);
    }
}
//

/* ISR Callback Functions  ---------------------------------------------------*/
/**
  * @brief  Error callback.
  */
__weak void BSP_CAMERA_ErrorCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_ErrorCallback could be implemented in the user file
   */
}
//

/**
  * @brief  Line Event callback.
  */
__weak void BSP_CAMERA_LineEventCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_LineEventCallback could be implemented in the user file
   */
}
//

/**
  * @brief  VSYNC Event callback.
  */
__weak void BSP_CAMERA_VsyncEventCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_VsyncEventCallback could be implemented in the user file
   */
}
//

/**
  * @brief  Frame Event callback.
  */
__weak void BSP_CAMERA_FrameEventCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_FrameEventCallback could be implemented in the user file
   */
}
//

/**
  * @brief  Error callback
  * @param  hdcmi: pointer to the DCMI handle  
  */
void HAL_DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi)
{        
  BSP_CAMERA_ErrorCallback();
}
//

/**
  * @brief  Line event callback
  * @param  hdcmi: pointer to the DCMI handle  
  */
void HAL_DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi)
{        
  BSP_CAMERA_LineEventCallback();
}
//

/**
  * @brief  VSYNC event callback
  * @param  hdcmi: pointer to the DCMI handle  
  */
void HAL_DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi)
{        
  BSP_CAMERA_VsyncEventCallback();
}
//

/**
  * @brief  Frame event callback
  * @param  hdcmi: pointer to the DCMI handle  
  */
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{        
  BSP_CAMERA_FrameEventCallback();
}
//

/* Interrupt Service Routines  -----------------------------------------------*/
/**
  * @brief  This function handles DCMI Handler.
  * @param  None
  * @retval None
  */
void DCMI_IRQHandler(void)
{
  HAL_DCMI_IRQHandler(&hdcmiSTF7SoM);
}
//

/**
  * @brief  This function handles DMA2 Stream1 Handler.
  * @param  None
  * @retval None
  */
void DMA2_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(hdcmiSTF7SoM.DMA_Handle);
}
//

/**
  * @}
  */  
  
/**
  * @}
  */
  
/**
  * @}
  */
  
/**
  * @}
  */      

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
