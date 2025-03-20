/********************************** (C) COPYRIGHT *******************************
 * File Name          : M4_PWM_ADC.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/15
 * Description        : ADC and PWM structure definition and state initialization.
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
ADCStruc_Type    ADC_M;
PWM_CHOPPER_TYPE PWMcopper_M;

/*********************************************************************
 * @fn      PWMADC_Timer_Configure
 *
 * @brief   PWM timer and PWM ports initialization
 *
 * @para    none
 *
 * @return  none
 */
void PWMADC_Timer_Configure(void)
{
#if 1
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure={0};
    TIM_OCInitTypeDef TIM_OCInitStructure={0};
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure={0};
    GPIO_InitTypeDef GPIO_InitStructure={0};

    RCC_PB2PeriphClockCmd( RCC_PB2Periph_GPIOA | RCC_PB2Periph_GPIOB | RCC_PB2Periph_GPIOC | RCC_PB2Periph_AFIO | RCC_PB2Periph_TIM1, ENABLE );
    GPIO_PinRemapConfig(GPIO_PartialRemap6_TIM1, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = U_Mos_H_Pin ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = V_Mos_H_Pin | W_Mos_H_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  U_Mos_L_Pin | V_Mos_L_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,U_Mos_L_Pin|V_Mos_L_Pin);

    GPIO_InitStructure.GPIO_Pin =  W_Mos_L_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOD, W_Mos_L_Pin);

    TIM_DeInit(TIM1);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

    TIM_TimeBaseStructure.TIM_Prescaler = TIMER_PRESCALER_PWM;

    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseStructure.TIM_Period = TIMER_PERIOD_PWM;

    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV2;

    TIM_TimeBaseStructure.TIM_RepetitionCounter = TIMER_UPDATE_RATE_PWM;

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_OCStructInit(&TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;

    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;

    TIM_OCInitStructure.TIM_Pulse = 0;

    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;

    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;

    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);

    TIM_OCStructInit(&TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;

    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;

    TIM_OCInitStructure.TIM_Pulse = 60;

    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OCInitStructure.TIM_OCNPolarity =TIM_OCNPolarity_Low;

    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;

    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Disable);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;

    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;

    TIM_BDTRInitStructure.TIM_DeadTime = DEADTIME_CONFIGURE;

    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;

    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;

    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;

    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

    TIM_SetCounter(TIM1,0);
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
#endif

#if 0
    RCC->PB2PCENR |=RCC_PB2Periph_GPIOA | RCC_PB2Periph_GPIOB | RCC_PB2Periph_GPIOC | RCC_PB2Periph_AFIO | RCC_PB2Periph_TIM1;
    AFIO->PCFR1 &=0xFFFFC3FF;
    AFIO->PCFR1 |=0x1800;

    GPIOA->CFGLR &= 0xFFFF0FFF;
    GPIOA->CFGLR |= 0x9000;

    GPIOB->CFGLR &= 0xFFFFFF00;
    GPIOB->CFGLR |= 0x99;

    GPIOA->CFGLR &= 0xFFFFF0F0;
    GPIOA->CFGLR |= 0x0101;
    GPIOA->BCR = 0x0001 | 0x0004 ;

    GPIOD->CFGLR &= 0xFFFFFFF0;
    GPIOD->CFGLR |= 0x0001;
    GPIOD->BCR = 0x0001 ;

    RCC->PB2PRSTR |= 0x800;
    RCC->PB2PRSTR &= ~0x800;
    TIM1->PSC  = 0;
    TIM1->CTLR1 &=0xFCEF;
    TIM1->CTLR1 |=0x100;
    TIM1->ATRLR = TIMER_PERIOD_PWM;
    TIM1->RPTCR =  0;

    TIM1->CHCTLR1 &= 0x8787;
    TIM1->CHCTLR1 |= 0x6060;
    TIM1->CHCTLR2 &= 0x8787;
    TIM1->CHCTLR2 |= 0x7860;

    TIM1->CCER&=0xC000;
    TIM1->CCER|=0x1111;

    TIM1->CH1CVR =  0;
    TIM1->CH2CVR =  0;
    TIM1->CH3CVR =  0;
    TIM1->CH4CVR = 60;

    TIM1->CTLR2 &= 0x00FF;
    TIM1->CTLR2 |= 0x0000;

    TIM1->BDTR = 0x2D00;

    TIM1->CNT = 0;
    TIM1->INTFR &= 0xFFFE;
    TIM1->DMAINTENR |= 0x01;

