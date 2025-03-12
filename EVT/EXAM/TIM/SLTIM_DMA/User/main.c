/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.1
 * Date               : 2025/01/10
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

 /*
 *@Note
 *SLTimer DMA routines:
 *SLTIM
 *SLTIM trigger the number of TIM1->ATRLR move to dbuf[X] by DMA.
 *Only for ch32V006, ch32V007, ch32M007 chips.
 */ 

#include "debug.h"

//TIM3_DMA

u32 dbuf[3] = {0};
u32 dbuf1[3] = {0};
u32 dbuf2[3] = {0};

/*********************************************************************
 * @fn      TIM3_INIT
 *
 * @brief   TIM3 INIT.
 *
 * @return  none
 */
void TIM3_INIT(void)
{
   RCC_PB2PeriphClockCmd(RCC_PB2Periph_TIM1, ENABLE);
   RCC_PB1PeriphClockCmd(RCC_PB1Periph_TIM3, ENABLE);

   TIM_CounterModeConfig(TIM1, TIM_CounterMode_Down);
   TIM_CounterModeConfig(TIM3, TIM_CounterMode_Down);

   TIM_SetAutoreload(TIM1, 0x77);
   TIM_SetAutoreload(TIM3, 0xFF);
   TIM_SetCompare1(TIM3, 0xFD);

   TIM_PrescalerConfig(TIM1, 48000 - 1, TIM_PSCReloadMode_Immediate);

   TIM_DMACmd(TIM3, TIM_DMA_CC3, ENABLE);
   TIM_ARRPreloadConfig(TIM3, ENABLE);

   TIM_Cmd(TIM1, ENABLE);
}

/*********************************************************************
 * @fn      TIM3_DMA_Init
 *
 * @brief   TIM3 DMA Init.
 *
 * @return  none
 */
void TIM3_DMA_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure = {0};
    DMA_InitTypeDef DMA_InitStructure1 = {0};
    DMA_InitTypeDef DMA_InitStructure2 = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_HBPeriphClockCmd(RCC_HBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA1_Channel1);//DMA_CH1

   if((DBGMCU_GetCHIPID()&(~0x000000F0))==0x00700800)
    {
    DMA_DeInit(DMA1_Channel2);
    DMA_DeInit(DMA1_Channel3);
    }

    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(TIM1->ATRLR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32) dbuf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 1000;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    if((DBGMCU_GetCHIPID()&(~0x000000F0))==0x00700800)
    {
    DMA_InitStructure1.DMA_PeripheralBaseAddr = (u32)&(TIM1->ATRLR);
    DMA_InitStructure1.DMA_MemoryBaseAddr = (u32) dbuf1;
    DMA_InitStructure1.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure1.DMA_BufferSize = 1000;
    DMA_InitStructure1.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure1.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure1.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure1.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStructure1.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure1.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure1.DMA_M2M = DMA_M2M_Disable;

    DMA_InitStructure2.DMA_PeripheralBaseAddr = (u32)&(TIM1->ATRLR);
    DMA_InitStructure2.DMA_MemoryBaseAddr = (u32) dbuf2;
    DMA_InitStructure2.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure2.DMA_BufferSize = 1000;
    DMA_InitStructure2.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure2.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure2.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure2.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStructure2.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure2.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure2.DMA_M2M = DMA_M2M_Disable;
    }

    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    if((DBGMCU_GetCHIPID()&(~0x000000F0))==0x00700800)
    {
    DMA_Init(DMA1_Channel2, &DMA_InitStructure1);
    DMA_Init(DMA1_Channel3, &DMA_InitStructure2);
    }

    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
    NVIC_Init(&NVIC_InitStructure);

    if((DBGMCU_GetCHIPID()&(~0x000000F0))==0x00700800)
    {
    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
    NVIC_Init(&NVIC_InitStructure);
    }

    DMA_ClearFlag(DMA1_FLAG_TC1);

    if((DBGMCU_GetCHIPID()&(~0x000000F0))==0x00700800)
    {
    DMA_ClearFlag(DMA1_FLAG_TC2);
    DMA_ClearFlag(DMA1_FLAG_TC3);
    }


    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    if((DBGMCU_GetCHIPID()&(~0x000000F0))==0x00700800)
    {
    DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
    DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);
    }

    DMA_Cmd(DMA1_Channel1, ENABLE);
    if((DBGMCU_GetCHIPID()&(~0x000000F0))==0x00700800)
    {
    DMA_Cmd(DMA1_Channel2, ENABLE);
    DMA_Cmd(DMA1_Channel3, ENABLE);
    }

}

void DMA1_Channel1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
/*********************************************************************
 * @fn      DMA1_Channel1_IRQHandler
 *
 * @brief   This function handles DMA1 Channel1 exception.
 *
 * @return  none
 */
void DMA1_Channel1_IRQHandler(void)
{
    if(DMA_GetFlagStatus(DMA1_FLAG_TC1) == SET)
    {
       printf("Channel1\r\n");
       DMA_ClearFlag(DMA1_FLAG_TC1);
    }
}

void DMA1_Channel2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
/*********************************************************************
 * @fn      DMA1_Channel2_IRQHandler
 *
 * @brief   This function handles DMA1 Channel2 exception.
 *
 * @return  none
 */
void DMA1_Channel2_IRQHandler(void)
{
    if(DMA_GetFlagStatus(DMA1_FLAG_TC2) == SET)
    {
       printf("Channel2\r\n");
       DMA_ClearFlag(DMA1_FLAG_TC2);
    }
}

void DMA1_Channel3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
/*********************************************************************
 * @fn      DMA1_Channel3_IRQHandler
 *
 * @brief   This function handles DMA1 Channel3 exception.
 *
 * @return  none
 */
void DMA1_Channel3_IRQHandler(void)
{
    if(DMA_GetFlagStatus(DMA1_FLAG_TC3) == SET)
    {

       printf("Channel3\r\n");
       DMA_ClearFlag(DMA1_FLAG_TC3);
    }
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
    printf( "CFGR2:%08x\r\n", *(u32*)(0x40022030) );

    TIM3_INIT();
    TIM3_DMA_Init();
    TIM_DMACmd(TIM3, TIM_DMA_CC3, ENABLE);
    TIM_Cmd(TIM3, ENABLE);

    while(1)
    {
    }
}
