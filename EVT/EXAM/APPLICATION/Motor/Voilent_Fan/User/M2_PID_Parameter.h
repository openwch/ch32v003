/********************************** (C) COPYRIGHT *******************************
 * File Name          : M2_PID_Parameter.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/15
 * Description        : Definition of PID control parameters.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __M2_MOTOR_PID_H
#define __M2_MOTOR_PID_H

/* Includes -----------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
//Speed regulator parameter PID adjustment
#define SPEED_REGULATOR_FREQ_M          SPEED_LOOP_CAL_FREQ              //Speed regulator execution frequency(unit-Hz)
#define SPEED_REGULATOR_PERIOD_M        (_IQ(1.0)/SPEED_REGULATOR_FREQ_M)//Speed regulator execution period(unit-s)
#define SPEED_REGULATOR_KP_M             _IQ(0.00090)                    //Ratio factor of speed regulator(Actual value)
#define SPEED_REGULATOR_KI_M             _IQ(0.0072)                     //Integration factor of speed regulator(Actual value)
#define SPEED_REGULATOR_KD_M             _IQ(0.0)                        //Differential factor of speed regulator
#define SPEED_REGULATOR_KPADJ_M          _IQ(1.0)                        //Adjustment factor of speed regulator ratio factor
#define SPEED_REGULATOR_KIADJ_M          _IQ(1.0)                        //Adjustment factor of speed regulator integration factor
#define SPEED_REGULATOR_KDADJ_M          _IQ(1.0)                        //Adjustment factor of speed regulator differential factor
#define SPEED_REGULATOR_ULIMINIT_M       _IQ(0.5)                        //Initial output limiter upper limit of the speed regulator(unit-A_per)
#define SPEED_REGULATOR_ULIM_M           _IQ(6.5)                        //The final limit of the output limiting value of the speed regulator(unit-A_per)
#define SPEED_REGULATOR_LLIM_M           _IQ(0.005)                      //Initial output limiter lower limit of the speed regulator(unit-A_per)
#define SPEED_REGULATOR_DLIM_M           _IQ(0.1)                        //The output of speed regulator is limited to one time(unit-A_per)

#define SPEED_REGULATOR_KPEH_M           _IQ(3.0)                        //KP reinforcement factor of speed regulator(Actual value)
#define SPEED_REGULATOR_KIEH_M           _IQ(2.0)                        //KI reinforcement factor of speed regulator(Actual value)
#define SPEED_REGULATOR_KPWE_M           _IQ(0.5)                        //KP attenuation factor of speed regulator(Actual value)
#define SPEED_REGULATOR_KIWE_M           _IQ(0.5)                        //KI attenuation factor of speed regulator(Actual value)
#define SPEED_REGULATOR_KIEN_THRESH_M    _IQ(0.05)                       //The enhancement threshold of the speed regulator(unit-rpm_per)
#define SPEED_REGULATOR_KIWE_THRESH_M    _IQ(-0.05)                      //Attenuation threshold of the speed regulator(unit-rpm_per)

//Speed regulator parameters  Open loop regulation
#define SPEED_SCONTROL_ISTEP1_M         2                                //The incremental step 1 of the speed regulation PWM
#define SPEED_SCONTROL_ISTEP2_M         1                                //The incremental step 2 of the speed regulation PWM
#define SPEED_SCONTROL_ISTEPSWITCH_M   _IQ(0.1)                          //Switching threshold ofspeed regulation PWM (unit-rpm_per)
#define SPEED_SCONTROL_DSTEP1_M         2                                //The decremental step 1 of the speed regulation PWM
#define SPEED_SCONTROL_DSTEP2_M         1                                //The decremental step 2 of the speed regulation PWM
#define SPEED_SCONTROL_OCDSTEP_M        1                                //The decremental step of the speed regulation PWM to limit current
#define SPEED_SCONTROL_OSDSTEP_M        1                                //The decremental step of the speed regulation PWM to limit speed

//Current regulator parameters  Open loop regulation
#define CURR_REGULATOR_FREQ_M       FREQ_PWM                             //Current regulator execution frequency(unit-Hz)
#define CURR_REGULATOR_BW_M         (uint16_t)(PI_2f * 400)              //bandwidth of current regulator(unit-rad/s)
#define CURR_REGULATOR_KD_M         _IQ(0.0)                             //Differential factor of current regulator
#define CURR_REGULATOR_KPADJ_M      _IQ(2.5)                             //Adjustment factor of current regulator ratio factor
#define CURR_REGULATOR_KIADJ_M      _IQ(0.5)                             //Adjustment factor of current regulator integration factor
#define CURR_REGULATOR_KDADJ_M      _IQ(1.0)                             //Adjustment factor of current regulator differential factor
#define CURR_REGULATOR_ULIM_M       _IQ(1.0)                             //The final limit of the output limiting value of the current regulator(unit-V_per)
#define CURR_REGULATOR_LLIM_M       _IQ18div(_IQ18(PWM_DUTY_MIN),_IQ18(TIMER_PERIOD_PWM+1))<<6
#define CURR_REGULATOR_DLIM_M       _IQ(0.1)                             //The output of current regulator is limited to one time(unit-A_per)


/* Exported variables -------------------------------------------------------*/
extern Speed_SContrl_Type Speedsctl_M;

/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/
void Speedreg_Prameter_Define (Speed_SContrl_Type *Speedsctl);

void Control_Switch_Sloop(Speedrampctr_Type *Speed_Ramp,Observer_Status_Type *EMFStatus);

void Speedregulator(_iq24 hReference, _iq24 hFeedback,Speedrampctr_Type *Speed_Ramp,PWM_CHOPPER_TYPE *PWMchopper,\
        ADCStruc_Type *ADCStruc,Speed_SContrl_Type *Speedsctl);

void Speedreg_Lim(Speedrampctr_Type *Speed_Ramp,PWM_CHOPPER_TYPE *PWMchopper,ADCStruc_Type *ADCStruc,\
        EMF_OBSERVER_TYPE *EMFCal,Speed_SContrl_Type *Speedsctl);

#endif
