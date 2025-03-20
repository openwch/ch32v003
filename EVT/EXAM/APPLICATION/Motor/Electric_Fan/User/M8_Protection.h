/********************************** (C) COPYRIGHT *******************************
 * File Name          : M8_Protection.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
 * Description        : Protection parameter definition

*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __M8_PROTECTION_H
#define __M8_PROTECTION_H

/* Includes -----------------------------------------------------------------*/
/* Exported types -----------------------------------------------------------*/
/* Exported constants -------------------------------------------------------*/
#define INST_PHASEOC_THRE_M       _IQ(1.2)
#define INST_PHASEOC_CNT_THRE_M   (uint16_t)3

#define DCBUS_OVER_VOLT_THRE    _IQ(1.2)
#define DCBUS_UNDER_VOLT_THRE   _IQ(0.8)
#define DCBUS_OVER_VOLT_CNT     (uint16_t)500
#define DCBUS_UNDER_VOLT_CNT    (uint16_t)500
#define DCBUS_UNDER_VOLT_CNT    (uint16_t)500

#define OVERLOAD_CURR_THRE      _IQ(1.2)
#define OVERLOAD_TIME_THRE      0.5
#define OVERLOAD_CAL_FREQ       SPEED_LOOP_CAL_FREQ
#define OVERLOAD_CAL_COEF       _IQ(1/1.91)
#define OVERLOADLPF_CF_M        500
#define OVERLOADLPF_WcT_M       (PI_2_Q24/OVERLOAD_CAL_FREQ)*OVERLOADLPF_CF_M

#define PHASELOSS_DETECT_TIME    0.2
#define PHASELOSS_CNT_THRE       3
#define PHASELOSS_CURR_LOWERTHRE _IQ(0.08)
#define PHASELOSS_CURR_THRE      _IQ(0.05)
#define PHASELOSS_TIME_THRE      0.08
#define PHASELOSS_CAL_FREQ       FREQ_SVPWM
#define PHASELOSS_CAL_BUFFSIZE   32
#define PHASELOSS_CNT_THRE       3

/* Exported variables -------------------------------------------------------*/
extern InstPhaseOC_Type  InstPhaseOC_M;
extern DCbusProtection_Type DCBusProtecion_M;
extern Phaseloss_Type Phaseloss_M;

/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/
void Protection_and_Emergency_Init (void);

void Protection_SetFault(uint16_t Fault_type);

Truth_Verify_Type Phase_Ins_OverCurrent_Verify (Component_3S_Type *PhaseABC, InstPhaseOC_Type * InstOC);

void DCOvervolt_Protection(_iq24 DCVolt, DCbusProtection_Type *DCbusProtec);

void DCUndervolt_Protection(_iq24 DCVolt, DCbusProtection_Type *DCbusProtec);

void Speed_Outrange_Verify(_iq24 Speedref, _iq24 Speedact, Observer_Status_Type *Status);

Truth_Verify_Type Speed_Reliable_Verify(BuffPara_Type *Buffpara,_iq24 *Buffer,_iq24 Averspeed,_iq24 Threshold);

void Speed_Stable_Verify(Observer_Status_Type *Status,_iq24 Speedact);

void Motor_Stall_Verify(_iq24 Speedref, Motor_Observer_Type *MObserver,_iq24 Iqref, Observer_Status_Type *Status);

void Motor_Stall_Verify2(_iq24 Speedref, _iq24 Speedact , _iq24 Iqref, Observer_Status_Type *Status);

Truth_Verify_Type Phaseloss_Verify2R3R (Component_3S_Type *PhaseABC,Phaseloss_Type *Phaseloss,Motor_Sructure_Type *MStruc,int8_t Direction);

#endif
