/********************************** (C) COPYRIGHT *******************************
 * File Name          : M1_Motor_Parameter_Init.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/15
 * Description        : Motor parameters and state initialization.
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
Motor_Base_Type  MBase_M;
Runningstatus_Type  RunningStatus_M;
volatile uint16_t System_Status_Global;

/*********************************************************************
 * @fn      Motor_Prameter_Define
 *
 * @brief   Motor parameter definition.
 *
 * @para    Basestruc: Motor base value parameter structure pointer
 *
 * @return  none
 */
void Motor_Prameter_Define(Motor_Base_Type *Basestruc)
{
    Basestruc->In = RATED_CURR_M;
}
/*********************************************************************
 * @fn      Global_Status_Initial
 *
 * @brief   Initialization of system state and motor operating state
 *
 * @return  none
 */
void Global_Status_Initial(void)
{
    System_Status_Global = 0;
    System_Status_Global |= SPEED_CONTROL_M;
    System_Status_Global |= FIRST_START_M;
    RunningStatus_M = IDLE;
}
