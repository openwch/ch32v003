/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/20
 * Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 *PWM Phase shift routine:
 *MODE0:TIM1_CH1(PD2) TIM1_CH4(PC4)
 *MODE1:TIM1_CH1(PD2) TIM2_CH1(PD4)
 *There is a phase difference between the PWMs of the two channels.
 *
 */

#include "debug.h"

#define  MODE0  0
#define  MODE1  1
#define  MODE  MODE0

/*********************************************************************
 * @fn      TIM1_Phase_shift_Init1
 *
 * @brief   Initializes Phase shift .
 *
 * @return  none
 */
void TIM1_Phase_shift_Init1( void )
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    TIM_OCInitTypeDef TIM_OCInitStructure={0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOC | RCC_APB2Periph_TIM1, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOD, &GPIO_InitStructure );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    TIM_TimeBaseInitStructure.TIM_Period = 100-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 4800-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC1Init( TIM1, &TIM_OCInitStructure );
    TIM_OCInitStructure.TIM_Pulse = 50;//Phase shift 90
    TIM_OC4Init( TIM1, &TIM_OCInitStructure );

    TIM_CtrlPWMOutputs(TIM1, ENABLE );
    TIM_OC1PreloadConfig( TIM1, TIM_OCPreload_Disable );
    TIM_OC4PreloadConfig( TIM1, TIM_OCPreload_Disable );
    TIM_ARRPreloadConfig( TIM1, ENABLE );
    TIM_Cmd( TIM1, ENABLE );
}


/*********************************************************************
 * @fn      TIM1_Phase_shift_Init2
 *
 * @brief   Initializes Phase shift .
 *
 * @return  none
 */
void TIM1_Phase_shift_Init2( void )
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    TIM_OCInitTypeDef TIM_OCInitStructure={0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE );
    RCC_APB1PeriphClockCmd(  RCC_APB1Periph_TIM2, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOD, &GPIO_InitStructure );

    TIM_TimeBaseInitStructure.TIM_Period = 100-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 4800-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);
    TIM_TimeBaseInit( TIM2, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 25;//Phase shift 90
    TIM_OC1Init( TIM1, &TIM_OCInitStructure );
    TIM_OCInitStructure.TIM_Pulse = 25;
    TIM_OC1Init( TIM2, &TIM_OCInitStructure );

    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);
    TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_OC1Ref);

    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Trigger);
    TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0);

    TIM_CtrlPWMOutputs(TIM1, ENABLE );

    TIM_OC1PreloadConfig( TIM1, TIM_OCPreload_Disable );
    TIM_OC1PreloadConfig( TIM2, TIM_OCPreload_Disable );
    TIM_ARRPreloadConfig( TIM1, ENABLE );
    TIM_ARRPreloadConfig( TIM2, ENABLE );
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
#if(MODE==MODE0)
    TIM1_Phase_shift_Init1();
#else
    TIM1_Phase_shift_Init2();
#endif
    while(1)
    {}
}
