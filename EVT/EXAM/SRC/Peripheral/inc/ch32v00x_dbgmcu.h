/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00X_dbgmcu.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/01/01
 * Description        : This file contains all the functions prototypes for the
 *                      DBGMCU firmware library.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#ifndef __CH32V00X_DBGMCU_H
#define __CH32V00X_DBGMCU_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ch32v00X.h>

/* CFGR0 Register */
#define DBGMCU_SLEEP                 ((uint32_t)0x00000001)
#define DBGMCU_STANDBY               ((uint32_t)0x00000004)
#define DBGMCU_IWDG_STOP             ((uint32_t)0x00000100)
#define DBGMCU_WWDG_STOP             ((uint32_t)0x00000200)
#define DBGMCU_TIM1_STOP             ((uint32_t)0x00001000)
#define DBGMCU_TIM2_STOP             ((uint32_t)0x00002000)
#define DBGMCU_TIM3_STOP             ((uint32_t)0x00004000)

uint32_t DBGMCU_GetREVID(void);
uint32_t DBGMCU_GetDEVID(void);
uint32_t __get_DEBUG_CR(void);
void __set_DEBUG_CR(uint32_t value);
void DBGMCU_Config(uint32_t DBGMCU_Periph, FunctionalState NewState);
uint32_t DBGMCU_GetCHIPID( void );
#ifdef __cplusplus
}
#endif

#endif /* __CH32V00X_DBGMCU_H */
