/**
  ******************************************************************************
  * @file    lcd_log.h
  * @author  yldzmuhammed
  * @version V1.0
  * @date    13-06-2020
  * @brief   header for the lcd_log.c file
  ******************************************************************************
  * @attention
  *
  * experiment platform: YiLDiZ IoT STF7-SoM Board BSP Library
  * web-site: https://www.yildiziot.com
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  __LCD_LOG_H__
#define  __LCD_LOG_H__

/* Includes ------------------------------------------------------------------*/

#include "lcd_log_conf.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup Common
  * @{
  */

/** @addtogroup LCD_LOG
  * @{
  */
  
/** @defgroup LCD_LOG
  * @brief 
  * @{
  */ 


/** @defgroup LCD_LOG_Exported_Defines
  * @{
  */ 

#if (LCD_SCROLL_ENABLED == 1)
 #define     LCD_CACHE_DEPTH     (YWINDOW_SIZE + CACHE_SIZE)
#else
 #define     LCD_CACHE_DEPTH     YWINDOW_SIZE
#endif
/**
  * @}
  */ 

/** @defgroup LCD_LOG_Exported_Types
  * @{
  */ 
typedef struct _LCD_LOG_line
{
  uint8_t  line[128];
  uint32_t color;

}LCD_LOG_line;

/**
  * @}
  */ 

/** @defgroup LCD_LOG_Exported_Macros
  * @{
  */ 
#define  LCD_ErrLog(...)    do{                                         \
                                LCD_LineColor = LCD_COLOR_RED;          \
                                printf("ERROR: ") ;                     \
                                printf(__VA_ARGS__);                    \
                                LCD_LineColor = LCD_LOG_DEFAULT_COLOR;  \
                               }while (0)

#define  LCD_UsrLog(...)    do{                                         \
                                LCD_LineColor = LCD_LOG_TEXT_COLOR;     \
                                printf(__VA_ARGS__);                    \
                              }while (0)


#define  LCD_DbgLog(...)    do{                                         \
                                LCD_LineColor = LCD_COLOR_CYAN;         \
                                printf(__VA_ARGS__);                    \
                                LCD_LineColor = LCD_LOG_DEFAULT_COLOR;  \
                              }while (0)
/**
  * @}
  */ 

/** @defgroup LCD_LOG_Exported_Variables
  * @{
  */ 
extern uint32_t LCD_LineColor;

/**
  * @}
  */ 

/** @defgroup LCD_LOG_Exported_FunctionsPrototype
  * @{
  */ 
void LCD_LOG_Init(void);
void LCD_LOG_DeInit(void);
void LCD_LOG_SetHeader(uint8_t *Title);
void LCD_LOG_SetFooter(uint8_t *Status);
void LCD_LOG_ClearTextZone(void);
void LCD_LOG_UpdateDisplay (void);

#if (LCD_SCROLL_ENABLED == 1)
 ErrorStatus LCD_LOG_ScrollBack(void);
 ErrorStatus LCD_LOG_ScrollForward(void);
#endif
/**
  * @}
  */ 


#endif /* __LCD_LOG_H__ */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
