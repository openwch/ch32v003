;/********************************** (C) COPYRIGHT *******************************
;* File Name          : startup_ch32v00x.s
;* Author             : WCH
;* Version            : V1.0.0
;* Date               : 2022/08/08
;* Description        : vector table for eclipse toolchain.
;* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
;* SPDX-License-Identifier: Apache-2.0
;*******************************************************************************/

	.section  .init, "ax", @progbits
	.globl  _start
	.align  2
_start:
	 ebreak          //insert ebreak at the front of the bin




