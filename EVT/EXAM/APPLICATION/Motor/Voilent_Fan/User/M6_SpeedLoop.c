/********************************** (C) COPYRIGHT *******************************
 * File Name          : M6_Speedloop.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/15
 * Description        : Speedloop operation process and change phase TIMER initialization.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Includes -----------------------------------------------------------------*/
#include "M0_Control_Library.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private functions --------------------------------------------------------*/
void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/* Private variables --------------------------------------------------------*/


/* Variables ----------------------------------------------------------------*/

/*********************************************************************
 * @fn      SysTick_Handler
 *
 * @brief   SysTick_Handler Process
 *
 * @para    none
 *
 * @return  none
 */
void SysTick_Handler(void)
{
    if (EMF_Cal_M.Timer_Overflow_Cnt<U16_MAX)
        EMF_Cal_M.Timer_Overflow_Cnt++;

    if(RunningStatus_M==START||RunningStatus_M==PRESTART)
    {
        if(EMF_Cal_M.Timer_Overflow_Cnt >= EMF_Cal_M.Timer_Overflow_thres)
            EMF_Status_M.Timeout =TRUE;
    }

   SysTick->SR &= ~(1 << 0);
}

/*********************************************************************
 * @fn      Systick_Init_48M
 *
 * @brief   Initialization of Systick
 *
 * @para    none
 *
 * @return  none
 */
void Systick_Init_48M(void)
{
    SysTick->CTLR  =0;
    SysTick->CMP   =0;
    SysTick->SR   &= ~(1 << 0);
    SysTick->CNT   = 0;
    SysTick->CMP   = (EMF_TIMER_PERIOD<<2);
    SysTick->CTLR |=(1 << 1)|(1 << 3);
}

/*********************************************************************
 * @fn      SYS_TIM_Cmd
 *
 * @brief   SYS_TIM_Cmd
 *
 * @para    NewState - enable or disable
 *
 * @return  none
 */
void SYS_TIM_Cmd(FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        SysTick->CTLR |= TIM_CEN;
    }
    else
    {
        SysTick->CTLR &= (uint16_t)(~((uint16_t)TIM_CEN));
    }
}

