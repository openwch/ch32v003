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
 *CMP positive input polling function:
 *
 *CMP_CHN0--PC2
 *CMP_OUT--PC0
 *P0-PC5 P1-PB3  P2-PD2
 *The number of positive terminals polling is 3, and the polling interval is 81uS.
 *When doing comparison follow-up, input different voltage values , the three positive
 *directions, and the output terminal voltage will output the positive terminal voltage according to the polling interval.
 *according to the polling interval.
 */

#include "debug.h"
#define POLL_CH      0
#define POLL_END     1

//#define CMP_IRQn   POLL_CH
#define CMP_IRQn   POLL_END
/*********************************************************************
 * @fn      CMP_Init
 *
 * @brief   Initializes CMP1.
 *
 * @return  none
 */
void CMP_Init( void )
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    CMP1_InitTypeDef  CMP_InitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOB|RCC_PB2Periph_GPIOC|RCC_PB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOB, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOD, &GPIO_InitStructure );

    NVIC_InitStructure.NVIC_IRQChannel = OPCM_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    CMP_InitStructure.NSEL = CMP_CHN0;
    CMP_InitStructure.POLL_NUM = CMP_POLL_NUM_3;
    CMP_InitStructure.PSEL_POLL = CHP_CMP1_POLL_ON;
    CMP_InitStructure.CMP_POLL_Interval = 0x50;
    CMP_InitStructure.HYEN = CMP_HYEN_ON;
#if  (CMP_IRQn == POLL_CH)
    CMP_InitStructure.OUT_IE= CMP_OUT_IE_ON;
#else
    CMP_InitStructure.CNT_IE= CMP_CNT_IE_ON;
#endif

    CMP_InitStructure.POLL_CH1 = CMP_POLL_CH1_PC5;
    CMP_InitStructure.POLL_CH2 = CMP_POLL_CH2_PB3;
    CMP_InitStructure.POLL_CH3 = CMP_POLL_CH3_PD2;
    CMP_InitStructure.CMP_Out_Mode = OUT_IO0;

    OPA_CMP1_Init(&CMP_InitStructure);

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


void OPCM_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      OPCM_IRQHandler
 *
 * @brief   This function handles OPA_CMP exception.
 *
 * @return  none
 */
void OPCM_IRQHandler(void)
{
#if  (CMP_IRQn == POLL_CH)
    if(OPA_CMP_GetFlagStatus(CMP_FLAG_OUT_POLL_CH_1)!=RESET)
    {
        printf("POLL_CH1\r\n");
        OPA_CMP_ClearFlag(CMP_FLAG_OUT_POLL_CH_1);     /* Clear Flag */
    }
    if(OPA_CMP_GetFlagStatus(CMP_FLAG_OUT_POLL_CH_2)!=RESET)
    {
        printf("POLL_CH2\r\n");
        OPA_CMP_ClearFlag(CMP_FLAG_OUT_POLL_CH_2);     /* Clear Flag */
    }
    if(OPA_CMP_GetFlagStatus(CMP_FLAG_OUT_POLL_CH_3)!=RESET)
    {
        printf("POLL_CH3\r\n");
        OPA_CMP_ClearFlag(CMP_FLAG_OUT_POLL_CH_3);     /* Clear Flag */
    }
#else
    if(OPA_CMP_GetFlagStatus(CMP_FLAG_POLL_END)!=RESET)
    {
        printf("RUN EXTI\r\n");
        OPA_CMP_ClearFlag(CMP_FLAG_POLL_END);     /* Clear Flag */
    }
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
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init( 115200 );
#endif
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    OPA_CMP_Unlock();
    CMP_Init();
    CMP_OUT();
    while(1);
}

