/********************************** (C) COPYRIGHT *******************************
 * File Name          : M3_Foc_Observer_Initial.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
 * Description        : Structure definition and state initialization of Foc observer.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "M0_Control_Library.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/

Motor_Observer_Type Observer_M;
Observer_Status_Type    Observer_Status_M;
_iq24 Observer_SpeedBuffer_M[OBSERVER_SPEED_BUFFERSIZE_M];
/*********************************************************************
 * @fn      Observer_Parameter_Define
 *
 * @brief   observer parameter definition
 *
 * @para    MObserver: observer structure pointer
 *          MStatus: observer state structure pointer
 *
 * @return  none
 */
void Observer_Parameter_Define (Motor_Observer_Type *MObserver,Observer_Status_Type *MStatus)
{
    MObserver->Para.CalFreq = OBSERVER_CAL_FREQ_M;
    MObserver->Para.CalT = OBSERVER_CAL_PERIOD_M;
    MObserver->Para.K1CerrC = OBSERVER_K1_CERROR_CURR_M;
    MObserver->Para.K2CerrE = OBSERVER_K2_CERROR_EMF_M;
    MObserver->Para.kCE_Adj = OBSERVER_K2_CERROR_EMF_ADJ_M;
    MObserver->Para.Caltimes = OBSERVER_CAL_TIMES_M;

    MObserver->Para.Kslide=OBSERVER_CERROR_EMF_M;
    MObserver->Para.MaxSMCError=OBSERVER_MAXSMCERROR;

    MObserver->Cal_Struc.EUlim = OBSERVER_MAX_EMF_M;
    MObserver->Cal_Struc.ELlim = OBSERVER_MIN_EMF_M;
    MObserver->Cal_Struc.CUlim = OBSERVER_MAX_CURR_M;
    MObserver->Cal_Struc.CLlim = OBSERVER_MIN_CURR_M;

    MObserver->Cal_Struc.SpeedLPF1st_M.Input_Coef = _IQdiv(OBSERVER_SPEEDLPF_WcT_M,(OBSERVER_SPEEDLPF_WcT_M + UNIT_Q24));
    MObserver->Cal_Struc.SpeedLPF1st_M.Output_Coef = _IQdiv(UNIT_Q24,(OBSERVER_SPEEDLPF_WcT_M + UNIT_Q24));
    MObserver->Cal_Struc.Posicom_Enable = OBSERVER_POSICOM_ENABLE;

    MObserver->Speed_PLL.Kp = OBSERVER_PLL_KP_M;
    MObserver->Speed_PLL.Ki = OBSERVER_PLL_KI_M;
    MObserver->Speed_PLL.Upper_Lim = OBSERVER_SPEED_MAX_M;
    MObserver->Speed_PLL.Lower_Lim = OBSERVER_SPEED_MIN_M;

    MObserver->Adaptor.Enable = OBSERVER_ADAPTOR_ENABLE;
    MObserver->Adaptor.Gain = OBSERVER_ADAPTOR_COEF_M;
    MObserver->Adaptor.EMF_Adj = OBSERVER_ADAPTOR_EMF_ADJ_M;

    MObserver->SpeedBuffPara.Length = OBSERVER_SPEED_BUFFERSIZE_M;

    MStatus->OverspeedThresh = OBSERVER_OVERSPEED_THRESH_M;
    MStatus->OverspeedcntThresh = OBSERVER_OVERSPEED_CNT_THRESH_M;
    MStatus->UnderspeedThresh = OBSERVER_UNDERSPEED_THRESH_M;
    MStatus->UnderspeedcntThresh = OBSERVER_UNDERSPEED_CNT_THRESH_M;
    MStatus->VarianceThresh = OBSERVER_SPEED_VARIANCE_THRESH_M;
    MStatus->UnstableCntThresh = OBSERVER_UNSTABLE_CNT_THRESH_M;
    MStatus->UnstableSpeedThresh = OBSERVER_UNSTABLE_SPEED_THRESH_M;
    MStatus->StallSpeedBand = OBSERVER_STALLBAND_M;
    MStatus->StallSpeed = OBSERVER_STALLSPEED_M;
    MStatus->StallCurr =  OBSERVER_STALLCURR_M;
    MStatus->StallCntThresh = OBSERVER_STALL_TIME_THRESH_M*SPEED_LOOP_CAL_FREQ;

}

