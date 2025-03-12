/********************************** (C) COPYRIGHT  *******************************
* File Name          : falsh.h
* Author             : WCH
* Version            : V1.0.1
* Date               : 2025/01/13
* Description        :
*******************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#ifndef __FLASH_H
#define __FLASH_H

#include "ch32v00X_it.h"
#include "stdio.h"

void CH32_IAP_Program(u32 adr, u32* buf);

#endif
