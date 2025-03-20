/********************************** (C) COPYRIGHT *******************************
 * File Name          : Touch.C
 * Author             : WCH
 * Version            : V1.6
 * Date               : 2025/01/21
 * Description        : Touch Key Libraries
 *******************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "Touch.h"

/*********************
 *      DEFINES
 *********************/
#define WAKEUPTIME 50          // Sleep Time = 250 * SLEEP_TRIGGER_TIME(100ms) = 25s

/**********************
 *      VARIABLES
 **********************/
__attribute__ ((aligned (4))) uint32_t TKY_MEMBUF[ (TKY_MEMHEAP_SIZE - 1) / 4 + 1 ] = {0};
static uint16_t keyData = 0;
static uint16_t WheelData = TOUCH_OFF_VALUE;
static uint16_t SilderData = TOUCH_OFF_VALUE;

static touch_cfg_t *p_touch_cfg = NULL;

uint8_t wakeUpCount = 0, wakeupflag = 0;

uint32_t tkyPinAll = 0;
uint16_t tkyQueueAll = 0;
static const TKY_ChannelInitTypeDef my_tky_ch_init[ TKY_QUEUE_END ] = {TKY_CHS_INIT};


typedef struct
{
    GPIO_TypeDef* GPIOx;
    uint32_t GPIO_Pin;
    uint32_t GPIO_CFG_MASK;
    uint32_t GPIO_CFG_OUT;
} TKY_CH_GPIO_S;

/*Sort gpio by touch channel hardware serial number*/
const TKY_CH_GPIO_S TKY_Pin[ 8 ] = {
    {.GPIOx = GPIOA, .GPIO_Pin = GPIO_Pin_2, .GPIO_CFG_MASK = 0xfffff0ff, .GPIO_CFG_OUT = 0x00000100}, //  PA2,ADC0
    {.GPIOx = GPIOA, .GPIO_Pin = GPIO_Pin_1, .GPIO_CFG_MASK = 0xffffff0f, .GPIO_CFG_OUT = 0x00000010}, //  PA1,ADC1

    {.GPIOx = GPIOC, .GPIO_Pin = GPIO_Pin_4, .GPIO_CFG_MASK = 0xfff0ffff, .GPIO_CFG_OUT = 0x00010000}, //  PC4,ADC2

    {.GPIOx = GPIOD, .GPIO_Pin = GPIO_Pin_2, .GPIO_CFG_MASK = 0xfffff0ff, .GPIO_CFG_OUT = 0x00000100}, //  PD2,ADC3
    {.GPIOx = GPIOD, .GPIO_Pin = GPIO_Pin_3, .GPIO_CFG_MASK = 0xffff0fff, .GPIO_CFG_OUT = 0x00001000}, //  PD3,ADC4
    {.GPIOx = GPIOD, .GPIO_Pin = GPIO_Pin_5, .GPIO_CFG_MASK = 0xff0fffff, .GPIO_CFG_OUT = 0x00100000}, //  PD5,ADC5
    {.GPIOx = GPIOD, .GPIO_Pin = GPIO_Pin_6, .GPIO_CFG_MASK = 0xf0ffffff, .GPIO_CFG_OUT = 0x01000000}, //  PD6,ADC6
    {.GPIOx = GPIOD, .GPIO_Pin = GPIO_Pin_4, .GPIO_CFG_MASK = 0xfff0ffff, .GPIO_CFG_OUT = 0x00010000}, //  PD4,ADC7
};

/**********************
 *  STATIC PROTOTYPES
 **********************/

static KEY_FIFO_T s_tKey; /* ¡ã¡ä?¨¹FIFO¡À?¨¢?,?¨¢11¨¬? */
static void touch_InitHard(void);
static void touch_InitVar(touch_cfg_t *p);
static void touch_PutKey(uint8_t _KeyCode);
static void touch_DetectKey(touch_button_cfg_t * p);
static void touch_Regcfg (void);
static void touch_Baseinit (void);
static void touch_Channelinit (void);
static uint16_t touch_DetecLineSlider(touch_slider_cfg_t * p_slider);
static uint16_t touch_DetectWheelSlider (touch_wheel_cfg_t * p_wheel);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/


/*********************************************************************
 * @fn      touch_InitKey
 *
 * @brief   Initialize the keys. This function is called by tky_Init().
 *
 * @param   none
 *
 * @return  none
 */
