/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.1
 * Date               : 2024/12/23
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *OPA PGA routine:
 *OPA1_CHP0--PA2
 *OPA1_OUT--PD4
 *OPA_PGA_Dif--OPA1_CHN--PA4
 *The negative input pin of OPA is an internal pin, and when the positive 
 *input voltage is given, the output gain is 4 times. Differential signal 
 *with bias can be selected, output multiplier and bias voltage can be 
 *referred to the manual.
 */

#include "debug.h"

/* OPA PGA Mode Definition*/
#define OPA_PGA_Dif        0
#define OPA_PGA_No_Dif     1

/* OPA PGA Mode Selection*/
#define OPA_PGA_MODE   OPA_PGA_Dif
//#define OPA_PGA_MODE   OPA_PGA_No_Dif
/*********************************************************************
 * @fn      OPA2_Init
 *
 * @brief   Initializes OPA2.
 *
 * @return  none
 */
void OPA2_Init( void )
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    OPA_InitTypeDef  OPA_InitStructure = {0};

    RCC_PB2PeriphClockCmd( RCC_PB2Periph_GPIOA, ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOA, &GPIO_InitStructure );

    OPA_InitStructure.PSEL = CHP0;
    OPA_InitStructure.NSEL = CHN_PGA_4xIN;
    OPA_InitStructure.Mode = OUT_IO_OUT0;
    OPA_InitStructure.FB = FB_ON;
#if  (OPA_PGA_MODE == OPA_PGA_Dif)
    /*Using the CMP Bias*/
    OPA_InitStructure.PGA_VBEN = PGA_VBEN_ON;
    OPA_InitStructure.PGA_VBSEL = PGA_VBSEL_VDD_DIV2;
    OPA_InitStructure.PGADIF = PGADIF_ON;
    OPA_InitStructure.VBCMPSEL = VBCMPSEL_Mode_0;

#endif

    OPA_InitStructure.OPA_HS = HS_ON;

    OPA_Init( &OPA_InitStructure );
#if  (OPA_PGA_MODE == OPA_PGA_Dif)
    OPA_CMP_Cmd(CMP2,ENABLE);
#endif
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
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOD, ENABLE);
    RCC_PB2PeriphClockCmd(RCC_PB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig( RCC_PCLK2_Div8 );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);
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
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    u16 ADC_val;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init( 115200 );
#endif
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    OPA_Unlock();
    OPA_CMP_Unlock();
    OPA2_Init();
    ADC_Function_Init();
    while(1)
    {
        ADC_val=Get_ADC_Val(ADC_Channel_7);
        printf("ADC_val  %04d\r\n",ADC_val);
        Delay_Ms(500);
    }

}



