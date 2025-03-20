/********************************** (C) COPYRIGHT *******************************
 * File Name          : M3_Foc_Observer_Parameter.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
 * Description        : Foc observer, PLL parameter definition, observer related protection parameter definition
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __M3_FOC_OBSERVER_H
#define __M3_FOC_OBSERVER_H

/* Includes -----------------------------------------------------------------*/
#define OBSERVER_CAL_FREQ_M          FREQ_SVPWM
#define OBSERVER_CAL_PERIOD_M        _IQ(1.0)/OBSERVER_CAL_FREQ_M

#define OBSERVER_K1_CERROR_CURR_M    2
#define OBSERVER_K2_CERROR_EMF_M     200
#define OBSERVER_K2_CERROR_EMF_ADJ_M 1
#define OBSERVER_CAL_TIMES_M         1

#define OBSERVER_CERROR_EMF_M        _IQ(10.0)
#define OBSERVER_MAXSMCERROR         _IQ(1.0)

#define OBSERVER_MAX_EMF_M           _IQ(2.0)
#define OBSERVER_MIN_EMF_M           _IQ(-2.0)

#define OBSERVER_MAX_CURR_M          _IQ(2.0)
#define OBSERVER_MIN_CURR_M          _IQ(-2.0)

#define OBSERVER_PLL_KP_M            140
#define OBSERVER_PLL_KI_M            25148

#define OBSERVER_SPEED_MAX_M         _IQ(1.5)
#define OBSERVER_SPEED_MIN_M         _IQ(0.01)

#define OBSERVER_ADAPTOR_ENABLE      0;

#define OBSERVER_ADAPTOR_COEF_M      10000
#define OBSERVER_ADAPTOR_EMF_ADJ_M   _IQ(0.5)
#define OBSERVER_SPEED_BUFFERSIZE_M    96

#define OBSERVER_POSICOM_ENABLE      0;

#define OBSERVER_SPEEDLPF_CF_M               50
#define OBSERVER_SPEEDLPF_PERIOD_M    (_IQ(1.0)/SPEED_LOOP_CAL_FREQ)
#define OBSERVER_SPEEDLPF_WcT_M       _IQmpyI32(_IQmpy(OBSERVER_SPEEDLPF_PERIOD_M,PI_2_Q24),OBSERVER_SPEEDLPF_CF_M)

#define OBSERVER_OVERSPEED_THRESH_M          _IQ(1.5)
#define OBSERVER_UNDERSPEED_THRESH_M         _IQ(0.05)
#define OBSERVER_OVERSPEED_CNT_THRESH_M      500
#define OBSERVER_UNDERSPEED_CNT_THRESH_M     500

#define OBSERVER_SPEED_VARIANCE_THRESH_M     _IQ(0.2)

#define OBSERVER_UNSTABLE_CNT_THRESH_M       300
#define OBSERVER_UNSTABLE_SPEED_THRESH_M     _IQ(0.02)

#define OBSERVER_CONVERGED_CNT_THRESH_M      50
#define OBSERVER_CONVERGED_UPPER_THRESH_M    _IQ(1.2)
#define OBSERVER_CONVERGED_LOWER_THRESH_M    _IQ(0.8)
#define OBSERVER_CONVERGED_EMFUP_THRESH_M    _IQ(0.6)
#define OBSERVER_CONVERGED_EMFDOWN_THRESH_M  _IQ(-0.25)

#define OBSERVER_STALLBAND_M         _IQ(0.5)
#define OBSERVER_STALLSPEED_M        _IQ(0.1)
#define OBSERVER_STALLCURR_M         _IQMPY_MACRO(_IQ(0.9),SPEED_REGULATOR_ULIM_M)

#define OBSERVER_STALL_TIME_THRESH_M     2
#define OBSERVER_REVERSE_CNT_THRESH_M    100
#define OBSERVER_REVERSE_SPEED_THRESH_M  _IQ(0.02)
/* Exported variables -------------------------------------------------------*/
extern Motor_Observer_Type Observer_M;
extern Observer_Status_Type    Observer_Status_M;
extern _iq24 Observer_SpeedBuffer_M[OBSERVER_SPEED_BUFFERSIZE_M];
/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/
void Observer_Status_Init (Motor_Observer_Type *MObserver,Observer_Status_Type *MStatus,int8_t Direction);

void Observer_Parameter_Define (Motor_Observer_Type *MObserver,Observer_Status_Type *MStatus);

void Observer_Paramter_Cal (Motor_Base_Type *Basestruc,Motor_Observer_Type *MObserver);

void Observer_EMF_Cal(Motor_Sructure_Type *MStruc,Motor_Observer_Type *MObserver);

void Observer_Speed_Posi_Cal(Motor_Observer_Type *MObserver,int8_t Direction);
#endif
