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
 使用PEC错误校验，Master/Slave 模式收发例程：
 I2C1_SCL(PC2)、I2C1_SDA(PC1)。
  本例程演示 带 PEC 错误校验 Master 发，Slave 收，若传输出错触发错误中断。
  注：两块板子分别下载 Master 和 Slave 程序，同时上电。
      硬件连线：PC2 —— PC2
            PC1 —— PC1

*/

#include "debug.h"

/* I2C Mode Definition */
#define HOST_MODE     0
#define SLAVE_MODE    1

/* I2C Communication Mode Selection */
#define I2C_MODE   HOST_MODE
//#define I2C_MODE      SLAVE_MODE

/* Global define */
#define Size          7
#define RXAdderss     0x02
#define TxAdderss     0x02

/* Global Variable */
u8 TxData[Size] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
u8 RxData[Size];

/*********************************************************************
 * @fn      IIC_Init
 *
 * @brief   Initializes the IIC peripheral.
 *
 * @return  none
 */
void IIC_Init(u32 bound, u16 address)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    I2C_InitTypeDef  I2C_InitTSturcture = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE );
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    I2C_InitTSturcture.I2C_ClockSpeed = bound;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitTSturcture.I2C_OwnAddress1 = address;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitTSturcture);

#if(I2C_MODE == SLAVE_MODE)
    NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    I2C_ITConfig(I2C1, I2C_IT_ERR, ENABLE);

#endif

    I2C_Cmd(I2C1, ENABLE);
    I2C_CalculatePEC(I2C1, ENABLE);

#if(I2C_MODE == HOST_MODE)
    I2C_AcknowledgeConfig(I2C1, ENABLE);

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
    u8 i = 0;
    u8 pecValue;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    Delay_Init();
    USART_Printf_Init(460800);
    printf("SystemClk:%d\r\n", SystemCoreClock);

#if(I2C_MODE == HOST_MODE)
    printf("IIC Host mode\r\n");
    IIC_Init(80000, TxAdderss);

    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET);
    I2C_GenerateSTART(I2C1, ENABLE);

    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, 0x02, I2C_Direction_Transmitter);

    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    while(i < 7)
    {
        if(i < 6)
        {
            if(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE) != RESET)
            {
                I2C_SendData(I2C1, TxData[i]);
                i++;
            }
        }

        if(i == 6)
        {
            if(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE) != RESET)
            {
                I2C_TransmitPEC(I2C1, ENABLE);
                i++;
            }
        }
    }

    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(I2C1, ENABLE);

#elif(I2C_MODE == SLAVE_MODE)
    printf("IIC Slave mode\r\n");
    IIC_Init(80000, RXAdderss);

    while(!I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED));

    while(i < 7)
    {
        if(i < 5)
        {
            if(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) != RESET)
            {
                RxData[i] = I2C_ReceiveData(I2C1);
                i++;
            }
        }

        if(i == 5)
        {
            pecValue = I2C_GetPEC(I2C1);
            if(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) != RESET)
            {
                RxData[i] = I2C_ReceiveData(I2C1);
                i++;
            }
        }

        if(i == 6)
        {
            I2C_TransmitPEC(I2C1, ENABLE);
            if(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) != RESET)
            {
                RxData[i] = I2C_ReceiveData(I2C1);
                i++;
            }
        }
    }

    printf("pecValue:%02x\r\n", pecValue);
    printf("RxData:\r\n");

    for(i = 0; i < 7; i++)
    {
        printf("%02x\r\n", RxData[i]);
    }

#endif

    while(1);
}

void I2C1_ER_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      I2C1_ER_IRQHandler
 *
 * @brief   This function IIC PEC error exception.
 *
 * @return  none
 */
void I2C1_ER_IRQHandler(void )
{
    if( I2C_GetITStatus( I2C1, I2C_IT_PECERR ) != RESET )
    {
        printf( "PECEER\r\n" );
        I2C_ClearITPendingBit( I2C1, I2C_IT_PECERR );
    }
}