void touch_Init(touch_cfg_t *p)
{
    touch_InitHard();
    touch_InitVar(p);
}

/*********************************************************************
 * @fn      touch_PutKey
 *
 * @brief   Presses 1 key value into the key FIFO buffer. Can be used to simulate a key.
 *
 * @param   _KeyCode - Key Code
 *
 * @return  none
 */
static void touch_PutKey(uint8_t _KeyCode)
{
    s_tKey.Buf[ s_tKey.Write ] = _KeyCode;

    if (++s_tKey.Write >= KEY_FIFO_SIZE)
    {
        s_tKey.Write = 0;
    }
}

/*********************************************************************
 * @fn      touch_GetKey
 *
 * @brief   Read a key value from the key FIFO buffer.
 *
 * @param   none
 *
 * @return  key code
 */
uint8_t touch_GetKey (void)
{
    uint8_t ret;

    if (s_tKey.Read == s_tKey.Write)
    {
        return KEY_NONE;
    }
    else
    {
        ret = s_tKey.Buf[ s_tKey.Read ];

        if (++s_tKey.Read >= KEY_FIFO_SIZE)
        {
            s_tKey.Read = 0;
        }
        return ret;
    }
}

/*********************************************************************
 * @fn      touch_GetKeyState
 *
 * @brief   Read the status of the keys.
 *
 * @param   _ucKeyID - Key IDs, starting from 0
 *
 * @return  1 - press a button
 *          0 - not pressed
 */
uint8_t touch_GetKeyState (KEY_ID_E _ucKeyID)
{
    return p_touch_cfg->touch_button_cfg->p_stbtn[_ucKeyID].State;
}


/*********************************************************************
 * @fn      touch_SetKeyParam
 *
 * @brief   Setting key parameters.
 *
 * @param   _ucKeyID     - Key IDs, starting from 0
 *          _LongTime    - Long press event time
 *          _RepeatSpeed - burst speed
 *
 * @return  none
 */
void touch_SetKeyParam (uint8_t _ucKeyID, uint16_t _LongTime, uint8_t _RepeatSpeed)
{
    p_touch_cfg->touch_button_cfg->p_stbtn[_ucKeyID].LongTime = _LongTime;
    p_touch_cfg->touch_button_cfg->p_stbtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;
    p_touch_cfg->touch_button_cfg->p_stbtn[_ucKeyID].RepeatCount = 0;
}

/*********************************************************************
 * @fn      touch_ClearKey
 *
 * @brief   Empty key FIFO buffer.
 *
 * @param   none
 *
 * @return  none
 */
void touch_ClearKey (void)
{
    s_tKey.Read = s_tKey.Write;
}


/*********************************************************************
 * @fn      touch_ScanWakeUp
 *
 * @brief   Touch Scan Wakeup Function.
 *
 * @param   none
 *
 * @return  none
 */
void touch_ScanWakeUp (void)
{
    wakeUpCount = WAKEUPTIME;
    wakeupflag = 1;

    dg_log ("wake up for a while\n");
    TKY_SaveAndStop();
    touch_GPIOSleep();
}


/*********************************************************************
 * @fn      touch_ScanEnterSleep
 *
 * @brief   Touch Scan Sleep Function.
 *
 * @param   none
 *
 * @return  none
 */
void touch_ScanEnterSleep (void)
{
    TKY_SaveAndStop();
    touch_GPIOSleep();
    wakeupflag = 0;
    dg_log ("Ready to sleep\n");
}

/*********************************************************************
 * @fn      touch_KeyScan
 *
 * @brief   Scans all keys. Non-blocking, called periodically by systick interrupt.
 *
 * @param   none
 *
 * @return  none
 */
void touch_Scan(void)
{

    TKY_LoadAndRun();          //---Load touch settings before initiating conversion---
    keyData = TKY_PollForFilter();
    TKY_SaveAndStop();
#if TKY_SLEEP_EN
    if (keyData)
    {
        wakeUpCount = WAKEUPTIME;
    }
#endif

    touch_DetectKey(p_touch_cfg->touch_button_cfg);

    WheelData = touch_DetectWheelSlider(p_touch_cfg->touch_wheel_cfg);

    SilderData = touch_DetecLineSlider(p_touch_cfg->touch_slider_cfg);
}


