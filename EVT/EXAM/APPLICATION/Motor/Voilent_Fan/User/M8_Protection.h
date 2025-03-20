/********************************** (C) COPYRIGHT *******************************
 * File Name          : M8_Protection.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/15
 * Description        : Protection_and emergency parameter define.
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
//Overcurrent protection parameter definition
#define INST_PHASEOC_THRE       _IQ(1.5)            //Instantaneous software overcurrent protection threshold(unit-A_per)
#define INST_PHASEOC_CAL_FREQ   FREQ_PWM            //Calculation frequency of instantaneous software overcurrent protection(unit-Hz)
#define INST_PHASEOC_TIME_THRE  0.015               //Time of instantaneous software overcurrent protection(unit-s)

//Bus voltage parameter definition
#define DCBUS_PROTEC_CALFREQ    SPEED_LOOP_CAL_FREQ //Bus voltage protection calculation frequency(unit-Hz)
#define DCBUS_OVER_THRE         _IQ(1.2)            //Bus overvoltage threshold(unit-V_per)
#define DCBUS_UNDER_THRE        _IQ(0.7)            //Bus undervoltage threshold(unit-V_per)
#define DCBUS_OVER_TIME         0.1                 //Bus overvoltage protection time threshold(unit-s)
#define DCBUS_UNDER_TIME        0.5                 //Bus undervoltage protection time threshold(unit-s)

//Overload protection parameter definition
#define OVERLOAD_CURR_THRE      _IQ(6.5)            //Overload current threshold(unit-A_per)
#define OVERLOAD_CAL_FREQ       SPEED_LOOP_CAL_FREQ //Overload calculation frequency(unit-Hz)
#define OVERLOAD_TIME_THRE      1                   //Overload time threshold(unit-s)
#define OVERLOADLPF_CF          50                  //Overload protection current Low pass filter cut-off frequency(unit-Hz)
#define OVERLOADLPF_WcT         (PI_2_Q24/OVERLOAD_CAL_FREQ)*OVERLOADLPF_CF

/* Exported variables -------------------------------------------------------*/
extern InstPhaseOC_Type         InstPhaseOC_M;
extern DCbusProtection_Type     DCBusProtecion_M;

/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/
void Protection_and_Emergency_Init (void);

void Protection_SetFault(uint16_t Fault_type);

void Protction_Fault_Reset_M(void);

Truth_Verify_Type Phase_Ins_OverCurrent_Verify_DC (_iq24 IDC, InstPhaseOC_Type * InstOC);

void DCBus_Protecion(_iq18 DCVolt, DCbusProtection_Type *DCbusProtec);

void Motor_Stall_Verify1(_iq24 Speedref, EMF_OBSERVER_TYPE *EMFCal, Observer_Status_Type *Status);

void Motor_Stall_Verify3( EMF_OBSERVER_TYPE *EMFCal, Observer_Status_Type *Status,PWM_CHOPPER_TYPE *PWMchopper);
#endif
