/********************************** (C) COPYRIGHT *******************************
 * File Name          : M3_BLDC_EMF.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/15
 * Description        : Definition BEMF detect and calculate parameters.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __M4_BLDC_EMF_H
#define __M4_BLDC_EMF_H

/* Includes -----------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define EMF_TIMER_CLOCK_FREQ         48000000uL       //Clock frequencies of EMF detect TIM(unit-Hz)
#define EMF_TIMER_PERIOD             U16_MAX          //Period  of EMF detect TIM
#define EMF_TIMER_PRESCALAR          32               //Predivision coefficient of EMF detect TIM
#define EMF_AVOID_PRESCALAR          48

#define EMF_OVERFLOW_THRESHOLD       1                //Threshold of TIM overflow
#define EMF_ERR_THRESHOLD            _IQ(0.01)        //Decision bias of EMF(Unit_Vper)
#define EMF_SECTOR_CHANGE_DELAY      800              //Decision delay time of the firet EMF sector(Unit_sample period)
#define EMF_SECTOR_CHANGE_COEF       _IQ12(0.40)      //Prechange phase factor of EMF
#define SCHANGE_Thre                 3                //Minimum speed sector number

#define  EMF_SECTOR_SAMPLE_DELAY     3                //cnt of afterflow TIM

#define EMF_SPEED_BUFFERSIZE_M1      16               //Length of calculating the mean speed
#define EMF_SPEED_BUFFERSIZE_M2      32               //Length of calculating the mean speed

#define EMF_SPEED_SWUP1_M            _IQ(0.80)        //Switch upper limit 1 of observation feedback speed(unit-rad/s_per)
#define EMF_SPEED_SWUP2_M            _IQ(0.90)        //Switch upper limit 2 of observation feedback speed(unit-rad/s_per)
#define EMF_SPEED_SWUP3_M            _IQ(1.00)        //Switch upper limit 3 of observation feedback speed(unit-rad/s_per)
#define EMF_SPEED_SWUP4_M            _IQ(1.40)        //Switch upper limit 4 of observation feedback speed(unit-rad/s_per)
#define EMF_SPEED_SWUP5_M            _IQ(1.40)        //Switch upper limit 5 of observation feedback speed(unit-rad/s_per)

#define EMF_SPEED_SINGLE_M           _IQ(0.4)         //Speed of switch BEMF single point sampling

#define EMF_MAX_SPEED_POS            _IQ(1.1)         //Forward direction maximun speed of EMF detected(Unit_per)
#define EMF_MAX_SPEED_NEG            _IQ(-1.1)        //Reverse direction maximun speed of EMF detected(Unit_per)
#define EMF_MIN_SPEED                _IQ(0.05)        //minimum speed of EMF detected(Unit_per)

#define EMF_CONTROLSWITCH_CNTTHRE    120              //Threshold of change phase to decide switch loop

#define EMF_HIGHSPEEDSWITCH_CNTTHRE  400

#define EMF_STALL_SPEEDVIBRA_CALFREQ    SPEED_LOOP_CAL_FREQ   //Calculation frequency of abnormal speed fluctuation to protect stall(unit-Hz)
#define EMF_STALL_SPEEDVIBRA_CALINTER   1                     //Time interval of abnormal speed fluctuation to protect stall(unit-s)
#define EMF_STALL_SPEEDVIBRA_CNTTHRESH  1000                  //Decision cnt threshhold of abnormal speed fluctuation to protect stall
#define EMF_STALL_SPEEDVIBRA_THRESHOLD  3                     //Speed fluctuation threshold of abnormal speed fluctuation to protect stall
#define UPPER_CAPTURE_TO_RATED          1.5                   //Minimum capture multiple

#define EMF_STALLCALFREQ_M       SPEED_LOOP_CAL_FREQ     //Calculation frequency of stall protection(unit-Hz)
#define EMF_STALL_TIME_M         3                       //Time of stall protection(unit-s)

#if defined SLOOP_CONTROL
#define EMF_STALLSPEEDBAND1_M    _IQ(0.20)               //speed error bandwidth 1 of stall protection(unit-PU,0-1)
#define EMF_STALLSPEEDBAND2_M    _IQ(0.20)               //speed error bandwidth 2 of stall protection(unit-PU,0-1)
#define EMF_STALLSPEEDBAND3_M    _IQ(0.30)               //speed error bandwidth 3 of stall protectionunit-PU,0-1)
#define EMF_STALLSPEEDBAND4_M    _IQ(0.30)               //speed error bandwidth 4 of stall protection(unit-PU,0-1)
#define EMF_STALLSPEEDBAND5_M    _IQ(0.30)               //speed error bandwidth 5 of stall protection(unit-PU,0-1)
#endif

#define EMF_STALLSPEED_M        _IQ(0.30)               //Speed of stall protection(unit-rad/s_per)
#define EMF_STALL_CURRTIME      3                       //Time of stall protection(unit-s)

#define EMF_OVERSPEED_CALFREQ   SPEED_LOOP_CAL_FREQ     //Calculation frequency of Over speed(unit-Hz)
#define EMF_OVERSPEED_TIMELIM   0.10                    //Time of Over speed unit-s)

/* Exported variables ----------------------------------------------------*/
extern EMF_OBSERVER_TYPE  EMF_Cal_M;
extern Observer_Status_Type EMF_Status_M;
extern _iq24 Speedbufffer1[];
extern _iq24 Speedbufffer2[];
/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/

void CHANGEPHASE_Timer2_Init(void);

void EMFObs_Parameter_Define_M(EMF_OBSERVER_TYPE *EMFCal,Observer_Status_Type *EMFStatus);

void EMFObs_Status_Init_M(EMF_OBSERVER_TYPE *EMFCal,Observer_Status_Type *EMFStatus);

void Captimer_Clear(void);

uint16_t Captimer_Cntget(void);

void EMF_Schange_Check(EMF_OBSERVER_TYPE *EMFCal,ADCStruc_Type *ADCStruc,Observer_Status_Type *EMFStatus);

void Schange_Delay_Set(uint16_t Delayset, uint16_t *Delaycounter);

Truth_Verify_Type Schange_Delay_Elapse(uint16_t Delaycounter);

void EMFObs_Speed_Cal(EMF_OBSERVER_TYPE *EMFCal,Observer_Status_Type*EMFStatus,Speedrampctr_Type *Speed_Ramp);

void Schange_Delay_Process(void);

void reset_tim2_time (uint16_t time);
#endif
