/********************************** (C) COPYRIGHT *******************************
 * File Name          : M1_Motor_Parameter.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/15
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

//Motor rating parameter:
#define RATED_FREQ_M        3333                 //Rated frequency of motor(unit-Hz)
#define RATED_VOLT_M        _IQ18(8.5)           //Rated voltage of motor(unit-V)
#define RATED_CURR_M        _IQ18(12)            //Rated current of motor(unit-A)
#define ROTOR_FLUX_M        _IQ(0.00563)         //Motor permanent magnet linkage(unit-Wb)
#define POLEPARE_M          7                    //Number of motor poles(unit-1)
#define STATOR_RES_M        _IQ(0.600)           //Motor stator resistance(unit-¦¸)
#define STATOR_LD_M         _IQ(0.000304)        //Motor rotor direct axis inductance(unit-H)
#define STATOR_LQ_M         _IQ(0.000668)        //Motor rotor quadrature  axis inductance(unit-H)

//Motor stator resistance + sampling resistance(unit-¦¸)
#define STATOR_RES_TOTAL_M  (STATOR_RES_M + (CURR_SAMPLE_RESISTOR_ADC<<6))

/* Exported variables -------------------------------------------------------*/
extern  Runningstatus_Type  RunningStatus_M;     //System operating status
extern volatile uint16_t System_Status_Global;   //System state variable definition
extern Motor_Base_Type  MBase_M;                 //Motor base value parameter structure

/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/
void Motor_Prameter_Define(Motor_Base_Type *Basestruc);

void Global_Status_Initial(void);

void Motor_FilerPara_Cal_M(void);

void Motor_BaseCal_M(Motor_Base_Type *Basestruc);

#endif
