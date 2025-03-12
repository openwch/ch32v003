/********************************** (C) COPYRIGHT *******************************
 * File Name          : M1_Motor_Parameter_Init.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
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
Motor_Sructure_Type MStruc_M;
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
    Basestruc->Fn = RATED_FREQ_M;
    Basestruc->Vn = RATED_VOLT_M;
    Basestruc->In = RATED_CURR_M;
    Basestruc->Flux = ROTOR_FLUX_M;
    Basestruc->Pn = POLEPARE_M;
    Basestruc->Rs = STATOR_RES_M;
    Basestruc->Ld = STATOR_LD_M;
    Basestruc->Lq = STATOR_LQ_M;
    Basestruc->Rstotal = STATOR_RES_TOTAL_M;
}
/*********************************************************************
 * @fn      Motor_Status_Initial_M
 *
 * @brief   Motor parameter definition.
 *
 * @para    MStruc: Motor overall state structure pointer
 *
 * @return  none
 */
void Motor_Status_Initial_M(Motor_Sructure_Type *MStruc)
{
    MStruc->VRef2R.cd = _IQ(0.0);
    MStruc->VRef2R.cq = _IQ(0.0);
    MStruc->VRef2R.peak = _IQ(0.0);
    MStruc->VRef2RC.cd = _IQ(0.0);
    MStruc->VRef2RC.cq = _IQ(0.0);
    MStruc->VRef2RC.peak = _IQ(0.0);
    MStruc->VRef2S.Alpha = _IQ(0.0);
    MStruc->VRef2S.Beta = _IQ(0.0);
    MStruc->VRef2SC.Alpha = _IQ(0.0);
    MStruc->VRef2SC.Beta = _IQ(0.0);
    MStruc->CRef2R.cd = _IQ(0.0);
    MStruc->CRef2R.cq = _IQ(0.0);
    MStruc->CRef2R.peak = _IQ(0.0);
    MStruc->C3S.PhaseA = _IQ(0.0);
    MStruc->C3S.PhaseB = _IQ(0.0);
    MStruc->C3S.PhaseC = _IQ(0.0);
    MStruc->C2S.Alpha = _IQ(0.0);
    MStruc->C2S.Beta = _IQ(0.0);

    MStruc->C2R_H.cd = _IQ(0.0);
    MStruc->C2R_H.cq = _IQ(0.0);
    MStruc->C2R.cd = _IQ(0.0);
    MStruc->C2R.cq = _IQ(0.0);

    MStruc->IdLPF1stH.Out_Pre = _IQ(0.0);
    MStruc->IdLPF1stH.Out_New = _IQ(0.0);

    MStruc->IqLPF1stH.Out_Pre = _IQ(0.0);
    MStruc->IqLPF1stH.Out_New = _IQ(0.0);
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
    System_Status_Global |= SPEED_CONTROL_M;
    RunningStatus_M = IDLE;
}

/*********************************************************************
 * @fn      Motor_FilerPara_Cal_M
 *
 * @brief   motor filter parameter initialization
 *
 * @para    MStruc: Motor overall state structure pointer
 *
 * @return  none
 */
void Motor_FilerPara_Cal_M(Motor_Sructure_Type *MStruc)
{
    MStruc->IdLPF1stH.Input_Coef = _IQdiv(IDQLPF_WcT_HIGH_M,(IDQLPF_WcT_HIGH_M + UNIT_Q24));
    MStruc->IdLPF1stH.Output_Coef = _IQdiv(UNIT_Q24,(IDQLPF_WcT_HIGH_M + UNIT_Q24));

    MStruc->IqLPF1stH.Input_Coef = _IQdiv(IDQLPF_WcT_HIGH_M,(IDQLPF_WcT_HIGH_M + UNIT_Q24));
    MStruc->IqLPF1stH.Output_Coef = _IQdiv(UNIT_Q24,(IDQLPF_WcT_HIGH_M + UNIT_Q24));

}
