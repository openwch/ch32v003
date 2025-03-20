/********************************** (C) COPYRIGHT *******************************
 * File Name          : M4_SVPWM_ADC2R.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
 * Description        : SVPWM parameter calculation, state initialization, related timer and pin configuration,
 *                      ADC parameter calculation, state initialization, related timer and pin configuration,
 *                      zero offset read, ADC trigger time configuration
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Includes------------------------------------------------------------------*/
#include "M0_Control_Library.h"
/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private functions --------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* variables ----------------------------------------------------------------*/
SVPWM_2R_Type    SVPWM_Cal_M;     //SVPWM calculation structure
ADCStruc_Type    ADC_M;           //ADC sampling calculation structure
/*********************************************************************
 * @fn      SVPWM_2R_Paramter_Init
 *
 * @brief   SVPWM parameter initialization
 *
 * @para    SVPWM_Cal: SVPWM calculation structure pointer
 *          Mbase :motor base value parameter structure pointer
 *
 * @return  none
 */
void SVPWM_2R_Paramter_Init(SVPWM_2R_Type *SVPWM_Cal,Motor_Base_Type *Mbase)
{
    SVPWM_Cal->T = ((uint16_t)(TIMER_CLOCK_FREQ_SVPWM/(uint32_t)(FREQ_SVPWM)));
    SVPWM_Cal->HALF_T = SVPWM_Cal->T/2;
    SVPWM_Cal->Quard_T = SVPWM_Cal->T/4;

    //矢量作用时间计算系数
    SVPWM_Cal->coef1 = _IQ16mpy(_IQ16div((_IQ16(SVPWM_Cal->T))/2,(_IQ16div(DCBUS_RATED_VOLT,Mbase->Vbase))),Three_Q16);
    SVPWM_Cal->coef2 = _IQ16mpy(_IQ16div((_IQ16(SVPWM_Cal->T))/2,(_IQ16div(DCBUS_RATED_VOLT,Mbase->Vbase))),SQRT3_Q16);

    SVPWM_Cal->Maxmodulesqure = MAX_MODULATIONSQUARE;
    SVPWM_Cal->Maxmodule = MAX_MODULATION;
}
/*********************************************************************
 * @fn      SVPWM_Status_Initial_M
 *
 * @brief   SVPWM status initialization
 *
 * @para    SVPWM_Cal: SVPWM calculation structure pointer
 *
 * @return  none
 */
void SVPWM_2R_Status_Init_M(SVPWM_2R_Type *SVPWM_Cal)
{
    TIM1->CH1CVR = SVPWM_Cal->Quard_T;
    TIM1->CH2CVR = SVPWM_Cal->Quard_T;
    TIM1->CH3CVR = SVPWM_Cal->Quard_T;

    SVPWM_Cal->cc1Load = SVPWM_Cal->Quard_T;
    SVPWM_Cal->cc2Load = SVPWM_Cal->Quard_T;
    SVPWM_Cal->cc3Load = SVPWM_Cal->Quard_T;

    SVPWM_Cal->sector = 1;
    SVPWM_Cal->VoltLimMark = 0;
}
/*********************************************************************
 * @fn      ADC_Parameter_Init
 *
 * @brief   ADC parameter calculation
 *
 * @para    ADCStruc: ADC sampling calculation structure pointer
 *          Mbase :motor base value parameter structure pointer
 *
 * @return  none
 */
void ADC_Parameter_Init (ADCStruc_Type *ADCStruc,Motor_Base_Type *Mbase)
{
    //额定电流对应的采样值计算
    ADCStruc->RatedCurr_ConvInv = _IQ16mpyIQX(_IQ16div(_IQ16(128),_IQ16(16384)),\
                            16,_IQ16div(CURR_SAMPLE_RANGE_ADC,Mbase->Ibase),16);

    //额定电压对应的采样值倒数计算
    ADCStruc->DCVoltRatedADCInv =_IQ16div(_IQ16(1.0),(_IQ16div(_IQ16mpy(DCVOLT_CONV_RATIO,DCBUS_RATED_VOLT),_IQ16(5.0))*4096));

    //母线电压平均值计算系数
    ADCStruc->DCVoltAverSizeInv1 = _IQ(1.0)/DCVOLT_AVER_SIZE;
    ADCStruc->DCVoltAverSizeInv2 = _IQ(1.0) - ADCStruc->DCVoltAverSizeInv1;

}
/*********************************************************************
 * @fn      SVPWM_2R_Timer_Configure
 *
 * @brief   SVPWM timer and PWM ports initialization
 *
 * @return  none
 */
