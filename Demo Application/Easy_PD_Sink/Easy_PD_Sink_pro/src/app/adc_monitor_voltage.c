#include "adc_monitor_voltage.h"
#include "led.h"


/* Global Variable */
volatile uint16_t adc_data_buffer[ADC_DATA_LENGTH] = {0};

void adc_dma_init(void)
{
    DMA_InitTypeDef DMA_InitStructure = {0};
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /*  DMA1_Channel1 */
    DMA_DeInit(DMA1_Channel1);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->RDATAR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)adc_data_buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = ADC_DATA_LENGTH;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    /* DMA interrupt   */
    DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
    NVIC_SetPriority(DMA1_Channel1_IRQn, 0xf0);
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);

    // DMA_Cmd(DMA1_Channel1, ENABLE);
}


void adc_init(void)
{
    /* init adc gpio  */
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* init adc unit */
    ADC_InitTypeDef ADC_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    ADC_DeInit(ADC1);

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;  
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Calibration_Vol(ADC1, ADC_CALVOL_50PERCENT);
    ADC_Cmd(ADC1,ENABLE);

    adc_dma_init();
    ADC_DMACmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));

    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_241Cycles ); 

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


__attribute__((interrupt("WCH-Interrupt-fast")))
void DMA1_Channel1_IRQHandler(void)
{
    uint32_t adc_data = 0;
    DMA_ClearITPendingBit(DMA1_IT_TC1);

    // printf("adc dma transfer complete\r\n");
    // for(uint16_t i=0; i<ADC_DATA_LENGTH; i++)
    // {
    //     printf("adc_data_buffer[%d] = %d\r\n",i,adc_data_buffer[i]);
    // }

    for(uint8_t i=0; i<ADC_DATA_LENGTH; i++)
    {
        adc_data += adc_data_buffer[i];
    }
    adc_data /= ADC_DATA_LENGTH;

    if(adc_data>915)        
    {
        led_turn_on(PD_VOLTAGE_20V);
        easyPDControl.setVoltage = PD_VOLTAGE_20V;
    }
    else if(adc_data>686)
    {
        led_turn_on(PD_VOLTAGE_15V);
        easyPDControl.setVoltage = PD_VOLTAGE_15V;
    }   
    else if(adc_data>510)
    {
        led_turn_on(PD_VOLTAGE_12V);
        easyPDControl.setVoltage = PD_VOLTAGE_12V;
    }   
    else if(adc_data>420)
    {
        led_turn_on(PD_VOLTAGE_9V);
        easyPDControl.setVoltage = PD_VOLTAGE_9V;
    }   
    else
    {
        led_turn_on(PD_VOLTAGE_5V);
        easyPDControl.setVoltage = PD_VOLTAGE_5V;
    }                    
}