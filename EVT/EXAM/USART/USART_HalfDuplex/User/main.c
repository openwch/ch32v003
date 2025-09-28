/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *Single wire half duplex mode, master/slave mode transceiver routine:
 *Master:USART1_Tx(PD5)
 *
 *This routine demonstrates the data transmission and reception of two boards in
 *single-wire half-duplex mode. After successful transmission and reception, PD0 is
 *connected to the LED, and the LED light flashes.
 *
 *    Hardware connection:PD5 -- PD5
 *                        PD0 -- LED
 * Note: The pin should be GPIO_Mode_AF_OD in single-wire half-duplex mode.
 *      The pin needs to connected a pull_up resistor 
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
#define size(a)    (sizeof(a) / sizeof(*(a)))

/* Global define */
#define TxSize1    (size(TxBuffer1))
#define size(a)    (sizeof(a) / sizeof(*(a)))

/* Global Variable */
u8 TxBuffer1[] = "*Buffer1 Send from USART1 using DMA!";
u8 RxBuffer1[TxSize1] = {0};
u8 TxCnt1 = 0, RxCnt1 = 0;
u8 Rxfinish1 = 0;
TestStatus TransferStatus1 = FAILED;

/* USART Mode Definition */
#define HOST_MODE     0
#define SLAVE_MODE    1

/* USART Communication Mode Selection */
//#define USART_MODE      HOST_MODE
#define USART_MODE     SLAVE_MODE

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
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
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

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);

    USART_HalfDuplexCmd(USART1, ENABLE);
}
/*********************************************************************
 * @fn      DMA_INIT
 *
 * @brief   Configures the DMA for USART1.
 *
 * @return  none
 */
int main(void)
{
    u8 i=0;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    GPIO_Toggle_INIT();

    USARTx_CFG();
    Delay_Ms(1000);

#if(USART_MODE == HOST_MODE)
    while(TxCnt1 < TxSize1)
    {
        Delay_Ms(50);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        {
        }
        USART_SendData(USART1, TxBuffer1[TxCnt1++]);
    }

    while(RxCnt1 < TxSize1)
    {
        while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
        {
        }
        RxBuffer1[RxCnt1++] = USART_ReceiveData(USART1);
    }

#elif(USART_MODE == SLAVE_MODE)
    while(RxCnt1 < TxSize1)
    {
        while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
        {
        }
        RxBuffer1[RxCnt1++] = USART_ReceiveData(USART1);
    }

    while(TxCnt1 < TxSize1)
    {
        Delay_Ms(50);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        {
        }
        USART_SendData(USART1, TxBuffer1[TxCnt1++]);
    }

#endif

    TransferStatus1 = Buffercmp(TxBuffer1, RxBuffer1, TxSize1);

    if(TransferStatus1)
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
