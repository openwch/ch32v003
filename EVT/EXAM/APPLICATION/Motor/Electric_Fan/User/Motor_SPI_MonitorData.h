/********************************** (C) COPYRIGHT *******************************
* File Name          :Motor_SPI_MonitorData.h
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
#ifndef __MOTOR_SPI_USART_MONITORDATA_H_
#define __MOTOR_SPI_USART_MONITORDATA_H_
/* Includes -----------------------------------------------------------------*/
#include "debug.h"
/* Exported constants -------------------------------------------------------*/
#define MaxUsbDataLen       8
#define USB_SENDBUF_LEN       (MaxUsbDataLen<<5)
#define USB_NEEDSEND_LEN1       (MaxUsbDataLen*14)
#define USB_NEEDSEND_LEN2       (MaxUsbDataLen*7)
#define USB_NEEDSEND_LEN3       (MaxUsbDataLen*4)
#define USB_NEEDSEND_LEN4       (MaxUsbDataLen*3)
#define USB_NEEDSEND_LEN5       (MaxUsbDataLen*2)
#define USB_NEEDSEND_LEN6       (MaxUsbDataLen*2)
#define USB_NEEDSEND_LEN7       (MaxUsbDataLen*2)
#define USB_NEEDSEND_LEN8       (MaxUsbDataLen)
#define USB_NEEDSEND_LEN9       (MaxUsbDataLen)
#define USB_NEEDSEND_LEN10       (MaxUsbDataLen)
#define USB_NEEDSEND_LEN11       (MaxUsbDataLen)

/* Exported types -----------------------------------------------------------*/
typedef struct
{
    u_int8_t heartBeat;
    u_int8_t delayNum;
    u_int8_t volatile sendDataLen;
    //UINT8 frameByteNum;
    u_int8_t curWriteFrame;
    u_int8_t recDataLen;
    u_int8_t end;
    u_int8_t volatile head;
    u_int8_t recDataBuf[16];
    u_int8_t sendDataType[MaxUsbDataLen];
    u_int16_t sendDataBuf[USB_SENDBUF_LEN];
}t_sendUsbData;

/* Exported variables -------------------------------------------------------*/
extern t_sendUsbData sendUsbData;

/* Exported functions--------------------------------------------------------*/
extern void Deal_sendMonitorData(void);
extern void SPI_FullDuplex_Init(void);
extern void SPI_Deal_sendMonitorData(void);
extern void Usart_Init(uint32_t Baud);
extern void Usart_Deal_sendMonitorData(void);
#endif
