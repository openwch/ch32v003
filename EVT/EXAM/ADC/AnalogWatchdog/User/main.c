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
 *Analog watchdog routine:
 *ADC channel 2 (PC4), detect that the ADC conversion data on the rule group channel is
 *outside 500 - 900 and trigger the simulation Watchdog interrupt.
 *
*/

#include "debug.h"

/* ADC WWDG Mode Definition*/
#define NoSCAN_MODE_WDT   0
#define SCAN_MODE_WDT     1

/* ADC WWDG Mode Selection*/
#define ADC_MODE_WDT   NoSCAN_MODE_WDT
//#define ADC_MODE_WDT   SCAN_MODE_WDT

/* WWDG Reset Enable Definition */
#define WDT_RST_ENABLE   0
#define WDT_RST_DISABLE  1

/* WWDG Reset Enable Selection */
#define WDT_RST   WDT_RST_DISABLE
//#define WDT_RST   WDT_RST_ENABLE

u16 TxBuf[3];
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
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOC|RCC_PB2Periph_GPIOD, ENABLE);
    RCC_PB2PeriphClockCmd(RCC_PB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
     GPIO_Init(GPIOC, &GPIO_InitStructure);
#if  (ADC_MODE_WDT == SCAN_MODE_WDT)
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 |GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
     GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif


    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
#if  (ADC_MODE_WDT == NoSCAN_MODE_WDT)
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
#else
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_NbrOfChannel = 3;
#endif
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_Init(ADC1, &ADC_InitStructure);

#if  (ADC_MODE_WDT == NoSCAN_MODE_WDT)
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_CyclesMode7);
    ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_2);
    ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);

#else
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_CyclesMode7);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 2, ADC_SampleTime_CyclesMode7);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 3, ADC_SampleTime_CyclesMode7);
    ADC_AnalogWatchdogScanCmd(ADC1, ENABLE);

    ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_AllRegEnable);
#endif

    /* Higher Threshold:900, Lower Threshold:500 */
    ADC_AnalogWatchdogThresholdsConfig(ADC1, 900, 500);
#if (WDT_RST == WDT_RST_ENABLE)
    ADC_AnalogWatchdogResetCmd(ADC1, ADC_AnalogWatchdog_0_RST_EN, ENABLE);
#else
    ADC_AnalogWatchdogResetCmd(ADC1, ADC_AnalogWatchdog_1_RST_EN, DISABLE);
#endif
#if  (ADC_MODE_WDT == SCAN_MODE_WDT)

    /* Higher Threshold:3500, Lower Threshold:2000 */
    ADC_AnalogWatchdog1ThresholdsConfig(ADC1, 3500, 2000);

#if (WDT_RST == WDT_RST_ENABLE)
    ADC_AnalogWatchdogResetCmd(ADC1, ADC_AnalogWatchdog_1_RST_EN, ENABLE);
#else
    ADC_AnalogWatchdogResetCmd(ADC1, ADC_AnalogWatchdog_1_RST_EN, DISABLE);
#endif

    /* Higher Threshold:3500, Lower Threshold:2000 */
    ADC_AnalogWatchdog2ThresholdsConfig(ADC1, 3500, 2000);

#if (WDT_RST == WDT_RST_ENABLE)
    ADC_AnalogWatchdogResetCmd(ADC1, ADC_AnalogWatchdog_2_RST_EN, ENABLE);
#else
    ADC_AnalogWatchdogResetCmd(ADC1, ADC_AnalogWatchdog_2_RST_EN, DISABLE);
#endif
#endif
    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);

    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    ADC_BufferCmd(ADC1, DISABLE);    //disable buffer
}

