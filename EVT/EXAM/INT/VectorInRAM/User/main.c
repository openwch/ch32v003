/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.1
 * Date               : 2026/10/15
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *Vector in RAM routine:
 *This example is used to demonstrate IRQ whose vector is in RAM
 */
#include "debug.h"

/* Global define */

/* Global Variable */


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
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("Vector in RAM Test\r\n");

    NVIC_EnableIRQ(Software_IRQn);

    while(1)
    {
        NVIC_SetPendingIRQ(Software_IRQn);
        Delay_Ms(1000);
        printf("This is a massage from main\n");
        Delay_Ms(1000);
    }
}

void SW_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast"))); 
void SW_Handler(void)
{
    NVIC_ClearPendingIRQ(Software_IRQn);
    #if 0
    printf("Software interrupt\n");
    #endif
}
