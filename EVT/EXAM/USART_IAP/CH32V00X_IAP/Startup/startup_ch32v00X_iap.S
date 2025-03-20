;/********************************** (C) COPYRIGHT *******************************
;* File Name          : startup_ch32v00X_iap.s
;* Author             : WCH
;* Version            : V1.0.2
;* Date               : 2025/01/10
;* Description        : ch32v002-ch32v004-ch32v005-ch32v006-ch32v007-ch32m007 vector table for eclipse toolchain.
;*********************************************************************************
;* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
;* Attention: This software (modified or not) and binary are used for 
;* microcontroller manufactured by Nanjing Qinheng Microelectronics.
;*******************************************************************************/

	.section  .init, "ax", @progbits
	.globl  _start
	.align  2
_start:
	.option   norvc;
    j       handle_reset
    .word   0
    .word   NMI_Handler                  /* NMI Handler */
    .word   HardFault_Handler            /* Hard Fault Handler */

	.option rvc;
	.section  .text.vector_handler, "ax", @progbits
	.weak   NMI_Handler
	.weak   HardFault_Handler

NMI_Handler:
HardFault_Handler:
1:
	j 1b

	.section  .text.handle_reset, "ax", @progbits
	.weak     handle_reset
	.align    1
handle_reset:
.option push
.option norelax
	la gp, __global_pointer$
.option pop
1:
	la sp, _eusrstack
2:
/* Load data section from flash to RAM */
	la a0, _data_lma
	la a1, _data_vma
	la a2, _edata
	bgeu a1, a2, 2f
1:
	lw t0, (a0)
	sw t0, (a1)
	addi a0, a0, 4
	addi a1, a1, 4
	bltu a1, a2, 1b
2:
/* Clear bss section */
    la a0, _sbss
    la a1, _ebss
    bgeu a0, a1, 2f
1:
    sw zero, (a0)
    addi a0, a0, 4
    bltu a0, a1, 1b
2:
/* Enable global interrupt and configure privileged mode */
    li t0, 0x1880
    csrw mstatus, t0
/* Enable interrupt nesting and hardware stack */
    li t0, 0x3
    csrw 0x804, t0
/* Configure the interrupt vector table recognition mode and entry address mode */
    la t0, _start
    ori t0, t0, 3
    csrw mtvec, t0
  
    jal SystemInit
    la t0, main
    csrw mepc, t0
    mret

