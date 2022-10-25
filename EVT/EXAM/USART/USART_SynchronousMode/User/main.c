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
 同步模式例程：
 Master：USART1_CK(PD4)、USART1_Tx(PD5)、USART1_Rx(PD6)。
 Slave：SPI1_SCK(PC5)、SPI1_MISO(PC7)、SPI1_MOSI(PC6)。
 本例程演示使用 USART1 作主机和 SPI1作从机，全双工收发数据,收发成功后PD0接LED，闪烁。
 UART1-LSB  SPI1-MSB

 硬件连线：PD4  —— PC5
          PD5  —— PC6
          PD6  —— PC7
          PDO —— LED
*/

#include "debug.h"

/* Global typedef */
typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
} TestStatus;

/* Global define */
#define TxSize1    (size(TxBuffer1))
#define TxSize2    (size(TxBuffer2))
#define size(a)    (sizeof(a) / sizeof(*(a)))

/* Global Variable */
u8 TxBuffer1[] = {0x01,0x02,0x03,0x04,0x05}; /* Send by UART1 */
u8 TxBuffer2[] = {0x01,0x02,0x03,0x04,0x05};; /* Send by SPI1  */
u8 pbuf[] = {0x80,0x40,0xC0,0x20,0xA0};
u8 RxBuffer1[TxSize2] = {0};                                                 /* USART1 Using  */
u8 RxBuffer2[TxSize1] = {0};                                                 /* SPI1   Using  */

u8 TxCnt1 = 0, RxCnt1 = 0;
u8 TxCnt2 = 0, RxCnt2 = 0;
u8 Tempdata = 0x00;

TestStatus TransferStatus1 = FAILED;
TestStatus TransferStatus2 = FAILED;

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
 * @brief   ReInitializes the USART1 peripheral.
 *
 * @return  none
 */
void USART1_ReCFG(void)
{
    GPIO_InitTypeDef       GPIO_InitStructure = {0};
    USART_InitTypeDef      USART_InitStructure = {0};
    USART_ClockInitTypeDef USART_ClockInitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

    /* USART1  Ck-->D.4   TX-->D.5   RX-->D.6 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
     GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_ClockInitStructure.USART_Clock = USART_Clock_Enable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_High;         /* Clock is active High */
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;        /* Data is captured on the second edge */
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Enable;
    USART_ClockInit(USART1, &USART_ClockInitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

/*********************************************************************
 * @fn      SPI1_INIT
 *
 * @brief   Initializes the SPI1 to be Slave Mode.
 *
 * @return  none
 */
void SPI1_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    SPI_InitTypeDef  SPI_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_SPI1, ENABLE);
    SPI_StructInit(&SPI_InitStructure);
    SPI_I2S_DeInit(SPI1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; /* SPI1 MISO-->PC.7 */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; /* SPI1 SCK-->PC.5 MOSI-->PC.6 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
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
    Delay_Ms(1000);
    GPIO_Toggle_INIT();
    USART1_ReCFG(); /* USART1 ReInitializes */
    SPI1_INIT();

    while(TxCnt1 < TxSize1) /* USART1--->SPI1 */
    {
        USART_SendData(USART1, TxBuffer1[TxCnt1++]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) /* waiting for sending finish */
        {
        }
        while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
        {
        }
        RxBuffer2[RxCnt2++] = SPI_I2S_ReceiveData(SPI1);
    }
    USART_ReceiveData(USART1); /* Clear the USART1 Data Register */
    while(TxCnt2 < TxSize2)    /* SPI1--->USART1 */
    {
        while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) /* waiting for sending finish */
        {
        }
        SPI_I2S_SendData(SPI1, TxBuffer2[TxCnt2++]);
        USART_SendData(USART1, Tempdata); /* Send Tempdata for SCK */
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        {
        }
        while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
        {
        }
        RxBuffer1[RxCnt1++] = USART_ReceiveData(USART1);
    }

    TransferStatus1 = Buffercmp(pbuf, RxBuffer2, TxSize1);
    TransferStatus2 = Buffercmp(pbuf, RxBuffer1, TxSize2);
    if(TransferStatus1 && TransferStatus2)
    {
        while(1){
            Delay_Ms(250);
            GPIO_WriteBit(GPIOD, GPIO_Pin_0, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
        }
    }
    else
    {
        GPIO_ResetBits(GPIOD, GPIO_Pin_0);
    }

    while(1)
    {
    }
}