void SVPWM_2R_Timer_Configure(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure={0};
    TIM_OCInitTypeDef TIM_OCInitStructure={0};
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure={0};
    GPIO_InitTypeDef GPIO_InitStructure={0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOA | RCC_PB2Periph_GPIOB| RCC_PB2Periph_GPIOD\
                          | RCC_PB2Periph_AFIO | RCC_PB2Periph_TIM1, ENABLE);

    GPIO_PinRemapConfig(GPIO_PartialRemap6_TIM1, ENABLE);
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    TIM_DeInit(TIM1);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

    TIM_TimeBaseStructure.TIM_Prescaler = TIMER_PRESCALER_SVPWM;

    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;

    TIM_TimeBaseStructure.TIM_Period = SVPWM_Cal_M.HALF_T-1;

    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseStructure.TIM_RepetitionCounter = TIMER_UPDATE_RATE_SVPWM;

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_OCStructInit(&TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;

    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;

    TIM_OCInitStructure.TIM_Pulse = SVPWM_Cal_M.HALF_T/2;

    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;

    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;

    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);

    TIM_OCStructInit(&TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_Pulse = 1;

    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;

    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;

    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;

    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;

    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;

    TIM_BDTRInitStructure.TIM_DeadTime = DEADTIME_CONFIGURE;

    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;

    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;

    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;

    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

    TIM_SetCounter(TIM1,0);
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    TIM_ITConfig(TIM1, TIM_IT_CC4,DISABLE);
}
/*********************************************************************
 * @fn      ADC_OPA_Configure
 *
 * @brief   ADC and OPA configuration
 *
 * @para    ADCStruc: ADC sampling calculation structure pointer
 *
 * @return  none
 */
void ADC_OPA_Configure(ADCStruc_Type *ADCStruc)
{
    ADC_InitTypeDef ADC_InitStructure={0};
    GPIO_InitTypeDef GPIO_InitStructure={0};
    CMP1_InitTypeDef  CMP_InitStructure = {0};

    OPA_Unlock();
    OPA_CMP_Unlock();

    RCC_ADCCLKConfig(RCC_PCLK2_Div2);

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOA|RCC_PB2Periph_GPIOB|RCC_PB2Periph_GPIOC|RCC_PB2Periph_GPIOD\
                          |RCC_PB2Periph_AFIO | RCC_PB2Periph_ADC1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    CMP_InitStructure.CMP_Out_Mode = 3;
    CMP_InitStructure.NSEL = CMP_CHN0;
    CMP_InitStructure.PSEL = CMP_CHP0;
    CMP_InitStructure.HYEN = CMP_HYEN_ON;
    OPA_CMP_SystemReset_Cmd(CMP1,DISABLE);
    OPA_CMP1_Init(&CMP_InitStructure);
    OPA_CMP_TIM1_BKINConfig(TIM1_Brake_Source_CMP1);
    OPA_CMP_FILT_Cmd(ENABLE);
    OPA_CMP_FILT_LEN_Config(CMP_FILT_Len_0);
    OPA_CMP_Cmd(CMP1,ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigInjecConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1,ADC_Channel_3, 1, ADC_SampleTime_CyclesMode1);

    ADC_Cmd(ADC1, ENABLE);

    ADC_Sample_ModeConfig(ADC1,ADC_Sample_Over_1M_Mode);
}
/*********************************************************************
 * @fn      DCBUS_Volt_Cal
 *
 * @brief   Bus voltage calculation
 *
 * @para    ADCStruc: ADC sampling calculation structure pointer
 *          ADCx:ADC register pointer
 *
 * @return  none
 */
void DCBUS_Volt_Cal(ADCStruc_Type *ADCStruc,ADC_TypeDef *ADCx)
{
    if((ADCx->STATR & ADC_FLAG_EOC) == ADC_FLAG_EOC)
   {
        //获取当前母线电压
        ADCStruc->DCVoltPres = _IQ16toIQ(_IQ16MPY_MACRO(_IQ16(ADCx->RDATAR>>4),ADCStruc->DCVoltRatedADCInv));

        //均值计算
        ADCStruc->DCVoltAver = _IQMPY_MACRO(ADCStruc->DCVoltAver,ADCStruc->DCVoltAverSizeInv2)\
                                + _IQMPY_MACRO(ADCStruc->DCVoltPres,ADCStruc->DCVoltAverSizeInv1);

        ADCStruc->DCVoltcali= (_IQdiv(_IQ(1.0),ADCStruc->DCVoltAver));
   }
}

/*********************************************************************
 * @fn      ADC_1R_Offset_Reading
 *
 * @brief   Current sample static offset calculation
 *
 * @para    ADCStruc: ADC sampling calculation structure pointer
 *          ADCx:ADC register pointer
 *          ADC_num: ADC number
 *
 * @return  none
 */
void ADC_Offset_Reading(ADC_TypeDef *ADCx, ADCStruc_Type *ADCStruc,u_int8_t ADC_num)
{
    OPA_InitTypeDef  OPA_InitStructure = {0};
    uint16_t i=0;

    ADCStruc->OffsetValue1 = 0;
    ADCStruc->OffsetValue2 = 0;

    OPA_Cmd(DISABLE);
    OPA_StructInit(&OPA_InitStructure);
    OPA_InitStructure.NSEL = CHN0;
    OPA_InitStructure.PSEL = CHP1;
    OPA_InitStructure.OPA_HS = HS_ON;
    OPA_InitStructure.Mode = OUT_IO_OUT1;
    OPA_Init(&OPA_InitStructure);
    OPA_Cmd(ENABLE);

    ADC_ITConfig(ADCx, ADC_IT_JEOC, DISABLE);

    ADC_ExternalTrigInjectedConvConfig(ADCx, ADC_ExternalTrigInjecConv_None);

    ADC_ExternalTrigInjectedConvCmd(ADCx,ENABLE);

    ADC_InjectedSequencerLengthConfig(ADCx,1);

    ADC_InjectedChannelConfig(ADCx, CURR_CHANNEL_PHASEA_M,1,CURR_SAMPLING_TIMER_CLOCK);

    ADC_ClearFlag(ADCx, ADC_FLAG_JEOC);

    ADC_SoftwareStartInjectedConvCmd(ADCx,ENABLE);

    for(i=0; i <CURR_OFFSET_READ_NUM_ADC; i++)
    {
         while(!ADC_GetFlagStatus(ADCx,ADC_FLAG_JEOC));
         ADCStruc->OffsetValue1 += (ADC_GetInjectedConversionValue(ADCx,ADC_InjectedChannel_1)>>3);
         ADC_ClearFlag(ADCx, ADC_FLAG_JEOC);
         ADC_SoftwareStartInjectedConvCmd(ADCx,ENABLE);
    }
    while(!ADC_GetFlagStatus(ADCx,ADC_FLAG_JEOC));
    ADC_ClearFlag(ADCx, ADC_FLAG_JEOC);

    OPA_Cmd(DISABLE);
    OPA_StructInit(&OPA_InitStructure);
    OPA_InitStructure.NSEL = CHN0;
    OPA_InitStructure.PSEL = CHP2;
    OPA_InitStructure.OPA_HS = HS_ON;
    OPA_InitStructure.Mode = OUT_IO_OUT1;
    OPA_Init(&OPA_InitStructure);
    OPA_Cmd(ENABLE);
    ADC_ClearFlag(ADCx, ADC_FLAG_JEOC);
    ADC_SoftwareStartInjectedConvCmd(ADCx,ENABLE);

   for(i=0; i <CURR_OFFSET_READ_NUM_ADC; i++)
   {
        while(!ADC_GetFlagStatus(ADCx,ADC_FLAG_JEOC));
        ADCStruc->OffsetValue2 += (ADC_GetInjectedConversionValue(ADCx,ADC_InjectedChannel_1)>>3);
        ADC_ClearFlag(ADCx, ADC_FLAG_JEOC);
        ADC_SoftwareStartInjectedConvCmd(ADCx,ENABLE);
   }
   while(!ADC_GetFlagStatus(ADCx,ADC_FLAG_JEOC));
   ADC_ClearFlag(ADCx, ADC_FLAG_JEOC);

   OPA_Cmd(DISABLE);
   OPA_StructInit(&OPA_InitStructure);
   OPA_InitStructure.Mode = OUT_IO_OUT1;
   OPA_InitStructure.POLL_NUM = CHP_POLL_NUM_2;
   OPA_InitStructure.PSEL_POLL = CHP_OPA_POLL_ON;
   OPA_InitStructure.POLL_CH1 = CHP1;
   OPA_InitStructure.POLL_CH2 = CHP2;
   OPA_InitStructure.POLL_AT = OPA_POLL_AUTO_ON;
   OPA_InitStructure.SETUP_CFG = OPA_SETUP_CFG_0;
   OPA_InitStructure.NSEL = CHN0;
   OPA_InitStructure.OPA_HS = HS_ON;
   OPA_InitStructure.POLL_SEL = OPA_POLL_SEL_TIM1_CH4;
   OPA_InitStructure.OUT_IE = DISABLE;
   OPA_Init(&OPA_InitStructure);
   OPA_Cmd( ENABLE );

   ADC_InjectedSequencerLengthConfig(ADC1, 2);
   ADC_InjectedChannelConfig(ADC1, CURR_CHANNEL_PHASEA_M, 1, CURR_SAMPLING_TIMER_CLOCK);
   ADC_InjectedChannelConfig(ADC1, CURR_CHANNEL_PHASEA_M, 2, CURR_SAMPLING_TIMER_CLOCK);

   ADC_ClearFlag(ADCx, ADC_FLAG_JEOC);
}

/*********************************************************************
 * @fn      ADC_1R_Start
 *
 * @brief   ADC conversion enable
 *
 * @param   NewState - ENABLE or DISABLE.
 *
 * @return  none
 */
void ADC_Start (FunctionalState cmd)
{
    if (cmd == ENABLE)
    {
        ADC_ExternalTrigInjectedConvConfig(ADC1,ADC_ExternalTrigInjecConv_Ext_PD1_PA2_OPA );

        ADC1->CTLR2 &= ~ADC_InjectedExTrigConv_OPA;
        ADC1->CTLR2 |= ADC_InjectedExTrigConv_OPA;

        ADC1->STATR = ~(uint32_t)ADC_FLAG_JEOC;
        ADC1->CTLR1 |= (1<<7);

    }
    else
    {
        ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);

        ADC1->CTLR1 &= ~(1<<7);

        ADC1->STATR = ~(uint32_t)ADC_FLAG_JEOC;

        ADC1->CTLR2 &= ((uint32_t)0xFFFF7FFF);

        TIM1->BDTR &= (uint16_t)(~((uint16_t)TIM_MOE));

        ADC1->CTLR1 &= ((uint32_t)0xFFFFEFFF);
    }
}

