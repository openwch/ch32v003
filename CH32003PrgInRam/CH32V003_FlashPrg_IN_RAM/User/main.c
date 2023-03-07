/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/08
 * Description        : Main program body.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/


#include "ch32v00x.h"

/* Flash Access Control Register bits */
#define ACR_LATENCY_Mask           ((uint32_t)0x00000038)

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

/* FLASH Mask */
#define RDPRT_Mask                 ((uint32_t)0x00000002)
#define WRP0_Mask                  ((uint32_t)0x000000FF)
#define WRP1_Mask                  ((uint32_t)0x0000FF00)
#define WRP2_Mask                  ((uint32_t)0x00FF0000)
#define WRP3_Mask                  ((uint32_t)0xFF000000)

/* FLASH Keys */
#define RDP_Key                    ((uint16_t)0x00A5)
#define FLASH_KEY1                 ((uint32_t)0x45670123)
#define FLASH_KEY2                 ((uint32_t)0xCDEF89AB)




/* flash unlock */
int Init(void)
{
    FLASH->ACTLR = 0; //Use default clock and zero latency
    //unlock fast program
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;

    FLASH->MODEKEYR = FLASH_KEY1;
    FLASH->MODEKEYR = FLASH_KEY2;
    return 0;
}

/* flash lock */
int Uninit(void)
{
    FLASH->CTLR |= CR_LOCK_Set;
    return 0;
}


/* all erase */
int EraseChip(void)
{
    FLASH->CTLR |= CR_MER_Set;
    FLASH->CTLR |= CR_STRT_Set;
    while(FLASH->STATR & SR_BSY)
    {
        IWDG->CTLR = 0xAAAA;
    }

    FLASH->CTLR &= ~CR_MER_Set;

    return 0;
}

/*
 * erase sector
 * addr: sector address
 * */
int EraseSector(uint32_t addr)
{
    FLASH->CTLR |= CR_PAGE_ER;
    FLASH->ADDR = addr;
    FLASH->CTLR |= CR_STRT_Set;
    while( FLASH->STATR & SR_BSY )
    {
        IWDG->CTLR = 0xAAAA;
    }
    FLASH->CTLR &= ~CR_STRT_Set;

    return 0;
}

/*
 * page program
 * addr: address
 * size: length of bytes
 * buffer: data buffer
 * */
int ProgramPage(uint32_t addr, uint32_t size, uint32_t *buffer)
{
    volatile uint32_t PAGE_ADD = addr;
    volatile uint32_t *p32 = buffer;
    volatile uint32_t  temp_addr = 0, i = 0;
    volatile uint8_t j;

    i = (size + 63) / 64;    //calculate pages

    FLASH->CTLR |= CR_PAGE_PG;
    do
    {
        FLASH->CTLR |= CR_BUF_RST;
        while(FLASH->STATR & SR_BSY)
        {
            IWDG->CTLR = 0xAAAA;
        }

        temp_addr = PAGE_ADD;
        j = 16;           //load 16 * 4Bytes

        do
        {
            *((volatile uint32_t*)temp_addr) = *p32++;
            FLASH->CTLR |= CR_BUF_LOAD;
            while(FLASH->STATR & SR_BSY)
            {
               IWDG->CTLR = 0xAAAA;
            }
            temp_addr += 4;
        }while(--j);


        FLASH->ADDR = PAGE_ADD;
        FLASH->CTLR |= CR_STRT_Set;
        while(FLASH->STATR & SR_BSY)  //start program
        {
            IWDG->CTLR = 0xAAAA;
        }
        if (FLASH->STATR & SR_WRPRTERR)   /*Ö´ÐÐ128×Ö½Ú±à³Ì*/
        {
            FLASH->STATR  |=  SR_WRPRTERR;
            FLASH->CTLR &= ~CR_PAGE_PG;
            return 1;
        }

        PAGE_ADD += 64;  //page address add 64

    }while(--i);

    FLASH->CTLR &= ~CR_PAGE_PG;
    return 0;
}

/*
 * Verify
 * addr: address
 * size: length of bytes
 * buffer: data buffer
 * */
int Verify(uint32_t addr, uint32_t size, uint32_t *buffer)
{
    volatile uint32_t verify_addr = addr;
    volatile uint32_t *p32 = buffer;
    volatile uint32_t i = 0, j = 0;

    j = (size + 3)/4;
    do
    {
        if(*p32++ != *(__IO u32*)(verify_addr + 4*i))
        {
            return 1;
        }
        i++;
    }while(i < j);

    return 0;
}

