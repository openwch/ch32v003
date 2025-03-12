/********************************** (C) COPYRIGHT *******************************
 * File Name          : M4_PWM_ADC.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/15
 * Description        : Definition ADC and PWM parameters.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __M3_ADC_PWM_H
#define __M3_ADC_PWM_H

/* Includes -----------------------------------------------------------------*/
/* Exported types -----------------------------------------------------------*/
/* Exported constants -------------------------------------------------------*/
#define DCBUS_RATED_VOLT         _IQ18(8.4)          //Rated bus voltage(unit-V)

#define SYSTEM_CLOCK_FREQ        48000000uL          //Frequency of system clock(unit-Hz)

#define TIMER_PRESCALER_PWM      0                   //Predivision coefficient of PWM timer

#define TIMER_CLOCK_FREQ_PWM     (SYSTEM_CLOCK_FREQ/(TIMER_PRESCALER_PWM+1))

#define TIMER_UPDATE_RATE_PWM    0                   //Update rate of PWM timer

#define FREQ_PWM                 30000               //Frequency of PWM timer(unit-Hz)

#define TIMER_PERIOD_PWM         (u16)(TIMER_CLOCK_FREQ_PWM/(u32)(FREQ_PWM))-1   //Period of PWM timer

#define PERIOD_PWM               _IQ(1.0)/FREQ_PWM   //Count backwards of PWM timer period(unit-s)

#define PWM_DUTY_MIN             300                 //Minimum duty
#define PWM_DUTY_MAX             1600                //Maximum duty
#define PWM_DUTY_MAX_INIT        1600                //Initial value of maximum duty
#define PWM_DUTY_MAX_RAMPTIME    0.5                 //Adjust time of Maximum duty(unit-s)
#define PWM_DUTY_MAX_CALFREQ     SPEED_LOOP_CAL_FREQ //Adjust frequency of Maximum duty
#define PWM_DUTY_LIM_CURR        _IQMPY_MACRO(INST_PHASEOC_THRE,_IQ(0.93))  //Limit current of PWM duty
#define PWM_DUTY_LIM_CURR_COEF   _IQ(0.666667)       //Coefficient of PWM duty limit current

#define DEADTIME_CONFIGURE  (uint8_t)0

#define BRK_GPIO_M    GPIOB                          //Emergency brake port group
#define BRK_PIN_M     GPIO_Pin_5                     //Emergency brake port

#define   U_Mos_H_Pin       GPIO_Pin_3               //PWM pin configuration
#define   V_Mos_H_Pin       GPIO_Pin_0
#define   W_Mos_H_Pin       GPIO_Pin_1

#define   U_Mos_L_Pin       GPIO_Pin_0
#define   V_Mos_L_Pin       GPIO_Pin_2
#define   W_Mos_L_Pin       GPIO_Pin_0

#define SAMPLING_TIMER_CLOCK        ADC_SampleTime_CyclesMode0

#define CURR_SAMPLE_RESISTOR_ADC    _IQ18(0.005)     //Resistance of current sampling
#define CURR_AMPLIFIER_ADC          _IQ18(8.0)       //Hardware magnification of current sampling
#define CURR_OFFSET_READ_NUM_ADC    ((uint8_t)16)    //calculate cnt of current sample

#define CURR_SAMPLE_RANGE_ADC       _IQ18div(_IQ18(2.5),_IQ18mpy(CURR_SAMPLE_RESISTOR_ADC,CURR_AMPLIFIER_ADC))

#define IDC_LPF2_CF_M         100                     //Cutoff frequency of a current low-pass filter(unit-Hz)
#define IDC_LPF2_PERIOD_M     SPEED_LOOP_CAL_PERIOD   //Calculate period of a current low-pass filter(unit-s
#define IDC_LPF2_WcT_M        _IQmpyI32(_IQmpy(IDC_LPF2_PERIOD_M,PI_2_Q24),IDC_LPF2_CF_M)

#define DCVOLT_INIT             _IQ(1.0)         //Initial value of the sampled bus voltage mean
#define DCVOLT_CONV_RATIO       _IQ18(0.28169)   //The ratio of the resistance to the busbar voltage sampling

#define BEMF_CONV_RATIO         _IQ18(0.28169)   //The ratio of the resistance to BEMF voltage sampling

#define VIN_CONV_RATIO          _IQ18(0.50)      //The ratio of the resistance to external voltage sampling
#define VIN_AVER_SIZE           32               //Calculate cnt of external voltage sampling

#define CURR_CHANNEL_PHASE_M    ADC_Channel_9   //Current sample channel
#define VBUS_CHANNEL_PHASE_M    ADC_Channel_1  //Bus voltage sample channel

#define UEMF_CHANNEL_PHASE_M    ADC_Channel_6   //U phase BEMF sample channel
#define VEMF_CHANNEL_PHASE_M    ADC_Channel_5   //V phase BEMF sample channel
#define WEMF_CHANNEL_PHASE_M    ADC_Channel_7   //W phase BEMF sample channel

/* Exported variables -------------------------------------------------------*/
extern ADCStruc_Type    ADC_M;
extern PWM_CHOPPER_TYPE PWMcopper_M;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void PWMADC_Timer_Configure(void);

void ADC_OPA_Configure(ADCStruc_Type *ADCStruc);

void ADC_Parameter_Cal (ADCStruc_Type *ADCStruc,Motor_Base_Type *Mbase);

void ADC_Status_Init (ADCStruc_Type *ADCStruc);

void ADC_Offset_Reading(ADCStruc_Type *ADCStruc);

void ADC_Start (FunctionalState cmd);

void PWM_Parameter_Define_M (PWM_CHOPPER_TYPE *PWMchopper);

void PWM_Status_Initial_M (PWM_CHOPPER_TYPE *PWMchopper);

void PWM_Enable_M (FunctionalState cmd);

void PWM_Update(PWM_CHOPPER_TYPE *PWMchopper);

void ADC_Trigger_Update(PWM_CHOPPER_TYPE *PWMchopper);

void Current_Cal(ADCStruc_Type *ADCStruc,ADC_TypeDef *ADCx);

void DCBUS_Volt_Cal(ADCStruc_Type *ADCStruc,ADC_TypeDef *ADCx);

void Bemf_Cal(ADCStruc_Type *ADCStruc,ADC_TypeDef *ADCx);

void IDC_Cal(ADCStruc_Type *ADCStruc,ADC_TypeDef *ADCx);

void PWM_Switch(PWM_CHOPPER_TYPE *PWMchopper);

void PWM_LON_HOFF(PWM_CHOPPER_TYPE *PWMchopper);

void PWM_LOFF_HOFF(PWM_CHOPPER_TYPE *PWMchopper);

#endif
