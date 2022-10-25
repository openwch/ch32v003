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
 硬件流控制模式例程：
 USART1_Tx(PD5)、USART1_Rx(PD6)、USART1_CTS(PD3)、USART1_RTS(PC2)
 本例程演示 UART1 硬件流控制数据收发，将UART1 Tx和Rx引脚分别连接串口工具，
 通过上位机工具查看，CTS连接VCC，无法发送数据，接GND，可正常发送数据

*/

#include "debug.h"

/* Global typedef */
typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
} TestStatus;

/* Global define */
#define TxSize     (size(TxBuffer))
#define size(a)    (sizeof(a) / sizeof(*(a)))

/* Global Variable */
u8 TxBuffer[] = "USART1 Hardware Flow Control Example: USART1-\
Hyperterminal communication using hardware flow control\n\r";
u8 RxBuffer[TxSize] = {0};
u8 TxCnt = 0, RxCnt = 0;

/*********************************************************************
 * @fn      Buffercmp
 *
 * @brief   Compares two buffers
 *
 * @param   Buf1,Buf2 - buffers to be compared
 *          BufferLength - buffer's length
 *
 * @return  PASSED - Buf1 identical to Buf
 *          FAILED - Buf1 differs from Buf2
 */
TestStatus Buffercmp(uint8_t *Buf1, uint8_t *Buf2, uint16_t BufLength)
{
    while(BufLength--)
    {
        if(*Buf1 != *Buf2)
        {
            return FAILED;
        }
        Buf1++;
        Buf2++;
    }
    return PASSED;
}

/*********************************************************************
 * @fn      USART1_ReCFG
 *
 * @brief   Reinitialize the USART1.
 *
 * @return  none
 */
void USART1_ReCFG(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC | RCC_APB2Periph_USART1, ENABLE);

    /* USART1 TX-->D.5   RX-->D.6 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; /* RTS-->C.2 */
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; /* CTS-->D.3 */
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
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
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    USART1_ReCFG();

    printf("USART1 Hardware Flow Control TEST\r\n");

    while(TxCnt < TxSize)
    {
        USART_SendData(USART1, TxBuffer[TxCnt++]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
    do
    {
        if((USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET) && (RxCnt < TxSize))
        {
            RxBuffer[RxCnt] = USART_ReceiveData(USART1);
            USART_SendData(USART1, RxBuffer[RxCnt++]); /* send it when receive a byte */
        }
    } while((RxBuffer[RxCnt - 1] != '\r') && (RxCnt != TxSize));

    while(1)
    {
    }
}
