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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STF7SOM_CAMERA_H
#define __STF7SOM_CAMERA_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
/* Include Camera component Driver */
#include "ov2640.h"
#include "stm32f7xx_hal.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STF7SOM
  * @{
  */
    
/** @addtogroup STF7SOM_CAMERA
  * @{
  */ 
   
/** @defgroup STF7SOM_CAMERA_Exported_Types STF7SOM CAMERA Exported Types
  * @{
  */
  
/** 
  * @brief  Camera State structures definition  
  */  
typedef enum 
{
  CAMERA_OK       = 0x00,
  CAMERA_ERROR    = 0x01,
  CAMERA_TIMEOUT  = 0x02 
}Camera_StatusTypeDef;
  
/**
  * @}
  */ 
 
/** @defgroup STF7SOM_CAMERA_Exported_Constants STF7SOM CAMERA Exported Constants
  * @{
  */
#define RESOLUTION_R160x120         CAMERA_R160x120      /* QQVGA Resolution     */
#define RESOLUTION_R320x240         CAMERA_R320x240      /* QVGA Resolution      */
#define RESOLUTION_R480x272         CAMERA_R480x272      /* 480x272 Resolution   */
#define RESOLUTION_R640x480         CAMERA_R640x480      /* VGA Resolution       */
#define CAMERA_I2C_ADDRESS          0x60


#define DCMI_LIGHT_Pin             GPIO_PIN_0
#define DCMI_PWRE_Pin              GPIO_PIN_7
#define DCMI_RST_Pin               GPIO_PIN_12

#define DCMI_D0_Pin                 GPIO_PIN_6
#define DCMI_D1_Pin                 GPIO_PIN_10
#define DCMI_D2_Pin                 GPIO_PIN_10
#define DCMI_D3_Pin                 GPIO_PIN_12
#define DCMI_D4_Pin                 GPIO_PIN_14
#define DCMI_D5_Pin                 GPIO_PIN_4
#define DCMI_D6_Pin                 GPIO_PIN_6
#define DCMI_D7_Pin                 GPIO_PIN_6
#define DCMI_HSYNC_Pin              GPIO_PIN_4
#define DCMI_VSYNC_Pin              GPIO_PIN_5
#define DCMI_PIXCLK_Pin             GPIO_PIN_6

#define DCMI_D0_GPIO_Port           GPIOC
#define DCMI_D1_GPIO_Port           GPIOH
#define DCMI_D2_GPIO_Port           GPIOG
#define DCMI_D3_GPIO_Port           GPIOH
#define DCMI_D4_GPIO_Port           GPIOH
#define DCMI_D5_GPIO_Port           GPIOI
#define DCMI_D6_GPIO_Port           GPIOI
#define DCMI_D7_GPIO_Port           GPIOE
#define DCMI_HSYNC_GPIO_Port        GPIOA
#define DCMI_VSYNC_GPIO_Port        GPIOI
#define DCMI_PIXCLK_GPIO_Port       GPIOA


/**
  * @}
  */

/** @defgroup STF7SOM_CAMERA_Exported_Functions STF7SOM CAMERA Exported Functions
  * @{
  */    
uint8_t BSP_CAMERA_Init(uint32_t Resolution);

uint8_t BSP_CAMERA_Stop(void);

void    BSP_CAMERA_SnapshotStart(uint8_t *buff);
void    BSP_CAMERA_ContinuousStart(uint8_t *buff);

void    BSP_CAMERA_Resume(void);
void    BSP_CAMERA_Suspend(void);

void    BSP_CAMERA_BlackWhiteConfig(uint32_t Mode);
void    BSP_CAMERA_ColorEffectConfig(uint32_t Effect);
void    BSP_CAMERA_ContrastBrightnessConfig(uint32_t contrast_level, uint32_t brightness_level);

void    BSP_CAMERA_ErrorCallback(void);
void    BSP_CAMERA_LineEventCallback(void);
void    BSP_CAMERA_VsyncEventCallback(void);
void    BSP_CAMERA_FrameEventCallback(void);

   
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

#ifdef __cplusplus
}
#endif

#endif /* __STF7SOM_CAMERA_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
