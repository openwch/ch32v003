/********************************** (C) COPYRIGHT *******************************
 * File Name          : M2_PID_Parameter_Initial.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
 * Description        : PID control structure definition and state initialization.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Includes------------------------------------------------------------------*/
#include "M0_Control_Library.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private functions --------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Variables ----------------------------------------------------------------*/
Motor_PID_Type      Mpid_M;
DCurrProcess_Type   DCurrProcess_M;

#ifdef MAG100_TABLE_USED
const _iq24 Magnitude_table[256] = MAG100_LIMIT_TABLE;
#elif  defined MAG098_TABLE_USED
const _iq24 Magnitude_table[256] = MAG098_LIMIT_TABLE;
#elif  defined MAG096_TABLE_USED
const _iq24 Magnitude_table[256] = MAG096_LIMIT_TABLE;
#elif  defined MAG094_TABLE_USED
const _iq24 Magnitude_table[256] = MAG094_LIMIT_TABLE;
#elif  defined MAG092_TABLE_USED
const _iq24 Magnitude_table[256] = MAG092_LIMIT_TABLE;
#elif  defined MAG090_TABLE_USED
const _iq24 Magnitude_table[256] = MAG090_LIMIT_TABLE;
#elif  defined MAG088_TABLE_USED
const _iq24 Magnitude_table[256] = MAG088_LIMIT_TABLE;
#elif  defined MAG086_TABLE_USED
const _iq24 Magnitude_table[256] = MAG086_LIMIT_TABLE;
#endif

/*********************************************************************
 * @fn      PID_Prameter_Define
 *
 * @brief   Speed regulator, current regulator parameter definition
 *
 * @para    Mpid: Motor PID regulator structure pointer
 *
 * @return  none
 */
void PID_Prameter_Define (Motor_PID_Type * Mpid)
{
    Mpid->Speed.CalFreq = SPEED_LOOP_CAL_FREQ;
    Mpid->Speed.CalT = SPEED_REGULATOR_PERIOD_M;
    Mpid->Speed.Kpreal= SPEED_REGULATOR_KP_M;
    Mpid->Speed.Kireal= SPEED_REGULATOR_KI_M;
    Mpid->Speed.Kdreal= SPEED_REGULATOR_KD_M;
    Mpid->Speed.Kpadj = SPEED_REGULATOR_KPADJ_M;
    Mpid->Speed.Kiadj = SPEED_REGULATOR_KIADJ_M;
    Mpid->Speed.Kdadj = SPEED_REGULATOR_KDADJ_M;

    if(STARTUP_DIRECTION ==1)
    {
        Mpid->Speed.Upper_Lim = SPEED_REGULATOR_ULIM_M;
        Mpid->Speed.Lower_Lim = SPEED_REGULATOR_LLIM_M;
    }
    else
    {
        Mpid->Speed.Upper_Lim = -SPEED_REGULATOR_LLIM_M;
        Mpid->Speed.Lower_Lim = -SPEED_REGULATOR_ULIM_M;
    }

    Mpid->Speed.Deltalim = SPEED_REGULATOR_DLIM_M;

    Mpid->dCurr.CalFreq= CURR_REGULATOR_FREQ_M;
    Mpid->dCurr.CalT= _IQ(1.0)/CURR_REGULATOR_FREQ_M;
    Mpid->dCurr.Bw= CURR_REGULATOR_BW_M;
    Mpid->dCurr.Kdreal= DAXIS_CURR_REGULATOR_KD_M;
    Mpid->dCurr.Kpadj = DAXIS_CURR_REGULATOR_KPADJ_M;
    Mpid->dCurr.Kiadj = DAXIS_CURR_REGULATOR_KIADJ_M;
    Mpid->dCurr.Kdadj = DAXIS_CURR_REGULATOR_KDADJ_M;
    Mpid->dCurr.Upper_Lim = DAXIS_CURR_REGULATOR_ULIM_M;
    Mpid->dCurr.Lower_Lim = DAXIS_CURR_REGULATOR_LLIM_M;
    Mpid->dCurr.Deltalim = QAXIS_CURR_REGULATOR_DLIM_M;

    Mpid->qCurr.CalFreq= CURR_REGULATOR_FREQ_M;
    Mpid->qCurr.CalT= _IQ(1.0)/CURR_REGULATOR_FREQ_M;
    Mpid->qCurr.Bw= CURR_REGULATOR_BW_M;
    Mpid->qCurr.Kdreal= QAXIS_CURR_REGULATOR_KD_M;
    Mpid->qCurr.Kpadj = QAXIS_CURR_REGULATOR_KPADJ_M;
    Mpid->qCurr.Kiadj = QAXIS_CURR_REGULATOR_KIADJ_M;
    Mpid->qCurr.Kdadj = QAXIS_CURR_REGULATOR_KDADJ_M;
    Mpid->qCurr.Upper_Lim = QAXIS_CURR_REGULATOR_ULIM_M;
    Mpid->qCurr.Lower_Lim = QAXIS_CURR_REGULATOR_LLIM_M;
    Mpid->qCurr.Deltalim = QAXIS_CURR_REGULATOR_DLIM_M;
}

