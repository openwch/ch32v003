/********************************** (C) COPYRIGHT  *******************************
* File Name          : iap.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2022/11/21
* Description        : CH32V003  fast program
*******************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include "flash.h"
#include "string.h"
u32 Verity_buf[32];


/*********************************************************************
 * @fn      CH32_IAP_Program
 *
 * @brief   adr - 64Byte stand
 *          buf - 64Byte stand
 *
 * @return  none
 */
void CH32_IAP_Program(u32 adr, u32* buf)
{
    adr &= 0xFFFFFFC0;
    FLASH_BufReset();
    for(int j=0;j<16;j++)
       {
           FLASH_BufLoad(adr+4*j, buf[j]);

       }
    FLASH_ProgramPage_Fast(adr);
}

