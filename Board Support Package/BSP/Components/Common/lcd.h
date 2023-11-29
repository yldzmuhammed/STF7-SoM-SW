/**
  ******************************************************************************
  * @file    lcd.h
  * @author  MCD Application Team
  * @version V4.0.1
  * @date    21-July-2015
  * @brief   This file contains all the functions prototypes for the LCD driver.   
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_H
#define __LCD_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
   
/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup LCD
  * @{
  */
 
/** @defgroup LCD_Exported_Types
  * @{
  */
#define HX8264                              (1)
#define ADAFRUIT_LCD                        (2)
#define YILDIZIoT_TFT7                      (3)
#define CUSTOM_SCREEN                       (4)


#define LCD_SCREEN                          YILDIZIoT_TFT7

#if LCD_SCREEN == HX8264
#include "Components/HX8264/hx8264.h"

#define HBP                                 HX8264_HBP
#define HFP                                 HX8264_HFP
#define VBP                                 HX8264_VBP
#define VFP                                 HX8264_VFP
#define HSYNC                               HX8264_HSYNC
#define VSYNC                               HX8264_VSYNC
#define WIDTH                               HX8264_WIDTH
#define HEIGHT                              HX8264_HEIGHT

#define PLLSAIN_CONSTANT                    HX8264_PLLSAIN
#define PLLSAIDIVR_CONSTANT                 HX8264_PLLSAIDIVR
#define FREQUENCY_DIVIDER                   HX8264_FREQUENCY_DIVIDER

#elif LCD_SCREEN == ADAFRUIT_LCD
#include "Components/Adafruit/tft_lcd.h"

#define HBP                                 ADAFRUIT_HBP
#define HFP                                 ADAFRUIT_HFP
#define VBP                                 ADAFRUIT_VBP
#define VFP                                 ADAFRUIT_VFP
#define HSYNC                               ADAFRUIT_HSYNC
#define VSYNC                               ADAFRUIT_VSYNC
#define WIDTH                               ADAFRUIT_WIDTH
#define HEIGHT                              ADAFRUIT_HEIGHT

#define PLLSAIN_CONSTANT                    ADAFRUIT_PLLSAIN
#define PLLSAIDIVR_CONSTANT                 ADAFRUIT_PLLSAIDIVR
#define FREQUENCY_DIVIDER                   ADAFRUIT_FREQUENCY_DIVIDER
#elif LCD_SCREEN == YILDIZIoT_TFT7
#include "Components/YILDIZIoT_TFT/AT070TN92.h"

#define HBP                                 AT070TN92_HBP
#define HFP                                 AT070TN92_HFP
#define VBP                                 AT070TN92_VBP
#define VFP                                 AT070TN92_VFP
#define HSYNC                               AT070TN92_HSYNC
#define VSYNC                               AT070TN92_VSYNC
#define WIDTH                               AT070TN92_WIDTH
#define HEIGHT                              AT070TN92_HEIGHT

#define PLLSAIN_CONSTANT                    AT070TN92_PLLSAIN
#define PLLSAIDIVR_CONSTANT                 AT070TN62_PLLSAIDIVR
#define FREQUENCY_DIVIDER                   AT070TN92_FREQUENCY_DIVIDER
#elif LCD_SCREEN == CUSTOM_SCREEN
#include "Components/Custom/custom_lcd.h"

#define HBP                                 CUSTOM_LCD_HBP
#define HFP                                 CUSTOM_LCD_HFP
#define VBP                                 CUSTOM_LCD_VBP
#define VFP                                 CUSTOM_LCD_VFP
#define HSYNC                               CUSTOM_LCD_HSYNC
#define VSYNC                               CUSTOM_LCD_VSYNC
#define WIDTH                               CUSTOM_LCD_WIDTH
#define HEIGHT                              CUSTOM_LCD_HEIGHT

#define PLLSAIN_CONSTANT                    CUSTOM_PLLSAIN
#define PLLSAIDIVR_CONSTANT                 CUSTOM_PLLSAIDIVR
#define FREQUENCY_DIVIDER                   CUSTOM_FREQUENCY_DIVIDER
#else
#error "Select an LCD Board!"
#endif

#define HORIZONTAL_SYNC                     (HSYNC - 1)
#define VERTICAL_SYNC                       (VSYNC - 1)
#define ACCUMULATED_HBP                     (HSYNC + HBP - 1)
#define ACCUMULATED_VBP                     (VSYNC + VBP - 1)
#define ACCUMULATED_ACTIVE_W                (WIDTH + HSYNC + HBP - 1)
#define ACCUMULATED_ACTIVE_H                (HEIGHT + VSYNC + VBP - 1)
#define TOTAL_HEIGH                         (HEIGHT + VSYNC + VBP + VFP - 1)
#define TOTAL_WIDTH                         (WIDTH + HSYNC + HBP + HFP - 1)

/** @defgroup LCD_Driver_structure  LCD Driver structure
  * @{
  */
typedef struct
{
  void     (*Init)(void);
  uint16_t (*ReadID)(void);
  void     (*DisplayOn)(void);
  void     (*DisplayOff)(void);
  void     (*SetCursor)(uint16_t, uint16_t);
  void     (*WritePixel)(uint16_t, uint16_t, uint16_t);
  uint16_t (*ReadPixel)(uint16_t, uint16_t);
  
   /* Optimized operation */
  void     (*SetDisplayWindow)(uint16_t, uint16_t, uint16_t, uint16_t);
  void     (*DrawHLine)(uint16_t, uint16_t, uint16_t, uint16_t);
  void     (*DrawVLine)(uint16_t, uint16_t, uint16_t, uint16_t);
  
  uint16_t (*GetLcdPixelWidth)(void);
  uint16_t (*GetLcdPixelHeight)(void);
  void     (*DrawBitmap)(uint16_t, uint16_t, uint8_t*);
  void     (*DrawRGBImage)(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t*);
}LCD_DrvTypeDef;    
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

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __LCD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
