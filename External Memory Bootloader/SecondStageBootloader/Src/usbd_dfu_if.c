/**
  ******************************************************************************
  * @file           : usbd_dfu_if.c
  * @brief          : Usb device for Download Firmware Update.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "usbd_dfu_if.h"
#include "stf7som_nor.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @defgroup USBD_DFU
  * @brief Usb DFU device module.
  * @{
  */

/** @defgroup USBD_DFU_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_DFU_Private_Defines
  * @brief Private defines.
  * @{
  */

#define FLASH_DESC_STR      "@External Flash   /0x60000000/02*128Ka,02*128Kg,14*128Kg,14*128Kg"

/* USER CODE BEGIN PRIVATE_DEFINES */

#define FLASH_ERASE_TIME    (uint16_t)500
#define FLASH_PROGRAM_TIME  (uint16_t)50

/**
  * @}
  */

/** @defgroup USBD_DFU_Private_Macros
  * @brief Private macros.
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_DFU_Private_Variables
  * @brief Private variables.
  * @{
  */
static NOR_IDTypeDef NOR_FlashID;          /* Paralel NOR Flash Device ID */

/**
  * @}
  */

/** @defgroup USBD_DFU_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceHS;

/**
  * @}
  */

/** @defgroup USBD_DFU_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static uint16_t MEM_If_Init_HS(void);
static uint16_t MEM_If_DeInit_HS(void);

static uint32_t GetBank(uint32_t Addr);
static uint32_t GetSector(uint32_t Address);
static uint16_t MEM_If_Erase_HS(uint32_t Add);
static uint8_t *MEM_If_Read_HS(uint8_t *src, uint8_t *dest, uint32_t Len);
static uint16_t MEM_If_Write_HS(uint8_t *src, uint8_t *dest, uint32_t Len);
static uint16_t MEM_If_GetStatus_HS(uint32_t Add, uint8_t Cmd, uint8_t *buffer);


/**
  * @}
  */

#if defined ( __ICCARM__ ) /* IAR Compiler */
  #pragma data_alignment=4
