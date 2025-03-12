/********************************** (C) COPYRIGHT *******************************
 * File Name          : M0_Control_Configure.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/14
 * Description        : Function selection and configuration.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __M0_CONTROL_CONFIGURE_H
#define __M0_CONTROL_CONFIGURE_H
/* Includes -----------------------------------------------------------------*/
/* Exported types -----------------------------------------------------------*/
/* Exported constants -------------------------------------------------------*/

#define SLOOP_CONTROL

#if ( (!defined(DLOOP_CONTROL))&&(!defined(SLOOP_CONTROL)))
#error "Invalid configuration: No Controlmode configure"
#endif

#if ( (defined(DLOOP_CONTROL))&&(defined(SLOOP_CONTROL)))
#error "Invalid configuration: Duplicate Controlmode configure"
#endif

/* Exported variables -------------------------------------------------------*/
/* Exported macro -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------ */

#endif
