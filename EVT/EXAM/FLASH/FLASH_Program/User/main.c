/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/01/01
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *FLASH erase/read/write, and fast programming:
 *Includes Standard Erase and Program, Fast Erase and Program.
 *
*/

#include "debug.h"


/* Global define */
#define Fadr    0x08003000
#define Fsize   (256>>2)
u32 buf[Fsize];

/*********************************************************************
 * @fn      Option_Byte_CFG
 *
 * @brief   Config Option byte and enable reset pin.
 *
 * @return  none
 */
void Option_Byte_CFG(void)
{
    FLASH_Status status = FLASH_COMPLETE;

    status = FLASH_UserOptionByteConfig(OB_IWDG_SW,  OB_STDBY_NoRST, OB_RST_EN_DT12ms, OB_PowerON_Start_Mode_BOOT);

    if(status == FLASH_RDP)
    printf("MCU in read protected state, user option byte cannot be configured. \
            Need to use WCHISPTool or WCH-LinkUtility for configuration ");
    else{
        printf("0x1FFFF800-%08x\r\n", *(u32*)0x1FFFF800);
        printf("0x1FFFF804-%08x\r\n", *(u32*)0x1FFFF804);
        printf("0x1FFFF808-%08x\r\n", *(u32*)0x1FFFF808);
        printf("0x1FFFF80C-%08x\r\n", *(u32*)0x1FFFF80C);
    }
}

/*********************************************************************
 * @fn      Flash_Test_Fast
 *
 * @brief   Flash Fast Program Test.
 *
 * @return  none
 */
void Flash_Test_Fast(void)
{
    u32 i;
    u8 Verify_Flag = 0;
    FLASH_Status s;

    for(i = 0; i < Fsize; i++){
        buf[i] = i;
    }

    printf("Read flash\r\n");
    for(i=0; i<Fsize; i++){
        printf("adr-%08x v-%08x\r\n", Fadr +4*i, *(u32*)(Fadr +4*i));
    }

    s = FLASH_ROM_ERASE(Fadr, Fsize*4);
    if(s != FLASH_COMPLETE)
    {
        printf("check FLASH_ADR_RANGE_ERROR FLASH_ALIGN_ERROR or FLASH_OP_RANGE_ERROR\r\n");
        return;
    }

    printf("Erase flash\r\n");
    for(i=0; i<Fsize; i++){
        printf("adr-%08x v-%08x\r\n", Fadr +4*i, *(u32*)(Fadr +4*i));
    }

    s = FLASH_ROM_WRITE(Fadr,  buf, Fsize*4);
    if(s != FLASH_COMPLETE)
    {
        printf("check FLASH_ADR_RANGE_ERROR FLASH_ALIGN_ERROR or FLASH_OP_RANGE_ERROR\r\n");
        return;
    }

    printf("Write flash\r\n");
    for(i=0; i<Fsize; i++){
        printf("adr-%08x v-%08x\r\n", Fadr +4*i, *(u32*)(Fadr +4*i));
    }

    for(i = 0; i < Fsize; i++){
        if(buf[i] == *(u32 *)(Fadr + 4 * i))
        {
            Verify_Flag = 0;
        }
        else
        {
            Verify_Flag = 1;
            break;
        }
    }

    if(Verify_Flag)
        printf("%d Byte Verify Fail\r\n", (Fsize*4));
    else
        printf("%d Byte Verify Suc\r\n", (Fsize*4));
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    SystemCoreClockUpdate();
    Delay_Init();
    Delay_Ms(1000);
    
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif

    printf("SystemClk-1:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    Flash_Test_Fast();

    while(1);
}