#endif
__ALIGN_BEGIN USBD_DFU_MediaTypeDef USBD_DFU_fops_HS __ALIGN_END =
{
    (uint8_t*)FLASH_DESC_STR,
    MEM_If_Init_HS,
    MEM_If_DeInit_HS,
    MEM_If_Erase_HS,
    MEM_If_Write_HS,
    MEM_If_Read_HS,
    MEM_If_GetStatus_HS
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Memory initialization routine.
  * @retval USBD_OK if operation is successful, MAL_FAIL else.
  */
uint16_t MEM_If_Init_HS(void)
{
  /* init fmc nor flash */
	if(BSP_NOR_Init() != NOR_STATUS_OK)
		return USBD_EMEM;
	
	/* try to read device id */
	if(BSP_NOR_Read_ID(&NOR_FlashID) != NOR_STATUS_OK)
		return USBD_EMEM;
	
	/* return to read mode */
	BSP_NOR_ReturnToReadMode();
	
	/* check device id */
	if(NOR_FlashID.Manufacturer_Code 	!= NOR_MANUFACTURER_ID	|| NOR_FlashID.Device_Code1 != NOR_DEVICE_CODE_1/* ||
		 NOR_FlashID.Device_Code2 			!= NOR_DEVICE_CODE_2 		|| NOR_FlashID.Device_Code3 != NOR_DEVICE_CODE_3*/)
		return USBD_EMEM;
	
	/* everything is ok */
  return (USBD_OK);
}
//

/**
  * @brief  De-Initializes Memory.
  * @retval USBD_OK if operation is successful, MAL_FAIL else.
  */
uint16_t MEM_If_DeInit_HS(void)
{
	/* deinit device */
	if(BSP_NOR_DeInit() != NOR_STATUS_OK)
		return USBD_EMEM;
	
  return (USBD_OK);
}
//

/**
  * @brief  Erase sector.
  * @param  Add: Address of sector to be erased.
  * @retval USBD_OK if operation is successful, MAL_FAIL else.
  */
uint16_t MEM_If_Erase_HS(uint32_t Add)
{
  uint32_t startsector = 0;

  /* Get the number of sector */
  startsector = GetSector(Add);
	
	/* try to erase pointed sector */
	if(BSP_NOR_Erase_Block(startsector) != NOR_STATUS_OK)
		return USBD_EMEM;

  return USBD_OK;
}
//

/**
  * @brief  Memory write routine.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be written (in bytes).
  * @retval USBD_OK if operation is successful, MAL_FAIL else.
  */
uint16_t MEM_If_Write_HS(uint8_t *src, uint8_t *dest, uint32_t Len)
{
  uint32_t i = 0;

	uint16_t readed = 0;
	uint16_t written = 0;
	uint16_t *src_addr = (uint16_t *)src;
	uint16_t *dest_addr = (uint16_t *)dest;
	
	while(i < Len)
	{
		written = *((uint16_t *)src + i);
		
		/* try to write value to the flash */
		if(BSP_NOR_WriteBuffer((uint32_t)src_addr, &written, 1) != NOR_STATUS_OK)
			return USBD_BUSY;
		
		/* try to read data */
		if(BSP_NOR_ReadBuffer((uint32_t)dest_addr, &readed, 1) != NOR_STATUS_OK)
			return USBD_BUSY;
		
		/* check written value */
		if(written != readed)
			return USBD_EMEM;
		
		/* increase addresses */
		src_addr++;
		dest_addr++;
		
		i += 2;
	}
	
	/* all good */
  return USBD_OK;
}
//

/**
  * @brief  Memory read routine.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be read (in bytes).
  * @retval Pointer to the physical address where data should be read.
  */
uint8_t *MEM_If_Read_HS(uint8_t *src, uint8_t *dest, uint32_t Len)
{
	uint16_t *pSrc = (uint16_t *)(src - NOR_DEVICE_ADDR);
	uint16_t buffer[32] = {0};
	
	uint32_t length = 32;
	uint32_t numread = Len;
	
	BSP_NOR_ReturnToReadMode();
	
	while(numread > 0)
	{
		if(numread > 32)
		{
			numread -= 32;
		}
		else
		{
			numread = 0;
			length = numread;
		}
		
		if(BSP_NOR_ReadBuffer((uint32_t)pSrc, buffer, length) != NOR_STATUS_OK)
			return src;
		
		pSrc += 14;
	}
	
  /* Return a valid address to avoid HardFault */
  return (uint8_t *) (dest);
}
//

/**
  * @brief  Get status routine.
  * @param  Add: Address to be read from.
  * @param  Cmd: Number of data to be read (in bytes).
  * @param  buffer: used for returning the time necessary for a program or an erase operation
  * @retval 0 if operation is successful
  */
uint16_t MEM_If_GetStatus_HS(uint32_t Add, uint8_t Cmd, uint8_t *buffer)
{
  switch (Cmd)
  {
		case DFU_MEDIA_PROGRAM:
			buffer[1] = (uint8_t) FLASH_PROGRAM_TIME;
			buffer[2] = (uint8_t) (FLASH_PROGRAM_TIME << 8);
			buffer[3] = 0;
			break;

		case DFU_MEDIA_ERASE:
		default:
			buffer[1] = (uint8_t) FLASH_ERASE_TIME;
			buffer[2] = (uint8_t) (FLASH_ERASE_TIME << 8);
			buffer[3] = 0;
			break;
  }
  return USBD_OK;
}
//

/**
  * @brief  Gets the sector of a given address
  * @param  Address Address of the FLASH Memory
  * @retval The sector of a given address
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;

  if ((Address >= ADDR_FLASH_SECTOR_0) && (Address < ADDR_FLASH_SECTOR_1))
  {
    sector = ADDR_FLASH_SECTOR_0;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_1) && (Address < ADDR_FLASH_SECTOR_2))
  {
    sector = ADDR_FLASH_SECTOR_1;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_2) && (Address < ADDR_FLASH_SECTOR_3))
  {
    sector = ADDR_FLASH_SECTOR_2;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_3) && (Address < ADDR_FLASH_SECTOR_4))
  {
    sector = ADDR_FLASH_SECTOR_3;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_4) && (Address < ADDR_FLASH_SECTOR_5))
  {
    sector = ADDR_FLASH_SECTOR_4;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_5) && (Address < ADDR_FLASH_SECTOR_6))
  {
    sector = ADDR_FLASH_SECTOR_5;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_6) && (Address < ADDR_FLASH_SECTOR_7))
  {
    sector = ADDR_FLASH_SECTOR_6;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_7) && (Address < ADDR_FLASH_SECTOR_8))
  {
    sector = ADDR_FLASH_SECTOR_7;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_8) && (Address < ADDR_FLASH_SECTOR_9))
  {
    sector = ADDR_FLASH_SECTOR_8;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_9) && (Address < ADDR_FLASH_SECTOR_10))
  {
    sector = ADDR_FLASH_SECTOR_9;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_10) && (Address < ADDR_FLASH_SECTOR_11))
  {
    sector = ADDR_FLASH_SECTOR_10;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_11) && (Address < ADDR_FLASH_SECTOR_12))
  {
    sector = ADDR_FLASH_SECTOR_11;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_12) && (Address < ADDR_FLASH_SECTOR_13))
  {
    sector = ADDR_FLASH_SECTOR_12;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_13) && (Address < ADDR_FLASH_SECTOR_14))
  {
    sector = ADDR_FLASH_SECTOR_13;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_14) && (Address < ADDR_FLASH_SECTOR_15))
  {
    sector = ADDR_FLASH_SECTOR_14;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_15) && (Address < ADDR_FLASH_SECTOR_16))
  {
    sector = ADDR_FLASH_SECTOR_15;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_16) && (Address < ADDR_FLASH_SECTOR_17))
  {
    sector = ADDR_FLASH_SECTOR_16;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_17) && (Address < ADDR_FLASH_SECTOR_18))
  {
    sector = ADDR_FLASH_SECTOR_17;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_18) && (Address < ADDR_FLASH_SECTOR_19))
  {
    sector = ADDR_FLASH_SECTOR_18;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_19) && (Address < ADDR_FLASH_SECTOR_20))
  {
    sector = ADDR_FLASH_SECTOR_19;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_20) && (Address < ADDR_FLASH_SECTOR_21))
  {
    sector = ADDR_FLASH_SECTOR_20;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_21) && (Address < ADDR_FLASH_SECTOR_22))
  {
    sector = ADDR_FLASH_SECTOR_21;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_22) && (Address < ADDR_FLASH_SECTOR_23))
  {
    sector = ADDR_FLASH_SECTOR_22;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_23) && (Address < ADDR_FLASH_SECTOR_24))
  {
    sector = ADDR_FLASH_SECTOR_23;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_24) && (Address < ADDR_FLASH_SECTOR_25))
  {
    sector = ADDR_FLASH_SECTOR_24;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_25) && (Address < ADDR_FLASH_SECTOR_26))
  {
    sector = ADDR_FLASH_SECTOR_25;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_26) && (Address < ADDR_FLASH_SECTOR_27))
  {
    sector = ADDR_FLASH_SECTOR_26;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_27) && (Address < ADDR_FLASH_SECTOR_28))
  {
    sector = ADDR_FLASH_SECTOR_27;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_28) && (Address < ADDR_FLASH_SECTOR_29))
  {
    sector = ADDR_FLASH_SECTOR_28;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_29) && (Address < ADDR_FLASH_SECTOR_30))
  {
    sector = ADDR_FLASH_SECTOR_29;
  }
  else if ((Address >= ADDR_FLASH_SECTOR_30) && (Address < ADDR_FLASH_SECTOR_31))
  {
    sector = ADDR_FLASH_SECTOR_30;
  }
  else
  {
    sector = ADDR_FLASH_SECTOR_31;
  }
  return sector;
}
//

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT YiLDiZ IoT END OF FILE ************************/
