/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/01/01
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *Slave mode routine:
 *TIM1_CH1(PD2)
 *This example demonstrates that when the TIM_CH1(PD2) pin detects a rising edge,
 *TIM1 works differently in different slave modes.
 *RESET_MODE: TIM1->CNT reset and recount
 *GATED_MODE: PA8 pin input low level, TIM1->CNT counts normally, otherwise stop counting.
 *TRIGGER__MODE: TIM1->CNT continues counting.
 *
 */

#include "debug.h"

/* Slave Mode Definition */
#define RESET_MODE      0
#define GATED_MODE      1
#define TRIGGER__MODE   2

/*  Slave Mode Selection */
//#define SLAVE_MODE   RESET_MODE
//#define SLAVE_MODE   GATED_MODE
#define SLAVE_MODE   TRIGGER__MODE

/*********************************************************************
 * @fn      TIM1_Synchro_ExtTrigger_Init
 *
 * @brief   Initializes TIM1 external trigger synchronization.
 *
 * @param   arr - the period value.
 *          psc - the prescaler value.
 *
 * @return  none
 */
void TIM1_Synchro_ExtTrigger_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    TIM_ICInitTypeDef TIM_ICInitStructure={0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

    RCC_PB2PeriphClockCmd( RCC_PB2Periph_GPIOD | RCC_PB2Periph_TIM1, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init( GPIOD, &GPIO_InitStructure);
    GPIO_ResetBits( GPIOD, GPIO_Pin_2 );

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter =  0x00;
    TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x00;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInit( TIM1, &TIM_ICInitStructure );

    TIM_SelectInputTrigger( TIM1, TIM_TS_TI1FP1 );

#if (SLAVE_MODE == RESET_MODE)
    TIM_SelectSlaveMode( TIM1, TIM_SlaveMode_Reset );

#elif (SLAVE_MODE == GATED_MODE)
    TIM_SelectSlaveMode( TIM1, TIM_SlaveMode_Gated );

#elif (SLAVE_MODE == TRIGGER__MODE)
    TIM_SelectMasterSlaveMode( TIM1, TIM_SlaveMode_Trigger );

#endif

    TIM_SelectMasterSlaveMode( TIM1, TIM_MasterSlaveMode_Enable );

#if (SLAVE_MODE != TRIGGER__MODE)
    TIM_Cmd( TIM1, ENABLE );

#endif
}
/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    SystemCoreClockUpdate();
    Delay_Init();
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif
    printf("SystemClk:%d\r\n",SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    TIM1_Synchro_ExtTrigger_Init( 0xFFFF, 48000-1);

    while(1)
    {
        printf("cnt:%d\r\n", TIM1->CNT);
    }
}

