/********************************** (C) COPYRIGHT *******************************
 * File Name          : ch32f10x_it.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2019/10/15
 * Description        : Main Interrupt Service Routines.
 *********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include "ch32f10x_it.h"

/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler( void )
{
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler( void )
{
    while( 1 )
    {
    }
}

/*********************************************************************
 * @fn      MemManage_Handler
 *
 * @brief   This function handles Memory Manage exception.
 *
 * @return  none
 */
void MemManage_Handler( void )
{
    while( 1 )
    {
    }
}

/*********************************************************************
 * @fn      BusFault_Handler
 *
 * @brief   This function handles Bus Fault exception.
 *
 * @return  none
 */
void BusFault_Handler( void )
{
    while( 1 )
    {
    }
}

/*********************************************************************
 * @fn      UsageFault_Handler
 *
 * @brief   This function handles Usage Fault exception.
 *
 * @return  none
 */
void UsageFault_Handler( void )
{
    while( 1 )
    {
    }
}

/*********************************************************************
 * @fn      SVC_Handler
 *
 * @brief   This function handles SVCall exception.
 *
 * @return  none
 */
void SVC_Handler( void )
{
}

/*********************************************************************
 * @fn      DebugMon_Handler
 *
 * @brief   This function handles Debug Monitor exception.
 *
 * @return  none
 */
void DebugMon_Handler( void )
{
}

/*********************************************************************
 * @fn      PendSV_Handler
 *
 * @brief   This function handles PendSVC exception.
 *
 * @return  none
 */
void PendSV_Handler( void )
{
}

/*********************************************************************
 * @fn      SysTick_Handler
 *
 * @brief   This function handles SysTick Handler.
 *
 * @return  none
 */
void SysTick_Handler( void )
{
}





