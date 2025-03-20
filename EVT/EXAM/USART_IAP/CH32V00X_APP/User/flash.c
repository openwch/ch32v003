/********************************** (C) COPYRIGHT  *******************************
* File Name          : iap.c
* Author             : WCH
* Version            : V1.0.1
* Date               : 2025/01/13
* Description        : CH32V00X  fast program
*******************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include "flash.h"
#include "string.h"
u32 Verify_buf[32];

/*********************************************************************
 * @fn      CH32_IAP_Program
 *
 * @brief   adr - 256Byte stand
 *          buf - 256Byte stand
 *
 * @return  none
 */
void CH32_IAP_Program(u32 adr, u32* buf)
{
    adr &= 0xFFFFFF00;

    //FLASH_BufReset();
    FLASH->CTLR |= ((uint32_t)0x00010000);
    FLASH->CTLR |= ((uint32_t)0x00080000);
    while(FLASH->STATR & ((uint32_t)0x00000001))
        ;
    FLASH->CTLR &= ~((uint32_t)0x00010000);
    for(int j=0;j<64;j++)
       {
           //FLASH_BufLoad(adr+4*j, buf[j]);
           FLASH->CTLR |= ((uint32_t)0x00010000);
           *(__IO uint32_t *)(adr+4*j) = buf[j];
           FLASH->CTLR |= ((uint32_t)0x00040000);
           while(FLASH->STATR & ((uint32_t)0x00000001))
               ;
           FLASH->CTLR &= ~((uint32_t)0x00010000);
       }
    //FLASH_ProgramPage_Fast(adr);
    FLASH->CTLR |= ((uint32_t)0x00010000);
    FLASH->ADDR = adr;
    FLASH->CTLR |= ((uint32_t)0x00000040);
    while(FLASH->STATR & ((uint32_t)0x00000001))
        ;
    FLASH->CTLR &= ~((uint32_t)0x00010000);
}

