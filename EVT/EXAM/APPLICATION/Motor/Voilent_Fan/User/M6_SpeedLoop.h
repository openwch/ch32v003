/********************************** (C) COPYRIGHT *******************************
 * File Name          : M6_Speedloop.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/15
 * Description        : Speedloop operation process and change phase TIMER initialization parameter define.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __M6_SPEEDLOOP_H
#define __M6_SPEEDLOOP_H

/* Includes -----------------------------------------------------------------*/
/* Exported constants -------------------------------------------------------*/
#define TIMER_CLOCK_FREQ_SPEED      48000000uL      //Timer clock frequency of speed loop(unit-Hz)
#define TIMER_FREQ_SPEED_LOOP       2000            //Interrupt frequency of speed loop(unit-Hz)

#define TIMER_PERIDO_SYSTICK        TIMER_CLOCK_FREQ_SPEED / (uint32_t)(TIMER_FREQ_SPEED_LOOP*8)//Systick period

#define SPEED_LOOP_CAL_FREQ         2000

#define SPEED_LOOP_CAL_PERIOD       _IQ(1.0)/SPEED_LOOP_CAL_FREQ

#define SPEED_SAMPLE_PID_PERIOD     FREQ_PWM /SPEED_LOOP_CAL_FREQ

/* Exported variables -------------------------------------------------------*/
/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/
void Systick_Init_48M(void);

void Systick_Init_6M(void);

void SYS_TIM_Cmd(FunctionalState NewState);

#endif
