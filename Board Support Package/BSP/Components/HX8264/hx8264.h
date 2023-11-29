/**
  ******************************************************************************
  * @file    HX8264.h
  * @author  yldzmuhammed
  * @brief   This file contains all the constants parameters for the HX8264-CT672B
  *          LCD component.
  ******************************************************************************
  * @attention
  *
  * experiment platform: YiLDiZ IoT STF7-SoM Board BSP Library
  * web-site: https://www.yildiziot.com
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HX8264_H
#define __HX8264_H

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
  
/** @addtogroup HX8264
  * @{
  */

/** @defgroup HX8264_Exported_Types
  * @{
  */
   
/**
  * @}
  */ 

/** @defgroup HX8264_Exported_Constants
  * @{
  */

/** 
  * @brief  HX8264 Size  
  */     
#define  HX8264_WIDTH    ((uint16_t)800)          /* LCD PIXEL WIDTH            */
#define  HX8264_HEIGHT   ((uint16_t)480)          /* LCD PIXEL HEIGHT           */

/** 
  * @brief  HX8264 Timing  
  */     
#define  HX8264_HSYNC            ((uint16_t)1)    /* Horizontal synchronization */
#define  HX8264_HBP              ((uint16_t)46)   /* Horizontal back porch      */
#define  HX8264_HFP              ((uint16_t)210)  /* Horizontal front porch     */
#define  HX8264_VSYNC            ((uint16_t)3)    /* Vertical synchronization   */
#define  HX8264_VBP              ((uint16_t)23)   /* Vertical back porch        */
#define  HX8264_VFP              ((uint16_t)22)   /* Vertical front porch       */

/** 
  * @brief  HX8264 frequency divider  
  */    
#define  HX8264_FREQUENCY_DIVIDER    4            /* LCD Frequency divider      */
/**
  * @}
  */
  
/** @defgroup HX8264_Exported_Functions
  * @{
  */

/**
  * @}
  */    
#ifdef __cplusplus
}
#endif

#endif /* __HX8264_H */
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