#endif
}

/*********************************************************************
 * @fn      ADC_OPA_Configure
 *
 * @brief   Initializes ADC&OPA&CMP collection
 *
 * @para    ADCStruc: ADC Sample computation structure pointer
 *
 * @return  none
 */
void ADC_OPA_Configure(ADCStruc_Type *ADCStruc)
{
#if 1
    ADC_InitTypeDef ADC_InitStructure={0};
    GPIO_InitTypeDef GPIO_InitStructure={0};
    OPA_InitTypeDef  OPA_InitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOA |RCC_PB2Periph_GPIOC | RCC_PB2Periph_GPIOD|RCC_PB2Periph_AFIO | RCC_PB2Periph_ADC1,ENABLE);

    RCC_ADCCLKConfig(RCC_PCLK2_Div4);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    OPA_Unlock();
    OPA_CMP_Unlock();

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOD, &GPIO_InitStructure );

    OPA_InitStructure.PSEL = CHP1;
    OPA_InitStructure.NSEL = CHN_PGA_8xIN;
    OPA_InitStructure.Mode=OUT_IO_OUT1;
    OPA_InitStructure.FB=FB_ON;

    OPA_InitStructure.PGA_VBEN = PGA_VBEN_ON;
    OPA_InitStructure.PGA_VBSEL = PGA_VBSEL_VDD_DIV2;
    OPA_InitStructure.PGADIF = PGADIF_OFF;
    OPA_InitStructure.VBCMPSEL = VBCMPSEL_Mode_0;

    OPA_InitStructure.OPA_HS = HS_OFF;
    OPA_Init( &OPA_InitStructure );

    OPA_CMP_Cmd(CMP2,ENABLE);
    OPA->CTLR2 |= 0x9010000;
    OPA_Cmd(ENABLE );

    ADC1->SAMPTR2 = 0X00000000;

    ADC_DeInit(ADC1);

    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_Mode =  ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1,CURR_CHANNEL_PHASE_M, 1, SAMPLING_TIMER_CLOCK);

    ADC_Cmd(ADC1, ENABLE);
#endif

#if 0
    RCC->PB2PCENR |= RCC_PB2Periph_GPIOA |RCC_PB2Periph_GPIOC | RCC_PB2Periph_GPIOD|RCC_PB2Periph_AFIO | RCC_PB2Periph_ADC1;//Ê±ÖÓÊ¹ÄÜ
    RCC->CFGR0 &=0x7FFF07FF;
    RCC->CFGR0 |=0x00004000;


    GPIOD->CFGLR &= 0xF000FFFF;
    GPIOA->CFGLR &= 0xFFFFFF0F;

    OPA->OPAKEY = 0x45670123;
    OPA->OPAKEY = 0xCDEF89AB;

    OPA->CMPKEY = 0x45670123;
    OPA->CMPKEY = 0xCDEF89AB;

    GPIOD->CFGLR &=0x0FFFFFFF;
    GPIOD->CFGLR |=0x40000000;

    OPA->CTLR1 &=0x70000000;
    OPA->CTLR1 |=0x010C13;

    OPA->CTLR2 |= 0x9010000;


    ADC1->SAMPTR1 = 0X00000000;
    ADC1->SAMPTR2 = 0X00000000;

    RCC->PB2PRSTR |= RCC_PB2Periph_ADC1;
    RCC->PB2PRSTR &= ~RCC_PB2Periph_ADC1;

    ADC1->CTLR1 &= 0xFFFFFEFF;
    ADC1->CTLR1 |= 0x100;

    ADC1->CTLR2 &=0xFFF0F7FD;
    ADC1->CTLR2 |=0xE0000;

    ADC1->RSQR1 &=0xFF0FFFFF;

    ADC1->RSQR3 &=0xFFFFFFE0;
    ADC1->RSQR3 |=0x09;

    ADC1->CTLR2 |=0x1;
