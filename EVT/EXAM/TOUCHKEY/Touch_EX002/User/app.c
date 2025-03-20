/********************************** (C) COPYRIGHT *******************************
 * File Name          : app.c
 * Author             : WCH
 * Version            : V1.1
 * Date               : 2025/01/14
 * Description        : Touch Key Routines
 *******************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "Touch.h"
#include "app.h"
#include "LED.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      VARIABLES
 **********************/
uint8_t volatile timerFlag = 0;
uint8_t volatile TKY_ScanFlag = 0;
extern volatile uint8_t led_scanflag;

static const uint8_t touch_key_ch[ TOUCH_KEY_ELEMENTS ] = {TOUCH_KEY_CHS};
KEY_T s_tBtn[TOUCH_KEY_ELEMENTS] = {0};

touch_button_cfg_t p_selfkey = 
{
    .num_elements = TOUCH_KEY_ELEMENTS,
    .p_elem_index = touch_key_ch,
    .p_stbtn = s_tBtn
};

touch_cfg_t touch_cfg = 
{
    .touch_button_cfg = &p_selfkey,
    .touch_slider_cfg = NULL,
    .touch_wheel_cfg = NULL
};
/**********************
 *  STATIC PROTOTYPES
 **********************/
static void TKY_PeripheralInit(void);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*********************************************************************
 * @fn      touch_dataProcess
 *
 * @brief   Touch data processing function (bare bones run), prints the key triggers obtained
 *
 * @return  none
 */
void TKY_dataProcess(void)
{
	static uint8_t key_val = 0xff;//key_val_old = 0xff;
    static uint16_t print_time = 0;

    if(timerFlag)
    {
        timerFlag = 0;
        touch_Scan();
#if PRINT_EN
        print_time++;
        if(print_time == 500)
        {
            print_time = 0;
            touch_InfoDebug();
        }
#endif
    }
    key_val = touch_GetKey();
    switch(key_val)
    {
       case KEY_NONE   :   break;
       case KEY_0_DOWN :   TKY_KeyBacklightOut(0,ENABLE);  PRINT("KEY_1_DOWN !\n");break;
       case KEY_0_LONG :   TKY_KeyBacklightOut(0,ENABLE);  PRINT("KEY_1_LONG !\n");break;
       case KEY_0_UP   :   TKY_KeyBacklightOut(0,DISABLE); PRINT("KEY_1_UP   !\n");break;
       case KEY_1_DOWN :   TKY_KeyBacklightOut(1,ENABLE);  PRINT("KEY_2_DOWN !\n");break;
       case KEY_1_LONG :   TKY_KeyBacklightOut(1,ENABLE);  PRINT("KEY_2_UP   !\n");break;
       case KEY_1_UP   :   TKY_KeyBacklightOut(1,DISABLE); PRINT("KEY_2_LONG !\n");break;
       case KEY_2_DOWN :   TKY_KeyBacklightOut(2,ENABLE);  PRINT("KEY_3_DOWN !\n");break;
       case KEY_2_LONG :   TKY_KeyBacklightOut(2,ENABLE);  PRINT("KEY_3_UP   !\n");break;
       case KEY_2_UP   :   TKY_KeyBacklightOut(2,DISABLE); PRINT("KEY_3_LONG !\n");break;
       case KEY_3_DOWN :   TKY_KeyBacklightOut(3,ENABLE);  PRINT("KEY_4_DOWN !\n");break;
       case KEY_3_LONG :   TKY_KeyBacklightOut(3,ENABLE);  PRINT("KEY_4_UP   !\n");break;
       case KEY_3_UP   :   TKY_KeyBacklightOut(3,DISABLE); PRINT("KEY_4_LONG !\n");break;
       case KEY_4_DOWN :   TKY_KeyBacklightOut(4,ENABLE);  PRINT("KEY_5_DOWN !\n");break;
       case KEY_4_LONG :   TKY_KeyBacklightOut(4,ENABLE);  PRINT("KEY_5_UP   !\n");break;
       case KEY_4_UP   :   TKY_KeyBacklightOut(4,DISABLE); PRINT("KEY_5_LONG !\n");break;
       case KEY_5_DOWN :   TKY_KeyBacklightOut(5,ENABLE);  PRINT("KEY_6_DOWN !\n");break;
       case KEY_5_LONG :   TKY_KeyBacklightOut(5,ENABLE);  PRINT("KEY_6_UP   !\n");break;
       case KEY_5_UP   :   TKY_KeyBacklightOut(5,DISABLE); PRINT("KEY_6_LONG !\n");break;
       case KEY_6_DOWN :   TKY_KeyBacklightOut(6,ENABLE);  PRINT("KEY_7_DOWN !\n");break;
       case KEY_6_LONG :   TKY_KeyBacklightOut(6,ENABLE);  PRINT("KEY_7_UP   !\n");break;
       case KEY_6_UP   :   TKY_KeyBacklightOut(6,DISABLE); PRINT("KEY_7_LONG !\n");break;
       case KEY_7_DOWN :   TKY_KeyBacklightOut(7,ENABLE);  PRINT("KEY_8_DOWN !\n");break;
       case KEY_7_LONG :   TKY_KeyBacklightOut(7,ENABLE);  PRINT("KEY_8_UP   !\n");break;
       case KEY_7_UP   :   TKY_KeyBacklightOut(7,DISABLE); PRINT("KEY_8_LONG !\n");break;
       default : break;
    }
}

/*********************************************************************
 * @fn      Timer_Init
 *
 * @brief   Timer Init function
 *
 * @param   none
 *
 * @return  none
 */
void Timer_Init(void)
{
    PRINT("%s\r\n", __FUNCTION__);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_PB1PeriphClockCmd(RCC_PB1Periph_TIM2, ENABLE);
    //--TIMER4--
    TIM_TimeBaseInitStructure.TIM_Period = SystemCoreClock/1000000 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1000 - 1;                    
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;          
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM2, ENABLE);
}
/*********************************************************************
 * @fn      touch_init
 *
 * @brief   Touch initialization function (does not use tmos, requires device to turn on timer)
 *
 * @return  none
 */
void TKY_Init(void)
{
	TKY_PeripheralInit();       /* Initialize peripherals such as backlight, buzzer, etc. */

	touch_Init(&touch_cfg);

     TKY_SetSleepStatusValue( ~tkyQueueAll );

    Timer_Init();               //Timing period of 1ms

    dg_log("Touch Key init Finish!\n");
}


/**********************
 *   STATIC FUNCTIONS
 **********************/

/*********************************************************************
 * @fn      TKY_PeripheralInit
 *
 * @brief   Touch-related peripheral initialization functions
 *
 * @return  none
 */
static void TKY_PeripheralInit(void)
{
    /*You code here*/
    TKY_BacklightInit();
    
}

/********************************************
 * @fn      TIM3_UP_IRQHandler
 *
 * @brief   This function handles TIM1 interrupt.
 *
 * @return  none
 */
__attribute__((interrupt("WCH-Interrupt-fast"))) void TIM2_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);        
    TKY_ScanFlag++;
    timerFlag++;
    if (led_scanflag) {
        /*led scan*/
        TKY_BacklightProcess();
    }
}
