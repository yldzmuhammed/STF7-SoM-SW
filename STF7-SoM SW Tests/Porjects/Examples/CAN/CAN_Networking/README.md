/**
  @page ADC_RegularConversion_DMA conversion using DMA for Data transfer

  @verbatim
  ******************************************************************************
  * @file    ADC/ADC_RegularConversion_DMA/readme.txt 
  * @author  yldzmuhammed
  * @brief   Description of the ADC RegularConversion DMA example.
  ******************************************************************************
  *
  * Copyright (c) 2020 YiLDiZ IoT. All rights reserved.
  *
  * @attention
  *
  * experiment platform: YiLDiZ IoT STF7-SoM Board BSP Library
  * web-site: https://www.yildiziot.com
  ******************************************************************************
  @endverbatim

@par Example Description 

This example shows how to configure the CAN peripheral to send and receive 
CAN frames in normal mode. The sent frames are used to control Leds by pressing  
Tamper push-button.
	
The CAN serial communication link is a bus to which a number of units may be
connected. This number has no theoretical limit. Practically the total number
of units will be limited by delay times and/or electrical loads on the bus line.

This program behaves as follows:
  - After reset, all Leds are OFF on the N eval boards
  - By Pressing on Tamper push-button : LED1 turns ON and all other Leds are OFF, on the N
    eval boards connected to the bus. 
  - Press on Tamper push-button again to send CAN Frame to command LEDn+1 ON, all other Leds 
    are OFF on the N eval boards.
    
@note This example is tested with a bus of 3 units. The same program example is 
      loaded in all units to send and receive frames.
@note Any unit in the CAN bus may play the role of sender (by pressing Tamper push-button)
      or receiver.

  The CAN is configured as follows:
    - Bit Rate   = 1 Mbit/s  
    - CAN Clock  = external clock (HSE)
    - ID Filter  = All identifiers are allowed
    - RTR = Data
    - DLC = 1 byte
    - Data: Led number that should be turned ON

@par Keywords

Connectivity, CAN, Communication, Transmission, Reception, Polling, Networking, Send, Receive, 

@Note If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
      then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
      The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
      It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
      In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
      Even though the user must manage the cache coherence for read accesses.
      Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
      Please refer to the AN4839 “Level 1 cache on STM32F7 Series”

@par Directory contents 

  - CAN/CAN_Networking/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - CAN/CAN_Networking/Inc/stm32f7xx_it.h          DMA interrupt handlers header file
  - CAN/CAN_Networking/Inc/main.h                  Header for main.c module  
  - CAN/CAN_Networking/Src/stm32f7xx_it.c          DMA interrupt handlers
  - CAN/CAN_Networking/Src/main.c                  Main program
  - CAN/CAN_Networking/Src/stm32f7xx_hal_msp.c     HAL MSP file
  - CAN/CAN_Networking/Src/system_stm32f7xx.c      STM32F7xx system source file
  
@par Hardware and Software environment

  - This example runs on STM32F756xx/STM32F746xx devices.
  - This example has been tested with STM327x6G-EVAL board revB and can be
    easily tailored to any other supported device and development board.

  - STM327x6G-EVAL Set-up 
    - Use LED1, LED2, LED3 and LED4
    - Use Tamper push-button connected to PC.13
    - Connect a male/male CAN cable between at least 2 EVAL CAN connectors (CN22)
    - You have to configure the jumpers as follows:

  @verbatim
  
       +-------------------------------+
       |   Jumper    |       CAN1      |
       +-------------------------------+
       |   JP8       |      fitted     |
       |   JP11      |       1-2       |
       |   JP12      |      fitted     |
       +-------------------------------+
  @endverbatim
  
@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 