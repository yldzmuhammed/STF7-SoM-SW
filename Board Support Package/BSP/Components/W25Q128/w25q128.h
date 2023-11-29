/**
  ******************************************************************************
  * @file    w25q128.h
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

#ifndef __STF7SOM_W25Q128_H
#define __STF7SOM_W25Q128_H

#include <stdint.h>

/* W25Q128FV Micron memory */
/* Size of the flash */
#define QSPIFLASH_SIZE                      23						/* Address bus width accesses the entire memory space */
#define QSPIFLASH_PAGE_SIZE                 256						/* Flash Page Size In Bytes */
#define QSPIFLASH_END_ADDR                  (1 << QSPIFLASH_SIZE)	/* End address of the QSPI memory */


/* Private define ------------------------------------------------------------*/
/* Command definition - beginning *******************************/
/** @brief  W25Q128FV configuration */
#define W25Q128FV_FLASH_SIZE                (0x01000000)			/* 128 MBits => 16MBytes */
#define W25Q128FV_SECTOR_SIZE               (0x00010000)			/* 256 sectors of 64KBytes */
#define W25Q128FV_SUBSECTOR_SIZE            (0x00001000)			/* 4096 subsectors of 4kBytes */
#define W25Q128FV_PAGE_SIZE                 (0x00000100)			/* 65536 pages of 256 bytes */

#define W25Q128FV_DUMMY_CYCLES_READ_DUAL    (0x04)
#define W25Q128FV_DUMMY_CYCLES_READ_QUAD    (0x08)

#define W25Q128FV_SECTOR_ERASE_MAX_TIME     (0x0BB8)
#define W25Q128FV_SUBSECTOR_ERASE_MAX_TIME  (0x0320)
#define W25Q128FV_BULK_ERASE_MAX_TIME       (0x3D090)

/* Default dummy clocks cycles */
#define DUMMY_CLOCK_CYCLES_READ             (0x08)
#define DUMMY_CLOCK_CYCLES_READ_QUAD        (0x0A)

#define DUMMY_CLOCK_CYCLES_READ_DTR         (0x06)
#define DUMMY_CLOCK_CYCLES_READ_QUAD_DTR    (0x08)

#define DUMMY_CLOCK_CYCLES_2                (0x00)
#define DUMMY_CLOCK_CYCLES_4                (0x10)
#define DUMMY_CLOCK_CYCLES_6                (0x20)
#define DUMMY_CLOCK_CYCLES_8                (0x30)

/** @brief  W25Q128FV Instruction Set */
/* Reset operation */
#define RESET_ENABLE_CMD                    (0x66)
#define RESET_MEMORY_CMD                    (0x99)

#define ENTER_QPI_MODE_CMD                  (0x38)
#define EXIT_QPI_MODE_CMD                   (0xFF)

/* Identification operation */
#define READ_ID_CMD                         (0x90)
#define DUAL_READ_ID_CMD                    (0x92)
#define QUAD_READ_ID_CMD                    (0x94)
#define READ_UNIQ_ID_CMD                    (0x4B)
#define READ_JEDEC_ID_CMD                   (0x9F)
/* Read operation */
#define READ_CMD                            (0x03)
#define FAST_READ_CMD                       (0x0B)
#define DUAL_OUT_FAST_READ_CMD	            (0x3B)
#define QUAD_OUT_FAST_READ_CMD	            (0x6B)
#define DUAL_INOUT_FAST_READ_CMD            (0xBB)
#define QUAD_INOUT_FAST_READ_CMD            (0xEB)


/* Write operation */
#define WRITE_ENABLE_CMD                    (0x06)
#define WRITE_DISABLE_CMD                   (0x04)
#define WRITE_ENABLE_VOLATILE_SR_CMD        (0x50)

/* Register operation */
#define READ_STATUS_REG1_CMD	            (0x05)
#define READ_STATUS_REG2_CMD	            (0x35)
#define READ_STATUS_REG3_CMD	            (0x15)

#define WRITE_STATUS_REG1_CMD	            (0x01)
#define WRITE_STATUS_REG2_CMD	            (0x31)
#define WRITE_STATUS_REG3_CMD	            (0x11)

#define SET_READ_PARAMETERS_CMD             (0xC0)

/* Programming operation */
#define PAGE_PROG_CMD                       (0x02)
#define QUAD_INPUT_PAGE_PROG_CMD            (0x32)
#define EXT_QUAD_IN_FAST_PROG_CMD           (0x12)

/* Erase operation */
#define SECTOR_ERASE_CMD                    (0x20)
#define BLOCK_32KB_ERASE_CMD                (0x52)
#define BLOCK_64KB_ERASE_CMD                (0xD8)
#define CHIP_ERASE_CMD                      (0xC7)

#define PROG_ERASE_RESUME_CMD               (0x7A)
#define PROG_ERASE_SUSPEND_CMD              (0x75)


/* Status register flag */
#define W25Q128FV_FSR_BUSY                  ((uint8_t)0x01)    /*!< busy */
#define W25Q128FV_FSR_WREN                  ((uint8_t)0x02)    /*!< write enable */
#define W25Q128FV_FSR_QE                    ((uint8_t)0x02)    /*!< quad enable */
/*Command definition - end*******************************/

typedef struct
{
	uint32_t UniqUD;
	uint32_t JEDECID;
	uint32_t DeviceID;
} xQSPIFlashID_TypeDef;
#endif

/************************ (C) COPYRIGHT YiLDiZ IoT END OF FILE ************************/
