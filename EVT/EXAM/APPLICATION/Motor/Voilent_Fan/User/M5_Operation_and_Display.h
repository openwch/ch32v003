/********************************** (C) COPYRIGHT *******************************
 * File Name          : M5_Operation_and_Display.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/15
 * Description        : Motor_Operation_Process parameters define.
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
typedef struct
{
    Truth_Verify_Type On;
} LED_Type;

/* Exported constants -------------------------------------------------------*/
/*********************** Define speed operation ******************************/
#define STARTUP_DIRECTION        CW                     //Running direction
#define SPEED_ADJUST_STEP       _IQ(0.1)                //Speed step of raise or down(unit-PU)
#define MAX_ANGSPEED_POS_M      _IQ(1.0)                //Maximun speed of CW(unit-per)
#define MAX_ANGSPEED_NEG_M      _IQ(-1.0)               //Maximun speed of CCW(unit-per)
#define MIN_ANGSPEED_POS_M      _IQ(0.025)              //Minimun speed of CW(unit-per)
#define MIN_ANGSPEED_NEG_M      _IQ(-0.025)             //Minimun speed of CCW(unit-per)
#define SPEED_RAMP_FREQ_M        SPEED_LOOP_CAL_FREQ    //Operation frequency(unit-Hz)
#define ACCE_TIME_M              0.5                    //Acceleration time(unit-s)
#define DECE_TIME_M              0.5                    //Deceleration time(unit-s)

#define SPEEDPOINT1             _IQ(1.0000)             //Speed 1(unit-per)
#define SPEEDPOINT2             _IQ(1.3000)             //Speed 2(unit-per)
#define SPEEDPOINT3             _IQ(1.4000)             //Speed 3(unit-per)
#define SPEEDSTOP               _IQ(0.4000)             //Stop speed(unit-per)
#define SPEEDRANK               3                       //Numbers of gears
#define SPEEDPOINT_DEFAULT      1                       //Gears of power-on

#define OPERATION_CAL_FREQ      SPEED_LOOP_CAL_FREQ     //Operate frequency(unit-Hz)
#define STOP_WAITING_TIME       0.5                     //Stop wait time(unit-s)
#define START_BUTTON_TIME       0.9                     //Start button time(unit-s)
#define SPEED_BUTTON_TIMEDOWN   0.1                     //Minimun time of button adjust speed(unit-s)
#define FAULT_HOLDING_TIME      0.5                     //Fault hold time(unit-s)
#define RUN_LIMIT_TIME          600                     //run time limit(unit-s)

#define Get_Key_Status          GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2)//Button detect

/* Exported variables -------------------------------------------------------*/
extern Speedrampctr_Type        SpeedRamp_M;
extern Operation_Type           Operation_M;

/* Exported macro -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------ */
void Motor_Operation_Process(void);

void Operation_Parameter_Define(Operation_Type *Operastruc);

void Operation_Status_Init(Operation_Type *Operastruc);

void Speedrampctr_Parameter_Define(Speedrampctr_Type *Speed_Ramp);

void SpeedrampInit(Speedrampctr_Type *Speed_Ramp);

void Speed_Setbutton(Speedrampctr_Type *Speed_Ramp);

void KEY_AND_LEDIO_Init(void);

void Key_Check(void);

void Runtimecheck(void);

void Time_Wait(uint16_t);

void Time_Delay_Set(uint16_t Delayset, uint16_t *Delaycounter);

Truth_Verify_Type Time_Delay_Elapse(uint16_t Delaycounter);

void Time_Delay_Process(void);

#endif