/*********************************************************************
 * @fn      PID_Output_Init
 *
 * @brief   Speed regulator and current regulator status and varialbles initialization.
 *
 * @para    Mpid: Motor PID regulator structure pointer
 *
 * @return  none
 */
void PID_Output_Init(Motor_PID_Type * Mpid, int8_t Direction)
{
    Mpid->Speed.Err_New= 0;
    Mpid->Speed.Err_Prev1= 0;
    Mpid->Speed.Err_Prev2= 0;
    Mpid->Speed.Output_New = 0;
    Mpid->Speed.Output_Prev = 0;

    Mpid->Speed.Deltaoutput = 0;
    Mpid->Speed.Differential = 0;

    Mpid->dCurr.Err_New= 0;
    Mpid->dCurr.Err_Prev1= 0;
    Mpid->dCurr.Err_Prev2= 0;
    Mpid->dCurr.Output_New= 0;
    Mpid->dCurr.Output_Prev= 0;
    Mpid->dCurr.Deltaoutput =0;

    Mpid->qCurr.Err_New= 0;
    Mpid->qCurr.Err_Prev1= 0;
    Mpid->qCurr.Err_Prev2= 0;
    Mpid->qCurr.Output_New = 0;
    Mpid->qCurr.Output_Prev = 0;
    Mpid->qCurr.Deltaoutput = 0;

    if(Direction ==1)
    {
        Mpid->Speed.Upper_Lim = SPEED_REGULATOR_ULIM_M;
        Mpid->Speed.Lower_Lim = SPEED_REGULATOR_LLIM_M;
    }
    else
    {
        Mpid->Speed.Upper_Lim = -SPEED_REGULATOR_LLIM_M;
        Mpid->Speed.Lower_Lim = -SPEED_REGULATOR_ULIM_M;
    }
}

/*********************************************************************
 * @fn      DCurrProcess_Parameter_Define
 *
 * @brief   D-axis current processing parameters definition
 *
 * @para    DCurrProcess: D-axis current processing structure pointer
 *
 * @return  none
 */

void DCurrProcess_Parameter_Define(DCurrProcess_Type * DCurrProcess)
{
    DCurrProcess->Target1 = DAXIS_CURR_REF_INIT1_M;
    DCurrProcess->Target2 = DAXIS_CURR_REF_INIT2_M;
    DCurrProcess->AdjustStep1 = DAXIS_CURR_REF_STEP1_M;
    DCurrProcess->AdjustStep2 = DAXIS_CURR_REF_STEP2_M;
    DCurrProcess->Weakpoint = DAXIS_CURR_WEAK_SPEED_M;
}
