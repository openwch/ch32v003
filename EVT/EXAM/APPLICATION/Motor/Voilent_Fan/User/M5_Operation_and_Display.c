/********************************** (C) COPYRIGHT *******************************
 * File Name          : M5_Operation_and_Display.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/15
 * Description        : Motor_Operation_Process initialization.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Includes -----------------------------------------------------------------*/
#include "M0_Control_Library.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private Functions --------------------------------------------------------*/

/* Private variables --------------------------------------------------------*/
/* Variables ----------------------------------------------------------------*/
Speedrampctr_Type   SpeedRamp_M;
Operation_Type      Operation_M;

static volatile uint16_t Timewait = 0;//Time of delay or waiting

/*********************************************************************
 * @fn      Motor_Operation_Process
 *
 * @brief   Motor Operation Process
 *
 * @para    RunningStatus_M: Motor status
 *
 * @return  none
 */
void Motor_Operation_Process(void)
{
    switch (RunningStatus_M)
    {
        case IDLE:
            {
                RunningStatus_M = INIT;
            }
            break;

        case INIT:
            Motor_Start_Init_M();
            RunningStatus_M = POSITION;
            break;

        case POSITION:
            break;

        case PRESTART:
            break;

        case START:
            break;

        case RUN:
            break;

        case STOP:
            PWM_Enable_M(DISABLE);
            TIM_Cmd(TIM2,DISABLE);
            ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);
            RunningStatus_M = WAIT;
            Time_Delay_Set(Operation_M.StopwaitSet,&Operation_M.StopwaitCnt);
            break;

        case WAIT:
            if (Time_Delay_Elapse(Operation_M.StopwaitCnt) == TRUE)
                RunningStatus_M=IDLE;
            break;

        case FAULT:
            break;

        default:
            break;
    }
}

/*********************************************************************
 * @fn      KEY_AND_LEDIO_Init
 *
 * @brief   GPIO port initialization of key and LED
 *
 * @para
 *
 * @return  none
 */
void KEY_AND_LEDIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_AFIO|RCC_PB2Periph_GPIOA|RCC_PB2Periph_GPIOB|RCC_PB2Periph_GPIOC|RCC_PB2Periph_GPIOD, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/*********************************************************************
 * @fn      Operation_Parameter_Define
 *
 * @brief   Define operation parameter
 *
 * @para    Operastruc: Operation structure pointer
 *
 * @return  none
 */
void Operation_Parameter_Define(Operation_Type *Operastruc)
{
    Operastruc->StopwaitSet = (uint16_t)(STOP_WAITING_TIME * OPERATION_CAL_FREQ);

    Operastruc->StartbuttonSet = (uint16_t)(START_BUTTON_TIME * OPERATION_CAL_FREQ);

    Operastruc->SpeedbuttonSetdown = (uint16_t)(SPEED_BUTTON_TIMEDOWN * OPERATION_CAL_FREQ);

    Operastruc->FaultHoldingSet = (uint16_t)(FAULT_HOLDING_TIME * OPERATION_CAL_FREQ);

    Operastruc->RuntimeSet = (uint32_t)(RUN_LIMIT_TIME * OPERATION_CAL_FREQ);

    Operastruc->SpeedbuttonCnt = 0;

    Operastruc->Startmark = FALSE;
    Operation_M.Restartmark = FALSE;
    Operation_M.Startfirstmark = TRUE;
}

/*********************************************************************
 * @fn      Operation_Status_Init
 *
 * @brief   Initialization operation status
 *
 * @para    Operastruc: Operation structure pointer
 *
 * @return  none
 */
void Operation_Status_Init(Operation_Type *Operastruc)
{
    Operastruc->StopwaitCnt =0;
    Operastruc->FaultHoldingCnt =0;
    Operastruc->RuntimeCnt = 0;

}

/*********************************************************************
 * @fn      Speedrampctr_Parameter_Define
 *
 * @brief   Define speed regular parameter
 *
 * @para    Speed_Ramp: acceleration and deceleration control structure pointer
 *
 * @return  none
 */
void Speedrampctr_Parameter_Define(Speedrampctr_Type *Speed_Ramp)
{
    Speed_Ramp->Speed_adncnt =SPEEDPOINT_DEFAULT;
    Speed_Ramp->Speed_rankset = SPEEDRANK;
    Speed_Ramp->Speed1 = SPEEDPOINT1;
    Speed_Ramp->Speed2 = SPEEDPOINT2;
    Speed_Ramp->Speed3 = SPEEDPOINT3;

    Speed_Ramp->Speed_Stop = SPEEDSTOP;

    Speed_Ramp->SpeedStopflag = FALSE;

    Speed_Setbutton(Speed_Ramp);

    Speed_Ramp->Aftramp = 0;

    Speed_Ramp->AccStep = _IQ(1.0)/(SPEED_LOOP_CAL_FREQ*ACCE_TIME_M);
    Speed_Ramp->DecStep = _IQ(1.0)/(SPEED_LOOP_CAL_FREQ*DECE_TIME_M);
}

/*********************************************************************
 * @fn      SpeedrampInit
 *
 * @brief   Initialization speedramp
 *
 * @para    Speed_Ramp: acceleration and deceleration control structure pointer
 *
 * @return  none
 */
