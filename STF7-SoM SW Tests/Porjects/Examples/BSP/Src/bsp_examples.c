/**
  ******************************************************************************
  * @file    main.c
  * @author  yldzmuhammed
  * @version V1.0
  * @date    13-06-2020
  * @brief   This file contains the bsp example functions.
  ******************************************************************************
  * @attention
  *
  * experiment platform: YiLDiZ IoT STF7-SoM Board BSP Library
  * web-site: https://www.yildiziot.com
  ******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "lcd_log.h"
#include "stf7som.h"
#include "stf7som_sd.h"
#include "stf7som_ts.h"
#include "stf7som_lcd.h"
#include "stf7som_nor.h"
#include "stf7som_fmc.h"
#include "bsp_examples.h"
#include "ts_calibration.h"
#include "yildiziot_logo.h"
#include "stf7som_communication.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE				((uint32_t)0x1000)
#define WRITE_READ_ADDR			((uint32_t)0x0800)
#define SDRAM_WRITE_READ_ADDR	((uint32_t)0xC0130000)

#define LCD_FEATURES_NUM		3
#define  CIRCLE_RADIUS        	30

#define NUM_OF_BLOCKS			5     								/* Total number of blocks   */
#define BLOCK_START_ADDR		0     								/* Block start address      */
#define BUFFER_WORDS_SIZE		((BLOCKSIZE * NUM_OF_BLOCKS) >> 2) 	/* Total data size in bytes */

/* Private macro -------------------------------------------------------------*/
#define  CIRCLE_XPOS(i)      	((i * BSP_LCD_GetXSize()) / 5)
#define  CIRCLE_YPOS(i)      	(BSP_LCD_GetYSize() - CIRCLE_RADIUS - 60)
/* Private variables ---------------------------------------------------------*/
static uint8_t 	ubIDStatus = 0;
static uint8_t 	LCD_Feature = 0;
static uint8_t 	ubReadStatus = 0;
static uint8_t 	ubInitStatus = 0;
static uint8_t 	ubEraseStatus = 0;
static uint8_t 	ubWriteStatus = 0;

static uint16_t nor_aTxBuffer[BUFFER_SIZE];
static uint16_t nor_aRxBuffer[BUFFER_SIZE];

static uint32_t sdram_aTxBuffer[BUFFER_SIZE];
static uint32_t sdram_aRxBuffer[BUFFER_SIZE];

static uint32_t aSDTxBuffer[BUFFER_WORDS_SIZE];
static uint32_t aSDRxBuffer[BUFFER_WORDS_SIZE];

static HAL_SD_CardStateTypedef cardstate =  HAL_SD_CARD_TRANSFER;

static TS_StateTypeDef  TS_State;
static HAL_SD_CardInfoTypeDef CardInfo;
/* Private function prototypes -----------------------------------------------*/
static void 	SD_SetHint(void);
static void 	LCD_SetHint(void);
static void     NOR_SetHint(void);
static void 	SDRAM_SetHint(void);
static void 	Touchscreen_SetHint(void);

static void 	LCD_Show_Feature(uint8_t feature);
static void 	Touchscreen_DrawBackground (uint8_t state);
static void     Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
static void 	Fill_SDRAMBuffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);

static uint8_t  Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength);
static uint8_t 	SDRAMBuffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);

extern SD_HandleTypeDef uSdHandle;
/* Private user code ---------------------------------------------------------*/
/**
  * @brief  Display NOR Demo Hint
  * @param  None
  * @retval None
  */
static void NOR_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"NOR", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)"This example shows how to write", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t*)"and read data on NOR", CENTER_MODE); 

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
}
//

/**
  * @brief  Display LCD demo hint
  * @param  None
  * @retval None
  */
static void LCD_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"LCD", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows the different", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"LCD Features, use Tamper push-button to display", CENTER_MODE); 
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"next page", CENTER_MODE); 

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
}
//

/**
  * @brief  Display SDRAM Demo Hint
  * @param  None
  * @retval None
  */
static void SDRAM_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"SDRAM", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to write", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"and read data on SDRAM", CENTER_MODE);

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
}
//

