/********************************** (C) COPYRIGHT  *******************************
 * File Name          : debug.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/08
 * Description        : This file contains all the functions prototypes for UART
 *                      Printf , Delay functions.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#ifndef __DEBUG_H
#define __DEBUG_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <ch32v00x.h>
#include <stdio.h>

/* UART Printf Definition */
#define DEBUG_UART1_NoRemap   1  //Tx-PD5
#define DEBUG_UART1_Remap1    2  //Tx-PD0
#define DEBUG_UART1_Remap2    3  //Tx-PD6
#define DEBUG_UART1_Remap3    4  //Tx-PC0

/* DEBUG UATR Definition */
#ifndef DEBUG
#define DEBUG   DEBUG_UART1_NoRemap
#endif

/* SDI Printf Definition */
#define SDI_PR_CLOSE   0
#define SDI_PR_OPEN    1

#ifndef SDI_PRINT
#define SDI_PRINT   SDI_PR_CLOSE
#endif

void Delay_Init(void);
void Delay_Us(uint32_t n);
void Delay_Ms(uint32_t n);
void USART_Printf_Init(uint32_t baudrate);
void SDI_Printf_Enable(void);

#ifdef __cplusplus
}
#endif

#endif /* __DEBUG_H */
