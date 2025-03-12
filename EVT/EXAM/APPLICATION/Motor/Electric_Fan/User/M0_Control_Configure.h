/********************************** (C) COPYRIGHT *******************************
 * File Name          : M0_Control_Configure.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
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

#define KEY_START

// Voltage limit table configuration, 8 configurations: 100,098,096,094,092,090,088,086
//#define MAG100_TABLE_USED
//#define MAG098_TABLE_USED
//#define MAG096_TABLE_USED
#define MAG094_TABLE_USED
//#define MAG092_TABLE_USED
//#define MAG090_TABLE_USED
//#define MAG088_TABLE_USED
//#define MAG086_TABLE_USED


#if ( (!defined(MAG100_TABLE_USED))&&(!defined(MAG098_TABLE_USED))&&\
        (!defined(MAG096_TABLE_USED))&&(!defined(MAG094_TABLE_USED))\
        &&(!defined(MAG092_TABLE_USED))&&(!defined(MAG090_TABLE_USED))\
        &&(!defined(MAG088_TABLE_USED))&&(!defined(MAG086_TABLE_USED)))
#error "Invalid configuration: No voltage limit selected"
#endif


#if ( (defined(SIX_SECTOR_POSITION))&&(defined(TWELVE_SECTOR_POSITION)))
#error "Invalid configuration: Duplicate HFVQI configure"
#endif


/* Exported variables -------------------------------------------------------*/
/* Exported macro -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------ */

#endif
