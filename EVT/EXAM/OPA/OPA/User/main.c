/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/08
 * Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 OPA1 is used as a voltage follower output, the external voltage
 is output to OPA1, and the ADC samples OPA1
 OPA1_CHP1--PD7
 OPA1_CHN1--PD0
 OPA1_OUT--PD4

 In this example, PD0 and PD4 are short-circuited, and the external voltage is input from PD7.
 If the negative feedback is connected to a resistor, it can also form an operational amplifier.
 Note: When using PD7, the reset function of this pin needs to be turned off.
*/

#include "debug.h"

/* Global define */


/* Global Variable */



/*********************************************************************
 * @fn      OPA1_Init
 *
 * @brief   Initializes OPA collection.
 *
 * @return  none
 */
void OPA1_Init( void )
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    OPA_InitTypeDef  OPA_InitStructure = {0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOD, &GPIO_InitStructure );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOD, &GPIO_InitStructure );

    OPA_InitStructure.PSEL = CHP1;
    OPA_InitStructure.NSEL = CHN1;
    OPA_Init( &OPA_InitStructure );
    OPA_Cmd( ENABLE );

}

/*********************************************************************
 * @fn      ADC_Channel7_Init
 *
 * @brief   Initializes ADC Channel7 collection.
 *
 * @return  none
 */
void ADC_Channel7_Init( void )
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    ADC_InitTypeDef ADC_InitStructure = {0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE );
    RCC_ADCCLKConfig( RCC_PCLK2_Div8 );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOD, &GPIO_InitStructure );

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init( ADC1, &ADC_InitStructure );

    ADC_Calibration_Vol(ADC1, ADC_CALVOL_50PERCENT);
    ADC_Cmd( ADC1, ENABLE );

    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
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
 *
 * @return  none
 */
u16 Get_ADC_Val( u8 ch )
{
    u16 val;

    ADC_RegularChannelConfig( ADC1, ch, 1, ADC_SampleTime_241Cycles );
    ADC_SoftwareStartConvCmd( ADC1, ENABLE );

    while( !ADC_GetFlagStatus( ADC1, ADC_FLAG_EOC ) );

    val = ADC_GetConversionValue( ADC1 );

    return val;
}

/*********************************************************************
 * @fn      Get_ADC_Average
 *
 * @brief   Returns ADCx conversion result average data.
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
 *
 * @return  val - The Data conversion value.
 */
u16 Get_ADC_Average( u8 ch, u8 times )
{
    u32 temp_val = 0;
    u8 t;
    u16 val;

    for( t = 0; t < times; t++ ){
        temp_val += Get_ADC_Val( ch );
        Delay_Ms( 5 );
    }

    val = temp_val / times;

    return val;
}


/*********************************************************************
 * @fn      main
 *
 * @brief   ADC_Channel0_Init
 *
 * @return  none
 */
int main( void )
{
    u16 ADC_val, i;

    Delay_Init();
    USART_Printf_Init( 115200 );
    printf( "SystemClk:%d\r\n", SystemCoreClock );
    printf( "OPA Test\r\n" );
    OPA1_Init();
    ADC_Channel7_Init();

    while( 1 )
    {
        for( i = 0; i < 6; i++ ){
            ADC_val = Get_ADC_Average( ADC_Channel_7, 10 );
            printf( "OPA_OUT:%04d\r\n", ADC_val );
            Delay_Ms( 500 );
        }
    }
}
