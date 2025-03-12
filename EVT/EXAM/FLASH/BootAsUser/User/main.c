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
 *BOOT FLASH area 1920Byte is used for User Flash usage tutorial:
 *The User FLASH extension for this routine is 16Byte+1920Byte.
 *There are a few points to note when using this routine:
 *1.If you want to put the function in BOOT FLASH,you need to add the prefix
 *__attribute__((section(".Bcode"))).
 *2.BOOT FLASH and User FLASH function between the jump,in theory ,there will
 *be a 1us delay ,so the use of BOOT FLASH need to pay attention to.
 *3.The project can only be compiled after the use of WCH-LinkUtility V1.80 or
 *later version of the tool to download,and configure the address to select 0x1FFFF000.
 *And the tool only supports Hex file download.
 *4.BOOT FLASH does not support user code erasure,so the FLASH data can only be
 *stored in User FLASH area.
*/ 

#include "debug.h"


/* Global define */


/* Global Variable */

/*********************************************************************
 * @fn      GPIO_Toggle_INIT
 *
 * @brief   Initializes GPIOA.0
 *
 * @return  none
 */
__attribute__((section(".Bcode")))
void GPIO_Toggle_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
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
    vu8 t=0, i;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    Delay_Init();
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif
    printf("SystemClk:%d\r\n",SystemCoreClock);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    GPIO_Toggle_INIT();

    while(1)
    {
        Delay_Ms(250);
        printf("-%02d\r\n", t++);
        GPIO_WriteBit(GPIOD, GPIO_Pin_0, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));

    }
}