/*********************************************************************
 * @fn      Observer_Status_Init
 *
 * @brief   observer state initialization
 *
 * @para    MObserver:observer structure pointer
 *          MStatus:observer state structure pointer
 *
 * @return  none
 */
void Observer_Status_Init (Motor_Observer_Type *MObserver,Observer_Status_Type *MStatus,int8_t Direction)
{
    MObserver->Cal_Struc.Inimark = FALSE;

    MObserver->Cal_Struc.EalphaNew = _IQ(0.0);
    MObserver->Cal_Struc.EalphaPrev = _IQ(0.0);

    MObserver->Cal_Struc.EbetaNew = _IQ(0.0);
    MObserver->Cal_Struc.EbetaPrev  = _IQ(0.0);

    MObserver->Cal_Struc.CalphaNew = _IQ(0.0);
    MObserver->Cal_Struc.CalphaPrev = _IQ(0.0);
    MObserver->Cal_Struc.CbetaNew = _IQ(0.0);
    MObserver->Cal_Struc.CbetaPrev = _IQ(0.0);

    MObserver->Cal_Struc.ThetaErr = _IQ(0.0);
    MObserver->Cal_Struc.PosiElecPU = _IQ(0.0);
    MObserver->Cal_Struc.PosiElecPU_Pre = _IQ(0.0);
    MObserver->Cal_Struc.PosiElecPU_Cali = _IQ(0.0);
    MObserver->Cal_Struc.PosiElecPU_Com = _IQ(0.0);
    MObserver->Cal_Struc.Angspeed_Inst = _IQ(0.0);
    MObserver->Cal_Struc.Angspeed_Aver1 = _IQ(0.0);
    MObserver->Cal_Struc.Angspeed_Aver2 = _IQ(0.0);
    MObserver->Cal_Struc.Triangle.Cos = _IQ(0.0);
    MObserver->Cal_Struc.Triangle.Sin = _IQ(0.0);

    MObserver->Cal_Struc.EmfErr = 0;
    MObserver->Cal_Struc.Obs_Calcnt =0;

    MStatus->Overspeed = FALSE;
    MStatus->Underspeed = FALSE;
    MStatus->Reliable = FALSE;
    MStatus->Stable = TRUE;
    MStatus->Stall = FALSE;

    MStatus->Overspeed_CNT =0;
    MStatus->Underspeed_CNT = 0;
    MStatus->Unstable_CNT = 0;
    MStatus->Stall_CNT = 0;

    MObserver->Speed_PLL.Error_New = _IQ(0.0);
    MObserver->Speed_PLL.Error_Prev = _IQ(0.0);
    MObserver->Speed_PLL.Output_New = _IQ(0.0);
    MObserver->Speed_PLL.Output_Prev = _IQ(0.0);

    MObserver->Speed_PLL.Upper_Lim = OBSERVER_SPEED_MAX_M * Direction;
    MObserver->Speed_PLL.Lower_Lim = OBSERVER_SPEED_MIN_M * Direction;

#if defined OBSERVER_ADAPTOR
    MObserver->Adaptor.Alpha_Err = _IQ(0.0);
    MObserver->Adaptor.Alpha_Pre = _IQ(0.0);
    MObserver->Adaptor.Alpha_New = _IQ(0.0);
    MObserver->Adaptor.Beta_Err = _IQ(0.0);
    MObserver->Adaptor.Beta_Pre = _IQ(0.0);
    MObserver->Adaptor.Beta_New = _IQ(0.0);
#endif
}

