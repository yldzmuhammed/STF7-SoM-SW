/**
  ******************************************************************************
  * STM32L4 Bootloader
  ******************************************************************************
  * @author Akos Pasztor
  * @file   bootloader.h
  * @brief  Bootloader header
  *	        This file contains the bootloader configuration parameters,
  *	        function prototypes and other required macros and definitions.
  * @see    Please refer to README for detailed information.
  ******************************************************************************
  * Copyright (c) 2018 Akos Pasztor.                    https://akospasztor.com
  ******************************************************************************
**/

#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "stm32f7xx_hal.h"

/* Public typedef ------------------------------------------------------------*/

/* Bootloader Error Codes */
typedef enum
{
	BL_OK = 0,
	BL_NO_APP,
	BL_SIZE_ERROR,
	BL_CHKS_ERROR,
	BL_ERASE_ERROR,
	BL_WRITE_ERROR,
	BL_FLASH_ERROR,
	BL_OBP_ERROR,
	
	UPD_NO_UPDATE,
	UPD_RELEASED,
	
} eBootloaderErrorCodes;
//

/* Flash Protection Types */
enum
{
	BL_PROTECTION_NONE  = 0,
	BL_PROTECTION_WRP   = 0x1,
	BL_PROTECTION_RDP   = 0x2,
	BL_PROTECTION_PCROP = 0x4,
};
//

/* sector index */
typedef enum
{
	eSecIndx_FirstSector 	= 0,
	eSecIndx_SecondSector = 1
} eSectorIndex_TypeDef;
//

/* Version struct */
#pragma anon_unions
typedef union
{
	struct
	{
		uint8_t							major;
		uint8_t							minor;
		uint8_t							patch;
		uint8_t							revision;
	};
	uint32_t all;
}Version_TypeDef;
//

/* xBinary struct */
typedef struct
{
	uint32_t 				crc;
	uint32_t 				size;
	uint32_t 				address;
	Version_TypeDef version;
} xSectorInfo_TypeDef;
//

typedef struct
{
	eSectorIndex_TypeDef 	eBootSector;
	eSectorIndex_TypeDef	eDownloadSector;
	
	xSectorInfo_TypeDef		xInfo[2];
} xFlashSectorTypeDef;
//

typedef struct
{
	xFlashSectorTypeDef Bootloader;
	xFlashSectorTypeDef UserApplication;
} xMasterBootRecoder_TypeDef;
//

/* Public define -------------------------------------------------------------*/
#define FLASHTYPE_INTERNAL							0
#define FLASHTYPE_EXTERNAL_FMC					1
#define FLASHTYPE_EXTERNAL_QSPI					2
#define FLASH_MEMORY										FLASHTYPE_EXTERNAL_FMC

#define FLASH_EXTERNAL_QSPI_BASE_ADDR		(QSPI_BASE)
#define FLASH_EXTERNAL_FMC_BASE_ADDR		(0x60000000U)

#define BOOTTABLE_SIZE									(0x00020000U)				/*<!  128 KB */
#define USEREEPROM_SIZE									(0x00020000U)				/*<!  128 KB */
#define BOOTLOADER1_SIZE								(0x00020000U)				/*<!  128 KB */
#define BOOTLOADER2_SIZE								(0x00020000U)				/*<!  128 KB */
#define APPFIRMWARE1_SIZE								(0x00180000U)				/*<! 1,75 MB */
#define APPFIRMWARE2_SIZE								(0x00180000U)				/*<! 1,75 MB */
#define EXTERNALFLASH_SIZE							(0x00400000U)				/*<!    4 MB */

#define EXTERNALFLASH_BASE							(0x00000000U)
#define EXTERNALFLASH_END								(EXTERNALFLASH_SIZE)

#define BOOTTABLE_BASE									(EXTERNALFLASH_BASE)
#define USEREEPROM_BASE									(BOOTTABLE_BASE 		+ BOOTTABLE_SIZE)
#define BOOTLOADER1_BASE								(USEREEPROM_BASE 		+ USEREEPROM_SIZE)
#define BOOTLOADER2_BASE								(BOOTLOADER1_BASE 	+ BOOTLOADER1_SIZE)
#define APPFIRMWARE1_BASE								(BOOTLOADER2_BASE		+ BOOTLOADER2_SIZE)
#define APPFIRMWARE2_BASE								(APPFIRMWARE1_BASE 	+ APPFIRMWARE1_SIZE)


#define BOOTTABLE_END										(BOOTTABLE_BASE 		+ BOOTTABLE_SIZE)
#define USEREEPROM_END									(USEREEPROM_BASE		+ USEREEPROM_SIZE)
#define BOOTLOADER1_END									(BOOTLOADER1_BASE		+ BOOTLOADER1_SIZE)
#define BOOTLOADER2_END									(BOOTLOADER2_BASE		+ BOOTLOADER2_SIZE)
#define APPFIRMWARE1_END								(APPFIRMWARE1_BASE 	+ APPFIRMWARE1_SIZE)
#define APPFIRMWARE2_END								(APPFIRMWARE2_BASE 	+ APPFIRMWARE2_SIZE)