void SpeedrampInit(Speedrampctr_Type *Speed_Ramp)
{
    Speed_Ramp->Aftramp = 0;
    Speed_Ramp->SpeedStopflag = FALSE;
}

/*********************************************************************
 * @fn      Speed_Setbutton
 *
 * @brief   Button set speed
 *
 * @para    Speed_Ramp: acceleration and deceleration control structure pointer
 *
 * @return  none
 */
void Speed_Setbutton(Speedrampctr_Type *Speed_Ramp)
{
    switch(Speed_Ramp->Speed_adncnt)
    {
       case 1:
           Speed_Ramp->Preramp = Speed_Ramp->Speed1;
           break;
       case 2:
           Speed_Ramp->Preramp = Speed_Ramp->Speed2;
           break;
       case 3:
           Speed_Ramp->Preramp = Speed_Ramp->Speed3;
           break;
       default:
           break;
    }
}

/*********************************************************************
 * @fn      Key_Check
 *
 * @brief   Key check
 *
 * @para    Operation_M: Operation structure
 *          SpeedRamp_M: Acceleration and deceleration control structure
 *          RunningStatus_M: System running status enumeration
 *
 * @return  none
 */
void Key_Check(void)
{
    if(Get_Key_Status == 0)
    {
        if(Operation_M.SpeedbuttonCnt <U16_MAX)
            Operation_M.SpeedbuttonCnt++;

        if(Operation_M.SpeedbuttonCnt >= Operation_M.StartbuttonSet)
        {
            if((RunningStatus_M ==IDLE||RunningStatus_M ==FAULT)&&(Operation_M.Checkmode ==0))
            {
                Operation_M.Startmark = TRUE;
            }
            else if(Operation_M.Checkmode ==1)
           {
                SpeedRamp_M.Preramp = SpeedRamp_M.Speed_Stop;
                if(SpeedRamp_M.Speed_adncnt!=0)
                    Time_Delay_Set(Operation_M.StopwaitSet,&Operation_M.StopwaitCnt);
                SpeedRamp_M.Speed_adncnt = 0;
                Operation_M.Startmark = FALSE;

           }
        }
    }
    else
    {
        if(Operation_M.SpeedbuttonCnt >= Operation_M.StartbuttonSet)
        {
            if(Operation_M.Checkmode == 0)
                Operation_M.Checkmode = 1;
            else if(Operation_M.Checkmode == 1)
                Operation_M.Checkmode = 0;
        }
        else if((Operation_M.SpeedbuttonCnt >= Operation_M.SpeedbuttonSetdown)&&(Operation_M.SpeedbuttonCnt < Operation_M.StartbuttonSet))
        {
            if(Operation_M.Startmark == TRUE)
            {
                SpeedRamp_M.Speed_adncnt++;

                if(SpeedRamp_M.Speed_adncnt > SpeedRamp_M.Speed_rankset)
                {
                    SpeedRamp_M.Speed_adncnt = 0;
                    SpeedRamp_M.Preramp = SpeedRamp_M.Speed_Stop;
                    Time_Delay_Set(Operation_M.StopwaitSet,&Operation_M.StopwaitCnt);
                }

                if(SpeedRamp_M.Speed_adncnt != 0)
                {
                    if((RunningStatus_M == IDLE)||(RunningStatus_M == FAULT))
                        RunningStatus_M = INIT;

                    if((RunningStatus_M == STOP)||(RunningStatus_M == WAIT))
                        Operation_M.Restartmark = TRUE;
                }

                Speed_Setbutton(&SpeedRamp_M);
            }
        }

        Operation_M.SpeedbuttonCnt=0;
    }
}

/*********************************************************************
 * @fn      Time_Delay_Process
 *
 * @brief   Time delay process
 *
 * @para    Operation_M: Operation structure
 *
 * @return  none
 */
void Time_Delay_Process()
{
    if (Timewait != 0)
        Timewait --;

    if (Operation_M.StopwaitCnt != 0)
        Operation_M.StopwaitCnt --;

    if (Operation_M.FaultHoldingCnt != 0)
        Operation_M.FaultHoldingCnt--;
}

/*********************************************************************
 * @fn      Time_Wait
 *
 * @brief   Set wait time
 *
 * @para    time
 *
 * @return  none
 */
void Time_Wait(uint16_t time)
{
    Timewait = time;
    while (Timewait != 0)
    {;}
}

/*********************************************************************
 * @fn      Time_Delay_Set
 *
 * @brief   Set delay time
 *
 * @para    Delayset
 *          Delaycounter
 *
 * @return  none
 */
__attribute__((section(".highcode")))
void Time_Delay_Set(uint16_t Delayset, uint16_t *Delaycounter)
{
    *Delaycounter = Delayset;
}

/*********************************************************************
 * @fn      Time_Delay_Elapse
 *
 * @brief   Elapse delay time
 *
 * @para    Delaycounter
 *
 * @return  none
 */
Truth_Verify_Type Time_Delay_Elapse(uint16_t Delaycounter)
{
     if (Delaycounter == 0)
     {
       return (TRUE);
     }
     else
     {
       return (FALSE);
     }
}

