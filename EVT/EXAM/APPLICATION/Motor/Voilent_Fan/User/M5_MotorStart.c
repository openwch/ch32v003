/********************************** (C) COPYRIGHT *******************************
 * File Name          : M5_MotorStart.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/15
 * Description        : Initialization before motor start.
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
 * @brief   Initialization before motor start
 *
 * @para    none
 *
 * @return  none
 */
void Motor_Start_Init_M(void)
{
    Global_Status_Initial();

    SpeedrampInit(&SpeedRamp_M);

    Operation_Status_Init(&Operation_M);

    ADC_Status_Init (&ADC_M);

    ADC_Offset_Reading(&ADC_M);

    PWM_Status_Initial_M(&PWMcopper_M);

    EMFObs_Status_Init_M(&EMF_Cal_M, &EMF_Status_M);

    PWM_LON_HOFF(&PWMcopper_M);

    PWM_Update(&PWMcopper_M);

    PWM_Enable_M (ENABLE);

    Time_Wait(8);

    PWM_LOFF_HOFF(&PWMcopper_M);

    PWM_Update(&PWMcopper_M);

    ADC_Start(ENABLE);

    SYS_TIM_Cmd(ENABLE);

    TIM_Cmd(TIM2, DISABLE);
}
