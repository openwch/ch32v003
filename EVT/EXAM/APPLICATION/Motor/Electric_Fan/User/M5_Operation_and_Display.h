/********************************** (C) COPYRIGHT *******************************
 * File Name          : M5_Operation_and_Display.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
 * Description        : Definition of relevant operation parameters such as buttons and displays
                        Definition of acceleration and deceleration parameters

*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __MOTOR_OPERATION_AND_DISPLAY_H
#define __MOTOR_OPERATION_AND_DISPLAY_H
/* Includes -----------------------------------------------------------------*/
/* Exported types -----------------------------------------------------------*/
typedef enum {PRESSUP = 0, PRESSDOWN = 1} PressStatus;
typedef struct
{
    GPIO_TypeDef *Group;
    uint16_t     Pin;
    uint16_t Effectivetime;
    uint16_t Effectivecnt;
    uint16_t Ineffectivetime;
    uint16_t Ineffectivecnt;
    Truth_Verify_Type Status;
    PressStatus         KeyPress;
} Keyrecover_Type;

typedef struct
{
    Keyrecover_Type RUN;
    Keyrecover_Type Speedadjust;
} Key_Manager_Type;
/* Exported constants -------------------------------------------------------*/
#define STARTUP_DIRECTION       1                       // Running direction (1: forward, -1: reverse)
#define MAX_ANGSPEED_POS_M      _IQ(1.0)                // Maximum forward speed (unit-per)
#define MAX_ANGSPEED_NEG_M      _IQ(-1.0)               // Maximum reverse speed (unit-per)
#define MIN_ANGSPEED_POS_M      _IQ(0.025)              // Minimum forward speed (unit-per)
#define MIN_ANGSPEED_NEG_M      _IQ(-0.025)             // Minimum reverse speed (unit-per)
#define SPEED_RAMP_FREQ_M       SPEED_LOOP_CAL_FREQ     // Acceleration and deceleration processing frequency (unit-Hz)
#define ACCE_TIME_M             5                       // Acceleration time (unit-s)
#define DECE_TIME_M             5                       // Deceleration time (unit-s)

#define SPEEDPOINT1             _IQ(0.35)
#define SPEEDPOINT2             _IQ(0.40)
#define SPEEDPOINT3             _IQ(0.55)
#define SPEEDPOINT4             _IQ(0.7)
#define SPEEDPOINT_DEFAULT      1
#define SPEEDPOINT_RANK         4

// Brake
#define FAST_DECE_SPEED         _IQ(0.25)               // Fast deceleration speed target (unit-per)
#define BREAKSPEED              _IQ(0.20)               // Speed trigger stator short-circuit brake (unit-per)
#define BREAKTIME               1.0                     // stator short-circuit brake last time(unit-s)
#define START_FINISH_SPEED      (FAST_DECE_SPEED +_IQ(0.05))//Speed threshold of start finish mark

#define KEY_CHECK_FREQ           SPEED_LOOP_CAL_FREQ    // Frequency of key detection calculation
#define SRKEY_EFFECTIVE_TIME     0.05
#define SRKEY_INEFFECTIVE_TIME   0.05

#define OPERATION_CAL_FREQ      SPEED_LOOP_CAL_FREQ     // Operation execution frequency(unit-Hz)
#define STOP_WAITING_TIME       0.1                     // Wait time before restart (unit-s)
#define FAULT_HOLDING_TIME      0.5                     // Fault status last time before reset(unit-s)

// Power control parameters
#define POWERVERY_FREQ              SPEED_LOOP_CAL_FREQ // Power-on check frequency (unit-Hz)
#define POWERON_VOLT                _IQ(0.70)           // Power-on voltage threshold (unit-Vper)
#define POWERON_TIME                0.1                 // Power-on verification effective time (unit-s)
#define POWEROFF_VOLT               _IQ(0.65)           // Power-off voltage threshold(unit-Vper)
#define POWERVERY_DCVOLTAVERSIZE    4                   // Power-on check voltage average calculation length

/* Exported variables -------------------------------------------------------*/
extern Speedrampctr_Type    SpeedRamp_M;
extern Operation_Type       Operation_M;
extern Powercontrol_Type    Powerctr_M;
extern Key_Manager_Type     Key_M;
/* Exported macro -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------ */
void Motor_Operation_Process(void);

void Speedrampctr_Parameter_Define(Speedrampctr_Type *Speed_Ramp);

void Speedrampctr_Initial(Speedrampctr_Type *Speed_Ramp);

void Operation_Parameter_Define(Operation_Type *Operation);

void Operation_Status_Init(Operation_Type *Operation);

void Speed_Set(Speedrampctr_Type *Speed_Ramp);

void KeyandLED_IOInit(void);

void KeyandLED_Parameter_Define(Key_Manager_Type *KeyM);

void KeyandLED_Status_Init(Key_Manager_Type *KeyM);

void Skeycheck_Recover(Keyrecover_Type *Keyrec);

void KeyCheck_Process(Key_Manager_Type *KeyM, Speedrampctr_Type *Speed_Ramp);

void Poweroperation_Initial(Powercontrol_Type *PowerOper);

void Poweroperation_Process(Speedrampctr_Type *SpeedRamp,ADCStruc_Type *ADCStruc,Powercontrol_Type *PowerOper,\
                            Runningstatus_Type *Runningstatus,_iq24 Speedact);

void Break_Verify(Speedrampctr_Type *SpeedRamp, Runningstatus_Type *Runningstatus);

void Time_Delay_Process(void);

void Time_Wait(uint16_t);

void Time_Delay_Set(uint16_t Delayset, uint16_t *Delaycounter);

Truth_Verify_Type Time_Delay_Elapse(uint16_t Delaycounter);
#endif