/**
  * @brief  Display TS Demo Hint
  * @param  None
  * @retval None
  */
static void Touchscreen_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set Touchscreen Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"Touchscreen", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Please use the Touchscreen to", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"activate the colored circle", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"inside the rectangle", CENTER_MODE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
   
}
//

/**
  * @brief  Show LCD Features
  * @param  feature : feature index
  * @retval None
  */
static void LCD_Show_Feature(uint8_t feature)
{
  Point Points[]= {{20, 150}, {80, 150}, {80, 200}};
  Point Points2[3];

  Points2[0].X = BSP_LCD_GetXSize() - 80;
  Points2[0].Y = 150;
  Points2[1].X = BSP_LCD_GetXSize() - 20;
  Points2[1].Y = 150;
  Points2[2].X = BSP_LCD_GetXSize() - 20;
  Points2[2].Y = 200;
  
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
  BSP_LCD_FillRect(12, 92, BSP_LCD_GetXSize() - 24, BSP_LCD_GetYSize()- 104);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  switch (feature)
  {
  case 0:
    /* Text Feature */
    
    BSP_LCD_DisplayStringAt(14, 100, (uint8_t *)"Left aligned Text", LEFT_MODE); 
    BSP_LCD_DisplayStringAt(0, 115, (uint8_t *)"Center aligned Text", CENTER_MODE); 
    BSP_LCD_DisplayStringAt(14, 130, (uint8_t*)"Right aligned Text", RIGHT_MODE);
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_DisplayStringAt(14, 180, (uint8_t *)"Font24", LEFT_MODE); 
    BSP_LCD_SetFont(&Font20);
    BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize()/2 -20, 180, (uint8_t *)"Font20", LEFT_MODE); 
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize() - 80, 184, (uint8_t *)"Font16", LEFT_MODE); 
    break;
    
  case 1:
    
    /* Draw misc. Shapes */
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK); 
    BSP_LCD_DrawRect(20, 100, 60 , 40);
    BSP_LCD_FillRect(100, 100, 60 , 40); 
    
    BSP_LCD_SetTextColor(LCD_COLOR_GRAY); 
    BSP_LCD_DrawCircle(BSP_LCD_GetXSize() - 120, 120, 20);
    BSP_LCD_FillCircle(BSP_LCD_GetXSize() - 40, 120, 20);
    
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN); 
    BSP_LCD_DrawPolygon(Points, 3);
    
    BSP_LCD_SetTextColor(LCD_COLOR_RED); 
    BSP_LCD_DrawEllipse(130, 170, 30, 20);
    BSP_LCD_FillEllipse(200, 170, 30, 20);
    
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK); 
    BSP_LCD_DrawHLine(20, BSP_LCD_GetYSize() - 30, BSP_LCD_GetXSize() / 5);
    BSP_LCD_DrawLine (100, BSP_LCD_GetYSize() - 20, 230, BSP_LCD_GetYSize()- 50);
    BSP_LCD_DrawLine (100, BSP_LCD_GetYSize()- 50, 230, BSP_LCD_GetYSize()- 20);
    
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN); 
    BSP_LCD_FillPolygon(Points2, 3);
    break;
    
  case 2:
    /* Draw Bitmap */
    BSP_LCD_DrawBitmap(20, 100, (uint8_t *)yildiziot_logo);
    HAL_Delay(500);
    
    BSP_LCD_DrawBitmap(BSP_LCD_GetXSize()/2 - 40, 100, (uint8_t *)yildiziot_logo);
    HAL_Delay(500);    
    
    BSP_LCD_DrawBitmap(BSP_LCD_GetXSize()-100, 100, (uint8_t *)yildiziot_logo);
    HAL_Delay(500);

    BSP_LCD_DrawBitmap(20, BSP_LCD_GetYSize()- 80, (uint8_t *)yildiziot_logo);
    HAL_Delay(500);
    
    BSP_LCD_DrawBitmap(BSP_LCD_GetXSize()/2 - 40, BSP_LCD_GetYSize()- 80, (uint8_t *)yildiziot_logo);
    HAL_Delay(500);     

    BSP_LCD_DrawBitmap(BSP_LCD_GetXSize()-100, BSP_LCD_GetYSize()- 80, (uint8_t *)yildiziot_logo);
    HAL_Delay(500);    
    break;
  }
}
//

