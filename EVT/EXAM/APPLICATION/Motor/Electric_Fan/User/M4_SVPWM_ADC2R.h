/********************************** (C) COPYRIGHT *******************************
 * File Name          : M4_SVPWM_ADC2R.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
 * Description        : SVPWM calculation and ADC related parameter definition
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __MOTOR_SVPWM_ADC1R_H
#define __MOTOR_SVPWM_ADC1R_H

/* Includes -----------------------------------------------------------------*/
/* Exported types -----------------------------------------------------------*/
/* Exported constants -------------------------------------------------------*/
#define DCBUS_RATED_VOLT    _IQ16(12)      // Rated bus voltage (unit-V)

#define SYSTEM_CLOCK_FREQ   48000000uL     // System clock Frequency (unit-Hz)

#define TIMER_PRESCALER_SVPWM       0
#define TIMER_CLOCK_FREQ_SVPWM      (SYSTEM_CLOCK_FREQ/(TIMER_PRESCALER_SVPWM+1))
#define TIMER_UPDATE_RATE_SVPWM     1
#define FREQ_SVPWM                  15000

#define DEADTIME_CONFIGURE  (uint16_t)(24)

#define BRK_GPIO_M    GPIOA
#define BRK_PIN_M     GPIO_Pin_7

#define CURR_SAMPLE_RESISTOR_ADC    _IQ16(0.1)
#define CURR_AMPLIFIER_ADC          _IQ16(3.125)

#define CURR_SAMPLE_RANGE_ADC       _IQ16div(_IQ16(1.25),_IQ16mpy(CURR_SAMPLE_RESISTOR_ADC,CURR_AMPLIFIER_ADC))

#define CURR_SAMPLING_TIMER_CLOCK        ADC_SampleTime_CyclesMode0

#define CURR_OFFSET_READ_NUM_ADC    ((uint8_t)16)

#define CURR_CHANNEL_PHASEA_M         ADC_Channel_OPA

#define DCVOLT_INIT         _IQ(1.0)
#define DCVOLT_CONV_RATIO   _IQ16(0.24193548387)
#define DCVOLT_AVER_SIZE    32

/* Exported variables -------------------------------------------------------*/
extern SVPWM_2R_Type    SVPWM_Cal_M;
extern ADCStruc_Type    ADC_M;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SVPWM_2R_Paramter_Init(SVPWM_2R_Type *SVPWM_Cal,Motor_Base_Type *Mbase);

void SVPWM_2R_Status_Init_M(SVPWM_2R_Type *SVPWM_Cal);

void ADC_Parameter_Init (ADCStruc_Type *ADCStruc,Motor_Base_Type *Mbase);

void ADC_Status_Init (ADCStruc_Type *ADCStruc);

void SVPWM_2R_Timer_Configure(void);

void ADC_OPA_Configure(ADCStruc_Type *ADCStruc);

void ADC_Offset_Reading(ADC_TypeDef *ADCx, ADCStruc_Type *ADCStruc,u_int8_t ADC_num);

void ADC_Start (FunctionalState cmd);

void SVPWM_Update (SVPWM_2R_Type *SVPWM_Cal);

void DCBUS_Volt_Cal(ADCStruc_Type *ADCStruc,ADC_TypeDef *ADCx);

void SVPWM_Calculation(SVPWM_2R_Type *SVPWM_Cal, Component_2S_Type *VoltRef);

Component_3S_Type Current_Cal_2R(ADCStruc_Type *ADCStruc);

void SVPWM_Zerovolt_Output(SVPWM_2R_Type *SVPWM_Cal);

void SVPWM_Lowside_Effective(SVPWM_2R_Type *SVPWM_Cal);

#endif
