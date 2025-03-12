/********************************** (C) COPYRIGHT  *******************************
 * File Name          : debug.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/01/01
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

#include <ch32v00X.h>
#include <stdio.h>

/* UART Printf Definition */
#define DEBUG_UART1_NoRemap   1  //Tx-PD5
#define DEBUG_UART1_Remap1    2  //Tx-PD6
#define DEBUG_UART1_Remap2    3  //Tx-PD0
#define DEBUG_UART1_Remap3    4  //Tx-PC0
#define DEBUG_UART1_Remap4    5  //Tx-PD1
#define DEBUG_UART1_Remap5    6  //Tx-PB3
#define DEBUG_UART1_Remap6    7  //Tx-PC5
#define DEBUG_UART1_Remap7    8  //Tx-PB5
#define DEBUG_UART1_Remap8    9  //Tx-PA0

/* USART2 print function only for V005,V006,V007,M007 series*/
#if defined(CH32V005) || defined(CH32V006) || defined(CH32V007_M007)
#define DEBUG_UART2_NoRemap   10  //Tx-PA7
#define DEBUG_UART2_Remap1    11  //Tx-PA4
#define DEBUG_UART2_Remap2    12  //Tx-PA2
#define DEBUG_UART2_Remap3    13  //Tx-PD2
#define DEBUG_UART2_Remap4    14  //Tx-PB0
#define DEBUG_UART2_Remap5    15  //Tx-PC4
#define DEBUG_UART2_Remap6    16  //Tx-PA6
#endif
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