#endif
}

/*********************************************************************
 * @fn      ADC_Parameter_Cal
 *
 * @brief   ADC parameter calculate
 *
 * @para    ADCStruc: ADC Sample computation structure pointer
 *
 * @return  none
 */
void ADC_Parameter_Cal (ADCStruc_Type *ADCStruc,Motor_Base_Type *Mbase)
{
    ADCStruc->AdcChannel1 = VBUS_CHANNEL_PHASE_M;
    ADCStruc->AdcChannel2 = UEMF_CHANNEL_PHASE_M;

    ADCStruc->RatedCurr_ConvInv = _IQ16mpyIQX(_IQ16div(_IQ16(128),_IQ16(32767)),\
                            16,_IQ18div(CURR_SAMPLE_RANGE_ADC,Mbase->Ibase),18);

    ADCStruc->IDCLPF1st_2.Input_Coef = _IQdiv(IDC_LPF2_WcT_M,(IDC_LPF2_WcT_M + UNIT_Q24));
    ADCStruc->IDCLPF1st_2.Output_Coef = _IQdiv(UNIT_Q24,(IDC_LPF2_WcT_M + UNIT_Q24));

    ADCStruc->DCVoltRatedADC = _IQ18div(_IQ18mpy(DCVOLT_CONV_RATIO,DCBUS_RATED_VOLT),_IQ18(5.0))*4096;

    ADCStruc->RatedDCVoltADCInv =_IQ18div(_IQ18(1.0),ADCStruc->DCVoltRatedADC);

    ADCStruc->DCVoltPres = DCVOLT_INIT;
    ADCStruc->DCVoltHalf = DCVOLT_INIT/2;

    ADCStruc->EMFRatedADC = _IQ18div(_IQ18mpy(BEMF_CONV_RATIO,DCBUS_RATED_VOLT),_IQ18(5.0))*4096;
    ADCStruc->EMFRated_ConvInv = _IQ18div(_IQ18(1.0),ADCStruc->EMFRatedADC);
}

/*********************************************************************
 * @fn      ADC_Status_Init
 *
 * @brief   ADC parameter init
 *
 * @para    ADCStruc: ADC Sample computation structure pointer
 *
 * @return  none
 */
void ADC_Status_Init (ADCStruc_Type *ADCStruc)
{
    ADCStruc->IDCPres = 0;
    ADCStruc->IDCFilter_2 = 0;
    ADCStruc->IDCLPF1st_2.Out_Pre=0;
    ADCStruc->IDCLPF1st_2.Out_New=0;

    ADCStruc->DCVoltPres_ADvalue = 0;

    ADCStruc->DCVoltPres = DCVOLT_INIT;
    ADCStruc->DCVoltHalf = DCVOLT_INIT>>1;

    ADCStruc->BEMFPres=0;
    ADCStruc->BEMFU=0;
    ADCStruc->BEMFV=0;
    ADCStruc->BEMFW=0;
}

/*********************************************************************
 * @fn      ADC_Offset_Reading
 *
 * @brief   Read current offset of ADC sample
 *
 * @para    ADCStruc: ADC Sample computation structure pointer
 *
 * @return  none
 */
