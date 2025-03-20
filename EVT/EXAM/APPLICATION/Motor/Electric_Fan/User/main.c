/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2022/09/01
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/*
 * @Note
 * The purpose of this description and routine is to provide customers with the application framework of electric fan cartridge
 * permanent magnet synchronous motor based on WCH MCU,including MCU peripherals, FOC control, observer, loop control, protection,
 * and other basic solutions and modules, to assist customers in shortening the product development cycle.
*/
/* Includes -----------------------------------------------------------------*/
#include "M0_Control_Library.h"
#include "IQmath_RV32.h"
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

    Delay_Ms(100);

    KeyandLED_IOInit();

    KeyandLED_Parameter_Define(&Key_M);

    Global_Status_Initial();

    Operation_Parameter_Define(&Operation_M);

    Motor_Prameter_Define(&MBase_M);

    Motor_FilerPara_Cal_M(&MStruc_M);

    Motor_BaseCal_M(&MBase_M);

    PID_Prameter_Define(&Mpid_M);

    DCurrProcess_Parameter_Define(&DCurrProcess_M);

    PID_ParaBase_Cal_M(&MBase_M,&Mpid_M);

    Protection_and_Emergency_Init();

    Speedrampctr_Parameter_Define(&SpeedRamp_M);

    ADC_Parameter_Init(&ADC_M,&MBase_M);

    ADC_OPA_Configure(&ADC_M);

    SVPWM_2R_Paramter_Init(&SVPWM_Cal_M ,&MBase_M);

    SVPWM_2R_Timer_Configure();

    Observer_Parameter_Define(&Observer_M,&Observer_Status_M);

    Observer_Paramter_Cal(&MBase_M,&Observer_M);

    Flystart_Prameter_Define(&Flystart_M);

    Flystart_Paramter_Cal(&Flystart_M,&MBase_M);

    Poweroperation_Initial(&Powerctr_M);

    Systick_Init();

    Interrupt_Configuration();

    TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    SYS_TIM_Cmd(ENABLE);

    while(1)
    {
        Motor_Operation_Process();    //µç»ú²Ù×÷
     }
}
