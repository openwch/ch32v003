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
 *Timer synchronization mode:
 *TIM1_CH1(PD2)
 *This example demonstrates 4 timer synchronization modes.
 *
 */

#include "debug.h"

#define TimSynchroMode1    1
#define TimSynchroMode2    2
#define TimSynchroMode3    3
#define TimSynchroMode4    4

#define TimSynchroMode     TimSynchroMode1

void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM1_UP_IRQHandler(void)
{
        if( TIM_GetITStatus( TIM1, TIM_IT_Update ) != RESET )
        {
            printf( "TIM1\r\n" );
        }
        TIM_ClearITPendingBit( TIM1, TIM_IT_Update );
}

void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_IRQHandler(void)
{
        if( TIM_GetITStatus( TIM2, TIM_IT_Update ) != RESET )
        {
            printf( "TIM2\r\n" );
        }
        TIM_ClearITPendingBit( TIM2, TIM_IT_Update );
}

/*********************************************************************
 * @fn      TIM_TimSynchroMode1_Init
 *
 * @brief   Using TIM2 as prescaler for TIM1.
 *
 * @return  none
 */
void TIM_TimSynchroMode1_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure={0};
    RCC_PB2PeriphClockCmd(RCC_PB2Periph_TIM1, ENABLE);
    RCC_PB1PeriphClockCmd(RCC_PB1Periph_TIM2, ENABLE);

    TIM_CounterModeConfig(TIM1, TIM_CounterMode_Up);
    TIM_CounterModeConfig(TIM2, TIM_CounterMode_Up);
    TIM_SetAutoreload(TIM1, 0xA6);
    TIM_SetAutoreload(TIM2, 0x2);
    TIM_PrescalerConfig(TIM1, 48000 - 1, TIM_PSCReloadMode_Immediate);
    TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);
    TIM_ITRxExternalClockConfig(TIM2, TIM_TS_ITR0);
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_External1);

    TIM_ClearITPendingBit( TIM1, TIM_IT_Update );
    TIM_ClearITPendingBit( TIM2, TIM_IT_Update);

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig( TIM1, TIM_IT_Update , ENABLE );
    TIM_ITConfig( TIM2, TIM_IT_Update , ENABLE );

    TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

/*********************************************************************
 * @fn      TIM_TimSynchroMode2_Init
 *
 * @brief   Using TIM2 to use TIM1.
 *
 * @return  none
 */
void TIM_TimSynchroMode2_Init(void)
{
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_TIM1, ENABLE);
    RCC_PB1PeriphClockCmd(RCC_PB1Periph_TIM2, ENABLE);

    TIM_CounterModeConfig(TIM1, TIM_CounterMode_Up);
    TIM_CounterModeConfig(TIM2, TIM_CounterMode_Up);
    TIM_SetAutoreload(TIM1, 0x3E8);
    TIM_PrescalerConfig(TIM1, 48000 - 1, TIM_PSCReloadMode_Immediate);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0x64;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_OC1Ref);
    TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0);
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Gated);

    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
}

/*********************************************************************
 * @fn      TIM_TimSynchroMode3_Init
 *
 * @brief   Using TIM2 to start TIM1.
 *
 * @return  none
 */
void TIM_TimSynchroMode3_Init(void)
{
    RCC_PB2PeriphClockCmd(RCC_PB2Periph_TIM1, ENABLE);
    RCC_PB1PeriphClockCmd(RCC_PB1Periph_TIM2, ENABLE);

    TIM_CounterModeConfig(TIM1, TIM_CounterMode_Up);
    TIM_CounterModeConfig(TIM2, TIM_CounterMode_Up);
    TIM_SetAutoreload(TIM1, 0xFFFF);
    TIM_PrescalerConfig(TIM1, 48000 - 1, TIM_PSCReloadMode_Immediate);
    TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);
    TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0);
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Trigger);
    TIM_Cmd(TIM1, ENABLE);
}

/*********************************************************************
 * @fn      TIM_TimSynchroMode4_Init
 *
 * @brief   Starting TIM1 and TIM2 synchronously in response to an external trigger.
 *
 * @return  none
 */
void TIM_TimSynchroMode4_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    TIM_ICInitTypeDef TIM_ICInitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOD | RCC_PB2Periph_TIM1, ENABLE);
    RCC_PB1PeriphClockCmd(RCC_PB1Periph_TIM2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOD, GPIO_Pin_2);

    TIM_CounterModeConfig(TIM1, TIM_CounterMode_Up);
    TIM_CounterModeConfig(TIM2, TIM_CounterMode_Up);
    TIM_SetAutoreload(TIM1, 0xFFF);
    TIM_PrescalerConfig(TIM1, 48000 - 1, TIM_PSCReloadMode_Immediate);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x00;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInit(TIM1, &TIM_ICInitStructure);

    TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Trigger);
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);
    TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Enable);
    TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0);
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Trigger);
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
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    /* Timer synchronization Mode Selection */

#if(TimSynchroMode==TimSynchroMode1)
    TIM_TimSynchroMode1_Init();
#elif(TimSynchroMode==TimSynchroMode2)
    TIM_TimSynchroMode2_Init();
#elif(TimSynchroMode==TimSynchroMode3)
    TIM_TimSynchroMode3_Init();
#elif(TimSynchroMode==TimSynchroMode4)
    TIM_TimSynchroMode4_Init();
#endif
    while(1)
    {
        #if((TimSynchroMode==TimSynchroMode2)|(TimSynchroMode==TimSynchroMode3)|(TimSynchroMode==TimSynchroMode4))
        printf("TIM1 cnt:%d\r\n", TIM1->CNT);
        printf("TIM2 cnt:%d\r\n", TIM2->CNT);
        #endif
    }
}
