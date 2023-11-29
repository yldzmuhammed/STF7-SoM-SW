/**
  ******************************************************************************
  * @file    AT070TN92.h
  * @author  yldzmuhammed
  * @brief   This file contains all the constants parameters for the AT070TN92
  *          LCD component.
  ******************************************************************************
  * @attention
  *
  * experiment platform: YiLDiZ IoT STF7-SoM Board BSP Library
  * web-site: https://www.yildiziot.com
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT070TN92_H
#define __AT070TN92_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/  

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @addtogroup AT070TN92
  * @{
  */

/** @defgroup AT070TN92_Exported_Types
  * @{
  */
   
/**
  * @}
  */ 

/** @defgroup AT070TN92_Exported_Constants
  * @{
  */

/** 
  * @brief  AT070TN92 Size  
  */     
#define  AT070TN92_WIDTH                    ((uint16_t)800)       /* LCD PIXEL WIDTH            */
#define  AT070TN92_HEIGHT                   ((uint16_t)480)       /* LCD PIXEL HEIGHT           */

/** 
  * @brief  AT070TN92 Timing  
  */
#define  AT070TN92_HSYNC                    ((uint16_t)1)         /* Horizontal synchronization */
#define  AT070TN92_HBP                      ((uint16_t)46)        /* Horizontal back porch      */
#define  AT070TN92_HFP                      ((uint16_t)210)       /* Horizontal front porch     */
#define  AT070TN92_VSYNC                    ((uint16_t)3)         /* Vertical synchronization   */
#define  AT070TN92_VBP                      ((uint16_t)23)        /* Vertical back porch        */
#define  AT070TN92_VFP                      ((uint16_t)22)        /* Vertical front porch       */

/**
  * @brief  AT070TN92 frequency multiplier
  */
#define  AT070TN92_PLLSAIN                  192

/**
  * @brief  AT070TN92 PLLSAIDIVR divider
  */
#define AT070TN62_PLLSAIDIVR                RCC_PLLSAIDIVR_4
/**
  * @brief  AT070TN92 frequency divider  
  */
#define AT070TN92_FREQUENCY_DIVIDER         5                       /* LCD Frequency divider      */
/**
  * @}
  */
  
/** @defgroup AT070TN92_Exported_Functions
  * @{
  */

/**
  * @}
  */    
#ifdef __cplusplus
}
#endif

#endif /* __AT070TN92_H */
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