/*********************************************************************
 * @fn      Get_ADC_Val
 *
 * @brief   Returns ADCx conversion result data.
 *
 * @param   ch - ADC channel.
 *            ADC_Channel_0 - ADC Channel0 selected.
 *            ADC_Channel_1 - ADC Channel1 selected.
 *            ADC_Channel_2 - ADC Channel2 selected.
 *            ADC_Channel_3 - ADC Channel3 selected.
 *            ADC_Channel_4 - ADC Channel4 selected.
 *            ADC_Channel_5 - ADC Channel5 selected.
 *            ADC_Channel_6 - ADC Channel6 selected.
 *            ADC_Channel_7 - ADC Channel7 selected.
 *            ADC_Channel_8 - ADC Channel8 selected.
 *            ADC_Channel_9 - ADC Channel9 selected.
 *
 * @return  none
 */
u16 Get_ADC_Val(u8 ch)
{
    u16 val;
	
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_CyclesMode7);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

    val = ADC_GetConversionValue(ADC1);

    return val;
}

/*********************************************************************
 * @fn      DMA_Tx_Init
 *
 * @brief   Initializes the DMAy Channelx configuration.
 *
 * @param   DMA_CHx - x can be 1 to 7.
 *          ppadr - Peripheral base address.
 *          memadr - Memory base address.
 *          bufsize - DMA channel buffer size.
 *
 * @return  none
 */
void DMA_Tx_Init(DMA_Channel_TypeDef *DMA_CHx, u32 ppadr, u32 memadr, u16 bufsize)
{
    DMA_InitTypeDef DMA_InitStructure = {0};

    RCC_HBPeriphClockCmd(RCC_HBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA_CHx);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ppadr;
    DMA_InitStructure.DMA_MemoryBaseAddr = memadr;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = bufsize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA_CHx, &DMA_InitStructure);
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
    u16 ADC_val;
    u8 i=0;
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

    ADC_Function_Init();

    while(1)
    {
#if  (ADC_MODE_WDT == SCAN_MODE_WDT)
        DMA_Tx_Init(DMA1_Channel1, (u32)&ADC1->RDATAR, (u32)TxBuf, 3);
        DMA_Cmd(DMA1_Channel1, ENABLE);
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
        Delay_Ms(50);
        ADC_SoftwareStartConvCmd(ADC1, DISABLE);

        for(i = 0; i <3; i++){
            printf("%04d\r\n", TxBuf[i]);
            Delay_Ms(10);
        }
#else
        ADC_val = Get_ADC_Val(ADC_Channel_2);
           Delay_Ms(500);
           printf("%04d\r\n", ADC_val);
           Delay_Ms(2);
#endif

    }
}

void ADC1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
/*********************************************************************
 * @fn      ADC1_IRQHandler
 *
 * @brief   This function handles analog wathdog exception.
 *
 * @return  none
 */
void ADC1_IRQHandler(void)
{
#if  (ADC_MODE_WDT == SCAN_MODE_WDT)
    if(ADC_GetAnalogWatchdogFlagStatus( ADC1, ADC_AnalogWatchdog_0_FLAG)){
        printf( "Enter AnalogWatchdog-1 Interrupt\r\n" );
    }
    ADC_ClearAnalogWatchdogFlag( ADC1, ADC_AnalogWatchdog_0_FLAG);
    if(ADC_GetAnalogWatchdogFlagStatus( ADC1, ADC_AnalogWatchdog_1_FLAG)){
        printf( "Enter AnalogWatchdog-2 Interrupt\r\n" );
    }
    ADC_ClearAnalogWatchdogFlag( ADC1, ADC_AnalogWatchdog_1_FLAG);
    if(ADC_GetAnalogWatchdogFlagStatus( ADC1, ADC_AnalogWatchdog_2_FLAG)){
        printf( "Enter AnalogWatchdog-3 Interrupt\r\n" );
    }
    ADC_ClearAnalogWatchdogFlag( ADC1, ADC_AnalogWatchdog_2_FLAG);
#endif
    if(ADC_GetITStatus( ADC1, ADC_IT_AWD)){
        printf( "Enter AnalogWatchdog Interrupt\r\n" );
    }
    ADC_ClearITPendingBit( ADC1, ADC_IT_AWD);
}