void ADC_Offset_Reading(ADCStruc_Type *ADCStruc)
{
    uint16_t i;
    ADCStruc->IOffsetValue = 0;

    ADC_ITConfig(ADC1, ADC_IT_JEOC, DISABLE);

    ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);

    ADC_ExternalTrigInjectedConvCmd(ADC1,ENABLE);

    ADC_InjectedSequencerLengthConfig(ADC1,1);

    ADC_InjectedChannelConfig(ADC1, CURR_CHANNEL_PHASE_M,1,SAMPLING_TIMER_CLOCK);

    ADC_ClearFlag(ADC1, ADC_FLAG_JEOC);

    ADC_SoftwareStartInjectedConvCmd(ADC1,ENABLE);

    for(i=0; i <CURR_OFFSET_READ_NUM_ADC; i++)
    {
        while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_JEOC));
        ADCStruc->IOffsetValue += (ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_1));
        ADC_ClearFlag(ADC1, ADC_FLAG_JEOC);
        ADC_SoftwareStartInjectedConvCmd(ADC1,ENABLE);
    }

    ADC_InjectedSequencerLengthConfig(ADC1,2);
    ADC_InjectedChannelConfig(ADC1, ADCStruc->AdcChannel1,1,SAMPLING_TIMER_CLOCK);
    ADC_InjectedChannelConfig(ADC1, ADCStruc->AdcChannel2,2,SAMPLING_TIMER_CLOCK);

    ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);
    ADC_ExternalTrigInjectedConvCmd(ADC1,ENABLE);

   ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);
}

/*********************************************************************
 * @fn      ADC_Start
 *
 * @brief   start ADC sample
 *
 * @para   cmd - enable or disable
 *
 * @return  none
 */
void ADC_Start (FunctionalState cmd)
{
    if (cmd == ENABLE)
    {
        ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T1_CC4);

        ADC1->STATR = ~(uint32_t)ADC_FLAG_JEOC;
    }
}

/*********************************************************************
 * @fn     PWM_Parameter_Define_M
 *
 * @brief   PWM parameter definition
 *
 * @para    PWMchopper:PWM chopper control structure pointer
 *
 * @return  none
 */
void PWM_Parameter_Define_M (PWM_CHOPPER_TYPE *PWMchopper)
{
    PWMchopper->T = TIMER_PERIOD_PWM + 1;
    PWMchopper->Dutymax = PWM_DUTY_MAX_INIT;
    PWMchopper->Dutymin = PWM_DUTY_MIN;

    PWMchopper->Dutylimcurr = PWM_DUTY_LIM_CURR;

    PWMchopper->UHpin = U_Mos_H_Pin;
    PWMchopper->ULpin = U_Mos_L_Pin;
    PWMchopper->VHpin = V_Mos_H_Pin;
    PWMchopper->VLpin = V_Mos_L_Pin;
    PWMchopper->WHpin = W_Mos_H_Pin;
    PWMchopper->WLpin = W_Mos_L_Pin;
    PWMchopper->EMFChanelU = UEMF_CHANNEL_PHASE_M;
    PWMchopper->EMFChanelV = VEMF_CHANNEL_PHASE_M;
    PWMchopper->EMFChanelW = WEMF_CHANNEL_PHASE_M;
}

/*********************************************************************
 * @fn     PWM_Status_Initial_M
 *
 * @brief  Initialization of the PWM status
 *
 * @para   PWMchopper:PWM chopper control structure pointer
 *
 * @return  none
 */
void PWM_Status_Initial_M (PWM_CHOPPER_TYPE *PWMchopper)
{
    PWMchopper->Sector = 1;
    PWMchopper->SectorLast = 0;
    PWMchopper->Duty = PWMchopper->Dutymin;
    PWMchopper->DutyU = 0;
    PWMchopper->DutyV = 0;
    PWMchopper->DutyW = 0;
}

/*********************************************************************
 * @fn     PWM_Enable_M
 *
 * @brief  PWM_Enable control
 *
 * @para   TIM1: TIM1 structure pointer
 *
 * @return  none
 */
void PWM_Enable_M (FunctionalState cmd)
{
    if (cmd == ENABLE)
    {
        TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
        TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
        TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
        TIM_CtrlPWMOutputs(TIM1, ENABLE);
    }
    else
    {
        TIM_CtrlPWMOutputs(TIM1, DISABLE);
        GPIO_ResetBits(GPIOA,U_Mos_L_Pin| V_Mos_L_Pin);
        GPIO_ResetBits(GPIOD,W_Mos_L_Pin);
        TIM1->CH1CVR = 0;
        TIM1->CH2CVR = 0;
        TIM1->CH3CVR = 0;
        TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
        TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
        TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable);
    }
}

