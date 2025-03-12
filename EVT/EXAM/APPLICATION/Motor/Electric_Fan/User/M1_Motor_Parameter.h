/********************************** (C) COPYRIGHT *******************************
 * File Name          : M1_Motor_Parameter.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
 * Description        : Definition of motor parameters file.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __M1_MOTOR_PARAMETER_H
#define __M1_MOTOR_PARAMETER_H

/* Includes -----------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define RATED_FREQ_M             100
#define RATED_VOLT_M             _IQ16(8.49)
#define RATED_CURR_M             _IQ16(1.25)
#define ROTOR_FLUX_M             _IQ(0.007643)
#define POLEPARE_M               5
#define STATOR_RES_M             _IQ(0.98)
#define STATOR_LD_M              _IQ(0.0020299)
#define STATOR_LQ_M              _IQ(0.0025204)

#define INERTIA_M                _IQ(0.00002)
#define FRICTION_M               _IQ(0.0001)

#define STATOR_RES_TOTAL_M  (STATOR_RES_M + (CURR_SAMPLE_RESISTOR_ADC<<6))

#define IDQLPF_CF_HIGH_M           400
#define IDQLPF_CF_LOW_M            200
#define IDQLPF_PERIOD_M            OBSERVER_CAL_PERIOD_M
#define IDQLPF_WcT_HIGH_M          _IQmpyI32(_IQmpy(IDQLPF_PERIOD_M,PI_2_Q24),IDQLPF_CF_HIGH_M)
#define IDQLPF_WcT_LOW_M           _IQmpyI32(_IQmpy(IDQLPF_PERIOD_M,PI_2_Q24),IDQLPF_CF_LOW_M)

/* Exported variables -------------------------------------------------------*/
extern Motor_Sructure_Type MStruc_M;
extern Motor_Base_Type  MBase_M;
extern  Runningstatus_Type  RunningStatus_M;
extern volatile uint16_t System_Status_Global;
/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/
void Motor_Prameter_Define(Motor_Base_Type *Basestruc);

void Motor_Status_Initial_M(Motor_Sructure_Type *MStruc);

void Global_Status_Initial(void);

void Motor_FilerPara_Cal_M(Motor_Sructure_Type *MStruc);

void Motor_BaseCal_M(Motor_Base_Type *Basestruc);

#endif