/*********************************************************************
 * @fn      SVPWM_Lowside_Effective
 *
 * @brief   All low side MOS on, effective immediately
 *
 * @para    SVPWM_Cal: SVPWM calculation structure pointer
 *
 * @return  none
 */
void SVPWM_Lowside_Effective(SVPWM_2R_Type *SVPWM_Cal)
{
    SVPWM_Cal->cc1Load = SVPWM_Cal->HALF_T+1;
    SVPWM_Cal->cc2Load = SVPWM_Cal->HALF_T+1;
    SVPWM_Cal->cc3Load = SVPWM_Cal->HALF_T+1;
}

/*********************************************************************
 * @fn      SVPWM_Zerovolt_Output
 *
 * @brief   0 voltage output, effective immediately
 *
 * @para    SVPWM_Cal: SVPWM calculation structure pointer
 *
 * @return  none
 */
void SVPWM_Zerovolt_Output(SVPWM_2R_Type *SVPWM_Cal)
{
    SVPWM_Cal->cc1Load = SVPWM_Cal->Quard_T;
    SVPWM_Cal->cc2Load = SVPWM_Cal->Quard_T;
    SVPWM_Cal->cc3Load = SVPWM_Cal->Quard_T;
}

/*********************************************************************
 * @fn      SVPWM_Update
 *
 * @brief   Time comparison value updated
 *
 * @para    SVPWM_Cal: SVPWM calculation structure pointer
 *
 * @return  none
 */
void SVPWM_Update (SVPWM_2R_Type *SVPWM_Cal)
{
    TIM1->CH1CVR = SVPWM_Cal->cc1Load;
    TIM1->CH2CVR = SVPWM_Cal->cc2Load;
    TIM1->CH3CVR = SVPWM_Cal->cc3Load;
}