/*********************************************************************
 * @fn      touch_GPIOModeCfg
 *
 * @brief   Touch Key Mode Configuration.
 *
 * @param   mode - key mode
 *          channel - touch channel
 *
 * @return  none
 */
void touch_GPIOModeCfg (GPIOMode_TypeDef mode, uint32_t channel)
{
    switch (mode)
    {
        case GPIO_Mode_AIN :
            TKY_Pin[ channel ].GPIOx->CFGLR &= TKY_Pin[ channel ].GPIO_CFG_MASK;
            break;
        case GPIO_Mode_Out_PP :
            TKY_Pin[ channel ].GPIOx->CFGLR |= TKY_Pin[ channel ].GPIO_CFG_OUT;
            TKY_Pin[ channel ].GPIOx->BCR |= TKY_Pin[ channel ].GPIO_Pin;
            break;
        default :
            break;
    }
}


/**********************
 *   STATIC FUNCTIONS
 **********************/

/*********************************************************************
 * @fn      touch_InitKeyHard
 *
 * @brief   Initializing Touch Keys.
 *
 * @param   none
 *
 * @return  none
 */
static void touch_InitHard (void)
{
    touch_Regcfg();
    touch_Baseinit();
    touch_Channelinit();
}

/*********************************************************************
 * @fn      touch_InitKeyVar
 *
 * @brief   Initialize Touch Key Variables.
 *
 * @param   none
 *
 * @return  none
 */
static void touch_InitVar(touch_cfg_t *p)
{
    uint8_t i;

    p_touch_cfg = p;


    s_tKey.Read = 0;
    s_tKey.Write = 0;


    for (i = 0; i < p_touch_cfg->touch_button_cfg->num_elements; i++)
    {
        p_touch_cfg->touch_button_cfg->p_stbtn[i].LongTime = KEY_LONG_TIME;             
        p_touch_cfg->touch_button_cfg->p_stbtn[i].Count = KEY_FILTER_TIME / 2;        
        p_touch_cfg->touch_button_cfg->p_stbtn[i].State = 0;                            
        p_touch_cfg->touch_button_cfg->p_stbtn[i].RepeatSpeed = 0;                     
        p_touch_cfg->touch_button_cfg->p_stbtn[i].RepeatCount = 0;                   
    }


}


/*********************************************************************
 * @fn      touch_InfoDebug
 *
 * @brief   Touch data print function.
 *
 * @param   none
 *
 * @return  none
 */
void touch_InfoDebug (void)
{
    uint8_t i;
    int16_t data_dispNum[ TKY_MAX_QUEUE_NUM ] = {0};

    for (i = 0; i < TKY_MAX_QUEUE_NUM; i++)
    {
        data_dispNum[ i ] = TKY_GetCurQueueValue (i);
    }

    for (i = 0; i < TKY_MAX_QUEUE_NUM; i++)
    {
        dg_log ("%04d,", data_dispNum[ i ]);
    } dg_log("\n");

    for (i = 0; i < TKY_MAX_QUEUE_NUM; i++)
    {
        data_dispNum[ i ] = TKY_GetCurQueueBaseLine (i);
    }

    for (i = 0; i < TKY_MAX_QUEUE_NUM; i++)
    {
        dg_log ("%04d,", data_dispNum[ i ]);
    } dg_log("\n");

    for (i = 0; i < TKY_MAX_QUEUE_NUM; i++)
    {
        dg_log ("%04d,", TKY_GetCurQueueRealVal (i));
    }dg_log("\r\n");
    dg_log ("\r\n");

}

/*********************************************************************
 * @fn      touch_DetectKey
 *
 * @brief   Detects a keystroke. Non-blocking state, must be called periodically.
 *
 * @param   i - Pointer to key structure variable
 *
 * @return  none
 */
