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
 *Output comparison mode routine:
 *TIM1_CH1(PD2)
 *This example demonstrates the output waveform of the TIM_CH1(PD2) pin in 4 output comparison modes.
 *Output compare modes include
 *OutCompare_Timing\OutCompare_Active\OutCompare_Inactive\OutCompare_Toggle.
 *
 */

#include "debug.h"

/* Output Compare Mode Definition */
#define OutCompare_Timing   0
#define OutCompare_Active   1
#define OutCompare_Inactive 2
#define OutCompare_Toggle   3

/* Output Compare Mode Selection */
//#define OutCompare_MODE OutCompare_Timing
//#define OutCompare_MODE OutCompare_Active
//#define OutCompare_MODE OutCompare_Inactive
#define OutCompare_MODE OutCompare_Toggle

/*********************************************************************
 * @fn      TIM1_OutCompare_Init
 *
 * @brief   Initializes TIM1 output compare.
 *
 * @param   arr - the period value.
 *          psc - the prescaler value.
 *          ccp - the pulse value.
 *
 * @return  none
 */
void TIM1_OutCompare_Init(u16 arr, u16 psc, u16 ccp)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    TIM_OCInitTypeDef TIM_OCInitStructure={0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

    RCC_PB2PeriphClockCmd( RCC_PB2Periph_GPIOD | RCC_PB2Periph_TIM1, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init( GPIOD, &GPIO_InitStructure );

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);

#if (OutCompare_MODE == OutCompare_Timing)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;

#elif (OutCompare_MODE == OutCompare_Active)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Active;

#elif (OutCompare_MODE == OutCompare_Inactive)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Inactive;

#elif (OutCompare_MODE == OutCompare_Toggle)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;

#endif

    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = ccp;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init( TIM1, &TIM_OCInitStructure );

    TIM_CtrlPWMOutputs(TIM1, ENABLE );
    TIM_OC1PreloadConfig( TIM1, TIM_OCPreload_Disable );
    TIM_ARRPreloadConfig( TIM1, ENABLE );
    TIM_Cmd( TIM1, ENABLE );
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

    TIM1_OutCompare_Init( 100, 48000-1, 50 );

    while(1);
}
