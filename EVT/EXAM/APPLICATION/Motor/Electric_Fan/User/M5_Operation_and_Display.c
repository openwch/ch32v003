/********************************** (C) COPYRIGHT *******************************
 * File Name          : M5_Operation_and_Display.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
 * Description        : Motor operation related processing
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
static volatile uint16_t Timewait = 0;
Speedrampctr_Type   SpeedRamp_M;
Operation_Type      Operation_M;
Powercontrol_Type   Powerctr_M;
Key_Manager_Type    Key_M;
/*********************************************************************
 * @fn      Motor_Operation_Process
 *
 * @brief   Motor operation
 *
 * @return  none
 */
void Motor_Operation_Process(void)
{
    switch (RunningStatus_M)
    {
        case IDLE:
            if (Powerctr_M.Poweron == TRUE)
                Speed_Set(&SpeedRamp_M);
            break;

        case INIT:
            Motor_Start_Init_M();
            RunningStatus_M = PRESTART;
            break;

        case PRESTART:
            break;

        case POSITION:
            break;

        case START:
            break;

        case RUN:
            break;

        case STOP:
            TIM_CtrlPWMOutputs(TIM1, DISABLE);
            ADC_Start(DISABLE);
            RunningStatus_M = WAIT;
            Time_Delay_Set(Operation_M.StopwaitSet,&Operation_M.StopwaitCnt);
		   break;

        case WAIT:
            if (Time_Delay_Elapse(Operation_M.StopwaitCnt) == TRUE)
                RunningStatus_M=IDLE;
            break;

        case FAULT:
            TIM_CtrlPWMOutputs(TIM1, DISABLE);
            ADC_Start(DISABLE);
            break;

        default:
            break;
    }
}

/*********************************************************************
 * @fn      Poweroperation_Initial
 *
 * @brief   Power-on and power-off parameters and status initialization
 *
 *  @para   PowerOper: Power-on and power-off control structure pointer
 *
 * @return  none
 */
void Poweroperation_Initial(Powercontrol_Type *PowerOper)
{
    PowerOper->Poweron_CntThresh = (uint16_t)(POWERVERY_FREQ*POWERON_TIME);
    PowerOper->Poweron_Volt = POWERON_VOLT;
    PowerOper->Poweroff_Volt = POWEROFF_VOLT;
    PowerOper->Poweron = FALSE;
    PowerOper->Poweron_Cnt = 0;

    PowerOper->DCVoltAverSizeInv1 = _IQ(1.0)/POWERVERY_DCVOLTAVERSIZE;
    PowerOper->DCVoltAverSizeInv2 = _IQ(1.0) - PowerOper->DCVoltAverSizeInv1;
    PowerOper->DCVoltAver = 0;
}

/*********************************************************************
 * @fn      Operation_Parameter_Define
 *
 * @brief   Operation parameter definition
 *
 *  @para   Operation: Operation control structure pointer
 *
 * @return  none
 */
void Operation_Parameter_Define(Operation_Type *Operation)
{
    Operation->StopwaitSet = (uint16_t)(STOP_WAITING_TIME * OPERATION_CAL_FREQ);
    Operation->FaultHoldingSet = (uint16_t)(FAULT_HOLDING_TIME * OPERATION_CAL_FREQ);
    Operation->Startmark = FALSE;
    Operation->Checkmode = 0;
}

/*********************************************************************
 * @fn      Operation_Status_Init
 *
 * @brief   Operation status initialization
 *
 *  @para   Operation: Operation control structure pointer
 *
 * @return  none
 */
void Operation_Status_Init(Operation_Type *Operation)
{
    Operation->StopwaitCnt = 0;
    Operation->FaultHoldingCnt = 0;
}

/*********************************************************************
 * @fn      Speedrampctr_Parameter_Define
 *
 * @brief   Speed control parameter definition
 *
 *  @para   Speed_Ramp: Acceleration and deceleration control structure pointer
 *
 * @return  none
 */
void Speedrampctr_Parameter_Define(Speedrampctr_Type *Speed_Ramp)
{
    Speed_Ramp->Direction = STARTUP_DIRECTION;
    Speed_Ramp->Speed_adncnt = SPEEDPOINT_DEFAULT;

    Speed_Ramp->Aftramp = SPEEDPOINT1*Speed_Ramp->Direction;

    Speed_Ramp->AccStep = _IQ(1.0)/(SPEED_LOOP_CAL_FREQ*ACCE_TIME_M);
    Speed_Ramp->DecStep = _IQ(1.0)/(SPEED_LOOP_CAL_FREQ*DECE_TIME_M);
}