static void touch_DetectKey (touch_button_cfg_t* p)
{
    KEY_T* pBtn;

    if (p == NULL)
    {
        return;
    }

    for (uint8_t i = 0; i < p->num_elements; i++)
    {
        /*°´¼ü°´ÏÂ*/
        pBtn = NULL;
        pBtn = &p_touch_cfg->touch_button_cfg->p_stbtn[ i ];
        if (keyData & (1 << p->p_elem_index[ i ]))          // pBtn->IsKeyDownFunc()==1
        {
            if (pBtn->State == 0)
            {
                pBtn->State = 1;
#if !KEY_MODE
            /* Send a button press message */
            touch_PutKey ((uint8_t) (3 * i + 1));
#endif
        }

        /*Handling long keystrokes*/
        if (pBtn->LongTime > 0)
        {
            if (pBtn->LongCount < pBtn->LongTime)
            {
                /* Sending a key long-pressed message */
                if (++pBtn->LongCount == pBtn->LongTime)
                {
#if !KEY_MODE
                    pBtn->State = 2;

                    /* Key value into key FIFO */
                    touch_PutKey ((uint8_t) (3 * i + 3));
#endif
                }
            }
            else
            {
                if (pBtn->RepeatSpeed > 0)
                {
                    if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
                    {
                        pBtn->RepeatCount = 0;
#if !KEY_MODE
                        /* After a long keystroke, send 1 keystroke every pBtn->RepeatSpeed*10ms */
                        touch_PutKey ((uint8_t) (3 * i + 1));
#endif
                    }
                }
            }
        }
    }
    else
    {
        if (pBtn->State)
        {
#if KEY_MODE
            if (pBtn->State == 1)
                /* Sends a key press message */
                touch_PutKey ((uint8_t) (3 * i + 1));
#endif
            pBtn->State = 0;

#if !KEY_MODE
            /* Sends a button pop-up message after releasing the button KEY_FILTER_TIME */
            touch_PutKey ((uint8_t) (3 * i + 2));
#endif
        }

        pBtn->LongCount = 0;
        pBtn->RepeatCount = 0;
        }
    }
}
/*********************************************************************
 * @fn      touch_Regcfg
 *
 * @brief   touch configration.
 *
 * @param   none
 *
 * @return  none
 */
static void touch_Regcfg (void)
{
    ADC_InitTypeDef ADC_InitStructure = {0};
    uint8_t i = 0;

    RCC_PB2PeriphClockCmd (RCC_PB2Periph_GPIOA | RCC_PB2Periph_GPIOC | RCC_PB2Periph_GPIOD, ENABLE);
    RCC_PB2PeriphClockCmd (RCC_PB2Periph_ADC1, ENABLE);
    for (i = 0; i < TKY_MAX_QUEUE_NUM; i++)
    {
        uint32_t tmpreg1 = 0, tmpreg2 = 0;
#if TKY_SHIELD_EN
        // Prior to testing, the overall channel is set to analog input
        touch_GPIOModeCfg (GPIO_Mode_AIN, my_tky_ch_init[ i ].channelNum);
#else
        // Push-pull ground discharge of the entire channel prior to testing
        touch_GPIOModeCfg (GPIO_Mode_Out_PP, my_tky_ch_init[ i ].channelNum);
#endif

        tmpreg1 = ADC1->SAMPTR2;
        tmpreg2 = ((uint32_t) 0x00000007) << (3 * my_tky_ch_init[ i ].channelNum);
        tmpreg1 &= ~tmpreg2;
        tmpreg2 = (uint32_t) ADC_SampleTime_CyclesMode0 << (3 * my_tky_ch_init[ i ].channelNum);
        tmpreg1 |= tmpreg2;
        ADC1->SAMPTR2 = tmpreg1;
    }

    ADC_DeInit (ADC1);

    RCC_ADCCLKConfig (RCC_PCLK2_Div8);

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init (ADC1, &ADC_InitStructure);

    ADC_Cmd (ADC1, ENABLE);

    ADC1->CTLR1 |= (1 << 24) | (1 << 25) | (1 << 26);                                                      // Enable ADC Buffer And TouchKey

#if TKY_SHIELD_EN
    for (uint8_t i = 0; i < TKY_MAX_QUEUE_NUM; i++)
    {
        printf ("channelNum:%d\r\n", my_tky_ch_init[ i ].channelNum);
        ADC_TKey_ChannelxMulShieldCmd (ADC1, my_tky_ch_init[ i ].channelNum, ENABLE);          // Enable MulShield Channelx
    }
    ADC_TKey_MulShieldCmd (ADC1, ENABLE);                                                      // Enable MulShield
#endif
    TKY_SaveCfgReg();
}

/*********************************************************************
 * @fn      touch_Baseinit
 *
 * @brief   Touch base library initialization.
 *
 * @param   none
 *
 * @return  none
 */