#define BOOTPARTITION										(BOOTTABLE_BASE)

#define BOOTPARTITION_ADDRESS						(BOOTPARTITION 			+ 	FLASH_EXTERNAL_FMC_BASE_ADDR)
#define BOOTLOADER1_ADDRESS							(BOOTLOADER1_BASE 	+ 	FLASH_EXTERNAL_FMC_BASE_ADDR)
#define BOOTLOADER2_ADDRESS							(BOOTLOADER2_BASE 	+ 	FLASH_EXTERNAL_FMC_BASE_ADDR)

#define APPFIRMWARE1_ADDRESS						(APPFIRMWARE1_BASE 	+ 	FLASH_EXTERNAL_FMC_BASE_ADDR)
#define APPFIRMWARE2_ADDRESS						(APPFIRMWARE2_BASE 	+ 	FLASH_EXTERNAL_FMC_BASE_ADDR)

#define FIRST_STAGE_BOOTLOADER_ADDRESS	(0x08000000U)
#define SECOND_STAGE_BOOTLOADER_ADDRESS (FLASHAXI_BASE			+ 	(32 * 1024))  									/* Start address of application space in flash */

#define DEFAULT_FLASH_VALUE							(0xFF)
#define DEFAULT_VERSION_VALUE						(0x00000000)

#define SDRAM_BASE_ADDRESS							(0xC0000000U)
#define DROM_STORE_ADDRESS							(SDRAM_BASE_ADDRESS + (9 		* 1024 * 1024))
#define IROM_STORE_ADDRESS							(DROM_STORE_ADDRESS + (128 	* 1024))

#define BOOTLOADER_BOOTLDRFIRMWARE_URL	("fota.yildiziot.com/stf7som/bootloader/bootloader.bin")
#define BOOTLOADER_APPFIRMWARE_URL			("fota.yildiziot.com/stf7som/appfirmware/appfirmware.bin")

/*** Bootloader Configuration *************************************************/
#define USE_CHECKSUM            				0       								/* Check application checksum on startup */
#define USE_WRITE_PROTECTION    				0       								/* Enable write protection after performing in-app-programming */
#define SET_VECTOR_TABLE        				1       								/* Automatically set vector table location before launching application */
#define CLEAR_RESET_FLAGS       				1       								/* If enabled: bootloader clears reset flags. (This occurs only when OBL RST flag is active.)
																																	If disabled: bootloader does not clear reset flags, not even when OBL RST is active. */

#define APP_ADDRESS     								(uint32_t)0x08008000    /* Start address of application space in flash */
#define END_ADDRESS     								(uint32_t)0x080FFFFB    /* End address of application space (addr. of last byte) */
#define CRC_ADDRESS     								(uint32_t)0x080FFFFC    /* Start address of application checksum in flash */
#define SYSMEM_ADDRESS  								(uint32_t)0x1FFF0000    /* Address of System Memory (ST Bootloader) */

/* MCU RAM size, used for checking accurately whether flash contains valid application */
#if (STM32F750xx)
#define INTERNAL_RAM_SIZE        				((uint32_t)(SRAM1_BASE - SRAM2_BASE))
#elif (STM32L476xx)
#define RAM_SIZE        								(uint32_t)0x00018000
#endif

#define FLASH_PAGE_NBPERBANK    				256             				/* Number of pages per bank in flash */
#define APP_SIZE        								(uint32_t)(((END_ADDRESS - APP_ADDRESS) + 3) / 4) /* Size of application in DWORD (32bits or 4bytes) */

/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
extern xMasterBootRecoder_TypeDef xMasterBootRecord;
/* Public function prototypes ------------------------------------------------*/

//void    BSP_Bootloader_FlashEnd(void);
//void    BSP_Bootloader_FlashBegin(void);
void    BSP_Bootloader_JumpToSysMem(void);

void    BSP_Bootloader_JumpToApplication(uint32_t address);
void 		BSP_Bootloader_Get_BootTable(xMasterBootRecoder_TypeDef *extFlash);

uint8_t BSP_Bootloader_Erase(void);
bool		BSP_BootLoader_FormatFlash(void);
uint8_t BSP_Bootloader_GetProtectionStatus(void); 

uint8_t BSP_Bootloader_FlashNext(uint64_t data);
uint8_t BSP_Bootloader_VerifyChecksum(xMasterBootRecoder_TypeDef *xROM);
uint8_t BSP_Bootloader_CheckForApplication(uint32_t address);
uint8_t BSP_Bootloader_ConfigProtection(uint32_t protection);
bool 		BSP_Bootloader_Set_BootTable(xMasterBootRecoder_TypeDef *extFlash);

#endif /* __BOOTLOADER_H */

