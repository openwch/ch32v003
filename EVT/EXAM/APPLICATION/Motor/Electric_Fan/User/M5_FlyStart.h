/********************************** (C) COPYRIGHT *******************************
 * File Name          : M5_FlyStart.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
 * Description        : Flystart parameter definition
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __M5_FLYSTART_H
#define __M5_FLYSTART_H
/* Includes -----------------------------------------------------------------*/
/* Exported types -----------------------------------------------------------*/
/* Exported constants -------------------------------------------------------*/

#define FLYSTART_FREQ_M                  FREQ_SVPWM
#define FLYSTART_PERIOD_M                _IQ(1.0)/FLYSTART_FREQ_M
#define FLYSTART_VERIFY_TIME_M           0.3
#define FLYSTART_STILL_VERIFY_TIME_M     0.25

#define FLYSTARTPLL_BANDWIDTH_M          _IQ(35)
#define FLYSTARTPLL_KPADJ_M              _IQ(10.0)
#define FLYSTARTPLL_KIADJ_M              _IQ(10.0)

#define FLYSTART_VERIFY_NUM_M            10

#define FLYSTART_SPEED_LPF_CF_M          200
#define FLYSTART_SPEED_LPF_PERIOD_M      FLYSTART_SPEED_LPF_CF_M
#define FLYSTART_SPEED_LPF_WcT_M         _IQmpyI32(_IQmpy(FLYSTART_SPEED_LPF_PERIOD_M,PI_2_Q24),FLYSTART_SPEED_LPF_CF_M)

#define FLYSTART_EMFLPF_CF_M             500
#define FLYSTART_EMFLPF_PERIOD_M         FLYSTART_PERIOD_M
#define FLYSTART_EMFLPF_WcT_M            _IQmpyI32(_IQmpy(FLYSTART_EMFLPF_PERIOD_M,PI_2_Q24),FLYSTART_EMFLPF_CF_M)

#define FLYSTART_STILL_EMFMIN_M          _IQ(0.0078125)
#define FLYSTART_BEMF_THRESHOLD_M        _IQ(0.002)
#define FLYSTART_SPEED_BOUNDARY_M        _IQ(0.03)
#define FLYSTART_STILL_CURR_M            _IQ(0.03)

#define FLYSTART_STILLTIME               0.15
#define FLYSTART_BREAKTIME               0.5

#define FLYSTART_SPEED_MAX_M            _IQ(1.0)
#define FLYSTART_SPEED_MIN_M            _IQ(-1.0)

#define VQ_AMPLITUDE_EFFSET              10
#define VQ_ZERO_EFFSET                   5
#define VQ_ZERODETECTION_EFFSET          10
/* Exported variables -------------------------------------------------------*/
extern Flystart_type  Flystart_M;
/* Exported macro -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------ */

void Flystart_Prameter_Define(Flystart_type *Flystart);

void Flystart_Paramter_Cal (Flystart_type *Flystart,Motor_Base_Type *Basestruc);

void Flystart_Status_Init(Flystart_type *Flystart,int8_t Direction);

void Flystart_Process(Flystart_type *Flystart,int8_t Direction,Motor_Sructure_Type *MStruc,SVPWM_2R_Type *SVPWM_Cal,\
        Motor_Observer_Type *MObserver,ADCStruc_Type *ADCStruc,Motor_PID_Type *Mpid);

void Flystart_Switch(Flystart_type *Flystart,Speedrampctr_Type *SpeedRamp);

void Flystart_CtrLoop_Inital(Flystart_type *Flystart,Speedrampctr_Type *Speed_Ramp,Motor_PID_Type *Mpid,\
        Motor_Sructure_Type *MStruc,Motor_Observer_Type *MObserver,DCurrProcess_Type *DCurrProcess);

#endif
