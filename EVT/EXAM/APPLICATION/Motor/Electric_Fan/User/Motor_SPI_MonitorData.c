/********************************** (C) COPYRIGHT *******************************
* File Name          :Motor_SPI_MonitorData.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2022/09/01
* Description        : SPI MonitorData
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/*
 * @Note
 * The purpose of this description and routine is to provide customers with the application framework of electric fan cartridge
 * permanent magnet synchronous motor based on WCH MCU,including MCU peripherals, FOC control, observer, loop control, protection,
 * and other basic solutions and modules, to assist customers in shortening the product development cycle.
*/
/* Includes -----------------------------------------------------------------*/
#include "Motor_SPI_MonitorData.h"
/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private functions --------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Variables ----------------------------------------------------------------*/
t_sendUsbData sendUsbData = {0,0,0,0,0,0,0};
const u_int8_t usbFrameByteNum[12] = {0, 32, 32, 28, 28, 24, 28, 32, 20, 22, 24, 26};
const u_int8_t needSendLen[12] = {0, USB_NEEDSEND_LEN1, USB_NEEDSEND_LEN2, USB_NEEDSEND_LEN3, USB_NEEDSEND_LEN4, USB_NEEDSEND_LEN5,
        USB_NEEDSEND_LEN6, USB_NEEDSEND_LEN7, USB_NEEDSEND_LEN8, USB_NEEDSEND_LEN9, USB_NEEDSEND_LEN10, USB_NEEDSEND_LEN11};
/*********************************************************************
 * @fn      SPI_Deal_sendMonitorData
 *
 * @brief   sendMonitorData
 *
 * @return  none
 */
__attribute__((section(".highcode")))
void SPI_Deal_sendMonitorData(void)
{
    u_int16_t data=0;
    u_int8_t i=0, frameByteNum=0;
    uint8_t reDataLen;
    if(sendUsbData.head >= sendUsbData.end)
    {
        reDataLen = sendUsbData.head - sendUsbData.end;
    }
    else
    {
        reDataLen = (uint16_t)USB_SENDBUF_LEN - sendUsbData.end + sendUsbData.head;
    }
    sendUsbData.sendDataLen=4;
    if(sendUsbData.sendDataLen)
    {
        if(reDataLen >= needSendLen[sendUsbData.sendDataLen])//头减尾>=14 且缓冲区为空
        {
            GPIOD->BCR = GPIO_Pin_5;                                      //拉低片选 告诉从机准备接收
            __asm volatile ("nop");                                       //等待数据通过光耦
            while((SPI1->STATR & SPI_I2S_FLAG_TXE) == (uint16_t)RESET);   //等待发送缓冲区空

            SPI1->DATAR = 0x55AA;                                         //包头AA55
            while((SPI1->STATR & SPI_I2S_FLAG_TXE) == (uint16_t)RESET);   //等待发送缓冲区空

            sendUsbData.heartBeat++;
            data=sendUsbData.heartBeat<<8;
            data|=usbFrameByteNum[sendUsbData.sendDataLen];
            SPI1->DATAR = data;                                            //心跳包+发送长度
            while((SPI1->STATR & SPI_I2S_FLAG_TXE) == (uint16_t)RESET);    //等待发送缓冲区空

            frameByteNum = 4;
            while(frameByteNum < usbFrameByteNum[sendUsbData.sendDataLen])//{0, 32, 32, 28, 28, 24, 28, 32, 20, 22, 24, 26};
            {
                for(i = 0; i < sendUsbData.sendDataLen; i++)
                {
                    SPI1->DATAR=sendUsbData.sendDataBuf[sendUsbData.end+i];
                    while((SPI1->STATR & SPI_I2S_FLAG_TXE) == (uint16_t)RESET);    //等待发送缓冲区空
                    frameByteNum+=2;
                }
                sendUsbData.end += MaxUsbDataLen;
            }

            while( (SPI1->STATR & SPI_I2S_FLAG_BSY) != (uint16_t)RESET);    //判断是否发送完毕 发送完毕
            __asm volatile ("nop");                                         //等待数据通过光耦
            GPIOD->BSHR = GPIO_Pin_5;                                       //拉高片选
        }
    }
}

/*********************************************************************
 * @fn      SPI_FullDuplex_Init
 *
 * @brief   SPI configuration
 *
 * @return  none
 */
void SPI_FullDuplex_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    SPI_InitTypeDef SPI_InitStructure={0};

    RCC_PB2PeriphClockCmd( RCC_PB2Periph_GPIOD | RCC_PB2Periph_SPI1|RCC_PB2Periph_AFIO, ENABLE );

    GPIO_PinRemapConfig(GPIO_PartialRemap4_SPI1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init( GPIOD, &GPIO_InitStructure );//SCK

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//NSS作为GPIO口使用，设置为推挽输出
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_SetBits(GPIOD,GPIO_Pin_5);    //拉高片选

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//2线全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;     //主机
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;//16位
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//48/4=12M
//    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//48/8=6M
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
    SPI_Init( SPI1, &SPI_InitStructure );

    GPIO_ResetBits(GPIOD,GPIO_Pin_5); //拉低片选
    SPI_Cmd( SPI1, ENABLE );           //启动SPI
}
