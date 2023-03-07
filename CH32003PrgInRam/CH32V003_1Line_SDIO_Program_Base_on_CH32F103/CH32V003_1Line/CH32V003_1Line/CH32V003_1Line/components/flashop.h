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
#ifndef __FLASHOP_H
#define __FLASHOP_H

#include "debug.h"
#include "singleline.h"


//FLASH unlock
#define UNLOCK_KEY1         0x45670123
#define UNLOCK_KEY2         0xcdef89ab

//Flash ²Ù×÷¼Ä´æÆ÷
#define FLASH_KEYR_ACR      0x40022000
#define FLASH_KEYR_Addr     0x40022004
#define FLASH_OPTKEYR_Addr  0x40022008
#define FLASH_SR_Addr       0x4002200C
#define FLASH_CR_Addr       0x40022010
#define FLASH_AR_Addr       0x40022014

#define FLASH_OBR_Addr      0x4002201C
#define FLASH_WRPR_Addr     0x40022020

#define FLASH_MODEKEYR_Addr 0x40022024




/* Flash Control Register bits */
#define CR_PG_Set                  ((uint32_t)0x00000001)
#define CR_PG_Reset                ((uint32_t)0xFFFFFFFE)
#define CR_PER_Set                 ((uint32_t)0x00000002)
#define CR_PER_Reset               ((uint32_t)0xFFFFFFFD)
#define CR_MER_Set                 ((uint32_t)0x00000004)
#define CR_MER_Reset               ((uint32_t)0xFFFFFFFB)
#define CR_OPTPG_Set               ((uint32_t)0x00000010)
#define CR_OPTPG_Reset             ((uint32_t)0xFFFFFFEF)
#define CR_OPTER_Set               ((uint32_t)0x00000020)
#define CR_OPTER_Reset             ((uint32_t)0xFFFFFFDF)
#define CR_STRT_Set                ((uint32_t)0x00000040)
#define CR_LOCK_Set                ((uint32_t)0x00000080)
#define CR_PAGE_PG                 ((uint32_t)0x00010000)
#define CR_PAGE_ER                 ((uint32_t)0x00020000)
#define CR_BUF_LOAD                ((uint32_t)0x00040000)
#define CR_BUF_RST                 ((uint32_t)0x00080000)


/* FLASH Status Register bits */
#define SR_BSY                     ((uint32_t)0x00000001)
#define SR_WRPRTERR                ((uint32_t)0x00000010)
#define SR_EOP                     ((uint32_t)0x00000020)








uint8_t SingleLine_OptionByte_PageErase(void);
uint8_t SingleLineSoftRest(void);
uint8_t SingleLine_Remove_ReadProtect(void);


uint8_t FLASH_FAST_MODE_UNLOCK(void);
uint8_t FLASH_BUF_RESET(void);
uint8_t FLASH_BUF_LOAD(uint32_t addr, uint32_t data);
uint8_t FLASH_ERASE_FAST(uint32_t addr);
uint8_t FLASH_PAGE_PG_FAST(uint32_t addr);



#endif

