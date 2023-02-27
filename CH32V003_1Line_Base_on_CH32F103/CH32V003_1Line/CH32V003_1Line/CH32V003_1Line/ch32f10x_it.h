/********************************** (C) COPYRIGHT *******************************
 * File Name          : ch32f10x_it.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2019/10/15
 * Description        : This file contains the headers of the interrupt handlers.
 *********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#ifndef __CH32F10x_IT_H
#define __CH32F10x_IT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ch32f10x.h"

void NMI_Handler( void );
void HardFault_Handler( void );
void MemManage_Handler( void );
void BusFault_Handler( void );
void UsageFault_Handler( void );
void SVC_Handler( void );
void DebugMon_Handler( void );
void PendSV_Handler( void );
void SysTick_Handler( void );

#ifdef __cplusplus
}
#endif

#endif /* __CH32F10x_IT_H */