/**
  * @brief  Draw Touchscreen Background
  * @param  state : touch zone state
  * @retval None
  */
static void Touchscreen_DrawBackground (uint8_t state)
{
  
  switch(state)
  {
    
  case 0:
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS);
    
    
    BSP_LCD_SetTextColor(LCD_COLOR_RED); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS);
    
    
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS);
    
    
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS);
    
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2);        
    BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2);
    BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2);
    BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2); 
    break;
    
  case 1:
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2);        
    break;
    
  case 2:
    BSP_LCD_SetTextColor(LCD_COLOR_RED); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2); 
    break;
    
  case 4:
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2); 
    break;
    
  case 8:
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(4), CIRCLE_RADIUS);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(4), CIRCLE_RADIUS - 2); 
    break;
    
  }
}
//

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}
//

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_SDBuffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}
//

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_SDRAMBuffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}
//

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static uint8_t Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}
//

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static uint8_t SDBuffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}
//

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static uint8_t SDRAMBuffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}
//

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SD Demo
  * @param  None
  * @retval None
  */
void SD_demo (void)
{ 
  uint8_t SD_state = MSD_OK;
  __IO uint8_t prev_status = 0; 

  SD_SetHint();

  SD_state = BSP_SD_Init();

   /* Check if the SD card is plugged in the slot */
  if(BSP_SD_IsDetected() == SD_PRESENT)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-30, (uint8_t *)"SD Connected    ", LEFT_MODE);
  }
  else 
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-30, (uint8_t *)"SD Not Connected", LEFT_MODE);
  }
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  
  if(SD_state != MSD_OK)
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"SD INITIALIZATION : FAIL.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"SD INITIALIZATION : OK.", LEFT_MODE); 
    
    BSP_SD_GetCardInfo(&CardInfo);

    if(SD_state != MSD_OK)
    {
      BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SD GET CARD INFO : FAIL.", LEFT_MODE);
      BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SD GET CARD INFO : OK.", LEFT_MODE);

      SD_state = BSP_SD_Erase(BLOCK_START_ADDR, BLOCK_START_ADDR + NUM_OF_BLOCKS - 1);

      /* Wait until SD card is ready to use for new operation */
      while(BSP_SD_GetCardState() != SD_TRANSFER_OK)
      {
      }

      if(SD_state != MSD_OK)
      {
        BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD ERASE : FAILED.", LEFT_MODE);
        BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
      }
      else
      {
        BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD ERASE : OK.", LEFT_MODE);
      
        /* Fill the buffer to write */
        Fill_SDBuffer(aSDTxBuffer, BUFFER_WORDS_SIZE, 0x22FF);
        SD_state = BSP_SD_WriteBlocks((uint32_t *)aSDTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS, 1000);
        
        /* Wait until SD card is ready to use for new operation */
        while(BSP_SD_GetCardState() != SD_TRANSFER_OK)
        {
        }
      
        if(SD_state != MSD_OK)
        {
          BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD WRITE : FAILED.", LEFT_MODE);
          BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
        }
        else
        {
          BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD WRITE : OK.", LEFT_MODE);
          SD_state = BSP_SD_ReadBlocks((uint32_t *)aSDRxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS, 1000);
          
          /* Wait until SD card is ready to use for new operation */
          while(BSP_SD_GetCardState() != SD_TRANSFER_OK)
          {
          }
          
          if(SD_state != MSD_OK)
          {
            BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD READ : FAILED.", LEFT_MODE);
            BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
          }
          else
          {
            BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD READ : OK.", LEFT_MODE);
            if(SDBuffercmp(aSDTxBuffer, aSDRxBuffer, BUFFER_WORDS_SIZE) > 0)
            {
              BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"SD COMPARE : FAILED.", LEFT_MODE);
              BSP_LCD_DisplayStringAt(20, 190, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
            }
            else
            {
              BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"SD TEST : OK.", LEFT_MODE);
            }
          }
        }
      }
    }
  }
  
  while (1)
  {
    /* Check if the SD card is plugged in the slot */
    if(BSP_SD_IsDetected() != SD_PRESENT)
    {
      if(prev_status == 0)
      {
        prev_status = 1; 
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-30, (uint8_t *)"SD Not Connected", LEFT_MODE);
      }
    }
    else if (prev_status == 1)
    {
      BSP_SD_Init();
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-30, (uint8_t *)"SD Connected    ", LEFT_MODE);
      prev_status = 0;
    }
    
    if(CheckForUserInput() > 0)
    {
      return;
    }
  }
}
//