/*********************************************************************
 * @fn     PWM_Update
 *
 * @brief  Update PWM
 *
 * @para   GPIOA: GPIO group A structure pointer
 *         GPIOD: GPIO group D structure pointer
 *         TIM1: TIM1 structure pointer
 *         ADC1: ADC1structure pointer
 *
 * @return  none
 */
__attribute__((section(".highcode")))
void PWM_Update(PWM_CHOPPER_TYPE *PWMchopper)
{
    GPIOA->BCR = PWMchopper->BCRApin;
    GPIOA->BSHR = PWMchopper->BSHRApin;
    GPIOD->BCR = PWMchopper->BCRDpin;
    GPIOD->BSHR = PWMchopper->BSHRDpin;

    TIM1->CH1CVR = PWMchopper->DutyU;
    TIM1->CH2CVR = PWMchopper->DutyV;
    TIM1->CH3CVR = PWMchopper->DutyW;

    ADC1->ISQR&=~(0X1F<<15);
    ADC1->ISQR|=((PWMchopper->EMFchannelselect)<<15);
}

/*********************************************************************
 * @fn     ADC_Trigger_Update
 *
 * @brief  Time of update ADC trigger
 *
 * @para   TIM1: TIM1 structure pointer
 *
 * @return  none
 */
__attribute__((section(".highcode")))
void ADC_Trigger_Update(PWM_CHOPPER_TYPE *PWMchopper)
{
    TIM1->CH4CVR = PWMchopper->Duty>>1;
}

/*********************************************************************
 * @fn     DCBUS_Volt_Cal
 *
 * @brief  Bus voltage calculation
 *
 * @para   ADCStruc: ADC Sample computation structure pointer
 *
 * @return  none
 */
__attribute__((section(".highcode")))
void DCBUS_Volt_Cal(ADCStruc_Type *ADCStruc,ADC_TypeDef *ADCx)
{
    ADCStruc->DCVoltPres_ADvalue =ADC1->IDATAR1;

    ADCStruc->DCVoltPres=_IQ18toIQ(_IQ18MPY_MACRO(_IQ18(ADCStruc->DCVoltPres_ADvalue),ADCStruc->RatedDCVoltADCInv));

    ADC_M.DCVoltHalf= ADCStruc->DCVoltPres>>1;
}

/*********************************************************************
 * @fn     Bemf_Cal
 *
 * @brief  BEMF calculation
 *
 * @para   ADCStruc: ADC Sample computation structure pointer
 *
 * @return  none
 */
__attribute__((section(".highcode")))
void Bemf_Cal(ADCStruc_Type *ADCStruc,ADC_TypeDef *ADCx)
{
    ADCStruc->BEMFPres=_IQ18toIQ(_IQ18MPY_MACRO((_IQ18(ADCx->IDATAR2)),ADC_M.EMFRated_ConvInv));
}

/*********************************************************************
 * @fn     IDC_Cal
 *
 * @brief  Current calculation
 *
 * @para   ADCStruc: ADC Sample computation structure pointer
 *
 * @return  none
 */
void IDC_Cal(ADCStruc_Type *ADCStruc,ADC_TypeDef *ADCx)
{
    if((ADCx->STATR & ADC_FLAG_EOC) == ADC_FLAG_EOC)
    {
        int32_t ADsample_read1;
        int16_t ADsample_int1;
        int32_t ADC_Result;
        ADC_Result=(ADCx->RDATAR)<<4;

        ADsample_read1 =ADC_Result-(int32_t)(ADCStruc->IOffsetValue);

        if (ADsample_read1> S16_MAX)
          ADsample_int1 = S16_MAX;
        else if (ADsample_read1 < 0)
          ADsample_int1 = 0;
        else
          ADsample_int1 = ADsample_read1;

        ADCStruc->IDCPres=_IQ16toIQ((_IQ16MPY_MACRO(_IQ16(ADsample_int1),ADCStruc->RatedCurr_ConvInv))>>7);

    }
}
