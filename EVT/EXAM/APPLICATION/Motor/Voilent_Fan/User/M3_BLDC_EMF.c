/********************************** (C) COPYRIGHT *******************************
 * File Name          : M3_BLEC_EMF.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/15
 * Description        : BEMF detect and calculate.
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
EMF_OBSERVER_TYPE  EMF_Cal_M;
Observer_Status_Type EMF_Status_M;
_iq24 Speedbufffer1[EMF_SPEED_BUFFERSIZE_M1] = {0};
_iq24 Speedbufffer2[EMF_SPEED_BUFFERSIZE_M2] = {0};

void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      TIM2_IRQHandler
 *
 * @brief   TIM2_IRQHandler Process
 *
 * @para    EMF_Cal_M: EMF observation result structure
 *          PWMcopper_M: PWM chopper control structure
 *
 * @return  none
 */
 __attribute__((section(".highcode")))
void TIM2_IRQHandler(void)
{
        if(EMF_Cal_M.Sprechangeflag == TRUE)
        {
            EMF_Cal_M.Sprechangeflag = FALSE;

            EMF_Cal_M.Sprechangemark = FALSE;

            TIM_Cmd(TIM2, DISABLE);

        }
        else if(EMF_Cal_M.Sprechangemark  == TRUE)
        {
            TIM_Cmd(TIM2, DISABLE);

            if(++EMF_Cal_M.Sector>6)
                EMF_Cal_M.Sector = 1;

            PWMcopper_M.Sector = EMF_Cal_M.Sector;

            PWM_Switch(&PWMcopper_M);

            PWM_Update(&PWMcopper_M);

            if(EMF_Cal_M.Schange_cnt>=EMF_Cal_M.Schange_cnt_Thre)
            {
                if(EMF_Cal_M.Timer_Overflow_Cnt !=0)
                {
                    EMF_Cal_M.Cap_Value_Origin =  Captimer_Cntget()+ (uint32_t)(EMF_Cal_M.Timerperiod) * EMF_Cal_M.Timer_Overflow_Cnt;
                    EMF_Cal_M.Timer_Overflow_Cnt = 0;
                    EMF_Status_M.Timeout =FALSE;
                }
                else
                    EMF_Cal_M.Cap_Value_Origin = Captimer_Cntget();
            }
            if(EMF_Cal_M.Cap_Value_Origin <= EMF_Cal_M.Cap_Upperlim)
               EMF_Cal_M.Cap_Value_Origin = EMF_Cal_M.Cap_Upperlim;

            Captimer_Clear();

            EMFObs_Speed_Cal(&EMF_Cal_M,&EMF_Status_M,&SpeedRamp_M);

            Buffer_Update(&(Speedbufffer1[0]), &(EMF_Cal_M.SpeedBuffPara1),EMF_Cal_M.Speed_Inst);
            Buffer_Update(&(Speedbufffer2[0]), &(EMF_Cal_M.SpeedBuffPara2),EMF_Cal_M.Speed_Inst);

            EMF_Cal_M.Speed_Aver1 = AverValue_Cal(EMF_Cal_M.Speed_Inst,EMF_Cal_M.Speed_Aver1,&(EMF_Cal_M.SpeedBuffPara1));

            Speedregulator(SpeedRamp_M.Aftramp, EMF_Cal_M.Speed_Aver1,&SpeedRamp_M,&PWMcopper_M,&ADC_M,&Speedsctl_M);

#if 1
            if(SpeedRamp_M.Preramp == SpeedRamp_M.Speed_Stop)
            {
                if((EMF_Cal_M.Speed_Aver1<=SpeedRamp_M.Speed_Stop)||(Operation_M.StopwaitCnt ==0))
                {
                    SpeedRamp_M.SpeedStopflag =TRUE;
                    ADC_ITConfig(ADC1, ADC_IT_JEOC, DISABLE);
                   PWM_Enable_M(DISABLE);
#if 0
                   GPIO_SetBits(GPIOC,U_Mos_L_Pin| V_Mos_L_Pin|W_Mos_L_Pin);
                   GPIO_SetBits(GPIOA,U_Mos_L_Pin| V_Mos_L_Pin);
                   GPIO_SetBits(GPIOD,W_Mos_L_Pin);
#endif
                   Time_Delay_Set(Operation_M.StopwaitSet,&Operation_M.StopwaitCnt);
                }
            }
#endif
            EMF_Cal_M.Sprechangeflag = TRUE;

            if(EMF_Cal_M.Schange_cnt<=1260)
                EMF_Cal_M.Schangedelay = _IQ12MPY_MACRO(_IQ12(EMF_Cal_M.Cap_Value_Origin),_IQ12(0.15))>>12;
            else
              EMF_Cal_M.Schangedelay = EMF_Cal_M.Sampledelay_H ;

            reset_tim2_time(EMF_Cal_M.Schangedelay);
        }

        TIM2->INTFR = 0xFFFE;
}

 /*********************************************************************
  * @fn      CHANGEPHASE_Timer2_Init
  *
  * @brief   Initializes TIM2 collection
  *
  * @para    none
  *
  * @return  none
  */
