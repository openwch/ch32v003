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
 *
 *This program used DMA and TIM to emulate the TX of uart,and used TIM and EXTI to emulate the RX of uart(9600bps).
 *This example is used to receive data and send specified quantity of data
 *You can redistribute it and/or modify it by changing the clock source and destination of struct in the function In SoftUART.c(RxInit & TxInit & Init)
 * 
 *Presently Hardware connection:PD2 -- Rx
 *                              PD4 -- Tx
 */

#include "debug.h"
#include "SoftUART.h"

/* Global define */

/* Global Variable */
vu8 val;

uint8_t tmp[20] = {0x01, 0x80, 0xaa, 0x55};

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("ChipID:%08x\r\n", DBGMCU_GetCHIPID());

    SoftUARTFunc.Init(9600);

    while (1)
    {
        if (SoftUARTFunc.ReceiveBytes(tmp, 0xff))
            SoftUARTFunc.TransmitBytes(tmp, 4);
    }
}