static void touch_Baseinit (void)
{
    uint8_t sta=0xff;
    TKY_BaseInitTypeDef TKY_BaseInitStructure = {0};

    //----------Initialization of touch key base settings--------
    TKY_BaseInitStructure.filterMode = TKY_FILTER_MODE;
    TKY_BaseInitStructure.shieldEn = TKY_SHIELD_EN;
    TKY_BaseInitStructure.singlePressMod = TKY_SINGLE_PRESS_MODE;
    TKY_BaseInitStructure.filterGrade = TKY_FILTER_GRADE;
    TKY_BaseInitStructure.maxQueueNum = TKY_MAX_QUEUE_NUM;
    TKY_BaseInitStructure.baseRefreshOnPress = TKY_BASE_REFRESH_ON_PRESS;
    /*---Speed of baseline update baseRefreshSampleNum and filterGrade Inversely proportional to the baseline update speed,
    the baseline update speed is also related to the code structure, which can be observed through the function GetCurQueueBaseLine---*/
    TKY_BaseInitStructure.baseRefreshSampleNum = TKY_BASE_REFRESH_SAMPLE_NUM;
    TKY_BaseInitStructure.baseUpRefreshDouble = TKY_BASE_UP_REFRESH_DOUBLE;
    TKY_BaseInitStructure.baseDownRefreshSlow = TKY_BASE_DOWN_REFRESH_SLOW;
    TKY_BaseInitStructure.tkyBufP = TKY_MEMBUF;
    sta = TKY_BaseInit( TKY_BaseInitStructure );
    dg_log("TKY_BaseInit:%02X\r\n",sta);
}

/*********************************************************************
 * @fn      touch_Channelinit
 *
 * @brief   Touch Channel Initialization.
 *
 * @param   none
 *
 * @return  none
 */
static void touch_Channelinit (void)
{
    uint8_t error_flag = 0;
    uint16_t chx_mean = 0,chx_mean_last = 0;
    for (uint8_t i = 0; i < TKY_MAX_QUEUE_NUM; i++)
    {
        TKY_CHInit (my_tky_ch_init[ i ]);
    }

    for (uint8_t i = 0; i < TKY_MAX_QUEUE_NUM; i++)
    {

        chx_mean = TKY_GetCurChannelMean (my_tky_ch_init[ i ].channelNum, my_tky_ch_init[ i ].chargeTime,
                                          my_tky_ch_init[ i ].disChargeTime, 1000);

        if (chx_mean < 3400 || chx_mean > 3800)
        {
            error_flag = 1;
        }
        else
        {
            TKY_SetCurQueueBaseLine (i, chx_mean);
        }
    }
    // Charge/discharge baseline value abnormal, recalibrate baseline value
    if (error_flag != 0)
    {
        dg_log ("\n\nCharging parameters error, preparing for recalibration ...\n\n");
        uint16_t charge_time;
        for (uint8_t i = 0; i < TKY_MAX_QUEUE_NUM; i++)
        { 
            charge_time = 0, chx_mean = 0;
#if !(TKY_SHIELD_EN)
            touch_GPIOModeCfg (GPIO_Mode_AIN, my_tky_ch_init[ i ].channelNum);
#endif
            while (1)
            {
                chx_mean = TKY_GetCurChannelMean (my_tky_ch_init[ i ].channelNum, charge_time, 16, 1000);

                //  dg_log("testing .... chg : %d, baseline : %d\n",charge_time,chx_mean);//¡ä¨°¨®??¨´???¦Ì

                if ((charge_time == 0) && ((chx_mean > 3800)))
                {
                    dg_log ("Error, %u KEY%u Too small Cap,Please check the hardware !\r\n", chx_mean, i);
                    break;
                }
                else
                {
                    if ((chx_mean > 3200) && (chx_mean < 3800))
                    {          // Charging parameters are normal
                        TKY_SetCurQueueBaseLine (i, chx_mean);
                        TKY_SetCurQueueChargeTime (i, charge_time, 16);
                        dg_log ("channel:%u, chargetime:%u,BaseLine:%u\r\n",
                                i, charge_time, chx_mean);
                        break;
                    }
                    else if (chx_mean >= 3800)
                    {
                        TKY_SetCurQueueBaseLine (i, chx_mean_last);
                        TKY_SetCurQueueChargeTime (i, charge_time - 1, 16);
                        dg_log ("Warning,channel:%u Too large Current, chargetime:%u,BaseLine:%u\r\n",
                                i, charge_time, chx_mean_last);
                        break;
                    }
                    charge_time++;
                    chx_mean_last = chx_mean;
                    if (charge_time > 0x3ff)
                    {
                        dg_log ("Error, Chargetime Max,KEY%u Too large Cap,Please check the hardware !\r\n", i);
                        break;
                    }
                }
            }
#if !(TKY_SHIELD_EN)
            touch_GPIOModeCfg (GPIO_Mode_Out_PP, my_tky_ch_init[ i ].channelNum);
#endif
        }
    }
   TKY_SaveAndStop();
}