/**
  * @brief  LCD Log demo 
  * @param  None
  * @retval None
  */
void Log_demo(void)
{
  uint8_t i = 0;

  /* Wait For User inputs */
  while(CheckForUserInput() == 0);
  
  /* Initialize LCD Log module */
  LCD_LOG_Init();
  
  /* Show Header and Footer texts */
  LCD_LOG_SetHeader((uint8_t *)"Log Example");
  LCD_LOG_SetFooter((uint8_t *)"Use Joystick to scroll up/down");
  
  /* Output User logs */
  for (i = 0; i < 10; i++)
  {
    LCD_UsrLog ("This is Line %d \n", i);
  }
  
  HAL_Delay(2000);
  
   /* Clear Old logs */
  LCD_LOG_ClearTextZone();
  
   /* Output new user logs */
  for (i = 0; i < 30; i++)
  {
    LCD_UsrLog ("This is Line %d \n", i);
  }
  
  /* Check for joystick user input for scroll (back and forward) */
//  while (1)
//  {
//    JoyState = BSP_JOY_GetState();
//    switch(JoyState)
//    {
//    case JOY_UP:
//      LCD_LOG_ScrollBack();
//      break;     
//    case JOY_DOWN:
//      LCD_LOG_ScrollForward();
//      break;          
//      
//    default:
//      break;           
//    }
//    if(CheckForUserInput() > 0)
//    {
//      return;
//    }    
//    HAL_Delay (10);
//  }
}
//

/**
  * @brief  NOR Demo
  * @param  None
  * @retval None
  */
void NOR_demo (void)
{ 
  /* NOR IDs structure */
  static NOR_IDTypeDef pNOR_ID;

  NOR_SetHint();

  /*##-1- Configure the NOR device ###########################################*/
  /* NOR device configuration */ 
  if(BSP_NOR_Init() != NOR_STATUS_OK)
  {
    ubInitStatus++; 
  }
  
  /*##-2- Read & check the NOR device IDs ####################################*/
  /* Initialize the ID structure */
  pNOR_ID.Manufacturer_Code = (uint16_t)0x00;
  pNOR_ID.Device_Code1 = (uint16_t)0x00;
  pNOR_ID.Device_Code2 = (uint16_t)0x00;
  pNOR_ID.Device_Code3 = (uint16_t)0x00;
  
  /* Read the NOR memory ID */
  BSP_NOR_Read_ID(&pNOR_ID);
  
  /* Test the NOR ID correctness */
  if(pNOR_ID.Manufacturer_Code != (uint16_t)0x0020)
    ubIDStatus++;
  else if(pNOR_ID.Device_Code1 != (uint16_t)0x227E)
    ubIDStatus++;
  else if (pNOR_ID.Device_Code2 != (uint16_t)0x2221)
    ubIDStatus++;
  else if (pNOR_ID.Device_Code3 != (uint16_t)0x2200)
    ubIDStatus++;
    
  /*##-3- Erase NOR memory ###################################################*/ 
  /* Return to read mode */
  BSP_NOR_ReturnToReadMode();
    
  if(BSP_NOR_Erase_Block(WRITE_READ_ADDR) != NOR_STATUS_OK)
  {
    ubEraseStatus++; 
  }
  
  /*##-4- NOR memory read/write access  ######################################*/   
  /* Fill the buffer to write */
  Fill_Buffer(nor_aTxBuffer, BUFFER_SIZE, 0xD20F);   
  
  /* Write data to the NOR memory */
  if(BSP_NOR_WriteData(WRITE_READ_ADDR, nor_aTxBuffer, BUFFER_SIZE) != NOR_STATUS_OK)
  {
    ubWriteStatus++; 
  }
  
  /* Read back data from the NOR memory */
  if(BSP_NOR_ReadData(WRITE_READ_ADDR, nor_aRxBuffer, BUFFER_SIZE) != NOR_STATUS_OK)
  {
    ubReadStatus++; 
  }

  /*##-5- Checking data integrity ############################################*/  
  /* Enable the LCD */
  BSP_LCD_DisplayOn();  

  if(ubInitStatus != 0)
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t*)"NOR Initialization : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t*)"NOR Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t*)"NOR Initialization : OK.", LEFT_MODE);
  }  
  if(ubEraseStatus != 0)
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t*)"NOR ERASE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t*)"NOR Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t*)"NOR ERASE : OK.   ", LEFT_MODE);
  }
  if(ubWriteStatus != 0)
  {
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t*)"NOR WRITE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t*)"NOR Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t*)"NOR WRITE : OK.     ", LEFT_MODE);
  }
  if(ubReadStatus != 0)
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t*)"NOR READ : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 160, (uint8_t*)"NOR Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t*)"NOR READ :  OK.    ", LEFT_MODE);
  }    
  if(Buffercmp(nor_aRxBuffer, nor_aTxBuffer, BUFFER_SIZE) > 0)
  {
    BSP_LCD_DisplayStringAt(20, 160, (uint8_t*)"NOR COMPARE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 175, (uint8_t*)"NOR Test Aborted.", LEFT_MODE);
  }
  else
  {    
    BSP_LCD_DisplayStringAt(20, 160, (uint8_t*)"NOR Test : OK.     ", LEFT_MODE);
  }  
  
  while (1)
  {    
    if(CheckForUserInput() > 0)
    {
      return;
    }
  }
}
//

