/********************************** (C) COPYRIGHT *******************************
 * File Name          : M2_PID_Parameter_Initial.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/15
 * Description        : PID control structure definition and state initialization.
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
///* Variables ----------------------------------------------------------------*/
Speed_SContrl_Type Speedsctl_M;

/*********************************************************************
 * @fn      Speedreg_Prameter_Define
 *
 * @brief   Speed regulator parameter definition
 *
 * @para    Speedsctl: Speed regulator structure pointer
 *
 * @return  none
 */
void Speedreg_Prameter_Define (Speed_SContrl_Type *Speedsctl)
{
    Speedsctl->Incstep1 = SPEED_SCONTROL_ISTEP1_M;
    Speedsctl->Incstep2 = SPEED_SCONTROL_ISTEP2_M;
    Speedsctl->Decstep1 = SPEED_SCONTROL_DSTEP1_M;
    Speedsctl->Decstep2 = SPEED_SCONTROL_DSTEP2_M;
    Speedsctl->OCDecstep = SPEED_SCONTROL_OCDSTEP_M;
    Speedsctl->OSDecstep = SPEED_SCONTROL_OSDSTEP_M;
}
