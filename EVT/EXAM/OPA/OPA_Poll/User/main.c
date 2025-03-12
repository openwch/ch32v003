/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.1
 * Date               : 2025/01/08
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *OPA positive input polling function:
 *
 *OPA1_CHN0--PA1
 *OPA1_OUT--PD4
 *P0-PA2 P1-PD7  P2-PD3
 *The number of positive terminals polling is 3. The polling time is 100ms, and the corresponding 
 *channel can be queried when the output is high voltage. OPA can be selected as the brake signal,
 *and the brake is effective when the output is high.
 */

#include "debug.h"
/* OPA BKIN Definition*/
#define OPA_BKIN_EABLE   0
#define OPA_BKIN_DISABLE     1

/* OPA BKIN Selection*/
#define OPA_BKIN   OPA_BKIN_EABLE
//#define OPA_BKIN   OPA_BKIN_DISABLE

/*********************************************************************
 * @fn      OPA1_Init
 *
 * @brief   Initializes OPA1.
 *
 * @return  none
 */
void OPA1_Init( void )
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    OPA_InitTypeDef  OPA_InitStructure = {0};
    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOA|RCC_PB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOA, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOD, &GPIO_InitStructure );

    OPA_InitStructure.Mode = OUT_IO_OUT0;
    OPA_InitStructure.POLL_NUM = CHP_POLL_NUM_3;
    OPA_InitStructure.PSEL_POLL = CHP_OPA_POLL_ON;
    OPA_InitStructure.POLL_CH1 = OPA_POLL_CH1_PA2;
    OPA_InitStructure.POLL_CH2 = OPA_POLL_CH2_PD7;
    OPA_InitStructure.POLL_CH3 = OPA_POLL_CH3_PD3;
    OPA_InitStructure.POLL_AT = OPA_POLL_AUTO_ON;
    OPA_InitStructure.NSEL = CHN0;
    OPA_InitStructure.OPA_HS = HS_ON;
    OPA_InitStructure.POLL_SEL = OPA_POLL_SEL_TIM2_CH4;
    OPA_InitStructure.OPA_HS = HS_ON;
#if (OPA_BKIN == OPA_BKIN_EABLE)
    OPA_CMP_TIM1_BKINConfig(TIM1_Brake_Source_OPA);
#endif
    OPA_Init(&OPA_InitStructure);
    OPA_Cmd( ENABLE );
}

/*********************************************************************
 * @fn      ADC_Function_Init
 *
 * @brief   Initializes ADC collection.
 *
 * @return  none
 */
void ADC_Function_Init(void)
{
    ADC_InitTypeDef  ADC_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    RCC_PB2PeriphClockCmd(RCC_PB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig( RCC_PCLK2_Div1 );

    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigInjecConv_Ext_PD1_PA2_OPA;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE);
    ADC_InjectedExTrigConvConfig(ADC1, ADC_InjectedExTrigConv_OPA);
    ADC_InjectedSequencerLengthConfig(ADC1, 3);
    ADC_InjectedChannelConfig(ADC1, ADC_Channel_OPA, 1, ADC_SampleTime_CyclesMode0);
    ADC_InjectedChannelConfig(ADC1, ADC_Channel_OPA, 2, ADC_SampleTime_CyclesMode0);
    ADC_InjectedChannelConfig(ADC1, ADC_Channel_OPA, 3, ADC_SampleTime_CyclesMode0);

    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);

    ADC_Cmd(ADC1, ENABLE);
}

/*********************************************************************
 * @fn      TIM2_PWM_In
 *
 * @brief   Initializes the trigger input source for Timer 2 to an update event.
 *
 * @param   arr - the period value.
 *          psc - the prescaler value.
 *          ccp - the pulse value.
 *
 * @return  none
 */
void TIM2_PWM_In(u16 arr, u16 psc, u16 ccp)
{

    TIM_OCInitTypeDef       TIM_OCInitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

    RCC_PB1PeriphClockCmd( RCC_PB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = ccp;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM2, ENABLE);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
    TIM_Cmd(TIM2, ENABLE);
}

/*********************************************************************
 * @fn      Input_Capture_Init
 *
 * @brief   Initializes TIM1 input capture.
 *
 * @param   arr - the period value.
 *          psc - the prescaler value.
 *          ccp - the pulse value.
 *
 * @return  none
 */
void TIM1_PWM_OUT( u16 arr, u16 psc, u16 ccp )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure;

    RCC_PB2PeriphClockCmd( RCC_PB2Periph_GPIOD | RCC_PB2Periph_TIM1, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init( GPIOD, &GPIO_InitStructure );

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = ccp;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
    TIM_OC1Init( TIM1, &TIM_OCInitStructure );

    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
    TIM_BDTRInitStructure.TIM_DeadTime = 0x01;
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
    TIM_BDTRConfig( TIM1, &TIM_BDTRInitStructure );

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
    Delay_Init();
    SystemCoreClockUpdate();
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init( 115200 );
#endif
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    OPA_Unlock();
    OPA_CMP_Unlock();
    OPA1_Init();
    TIM2_PWM_In(4800-1,1000,2400);
#if (OPA_BKIN == OPA_BKIN_EABLE)
    TIM1_PWM_OUT(4800-1,1000,2400);
#endif
    ADC_Function_Init();
    while(1);
}

void ADC1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      ADC1_IRQHandler
 *
 * @brief   ADC1 Interrupt Service Function.
 *
 * @return  none
 */
void ADC1_IRQHandler(void)
{
    u16 ADC_val1,ADC_val2,ADC_val3;
    if(OPA_GetFlagStatus( OPA_FLAG_OUT_POLL_CH_1))
    {
        printf("JADC-%04d \r\n", ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1));
    }
    OPA_ClearFlag( OPA_FLAG_OUT_POLL_CH_1);
    if(OPA_GetFlagStatus( OPA_FLAG_OUT_POLL_CH_2))
    {
        printf("JADC-%04d \r\n", ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_2));
    }
    OPA_ClearFlag( OPA_FLAG_OUT_POLL_CH_2) ;
    if(OPA_GetFlagStatus( OPA_FLAG_OUT_POLL_CH_3))
    {
        printf("JADC-%04d  \r\n", ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_3));
    }
    OPA_ClearFlag( OPA_FLAG_OUT_POLL_CH_3);
    if(ADC_GetITStatus(ADC1, ADC_IT_JEOC))
    {
        ADC_val1=ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1);
        ADC_val2=ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_2);
        ADC_val3=ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_3);
        printf("JADC-%04d   -%04d  -%04d \r\n", ADC_val1,ADC_val2,ADC_val3);
    }

    ADC_ClearITPendingBit(ADC1, ADC_IT_JEOC);
}