/**
  * @brief  LCD demo
  * @param  None
  * @retval None
  */
void LCD_demo (void)
{ 
  LCD_SetHint();
  LCD_Feature = 0;
  LCD_Show_Feature (LCD_Feature); 
  
  while (1)
  {
    
    if(CheckForUserInput() > 0)
    {
      if(++LCD_Feature < LCD_FEATURES_NUM)
      {
       LCD_Show_Feature (LCD_Feature); 
      }
      else
      {
        return;
      }
    }
    HAL_Delay(100);
  }
}
//

/**
  * @brief  SDRAM Demo
  * @param  None
  * @retval None
  */
void SDRAM_demo (void)
{ 
  
  SDRAM_SetHint();

  /* SDRAM device configuration */ 
  if(BSP_SDRAM_Init() != SDRAM_OK)
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SDRAM Initialization : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"SDRAM Initialization : OK.", LEFT_MODE);
  }
  /* Fill the buffer to write */
  Fill_SDRAMBuffer(sdram_aTxBuffer, BUFFER_SIZE, 0xA244250F);   
  
  /* Write data to the SDRAM memory */
  if(BSP_SDRAM_WriteData(SDRAM_WRITE_READ_ADDR + WRITE_READ_ADDR, sdram_aTxBuffer, BUFFER_SIZE) != SDRAM_OK)
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SDRAM WRITE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  { 
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SDRAM WRITE : OK.", LEFT_MODE);
  }
  
  /* Read back data from the SDRAM memory */
  if(BSP_SDRAM_ReadData(SDRAM_WRITE_READ_ADDR + WRITE_READ_ADDR, sdram_aRxBuffer, BUFFER_SIZE) != SDRAM_OK)
  {
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM READ : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM READ : OK.", LEFT_MODE);
  }
  
  if(SDRAMBuffercmp(sdram_aTxBuffer, sdram_aRxBuffer, BUFFER_SIZE) > 0)
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM COMPARE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM Test : OK.", LEFT_MODE);
  }
 
  while (1)
  {    
    if(CheckForUserInput() > 0)
    {
      return;
    }
  }
}
//

/**
  * @brief  Check for user input
  * @param  None
* @retval Input state (1 : active / 0 : Inactive)
  */