/*********************************************************************
 * @fn      touch_DetectWheelSlider
 *
 * @brief   Touch Pulley Data Processing.
 *
 * @param   p_wheel -
 *          wheel_data -
 *
 * @return  none
 */
static  uint16_t touch_DetectWheelSlider (touch_wheel_cfg_t * p_wheel)
{
    uint8_t loop;
    uint8_t max_data_idx;
    uint16_t d1;
    uint16_t d2;
    uint16_t d3;
    uint16_t wheel_rpos;
    uint16_t dsum;
    uint16_t unit;
    uint8_t num_elements;
    uint16_t p_threshold;
    uint16_t * wheel_data;

    if (p_wheel == NULL)
    {
        return TOUCH_OFF_VALUE;
    }

    num_elements = p_wheel->num_elements;
    p_threshold = p_wheel->threshold;
    wheel_data = p_wheel->pdata;

    if (num_elements < 3)
    {
        return TOUCH_OFF_VALUE;
    }

    for (loop = 0; loop < p_wheel->num_elements; loop++)
    {
        wheel_data[ loop ] = TKY_GetCurQueueValue (p_wheel->p_elem_index[ loop ]);
    }

    /* Search max data in slider */
    max_data_idx = 0;
    for (loop = 0; loop < (num_elements - 1); loop++)
    {
        if (wheel_data[ max_data_idx ] < wheel_data[ loop + 1 ])
        {
            max_data_idx = (uint8_t) (loop + 1);
        }
    }
    /* Array making for wheel operation          */
    /*    Maximum change CH_No -----> Array"0"    */
    /*    Maximum change CH_No + 1 -> Array"2"    */
    /*    Maximum change CH_No - 1 -> Array"1"    */
    if (0 == max_data_idx)
    {
        d1 = (uint16_t) (wheel_data[ 0 ] - wheel_data[ num_elements - 1 ]);
        d2 = (uint16_t) (wheel_data[ 0 ] - wheel_data[ 1 ]);
        dsum = (uint16_t) (wheel_data[ 0 ] + wheel_data[ 1 ] + wheel_data[ num_elements - 1 ]);
    }
    else if ((num_elements - 1) == max_data_idx)
    {
        d1 = (uint16_t) (wheel_data[ num_elements - 1 ] - wheel_data[ num_elements - 2 ]);
        d2 = (uint16_t) (wheel_data[ num_elements - 1 ] - wheel_data[ 0 ]);
        dsum = (uint16_t) (wheel_data[ 0 ] + wheel_data[ num_elements - 2 ] + wheel_data[ num_elements - 1 ]);
    }
    else
    {
        d1 = (uint16_t) (wheel_data[ max_data_idx ] - wheel_data[ max_data_idx - 1 ]);
        d2 = (uint16_t) (wheel_data[ max_data_idx ] - wheel_data[ max_data_idx + 1 ]);
        dsum = (uint16_t) (wheel_data[ max_data_idx + 1 ] + wheel_data[ max_data_idx ] + wheel_data[ max_data_idx - 1 ]);
    }

    if (0 == d1)
    {
        d1 = 1;
    }
    /* Constant decision for operation of angle of wheel */
    if (dsum > p_threshold)
    {
        d3 = (uint16_t) (p_wheel->decimal_point_percision + ((d2 * p_wheel->decimal_point_percision) / d1));

        unit = (uint16_t) (p_wheel->wheel_resolution / num_elements);
        wheel_rpos = (uint16_t) (((unit * p_wheel->decimal_point_percision) / d3) + (unit * max_data_idx));

        /* Angle division output */
        /* diff_angle_ch = 0 -> 359 ------ diff_angle_ch output 1 to 360 */
        if (0 == wheel_rpos)
        {
            wheel_rpos = p_wheel->wheel_resolution ;
        }
        else if ((p_wheel->wheel_resolution + 1) < wheel_rpos)
        {
            wheel_rpos = 1;
        }
        else
        {
            /* Do Nothing */
        }
    }
    else
    {
        wheel_rpos = TOUCH_OFF_VALUE;
    }

    return wheel_rpos;
}

