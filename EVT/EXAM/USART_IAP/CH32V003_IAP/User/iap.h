/********************************** (C) COPYRIGHT  *******************************
 * File Name          : iap.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2020/12/16
 * Description        : IAP
 *******************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#ifndef __IAP_H
#define __IAP_H

#include "ch32v00x.h"
#include "stdio.h"

#define USBD_DATA_SIZE               64

#define BUILD_UINT16(loByte, hiByte) ((UINT16)(((loByte) & 0x00FF) | (((hiByte) & 0x00FF) << 8)))
#define BUILD_UINT32(Byte0, Byte1, Byte2, Byte3) \
          ((UINT32)((UINT32)((Byte0) & 0x00FF) \
          + ((((UINT32)Byte1) & 0x00FF) << 8) \
          + ((((UINT32)Byte2) & 0x00FF) << 16) \
          + ((((UINT32)Byte3) & 0x00FF) << 24)))

#define Uart_Sync_Head1   0x57
#define Uart_Sync_Head2   0xab

#define CMD_IAP_PROM      0x80
#define CMD_IAP_ERASE     0x81
#define CMD_IAP_VERIFY    0x82
#define CMD_IAP_END       0x83

#define ERR_SCUESS        0x00
#define ERR_ERROR         0x01
#define ERR_End           0x02

typedef struct __attribute__ ((aligned(4)))_ISP_CMD {
    u8 Cmd;
    u8 Len;
    u8 Rev[2];
    u8 data[60];
} isp_cmd;

typedef void (*iapfun)(void);

extern u8 EP2_Rx_Buffer[USBD_DATA_SIZE];

u8 RecData_Deal(void);
void GPIO_Cfg_init(void);
u8 PC0_Check(void);
void USART1_CFG(u32 baudrate);
void UART_Rx_Deal(void);

#endif