void CHANGEPHASE_Timer2_Init(void)
{
    TIM_TimeBaseInitTypeDef TimeBaseInitStructure={0};

    RCC_PB1PeriphClockCmd( RCC_PB1Periph_TIM2, ENABLE );
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructInit(&TimeBaseInitStructure);
    TimeBaseInitStructure.TIM_Period = EMF_TIMER_PERIOD;
    TimeBaseInitStructure.TIM_Prescaler = EMF_TIMER_PRESCALAR-1;
    TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2,&TimeBaseInitStructure);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM2, DISABLE);
}

/*********************************************************************
 * @fn      EMFObs_Parameter_Define_M
 *
 * @brief   Parameter_Define of EMF observe
 *
 * @para    EMFCal: EMF observation result structure pointer
 *          EMFStatus: Observer state structure pointer
 *
 * @return  none
 */
void EMFObs_Parameter_Define_M(EMF_OBSERVER_TYPE *EMFCal,Observer_Status_Type *EMFStatus)
{
    EMFCal->SpeedBuffPara1.Length = EMF_SPEED_BUFFERSIZE_M1;
    EMFCal->SpeedBuffPara1.Inv1 = _IQdiv(_IQ(1.0),_IQ(EMF_SPEED_BUFFERSIZE_M1));
    EMFCal->SpeedBuffPara1.Inv2 = _IQdiv(_IQ(EMF_SPEED_BUFFERSIZE_M1-1),_IQ(EMF_SPEED_BUFFERSIZE_M1));

    EMFCal->SpeedBuffPara2.Length = EMF_SPEED_BUFFERSIZE_M2;
    EMFCal->SpeedBuffPara2.Inv1 = _IQdiv(_IQ(1.0),_IQ(EMF_SPEED_BUFFERSIZE_M2));
    EMFCal->SpeedBuffPara2.Inv2 = _IQdiv(_IQ(EMF_SPEED_BUFFERSIZE_M2-1),_IQ(EMF_SPEED_BUFFERSIZE_M2));

    EMFCal->Directioncmd = STARTUP_DIRECTION;

    EMFCal->Upper_Lim = EMF_MAX_SPEED_POS;
    EMFCal->Lower_Lim = EMF_MAX_SPEED_NEG;
    EMFCal->Owe_Speed = EMF_MIN_SPEED;

    EMFCal->Timerperiod =EMF_TIMER_PERIOD;

    EMFCal->Cap_Rated = (EMF_TIMER_CLOCK_FREQ)/(6*RATED_FREQ_M)/EMF_TIMER_PRESCALAR;

    EMFCal->Cap_Lowerlim = EMFCal->Cap_Rated*200;
    EMFCal->Cap_Upperlim = EMFCal->Cap_Rated*10/15;
    EMFCal->Compulcntthrsh = EMFCal->Cap_Rated*200;

    EMFCal->Timer_Overflow_thres = EMF_OVERFLOW_THRESHOLD;

    EMFCal->Schangedelayset = EMF_SECTOR_CHANGE_DELAY;

    EMFCal->Schange_cnt_Thre = SCHANGE_Thre;

    EMFCal->Sampledelay_H = EMF_SECTOR_SAMPLE_DELAY ;

    EMFCal->Sprechangeflag = FALSE;

    EMFCal->Sprechangecoef = EMF_SECTOR_CHANGE_COEF;

    EMFCal->Ctrlswitchcntthre = EMF_CONTROLSWITCH_CNTTHRE;

    EMFCal->HighSpeedswitchcntthre = EMF_HIGHSPEEDSWITCH_CNTTHRE;

    EMFCal->Speed_Singlesamp =  EMF_SPEED_SINGLE_M;

    EMFStatus->SpeedstallBand1 = EMF_STALLSPEEDBAND1_M;
    EMFStatus->SpeedstallBand2 = EMF_STALLSPEEDBAND2_M;
    EMFStatus->SpeedstallBand3 = EMF_STALLSPEEDBAND3_M;
    EMFStatus->SpeedstallBand4 = EMF_STALLSPEEDBAND4_M;
    EMFStatus->SpeedstallBand5 = EMF_STALLSPEEDBAND5_M;
    EMFStatus->SpeedstallThresh = EMF_STALLSPEED_M;
    EMFStatus->SpeedstallCntThresh = EMF_STALLCALFREQ_M*EMF_STALL_TIME_M;
    EMFStatus->Currinterval_Set = EMF_STALLCALFREQ_M*EMF_STALL_TIME_M*2;
    EMFStatus->CurrCntThresh = EMF_STALLCALFREQ_M*EMF_STALL_CURRTIME;


    EMFStatus->Speedstallvibra_Cntinterval = EMF_STALL_SPEEDVIBRA_CALFREQ*EMF_STALL_SPEEDVIBRA_CALINTER;
    EMFStatus->Speedstallvibra_CntThresh = EMF_STALL_SPEEDVIBRA_CNTTHRESH;
    EMFStatus->Speedstallvibra_Interval_set = EMF_STALL_TIME_M/EMF_STALL_SPEEDVIBRA_CALINTER;
    EMFStatus->CapminSet = EMFCal->Cap_Rated/UPPER_CAPTURE_TO_RATED;
    EMFStatus->Speedstallvibra_Thresh = EMF_STALL_SPEEDVIBRA_THRESHOLD;
}