/*********************************************************************
 * @fn      touch_DetecLineSlider
 *
 * @brief   Touch Slider Data Processin.
 *
 * @param   p_slider -
 *          slider_data -
 *
 * @return  slider coordinate
 */
static uint16_t touch_DetecLineSlider(touch_slider_cfg_t * p_slider)
{

    uint8_t loop;
    uint8_t max_data_idx;
    uint16_t d1;
    uint16_t d2;
    uint16_t d3;
    uint16_t slider_rpos;
    uint16_t dsum;
    uint8_t num_elements;
    uint16_t p_threshold;
    uint16_t * slider_data;

    if (p_slider == NULL)
    {
        return TOUCH_OFF_VALUE;
    }
    num_elements = p_slider->num_elements;
    p_threshold = p_slider->threshold;
    slider_data = p_slider->pdata;

    if (num_elements < 3)
    {
        return TOUCH_OFF_VALUE;
    }

    for (uint8_t loop = 0; loop < num_elements; loop++)
    {
        slider_data[ loop ] = TKY_GetCurQueueValue (p_slider->p_elem_index[ loop ]);
    }
    /* Search max data in slider */
    max_data_idx = 0;
    for (loop = 0; loop < (num_elements - 1); loop++)
    {
        if (slider_data[max_data_idx] < slider_data[loop + 1])
        {
            max_data_idx = (uint8_t)(loop + 1);
        }
    }

    /* Array making for slider operation-------------*/
    /*     |    Maximum change CH_No -----> Array"0"    */
    /*     |    Maximum change CH_No + 1 -> Array"2"    */
    /*     |    Maximum change CH_No - 1 -> Array"1"    */

    uint16_t unit;

    if (0 == max_data_idx)
    {
        d1 = (uint16_t) (slider_data[ 0 ] - slider_data[ num_elements - 1 ]);
        d2 = (uint16_t) (slider_data[ 0 ] - slider_data[ 1 ]);
        dsum = (uint16_t) (slider_data[ 0 ] + slider_data[ 1 ] + slider_data[ num_elements - 1 ]);
    }
    else if ((num_elements - 1) == max_data_idx)
    {
        d1 = (uint16_t) (slider_data[ num_elements - 1 ] - slider_data[ num_elements - 2 ]);
        d2 = (uint16_t) (slider_data[ num_elements - 1 ] - slider_data[ 0 ]);
        dsum = (uint16_t) (slider_data[ 0 ] + slider_data[ num_elements - 2 ] + slider_data[ num_elements - 1 ]);
    }
    else
    {
        d1 = (uint16_t) (slider_data[ max_data_idx ] - slider_data[ max_data_idx - 1 ]);
        d2 = (uint16_t) (slider_data[ max_data_idx ] - slider_data[ max_data_idx + 1 ]);
        dsum = (uint16_t) (slider_data[ max_data_idx + 1 ] + slider_data[ max_data_idx ] + slider_data[ max_data_idx - 1 ]);
    }

    if (0 == d1)
    {
        d1 = 1;
    }
    /* Constant decision for operation of angle of wheel    */
    if (dsum > p_threshold)
    {
        d3 = (uint16_t) (p_slider->decimal_point_percision + ((d2 * p_slider->decimal_point_percision) / d1));

        unit = (uint16_t) (p_slider->slider_resolution / num_elements);
        slider_rpos = (uint16_t) (((unit * p_slider->decimal_point_percision) / d3) + (unit * max_data_idx));

        /* Angle division output */
        /* diff_angle_ch = 0 -> 359 ------ diff_angle_ch output 1 to 360 */
        if (0 == slider_rpos)
        {
            slider_rpos = p_slider->slider_resolution;
        }
        else if ((p_slider->slider_resolution + 1) < slider_rpos)
        {
            slider_rpos = 1;
        }
        else
        {
            /* Do Nothing */
        }
    }
    else
    {
        slider_rpos = TOUCH_OFF_VALUE;
    }

    return slider_rpos;
}

uint16_t touch_GetLineSliderData(void)
{
    return SilderData;
}

uint16_t touch_GetWheelSliderData(void)
{
    return WheelData;
}