uint8_t CheckForUserInput(void)
{
  if(BSP_PB_Get() == GPIO_PIN_SET)
  {
    while (BSP_PB_Get() == GPIO_PIN_SET);
    return 1 ;
  }
  return 0;
}
//

/**
  * @brief  Touchscreen Demo
  * @param  None
  * @retval None
  */
void Touchscreen_demo (void)
{ 
//  uint8_t  status = 0;
//  uint16_t x,y;
//  uint8_t state = 0;

//  if(stmpe811_ts_drv.ReadID(TS_I2C_ADDRESS) == STMPE811_ID)
//  { 
//  if(IsCalibrationDone() == 0)
//  {
//    Touchscreen_Calibration();
//  }
//  }
//  
//  Touchscreen_SetHint();
//  
//  status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
//  
//  if (status != TS_OK)
//  {
//    BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
//    BSP_LCD_SetTextColor(LCD_COLOR_RED);
//    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 95, (uint8_t *)"ERROR", CENTER_MODE);
//    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 80, (uint8_t *)"Touchscreen cannot be initialized", CENTER_MODE);
//  }
//  else
//  {
//    Touchscreen_DrawBackground(state);
//  }
//  
//  while (1)
//  {
//    if (status == TS_OK)
//    {
//      BSP_TS_GetState(&TS_State);
//      
//      if(stmpe811_ts_drv.ReadID(TS_I2C_ADDRESS) == STMPE811_ID)
//      {      
//        x = Calibration_GetX(TS_State.x);
//        y = Calibration_GetY(TS_State.y);
//      }
//      else
//      {
//        x = TS_State.x;
//        y = TS_State.y;  
//      }
//      
//      if((TS_State.TouchDetected) && 
//         (y > (CIRCLE_YPOS(1) - CIRCLE_RADIUS))&&
//           (y < (CIRCLE_YPOS(1) + CIRCLE_RADIUS)))
//      {
//        
//        if((x > (CIRCLE_XPOS(1) - CIRCLE_RADIUS))&&
//           (x < (CIRCLE_XPOS(1) + CIRCLE_RADIUS)))
//        {
//          if((state & 1) == 0)
//          {
//            Touchscreen_DrawBackground(state);
//            BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
//            BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS);
//            state = 1;
//          }
//        }
//        if((x > (CIRCLE_XPOS(2) - CIRCLE_RADIUS))&&
//           (x < (CIRCLE_XPOS(2) + CIRCLE_RADIUS)))
//        {
//          if((state & 2) == 0)
//          {          
//            Touchscreen_DrawBackground(state);
//            BSP_LCD_SetTextColor(LCD_COLOR_RED); 
//            BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS);
//            state = 2;
//          }          
//        }
//        
//        if((x > (CIRCLE_XPOS(3) - CIRCLE_RADIUS))&&
//           (x < (CIRCLE_XPOS(3) + CIRCLE_RADIUS)))
//        {
//          if((state & 4) == 0)
//          {           
//            Touchscreen_DrawBackground(state);
//            BSP_LCD_SetTextColor(LCD_COLOR_YELLOW); 
//            BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS);
//            state = 4;
//          }            
//        }
//        
//        if((x > (CIRCLE_XPOS(4) - CIRCLE_RADIUS))&&
//           (x < (CIRCLE_XPOS(4) + CIRCLE_RADIUS)))
//        {
//          if((state & 8) == 0)
//          {           
//            Touchscreen_DrawBackground(state);
//            BSP_LCD_SetTextColor(LCD_COLOR_GREEN); 
//            BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS);
//            state = 8;
//          }           
//        }        
//      }   
//    }
//    
//    if(CheckForUserInput() > 0)
//    {
//      return;
//    }
//    
//    HAL_Delay(10);
//  }
}
//

/**
  * @brief  Display SD Demo Hint
  * @param  None
  * @retval None
  */
static void SD_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"SD", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to write", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"and read data on the microSD and also", CENTER_MODE); 
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"how to detect the presence of the card", CENTER_MODE); 

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
}
//

/************************ (C) COPYRIGHT YiLDiZ IoT END OF FILE ************************/
