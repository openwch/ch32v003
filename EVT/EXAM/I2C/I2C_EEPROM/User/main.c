/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.1
 * Date               : 2025/01/08
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *I2C interface routine to operate EEPROM peripheral:
 *I2C1_SCL(PC2)\I2C1_SDA(PC1).
 *This example uses EEPROM for AT24Cxx series.
 *Steps:
 *READ EEPROM:Start + 0xA0 + 8bit Data Address + Start + 0xA1 + Read Data + Stop.
 *WRITE EERPOM:Start + 0xA0 + 8bit Data Address + Write Data + Stop.
 *
 */

#include "debug.h"

/**********************************************************************
*@Note:
AT24Cxx:

READ EEPROM:Start + 0xA0 + 8bit Data Address + Start + 0xA1 + Read Data + Stop.
WRITE EERPOM:Start + 0xA0 + 8bit Data Address + Write Data + Stop.
*******************************************************************************/
/* EERPOM DATA ADDRESS Length Definition */
#define Address_8bit  0
#define Address_16bit  1

/* EERPOM DATA ADDRESS Length Selection */
#define Address_Lenth   Address_8bit
//#define Address_Lenth   Address_16bit

/* Global define */
#define SIZE sizeof(TEXT_Buffer)

/* Global Variable */
const u8 TEXT_Buffer[]={"CH32V00X I2C TEST"};

/*********************************************************************
 * @fn      IIC_Init
 *
 * @brief   Initializes the IIC peripheral.
 *
 * @return  none
 */
void IIC_Init(u32 bound, u16 address)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    I2C_InitTypeDef I2C_InitTSturcture={0};

    RCC_PB2PeriphClockCmd( RCC_PB2Periph_GPIOC | RCC_PB2Periph_AFIO, ENABLE );
    RCC_PB1PeriphClockCmd( RCC_PB1Periph_I2C1, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    I2C_InitTSturcture.I2C_ClockSpeed = bound;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitTSturcture.I2C_OwnAddress1 = address;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init( I2C1, &I2C_InitTSturcture );

    I2C_Cmd( I2C1, ENABLE );
}

/*********************************************************************
 * @fn      AT24CXX_Init
 *
 * @brief   Initializes AT24xx EEPROM.
 *
 * @return  none
 */
void AT24CXX_Init(void)
{
    IIC_Init( 100000, 0xA0);
}

/*********************************************************************
 * @fn      AT24CXX_ReadOneByte
 *
 * @brief   Read one data from EEPROM.
 *
 * @param   ReadAddr - Read frist address.
 *
 * @return  temp - Read data.
 */
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{
    u8 temp=0;

    while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );
    I2C_GenerateSTART( I2C1, ENABLE );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( I2C1, 0XA0, I2C_Direction_Transmitter );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

#if (Address_Lenth  == Address_8bit)
    I2C_SendData( I2C1, (u8)(ReadAddr&0x00FF) );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

#elif (Address_Lenth  == Address_16bit)
    I2C_SendData( I2C1, (u8)(ReadAddr>>8) );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    I2C_SendData( I2C1, (u8)(ReadAddr&0x00FF) );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

#endif

    I2C_GenerateSTART( I2C1, ENABLE );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( I2C1, 0XA0, I2C_Direction_Receiver );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) );
	while( I2C_GetFlagStatus( I2C1, I2C_FLAG_RXNE ) ==  RESET );
    I2C_AcknowledgeConfig( I2C1, DISABLE );

    temp = I2C_ReceiveData( I2C1 );
    I2C_GenerateSTOP( I2C1, ENABLE );

    return temp;
}

/*********************************************************************
 * @fn      AT24CXX_WriteOneByte
 *
 * @brief   Write one data to EEPROM.
 *
 * @param   WriteAddr - Write frist address.
 *
 * @return  DataToWrite - Write data.
 */
void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite)
{
    while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );
    I2C_GenerateSTART( I2C1, ENABLE );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( I2C1, 0XA0, I2C_Direction_Transmitter );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

#if (Address_Lenth  == Address_8bit)
    I2C_SendData( I2C1, (u8)(WriteAddr&0x00FF) );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

#elif (Address_Lenth  == Address_16bit)
    I2C_SendData( I2C1, (u8)(WriteAddr>>8) );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    I2C_SendData( I2C1, (u8)(WriteAddr&0x00FF) );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

#endif

    if( I2C_GetFlagStatus( I2C1, I2C_FLAG_TXE ) !=  RESET )
    {
        I2C_SendData( I2C1, DataToWrite );
    }

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
    I2C_GenerateSTOP( I2C1, ENABLE );
}

/*********************************************************************
 * @fn      AT24CXX_Read
 *
 * @brief   Read multiple data from EEPROM.
 *
 * @param   ReadAddr - Read frist address. (AT24c02: 0~255)
 *          pBuffer - Read data.
 *          NumToRead - Data number.
 *
 * @return  none
 */
void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead)
{
    while(NumToRead)
    {
        *pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);
        NumToRead--;
    }
}

/*********************************************************************
 * @fn      AT24CXX_Write
 *
 * @brief   Write multiple data to EEPROM.
 *
 * @param   WriteAddr - Write frist address. (AT24c02: 0~255)
 *          pBuffer - Write data.
 *          NumToWrite - Data number.
 *
 * @return  none
 */
void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite)
{
    while(NumToWrite--)
    {
        AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
        WriteAddr++;
        pBuffer++;
        Delay_Ms(2);
    }
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
    u8 data[SIZE];

    SystemCoreClockUpdate();
    Delay_Init();

#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init( 460800 );
#endif

    printf("SystemClk:%d\r\n",SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    AT24CXX_Init();

    printf("Start Write 24Cxx....\r\n");
    AT24CXX_Write(100,(u8*)TEXT_Buffer,SIZE);
    printf("24Cxx Write Sucess!\r\n");

    Delay_Ms(500);

    printf("Start Read 24Cxx....\r\n");
    AT24CXX_Read(100,data,SIZE);
    printf("The Data Readed Is: \r\n");

    printf("%s\r\n", data);

    while(1);
}
