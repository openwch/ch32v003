/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2025/01/14
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/*
 * @Note
 * The purpose of this description and routine is to provide customers with the application framework of voilent fan cartridge
 * permanent magnet synchronous motor based on WCH MCU,including MCU peripherals, SQUARE control, observer, speed control, protection,
 * and other basic solutions and modules, to assist customers in shortening the product development cycle.
*/
/* Includes -----------------------------------------------------------------*/
#include "M0_Control_Library.h"

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    Delay_Init();

    Delay_Ms(1000);

    KEY_AND_LEDIO_Init();

    Global_Status_Initial();

    Motor_Prameter_Define(&MBase_M);

    Motor_BaseCal_M(&MBase_M);

    Speedreg_Prameter_Define (&Speedsctl_M);

    Speedrampctr_Parameter_Define(&SpeedRamp_M);

    PWMADC_Timer_Configure();

    ADC_OPA_Configure(&ADC_M);

    ADC_Parameter_Cal(&ADC_M,&MBase_M);

    PWM_Parameter_Define_M(&PWMcopper_M);

    Systick_Init_48M();

    CHANGEPHASE_Timer2_Init();

    EMFObs_Parameter_Define_M(&EMF_Cal_M,&EMF_Status_M);

    Protection_and_Emergency_Init();

    Operation_Parameter_Define(&Operation_M);

    Interrupt_Configuration();

    TIM_Cmd(TIM1, ENABLE);

    ADC_Offset_Reading(&ADC_M);

    while(1)
    {
        Motor_Operation_Process();
    }
}
