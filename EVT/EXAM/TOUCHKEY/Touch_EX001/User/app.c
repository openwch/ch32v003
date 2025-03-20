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

static const uint8_t touch_wheel_ch[ TOUCH_WHEEL_ELEMENTS ] = {TOUCH_WHEEL_CHS};
uint16_t wheel_data[ TOUCH_WHEEL_ELEMENTS ] = {0};

static const uint8_t touch_slidel_ch[ TOUCH_SLIDER_ELEMENTS ] = {TOUCH_SLIDER_CHS};
uint16_t slider_data[ TOUCH_SLIDER_ELEMENTS ] = {0};

touch_button_cfg_t p_selfkey = 
{
    .num_elements = TOUCH_KEY_ELEMENTS,
    .p_elem_index = touch_key_ch,
    .p_stbtn = s_tBtn
};

touch_wheel_cfg_t p_wheel = {
    .num_elements = TOUCH_WHEEL_ELEMENTS,
    .p_elem_index = touch_wheel_ch,
    .threshold = 200,
    .decimal_point_percision = TOUCH_DECIMAL_POINT_PRECISION,
    .wheel_resolution = TOUCH_WHEEL_RESOLUTION,
    .pdata = wheel_data};

touch_slider_cfg_t p_slider = {
    .num_elements = TOUCH_SLIDER_ELEMENTS,
    .p_elem_index = touch_slidel_ch,
    .threshold = 110,
    .decimal_point_percision = TOUCH_DECIMAL_POINT_PRECISION,
    .slider_resolution = TOUCH_SLIDER_RESOLUTION,
    .pdata = slider_data
    };

touch_cfg_t touch_cfg = 
{
    .touch_button_cfg = &p_selfkey,
    .touch_slider_cfg = &p_slider,
    .touch_wheel_cfg = &p_wheel
};
/**********************
 *  STATIC PROTOTYPES
 **********************/
static void TKY_PeripheralInit(void);
void TKY_LineSliderLedProcess (uint16_t pros);
void TKY_WheelSliderLedProcess (uint16_t pros);
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
void TKY_dataProcess (void)
{
    static uint8_t key_val = 0xff;          // key_val_old = 0xff;
    static uint16_t print_time = 0;
    uint16_t Wheel_pros = 0;
    uint16_t Slider_pros = 0;
    static uint8_t touchinsflag = 0;

    if (timerFlag)
    {
        timerFlag = 0;
        touch_Scan();
#if PRINT_EN
        print_time++;
        if (print_time == 500)
        {
            print_time = 0;
            touch_InfoDebug();
        }
#endif
    }
    key_val = touch_GetKey();
    switch (key_val)
    {
        case KEY_0_DOWN :
        case KEY_0_LONG :
            key_val = 0;
            touchinsflag |= 1 << 0;
            TKY_KeyBacklightOut (0, ENABLE);
            break;
        case KEY_1_DOWN :
        case KEY_1_LONG :
            key_val = 1;
            if (!(touchinsflag & 0x0C))
            {
                touchinsflag |= 1 << 1;
            TKY_KeyBacklightOut (7, ENABLE);
            }
            break;
        case KEY_NONE :
            key_val = 0xfe;
            break;
        case KEY_0_UP :
            key_val = 0xff;
            touchinsflag &= ~(1 << 0);
            TKY_KeyBacklightOut (0, DISABLE);
            break;
        case KEY_1_UP :
            key_val = 0xff;
            touchinsflag &= ~(1 << 1);
            TKY_KeyBacklightOut (7, DISABLE);
            break;
        default :
            break;
    }

    if (!(touchinsflag & 0x0A))
    {
        Wheel_pros = touch_GetWheelSliderData ();
        if (Wheel_pros < TOUCH_OFF_VALUE)
        {
            touchinsflag |= 1 << 2;
        }
        else
        {
            touchinsflag &= ~(1 << 2);
        }
        TKY_WheelSliderLedProcess (Wheel_pros);
    }

    if (!(touchinsflag & 0x06))
    {
        Slider_pros = touch_GetLineSliderData();
        if (Slider_pros < TOUCH_OFF_VALUE)
        {
            touchinsflag |= 1 << 3;
        }
        else
        {
            touchinsflag &= ~(1 << 3);
        }
        TKY_LineSliderLedProcess(Slider_pros);
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

void TKY_LineSliderLedProcess (uint16_t pros)
{
    uint8_t idx = 0;
    if (pros != TOUCH_OFF_VALUE)
    {
        if (pros > 20)
            idx = (pros - 20) / 20 + 1;
        else
            idx = (pros + 100) / 20 + 1;
        for (uint8_t i = 1; i < 7; i++)
        {
            if (i == idx)
            {
                TKY_KeyBacklightOut (i, ENABLE);
            }
            else
            {
                TKY_KeyBacklightOut (i, DISABLE);
            }
        }
         dg_log ("%d, %d\n", pros, idx);
    }
    else
    {
        for (uint8_t i = 1; i < 7; i++)
        {
            TKY_KeyBacklightOut (i, DISABLE);
        }
    }
}

void TKY_WheelSliderLedProcess (uint16_t pros)
{
    uint8_t idx = 0;
    if (pros != TOUCH_OFF_VALUE)
    {
        if (pros > 10)
            idx = (pros - 10) / 10 + 8;
        else
            idx = (pros + 110) / 10 + 8;

        for (uint8_t i = 8; i < 20; i++)
        {
            if (idx == i)
            {
                TKY_KeyBacklightOut (i, ENABLE);
            }
            else
            {
                TKY_KeyBacklightOut (i, DISABLE);
            }
        }
        // dg_log ("%d, %d\n", pros, idx);
    }
    else
    {
        for (uint8_t i = 8; i < 20; i++)
        {
            TKY_KeyBacklightOut (i, DISABLE);
        }
    }
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
