/********************************** (C) COPYRIGHT *******************************
 * File Name          : M5_Operation_KeyandLed.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
 * Description        : Key and LED operation
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Includes -----------------------------------------------------------------*/
#include "M0_Control_Library.h"
/* Private typedef ----------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Variables ----------------------------------------------------------------*/

/*********************************************************************
 * @fn      KEY_AND_LEDIO_Init
 *
 * @brief   Button and LED indicator I/O configuration
 *
 * @return  none
 */
void KeyandLED_IOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_AFIO|RCC_PB2Periph_GPIOA|RCC_PB2Periph_GPIOB|RCC_PB2Periph_GPIOC, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

}

/*********************************************************************
 * @fn      KeyandLED_Parameter_Define
 *
 * @brief   Key detection operation parameter definition
 *
 *  @para   KeyM: Key management structure pointer
 *
 * @return  none
 */
void KeyandLED_Parameter_Define(Key_Manager_Type *KeyM)
{
    KeyM->RUN.Effectivetime = (uint16_t)(SRKEY_EFFECTIVE_TIME*KEY_CHECK_FREQ);
    KeyM->RUN.Ineffectivetime = (uint16_t)(SRKEY_INEFFECTIVE_TIME*KEY_CHECK_FREQ);
    KeyM->RUN.Group = GPIOC;
    KeyM->RUN.Pin = GPIO_Pin_0;
    KeyM->RUN.KeyPress=PRESSUP;

    KeyM->Speedadjust.Effectivetime = (uint16_t)(SRKEY_EFFECTIVE_TIME*KEY_CHECK_FREQ);
    KeyM->Speedadjust.Ineffectivetime = (uint16_t)(SRKEY_INEFFECTIVE_TIME*KEY_CHECK_FREQ);
    KeyM->Speedadjust.Group = GPIOB;
    KeyM->Speedadjust.Pin = GPIO_Pin_3;
    KeyM->Speedadjust.KeyPress=PRESSUP;
}

/*********************************************************************
 * @fn      KeyandLED_Status_Init
 *
 * @brief   Key detection operation status initialization
 *
 *  @para   KeyM: Key management structure pointer
 *
 * @return  none
 */
void KeyandLED_Status_Init(Key_Manager_Type *KeyM)
{
    KeyM->Speedadjust.Effectivecnt = 0;
    KeyM->Speedadjust.Ineffectivecnt = 0;
    KeyM->Speedadjust.Status = FALSE;

    KeyM->RUN.Effectivecnt = 0;
    KeyM->RUN.Ineffectivecnt = 0;
    KeyM->RUN.Status = FALSE;
}

/*********************************************************************
 * @fn      Skeycheck_Recover
 *
 * @brief   Recover type Speed key detection
 *
 *  @para   Keyrec: Recove Key structure pointer
 *
 * @return  none
 */
void Skeycheck_Recover(Keyrecover_Type *Keyrec)
{
    if(GPIO_ReadInputDataBit(Keyrec->Group, Keyrec->Pin) == 0)
    {
        if(Keyrec->Effectivecnt <= U16_MAX)
            Keyrec->Effectivecnt++;

        Keyrec->Ineffectivecnt=0;
    }
    else
    {
        if(Keyrec->Ineffectivecnt <= U16_MAX)
            Keyrec->Ineffectivecnt++;

            Keyrec->Effectivecnt=0;
    }
    if((Keyrec->Effectivecnt >= Keyrec->Effectivetime)&&(Keyrec->KeyPress==PRESSUP))
    {
        Keyrec->Status=TRUE;
        Keyrec->Effectivecnt=0;
        Keyrec->KeyPress=PRESSDOWN;
    }
    else if((Keyrec->Ineffectivecnt>=Keyrec->Ineffectivetime))
    {
        Keyrec->Effectivecnt=0;
        Keyrec->Ineffectivecnt=0;
        Keyrec->KeyPress=PRESSUP;
    }

}

/*********************************************************************
 * @fn      KeyCheck_Process
 *
 * @brief   Key detection and Process
 *
 *  @para   KeyM: Key management structure pointer
 *          Speed_Ramp: acceleration and deceleration control structure
 *
 * @return  none
 */
void KeyCheck_Process(Key_Manager_Type *KeyM, Speedrampctr_Type *Speed_Ramp)
{
    Skeycheck_Recover(&(KeyM->RUN));

    if(((RunningStatus_M == RUN)&&(Speed_Ramp->Decemark == FALSE))&&(KeyM->RUN.KeyPress==PRESSUP))
    {
        Skeycheck_Recover(&(KeyM->Speedadjust));
    }
    if(KeyM->RUN.Status == TRUE)
    {
     switch(Operation_M.Checkmode)
     {
         case 0:
             if(( (RunningStatus_M == IDLE)||((RunningStatus_M == FAULT)) )&&(Operation_M.Checkmode ==0))
             {
                   RunningStatus_M = INIT;

                   Speed_Set(Speed_Ramp);

                   Operation_M.Checkmode =1;
             }
             if((Operation_M.Checkmode ==0)&&(Speed_Ramp->Decemark ==TRUE))
             {
                   Speed_Set(Speed_Ramp);

                   Speed_Ramp->Decemark = FALSE;
                   Operation_M.Checkmode = 1;
             }
            break;
        case 1:
            if(Operation_M.Checkmode ==1)
            {
                Speed_Ramp->Preramp = BREAKSPEED;
                Speed_Ramp->Decemark =TRUE;
                Operation_M.Checkmode =0;
            }
            break;
        default:
            break;
       }

     KeyM->RUN.Status = FALSE;
    }
    if((KeyM->Speedadjust.Status == TRUE) &&(Speed_Ramp->Decemark == FALSE))
    {
         Speed_Ramp->Speed_adncnt++;

         if(Speed_Ramp->Speed_adncnt > SPEEDPOINT_RANK)
             Speed_Ramp->Speed_adncnt = 1;

         KeyM->Speedadjust.Status = FALSE;

         Speed_Set(Speed_Ramp);
    }
    if(Speed_Ramp->Decemark ==TRUE)
    {
         if(_IQabs(Speed_Ramp->Aftramp)<=FAST_DECE_SPEED)
         {
             if(RunningStatus_M!=FAULT)
                RunningStatus_M = STOP;
             Speed_Ramp->Decemark = FALSE;
         }
    }
}