/*********************************************************************
 * @fn      EMFObs_Status_Init_M
 *
 * @brief   Initialization of the EMFObs_Status
 *
 * @para    EMFCal: EMF observation result structure pointer
 *          EMFStatus: Observer state structure pointer
 *
 * @return  none
 */
void EMFObs_Status_Init_M(EMF_OBSERVER_TYPE *EMFCal,Observer_Status_Type *EMFStatus)
{
    SYS_TIM_Cmd(DISABLE);

    EMFCal->Speed_Inst = 0;
    EMFCal->Speed_Inst2 = 0;
    EMFCal->Speed_Aver1 =0;
    EMFCal->Speed_Aver2 =0;

    EMFCal->Timercnt = 0;
    EMFCal->Cap_Value_Origin = 0;
    EMFCal->Cap_Value_Total = 0;
    EMFCal->Cap_Value_Revise = 0;
    EMFCal->Speed_Divider =0;
    EMFCal->Timer_Overflow_Cnt = 0;
    EMFCal->Sector = 6;
    EMFCal->Sprechangemark = FALSE;
    EMFCal->Schangemark = FALSE;
    EMFCal->Schangetimer = 0;
    EMFCal->Schange_cnt = 0;
    EMFCal->Schangedelaycnt = EMF_SECTOR_CHANGE_DELAY;

    EMFCal->Sprechangeflag = FALSE;

    EMFCal->changerightflag = FALSE;

    EMFCal->Singlesampleflag = FALSE;

    EMFCal->Compulcnt = 0;

    Buffer_Clear(&(Speedbufffer1[0]), &(EMFCal->SpeedBuffPara1));
    Buffer_Clear(&(Speedbufffer2[0]), &(EMFCal->SpeedBuffPara2));

    EMFStatus->Timeout = FALSE;
    EMFStatus->SpeedstallBand = EMFStatus->SpeedstallBand1;
    EMFStatus->Stall = FALSE;
    EMFStatus->Speedstall_CNT = 0;
    EMFStatus->Currover_CNT =0;
    EMFStatus->Currinterval_CNT =0;

    for(uint8_t i=0;i<6;i++)
          EMFStatus->Capbuff[i] = 0;
    EMFStatus->Captotal = 0;
    EMFStatus->Capmax = 0;
    EMFStatus->Capmin = 0;
    EMFStatus->Capave = 0;

    EMFStatus->Speedstallvibra_Calcnt = 0;
    EMFStatus->Speedstallvibra_Cnt = 0;
    EMFStatus->Speedstallvibra_Interval_num = 0;

    Captimer_Clear();
}

/*********************************************************************
 * @fn      Captimer_Clear
 *
 * @brief   Clear capture TIM cnt
 *
 * @para    none
 *
 * @return  none
 */
__attribute__((section(".highcode")))
void Captimer_Clear(void)
{
    SysTick->CNT =0;
}

/*********************************************************************
 * @fn      Captimer_Cntget
 *
 * @brief   Get capture TIM cnt
 *
 * @para    none
 *
 * @return  none
 */
__attribute__((section(".highcode")))
uint16_t Captimer_Cntget(void)
{
    uint16_t Capvalue =0;

    Capvalue = (SysTick->CNT>>2);

    return(Capvalue);
}

/*********************************************************************
 * @fn      Schange_Delay_Set
 *
 * @brief   Set change phase delay cnt.
 *
 * @para    Delayset - Set delay.
 *          Delaycounter - counter delay.
 *
 * @return  none
 */
__attribute__((section(".highcode")))
void Schange_Delay_Set(uint16_t Delayset, uint16_t *Delaycounter)
{
    *Delaycounter = Delayset;
}

/*********************************************************************
 * @fn      Schange_Delay_Elapse
 *
 * @brief   time of delay detect change phase.
 *
 * @para    Delaycounter - counter delay.
 *
 * @return  none
 */
__attribute__((section(".highcode")))
Truth_Verify_Type Schange_Delay_Elapse(uint16_t Delaycounter)
{
     if (Delaycounter == 0)
       return (TRUE);
     else
       return (FALSE);
}

/*********************************************************************
 * @fn      Schange_Delay_Process
 *
 * @brief   Decrement of variable.
 *
 * @para    none
 *
 * @return  none
 */
__attribute__((section(".highcode")))
void Schange_Delay_Process(void)
{
    if (EMF_Cal_M.Schangedelaycnt != 0)
        EMF_Cal_M.Schangedelaycnt --;
}

/*********************************************************************
 * @fn      reset_tim2_time
 *
 * @brief   Resst TIM2.
 *
 * @para    time
 *
 * @return  none
 */
__attribute__((section(".highcode")))
void reset_tim2_time (uint16_t time)
{
    TIM2->ATRLR = time;
    TIM2->CNT = 0;
    TIM2->CTLR1 |= 0x0001;
}
