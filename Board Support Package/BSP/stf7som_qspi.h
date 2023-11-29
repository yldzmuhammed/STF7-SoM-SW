/**
  ******************************************************************************
  * @file    stf7som_qspi.h
  * @author  yldzmuhammed
  * @version V1.0
  * @date    13-06-2020
  * @brief   qspi flash Underlying application function bsp 
  ******************************************************************************
  * @attention
  *
  * experiment platform: YiLDiZ IoT STF7-SoM Board BSP Library
  * web-site: https://www.yildiziot.com
  ******************************************************************************
*/

#ifndef __STF7SOM_QSPI_H
#define __STF7SOM_QSPI_H

#include <stdio.h>
#include <stdbool.h>
#include "stm32f7xx.h"

#include "w25q128.h"

#define MODULE_ID						(0x000000)
/* Public typedef -----------------------------------------------------------*/
/* QSPI Info */
typedef struct {
  uint32_t FlashSize;         							/*!< Flash size */
  uint32_t EraseSectorSize;   							/*!< Sector size of the erase operation */
  uint32_t EraseSectorsNumber;							/*!< Number of sectors for erase operation */
  uint32_t ProgPageSize;      							/*!< Page size for programming operations */
  uint32_t ProgPagesNumber;   							/*!< Number of pages for programming operations */
} QSPI_Info;

typedef enum
{
	eReadMode_Read,
	eReadMode_FastRead,
	eReadMode_FastReadDualO,
	eReadMode_FastReadQuadO,
	eReadMode_FastReadDualIO,
	eReadMode_FastReadQuadIO
}eReadMode;

/* Public define ------------------------------------------------------------*/
//#define  FLASHID_16							            0xEF3015	//W25X16
#define  FLASHID_16							            0xEF4015	//W25Q16
#define  FLASHID_64							            0XEF4017	//W25Q64
#define  FLASHID_128Mbit					            0XEF4018	//W25Q128

/* QSPI Error codes */
#define QSPI_OK            					        ((uint8_t)0x00)
#define QSPI_ERROR         					        ((uint8_t)0x01)
#define QSPI_BUSY          					        ((uint8_t)0x02)
#define QSPI_NOT_SUPPORTED 					        ((uint8_t)0x04)
#define QSPI_SUSPENDED     					        ((uint8_t)0x08)

#define BLOCKSIZE_32KB						          (BLOCK_32KB_ERASE_CMD)
#define BLOCKSIZE_64KB						          (BLOCK_64KB_ERASE_CMD)


/* QSPI interface definition - beginning****************************/
#define QSPI_FLASH                         	QUADSPI
#define QSPI_FLASH_CLK_ENABLE()            	__HAL_RCC_QSPI_CLK_ENABLE()

#define QSPI_FLASH_CLK_PIN                 	GPIO_PIN_2
#define QSPI_FLASH_CLK_GPIO_PORT           	GPIOB
#define QSPI_FLASH_CLK_GPIO_AF             	GPIO_AF9_QUADSPI

#define QSPI_FLASH_BK1_IO0_PIN             	GPIO_PIN_8
#define QSPI_FLASH_BK1_IO0_PORT            	GPIOF
#define QSPI_FLASH_BK1_IO0_AF              	GPIO_AF10_QUADSPI

#define QSPI_FLASH_BK1_IO1_PIN             	GPIO_PIN_9
#define QSPI_FLASH_BK1_IO1_PORT            	GPIOF
#define QSPI_FLASH_BK1_IO1_AF              	GPIO_AF10_QUADSPI

#define QSPI_FLASH_BK1_IO2_PIN             	GPIO_PIN_2
#define QSPI_FLASH_BK1_IO2_PORT            	GPIOE
#define QSPI_FLASH_BK1_IO2_AF              	GPIO_AF9_QUADSPI

#define QSPI_FLASH_BK1_IO3_PIN             	GPIO_PIN_6
#define QSPI_FLASH_BK1_IO3_PORT            	GPIOF
#define QSPI_FLASH_BK1_IO3_AF              	GPIO_AF9_QUADSPI

#define QSPI_FLASH_CS_PIN                 	GPIO_PIN_6
#define QSPI_FLASH_CS_GPIO_PORT           	GPIOB
#define QSPI_FLASH_CS_GPIO_AF             	GPIO_AF10_QUADSPI



uint8_t BSP_QSPI_Init(void);
uint8_t BSP_QSPI_DeInit(void);
uint8_t BSP_QSPI_GetStatus(void);
uint8_t BSP_QSPI_Erase_Chip(void);
uint8_t BSP_QSPI_Enter_QPIMode(void);
uint8_t BSP_QSPI_Enable_QPIMode(void);

uint8_t BSP_QSPI_ReadID(uint32_t *id);
uint8_t BSP_QSPI_GetInfo(QSPI_Info* pInfo);
uint8_t BSP_QSPI_ReadDeviceID(uint32_t *id);
uint8_t BSP_QSPI_ReadUniqDevID(uint32_t *devid);
uint8_t BSP_QSPI_EnableMemoryMappedMode(bool enable);
uint8_t BSP_QSPI_Erase_Sector(uint32_t SectorAddress);
uint8_t BSP_QSPI_Erase_Block(uint32_t BlockAddress, uint8_t blockSize);
uint8_t BSP_QSPI_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size);
uint8_t BSP_QSPI_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size, eReadMode readmode);


#endif /* __SPI_FLASH_H */

/************************ (C) COPYRIGHT YiLDiZ IoT END OF FILE ************************/

