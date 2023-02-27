/********************************** (C) COPYRIGHT *******************************
 * File Name          : system_ch32f10x.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2019/10/15
 * Description        : CMSIS Cortex-M3 Device Peripheral Access Layer System Header File. 
 *********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#ifndef __SYSTEM_CH32F10x_H
#define __SYSTEM_CH32F10x_H

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned int SystemCoreClock;          /* System Clock Frequency (Core Clock) */

/* CH32F10x_System_Exported_Functions */
extern void SystemInit( void );
extern void SystemCoreClockUpdate( void );

#ifdef __cplusplus
}
#endif

#endif /*__CH32F10x_SYSTEM_H */



