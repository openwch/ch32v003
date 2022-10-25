/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/08
 * Description        : Main program body.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/*
 *@Note
 多处理器通信模式例程：
 Master：USART1_Tx(PD5)、USART1_Rx(PD6)。
 Slave：USART1_Tx(PD5)、USART1_Rx(PD6)。

 本例程演示 一块板子作主机，一块板子作从机，主机发送地址 0x02，使 从机退出
 静默模式，完成后续通信。
 注：
     硬件连线：PD5 —— PD6
           PD6 —— PD5
           PDO —— LED

*/

#include "debug.h"

/* Global typedef */

/* Global define */

/* Global Variable */

/* USART Mode Definition */
#define HOST_MODE     0
#define SLAVE_MODE    1

/* USART Communication Mode Selection */
#define USART_MODE      HOST_MODE
//#define USART_MODE     SLAVE_MODE

/*********************************************************************
 * @fn      GPIO_Toggle_INIT
 *
 * @brief   Initializes GPIOA.0
 *
 * @return  none
 */
void GPIO_Toggle_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_SetBits(GPIOD, GPIO_Pin_0);
}

/*********************************************************************
 * @fn      USARTx_CFG
 *
 * @brief   Initializes the USART1 peripheral.
 *
 * @return  none
 */
void USARTx_CFG(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

    /* USART1 TX-->D.5   RX-->D.6 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);

#if(USART_MODE == HOST_MODE)
    USART_SetAddress(USART1, 0x1);

#elif(USART_MODE == SLAVE_MODE)
    USART_SetAddress(USART1, 0x2);
    USART_WakeUpConfig(USART1, USART_WakeUp_AddressMark);
    USART_ReceiverWakeUpCmd(USART1, ENABLE); /* USART1 Into Silence */
#endif
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
    u8 i=0;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();

    GPIO_Toggle_INIT();
    USARTx_CFG();
    Delay_Ms(1000);

    while(1)
    {
#if(USART_MODE == HOST_MODE)
        Delay_Ms(1000);
        USART_SendData(USART1, 0x102);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) /* waiting for sending finish */
        {
        }
        USART_SendData(USART1, 0xAA);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) /* waiting for sending finish */
        {
        }


#elif(USART_MODE == SLAVE_MODE)
        if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
        {
            if(USART_ReceiveData(USART1) == 0xAA)
            {
                GPIO_WriteBit(GPIOD, GPIO_Pin_0, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
            }
        }
#endif
    }
}
