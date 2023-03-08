/********************************** (C) COPYRIGHT *******************************
 * File Name          : flashop.h
 * Author             : RVMaking
 * Version            : V1.0.0
 * Date               : 2022/08/01
 * Description        : single line debug interface for
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#ifndef __FLASH_PRG_ALGORITHM_H
#define __FLASH_PRG_ALGORITHM_H

#include "debug.h"
#include "singleline.h"

typedef struct
{
		uint32_t breakpoint;   
		uint32_t stackpoint;  //sp
}prggram_syscall_t;


typedef struct
{
		const uint32_t  init;
		const uint32_t  uninit;           
	  const uint32_t  erasechip;     
		const uint32_t  erasesector;
 	  const uint32_t  programpage;
	  const prggram_syscall_t sys_call_s;
		const uint32_t  program_buffer;      // mem buffer location
		const uint32_t  algo_start;          // location to write prog_blob in target RAM   
		const uint32_t  algo_size;           // prog_blob size
	  const uint8_t   *algo_blob;           // address of prog_blob
		const uint32_t  program_buffer_size; // ram_to_flash_bytes_to_be_written
}program_target_t;


uint8_t SingleLine_Flash_Init(void);
uint8_t SingleLine_Flash_Uninit(void);
uint8_t SingleLine_Flash_ProgramPages(uint32_t addr, uint32_t *buf, uint32_t size);
uint8_t SingleLine_Flash_EraseSector(uint32_t addr);
uint8_t SingleLine_Flash_EraseChip(void);

#endif

