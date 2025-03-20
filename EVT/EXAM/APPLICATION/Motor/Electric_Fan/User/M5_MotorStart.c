/********************************** (C) COPYRIGHT *******************************
 * File Name          : M5_MotorStart.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
 * Description        : Initialization of the motor before starting
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
/* Private variables --------------------------------------------------------*/
/* Variables ----------------------------------------------------------------*/
/*********************************************************************
 * @fn      Motor_Start_Init_M
 *
 * @brief   Initialization of the motor before starting
 *
 * @return  none
 */
void Motor_Start_Init_M(void)
{
    Speedrampctr_Initial(&SpeedRamp_M);

    KeyandLED_Status_Init(&Key_M);

    Operation_Status_Init(&Operation_M);

    Protection_and_Emergency_Init();

    PID_Output_Init(&Mpid_M,SpeedRamp_M.Direction);

    Motor_Status_Initial_M(&MStruc_M);

    Dcurr_Process_Init(&DCurrProcess_M,1);

    SVPWM_2R_Status_Init_M(&SVPWM_Cal_M);

    Observer_Status_Init(&Observer_M,&Observer_Status_M,SpeedRamp_M.Direction);

    Buffer_Clear(&(Observer_SpeedBuffer_M[0]),&(Observer_M.SpeedBuffPara));

    ADC_Offset_Reading(ADC1,&ADC_M,1);

    Flystart_Status_Init(&Flystart_M,SpeedRamp_M.Direction);

    SVPWM_Lowside_Effective(&SVPWM_Cal_M);

    TIM_CtrlPWMOutputs(TIM1,ENABLE);
}
