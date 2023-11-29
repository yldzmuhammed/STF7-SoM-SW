/**
 * @file    FaultHandler.c
 * @author  Ferenc Nemeth
 * @date    12 Aug 2018
 * @brief   This is the fault handler, where the program gets, if there is a problem.
 *          In case of a Hard fault, the following actions are going to be executed:
 *          - Prints out the stack frame registers (R0-R3, R12, LR, PC, PSR).
 *          - Prints out the fault status registers (HFSR, CFSR, MMFAR, BFAR, AFSR).
 *          - Prints out the EXC_RETURN register.
 *          - After these, the software analyzes the value of the registers
 *            and prints out a report regarding the fault.
 *          - Stop the execution with a breakpoint.
 *
 *          Copyright (c) 2018 Ferenc Nemeth - https://github.com/ferenc-nemeth
 */
#include <stdio.h>

#include "stm32g0xx_hal.h"//"stm32f7xx_hal.h"
#include "faulthandler.h"

/* Local function. */
void ReportHardFault(uint32_t *hard_fault_values, uint32_t exc);

/**
 * @brief   Prints the registers and gives detailed information about the error(s).
 * @param   *stack_frame: Stack frame registers (R0-R3, R12, LR, LC, PSR).
 * @param   exc: EXC_RETURN register.
 * @return  void
 */
void ReportHardFault(uint32_t *stack_frame, uint32_t exc)
{
  volatile uint32_t r0   = stack_frame[0];
  volatile uint32_t r1   = stack_frame[1];
  volatile uint32_t r2   = stack_frame[2];
  volatile uint32_t r3   = stack_frame[3];
  volatile uint32_t r12  = stack_frame[4];
  volatile uint32_t lr   = stack_frame[5];
  volatile uint32_t pc   = stack_frame[6];
  volatile uint32_t psr  = stack_frame[7];
  volatile uint32_t hfsr = 0;//SCB->HFSR;
  volatile uint32_t cfsr = 0;//SCB->CFSR;
  volatile uint32_t mmar = 0;//SCB->MMFAR;
  volatile uint32_t bfar = 0;//SCB->BFAR;
  volatile uint32_t afsr = 0;//SCB->AFSR;

  printf("\n!!!Hard Fault detected!!!\n");

  printf("\nStack frame:\n");
  printf("R0 :        0x%08X\n", r0);
  printf("R1 :        0x%08X\n", r1);
  printf("R2 :        0x%08X\n", r2);
  printf("R3 :        0x%08X\n", r3);
  printf("R12:        0x%08X\n", r12);
  printf("LR :        0x%08X\n", lr);
  printf("PC :        0x%08X\n", pc);
  printf("PSR:        0x%08X\n", psr);

  printf("\nFault status:\n");
  printf("HFSR:       0x%08X\n", hfsr);
  printf("CFSR:       0x%08X\n", cfsr);
  printf("MMAR:       0x%08X\n", mmar);
  printf("BFAR:       0x%08X\n", bfar);
  printf("AFSR:       0x%08X\n", afsr);

  printf("\nOther:\n");
  printf("EXC_RETURN: 0x%08X\n", exc);

  printf("\nDetails of the fault status:\n");
  printf("Hard fault status:\n");
  if (CHECK_BIT(hfsr, FORCED))
  {
    printf(" - Forced Hard fault.\n");
  }
  if (CHECK_BIT(hfsr, VECTTBL))
  {
    printf(" - Bus fault on vector table read.\n");
  }
  printf("MemManage fault status:\n");
  if (CHECK_BIT(cfsr, MMARVALID))
  {
    printf(" - MMAR holds a valid address.\n");
  }
  else
  {
    printf(" - MMAR holds an invalid address.\n");
  }
  if (CHECK_BIT(cfsr, MLSPERR))
  {
    printf(" - Fault occurred during floating-point lazy state preservation.\n");
  }
  if (CHECK_BIT(cfsr, MSTKERR))
  {
    printf(" - Stacking has caused an access violation.\n");
  }
  if (CHECK_BIT(cfsr, MUNSTKERR))
  {
    printf(" - Unstacking has caused an access violation.\n");
  }
  if (CHECK_BIT(cfsr, DACCVIOL))
  {
    printf(" - Load or store at a location that does not permit the operation.\n");
  }
  if (CHECK_BIT(cfsr, IACCVIOL))
  {
    printf(" - Instruction fetch from a location that does not permit execution.\n");
  }
  printf("Bus fault status:\n");
  if (CHECK_BIT(cfsr, BFARVALID))
  {
    printf(" - BFAR holds a valid address.\n");
  }
  else
  {
    printf(" - BFAR holds an invalid address.\n");
  }
  if (CHECK_BIT(cfsr, LSPERR))
  {
    printf(" - Fault occurred during floating-point lazy state preservation.\n");
  }
  if (CHECK_BIT(cfsr, STKERR))
  {
    printf(" - Stacking has caused a Bus fault.\n");
  }
  if (CHECK_BIT(cfsr, UNSTKERR))
  {
    printf(" - Unstacking has caused a Bus fault.\n");
  }
  if (CHECK_BIT(cfsr, IMPRECISERR))
  {
    printf(" - Data bus error has occurred, but the return address in the stack is not related to the fault.\n");
  }
  if (CHECK_BIT(cfsr, PRECISERR))
  {
    printf(" - Data bus error has occurred, and the return address points to the instruction that caused the fault.\n");
  }
  if (CHECK_BIT(cfsr, IBUSERR))
  {
    printf(" - Instruction bus error.\n");
  }
  printf("Usage fault status:\n");
  if (CHECK_BIT(cfsr, DIVBYZERO))
  {
    printf(" - The processor has executed an SDIV or UDIV instruction with a divisor of 0.\n");
  }
  if (CHECK_BIT(cfsr, UNALIGNED))
  {
    printf(" - The processor has made an unaligned memory access.\n");
  }
  if (CHECK_BIT(cfsr, NOCP))
  {
    printf(" - Attempted to access a coprocessor.\n");
  }
  if (CHECK_BIT(cfsr, INVPC))
  {
    printf(" - Illegal attempt to load of EXC_RETURN to the PC.\n");
  }
  if (CHECK_BIT(cfsr, INVSTATE))
    printf(" - Attempted to execute an instruction that makes illegal use of the EPSR.\n");
  
  if (CHECK_BIT(cfsr, INVSTATE))
    printf(" - The processor has attempted to execute an undefined instruction.\n");
  

  /* Breakpoint. */
  __asm volatile("BKPT #0");

  /* Infinite loop to stop the execution. */
  while(1);
}
//

/**
 * @brief   The program jumps here in case of a Hard fault.
 *          Checks which stack (MSP or PSP) was used.
 *          Extracts the stack frame location (and EXC_return),
 *          then pass them to ReportHardFault().
 * @param   void
 * @return  void
 */
void HardFault_Handler(void)
{
	register uint32_t LR __ASM("lr");
	
  __asm
  (
      "TST    LR, #0x04;      	"
      "ITE    eq;               "
      "MRSEQ  r0, MSP;          "
      "MRSNE  r0, PSP;          "
      "MOV    r1, LR;           "
      "BL			ReportHardFault;	"
  );
  /* Breakpoint. */
  __asm volatile("BKPT #0");
}
