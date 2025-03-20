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
 *Comparator routines:
 *CMP1_P0--PC5
 *CMP1_N0--PC2
 *CMP1_OUT--PC0
 *In this example, PC5 and PC2 are postive and negative input, PC0 is the output of the comparator.
 *When the voltage of PC5 is greater than the voltage of PC2, the output of PC0 is high level,
 *otherwise the output of PC0 is low level.
 */

#include "debug.h"
#define OUT_IO      0
#define OUT_TIM     1

//#define CMP_Mode_Out   OUT_IO
#define CMP_Mode_Out   OUT_TIM

/* CMP Reset Enable Definition */
#define CMP_RST_ENABLE   0
#define CMP_RST_DISABLE  1

/* CMP Reset Enable Selection */
#define CMP_RST   CMP_RST_DISABLE
//#define CMP_RST   CMP_RST_ENABLE
/*********************************************************************
 * @fn      CMP_Init
 *
 * @brief   Initializes CMP.
 *
 * @return  none
 */
void CMP_Init( void )
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    CMP1_InitTypeDef  CMP_InitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

#if  (CMP_Mode_Out == OUT_IO)
    CMP_InitStructure.CMP_Out_Mode = OUT_IO0;
#else
     CMP_InitStructure.CMP_Out_Mode = OUT_IO_TIM1_CH4;
#endif

    CMP_InitStructure.NSEL = CMP_CHN0;
    CMP_InitStructure.PSEL = CMP_CHP0;
    CMP_InitStructure.HYEN = CMP_HYEN_ON;
    OPA_CMP1_Init(&CMP_InitStructure);

#if  (CMP_RST == WDT_RST_ENABLE)
    OPA_CMP_SystemReset_Cmd(CMP1,ENABLE);
#else
    OPA_CMP_SystemReset_Cmd(CMP1,DISABLE);
#endif


    OPA_CMP_FILT_Cmd(ENABLE);
    OPA_CMP_FILT_LEN_Config(CMP_FILT_Len_0);
    OPA_CMP_Cmd(CMP1,ENABLE);
}

/*********************************************************************
 * @fn      CMP_OUT
 *
 * @brief   CMP out program.
 *
 * @return  none
 */
void CMP_OUT( void )
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_PB2PeriphClockCmd( RCC_PB2Periph_GPIOC, ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );
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
void Input_Capture_Init(u16 arr, u16 psc)
{

    TIM_ICInitTypeDef       TIM_ICInitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};
    NVIC_InitTypeDef        NVIC_InitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_TIM1, ENABLE);

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x00;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;

    TIM_ICInit(TIM1, &TIM_ICInitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM1, TIM_IT_CC4, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
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
    USART_Printf_Init( 115200 );
#endif
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
#if  (CMP_RST == WDT_RST_ENABLE)
    printf( "RCC->RSTSCKR:%08x\r\n", RCC->RSTSCKR );
    RCC->RSTSCKR |= (1<<24);
    RCC->RSTSCKR &= ~(1<<24);
#endif

    OPA_CMP_Unlock();
    CMP_Init();
#if  (CMP_Mode_Out == OUT_IO)
       CMP_OUT();
#else
       Input_Capture_Init(0XFFFF,48);
#endif

    while(1);

}
#if  (CMP_Mode_Out == OUT_TIM)

void TIM1_CC_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      TIM1_CC_IRQHandler
 *
 * @brief   This function handles TIM1 Capture Compare Interrupt exception.
 *
 * @return  none
 */
void TIM1_CC_IRQHandler(void)
{
    if( TIM_GetITStatus( TIM1, TIM_IT_CC4 ) != RESET )
    {
        printf( "CH1_Val:%d\r\n", TIM_GetCapture4( TIM1 ) );
        TIM_SetCounter( TIM1, 0 );
    }

    TIM_ClearITPendingBit( TIM1, TIM_IT_CC4 );
}
#endif