/*********************************************************************
 * @fn      Speedrampctr_Initial
 *
 * @brief   Speed control initialization
 *
 *  @para   Speed_Ramp: Acceleration and deceleration control structure pointer
 *
 * @return  none
 */
void Speedrampctr_Initial(Speedrampctr_Type *Speed_Ramp)
{
    Speed_Set(Speed_Ramp);
    Speed_Ramp->Aftramp = SPEEDPOINT1*Speed_Ramp->Direction;
    Speed_Ramp->Startfinish =  FALSE;
    Speed_Ramp->Decemark =FALSE;
}

/*********************************************************************
 * @fn      Speed_Set
 *
 * @brief   Set speed according speed rank
 *
 *  @para   Speed_Ramp: Acceleration and deceleration control structure pointer
 *
 * @return  none
 */
void Speed_Set(Speedrampctr_Type *Speed_Ramp)
{
    switch(Speed_Ramp->Speed_adncnt)
    {
        case 1:
           Speed_Ramp->Preramp =  SPEEDPOINT1*Speed_Ramp->Direction;
           break;
       case 2:
           Speed_Ramp->Preramp =  SPEEDPOINT2*Speed_Ramp->Direction;
           break;
       case 3:
           Speed_Ramp->Preramp =  SPEEDPOINT3*Speed_Ramp->Direction;
           break;
       case 4:
           Speed_Ramp->Preramp =  SPEEDPOINT4*Speed_Ramp->Direction;
           break;
       default:
           break;
    }
}

/*********************************************************************
 * @fn      Poweroperation_Process
 *
 * @brief   Power-on control logic
 *
 *  @para   Speed_Ramp: Acceleration and deceleration control structure pointer
 *          ADCStruc:ADC sampling structure pointer
 *          PowerOper:power-on and power-off control structure pointer
 *          Runningstatus: running status
 *          Speedact actual speed;
 *
 * @return  none
 */
void Poweroperation_Process(Speedrampctr_Type *SpeedRamp,ADCStruc_Type *ADCStruc,Powercontrol_Type *PowerOper,\
                    Runningstatus_Type *Runningstatus,_iq24 Speedact)
{
    PowerOper->DCVoltAver = _IQMPY_MACRO(PowerOper->DCVoltAver,PowerOper->DCVoltAverSizeInv2)\
                            + _IQMPY_MACRO(ADCStruc->DCVoltPres,PowerOper->DCVoltAverSizeInv1);

    if((_IQabs(SpeedRamp->Aftramp) >= START_FINISH_SPEED)&&(_IQabs(Speedact) >= START_FINISH_SPEED))
        SpeedRamp->Startfinish = TRUE;

    if( (PowerOper->DCVoltAver <= PowerOper->Poweroff_Volt))
    {
        switch(*Runningstatus)
        {
            case INIT:
            case DIRCHECK:
            case PRESTART:
            case POSITION:
            case START:
            case RUN:
                *Runningstatus = STOP;
                PowerOper->Poweron = FALSE;
                break;
        }
    }
    if((PowerOper->Poweron == FALSE))
    {
        if( PowerOper->DCVoltAver > PowerOper->Poweron_Volt)
        {
            PowerOper->Poweron_Cnt++;
            if(PowerOper->Poweron_Cnt >= PowerOper->Poweron_CntThresh)
            {
                PowerOper->Poweron = TRUE;
                PowerOper->Poweron_Cnt = 0;
            }
        }
    }
}

/*********************************************************************
 * @fn      Time_Delay_Process
 *
 * @brief   Variables decrement processing based on interrupt time (0.5ms)
 *
 * @return  none
 */
void Time_Delay_Process(void)
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
 * @brief   The function wait for a delay to be over.
 *
 *  @para   time: Time Wait set value
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
 * @brief   The function wait for a delay to be over.
 *
 *  @para   Delayset: Time delay set value
 *          Delaycounter:timing count variable pointer
 *
 * @return  none
 */
void Time_Delay_Set(uint16_t Delayset, uint16_t *Delaycounter)
{
    *Delaycounter = Delayset;
}

/*********************************************************************
 * @fn      Time_Delay_Elapse
 *
 * @brief   Time delay elapse check
 *
 *  @para   Delaycounter: Timing delay counter variable
 *
 * @return  Enumeration of true and false
 */
Truth_Verify_Type Time_Delay_Elapse(uint16_t Delaycounter)
{
     if (Delaycounter == 0)
       return (TRUE);
     else
       return (FALSE);
}
