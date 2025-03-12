/********************************** (C) COPYRIGHT *******************************
 * File Name          : Touch.h
 * Author             : WCH
 * Version            : V1.7
 * Date               : 2025/01/14
 * Description        : Touch Key Libraries
 *******************************************************************************/
#ifndef TOUCH_KEY_H_
#define TOUCH_KEY_H_

#include <ch32v00X.h>
#include "TouchKey_CFG.h"
#include "wchtouch.h"


#ifdef  DEBUG
#define PRINT_EN 1
#else
#define PRINT_EN 0
#endif

#if (PRINT_EN)
  #define dg_log               printf
#else
  #define dg_log(x,...)
#endif

/************************KEY_FIFO_DEFINE******************************/
#define KEY_COUNT       	TKY_MAX_QUEUE_NUM               // Number of keys

#define TKY_PollForFilter() TKY_PollForFilterMode_3()

#define TKY_MEMHEAP_SIZE   		(TKY_MAX_QUEUE_NUM*TKY_BUFLEN)     	 //Externally defined data buffer length

#define TOUCH_OFF_VALUE    					(0xFFFF)

/* Key ID, mainly used as entry parameter of tky_GetKeyState() function. */
typedef enum
{
    KID_K0 = 0,
    KID_K1,
    KID_K2,
    KID_K3,
    KID_K4,
    KID_K5,
    KID_K6,
    KID_K7,
    KID_K8,
    KID_K9,
    KID_K10,
    KID_K11
}KEY_ID_E;

/*
    Key press filtering time 50ms, unit 10ms.
    Only the continuous detection of 50ms state unchanged is considered valid, including the two events of pop up and press down.
    This filtering mechanism ensures reliable detection of key events even if no hardware filtering is done on the key circuit.
*/
#define NORMAL_KEY_MODE 0                //Independent key trigger mode
#define TOUCH_KEY_MODE  1                //Touch Key Trigger Mode

#define KEY_MODE    NORMAL_KEY_MODE      //Key Mode Setting
#define KEY_FILTER_TIME   2              //Number of keystroke filters
#define KEY_LONG_TIME     0              //Unit: based on the interval of tky_KeyScan() call, if it exceeds the number of times, it will be regarded as a long press event.

typedef uint8_t (*pIsKeyDownFunc)(void);

/*
    Each key corresponds to 1 global structure variable
*/
typedef struct
{
    /* Here is a function pointer to a function that determines whether a key is pressed or not */
    /* The judgment function of key press,1 means pressing */
    pIsKeyDownFunc IsKeyDownFunc;
    uint8_t  Count;         //Filter Counter
    uint16_t LongCount;     //Long Press Counter
    uint16_t LongTime;      //Duration of key press, 0 means long press is not detected.
    uint8_t  State;         //Current state of the key (pressed or popped up)
    uint8_t  RepeatSpeed;   //Continuous Keystroke Cycle
    uint8_t  RepeatCount;   //Continuous Keystroke Counter
}KEY_T;

/*
    To define the key-value code, each key press, pop-up, and long-press event must be timed in the following order

    We recommend using enum, not #define, for this reason:
    (1) Easy to add new keys, easy to adjust the order, make the code look more comfortable
    (2) The compiler helps us avoid duplicate keys and values
*/
typedef enum
{
    KEY_NONE = 0,           //0 indicates a key event */

    KEY_0_DOWN,             // 1 key pressed
    KEY_0_UP,               // 1 key pop up
    KEY_0_LONG,             // 1 key long pressed

    KEY_1_DOWN,             // 2 key pressed
    KEY_1_UP,               // 2 key pop up
    KEY_1_LONG,             // 2 key long pressed

    KEY_2_DOWN,             // 3 key pressed
    KEY_2_UP,               // 3 key pop up
    KEY_2_LONG,             // 3 key long pressed

    KEY_3_DOWN,             // 4 key pressed
    KEY_3_UP,               // 4 key pop up
    KEY_3_LONG,             // 4 key long pressed

    KEY_4_DOWN,             // 5 key pressed
    KEY_4_UP,               // 5 key pop up
    KEY_4_LONG,             // 5 key long pressed

    KEY_5_DOWN,             // 6 key pressed
    KEY_5_UP,               // 6 key pop up
    KEY_5_LONG,             // 6 key long pressed

    KEY_6_DOWN,             // 7 key pressed
    KEY_6_UP,               // 7 key pop up
    KEY_6_LONG,             // 7 key long pressed

    KEY_7_DOWN,             // 8 key pressed
    KEY_7_UP,               // 8 key pop up
    KEY_7_LONG,             // 8 key long pressed

    KEY_8_DOWN,             // 8 key pressed
    KEY_8_UP,               // 8 key pop up
    KEY_8_LONG,             // 8 key long pressed

    KEY_9_DOWN,             // 9 key pressed
    KEY_9_UP,               // 9 key pop up
    KEY_9_LONG,             // 9 key long pressed

    KEY_10_DOWN,            // # key pressed
    KEY_10_UP,              // # key pop up
    KEY_10_LONG,            // # key long pressed

    KEY_11_DOWN,            // * key pressed
    KEY_11_UP,              // * key pop up
    KEY_11_LONG,            // * key long pressed
}KEY_ENUM;

/* Variables used in key FIFO */
#define KEY_FIFO_SIZE   64          //Can be modified to meet the needs of the usage environment and hardware*/



typedef struct
{
    uint8_t Buf[KEY_FIFO_SIZE];     // key-value buffer
    uint8_t Read;                   // buffer readout pointer
    uint8_t Write;                  // Buffer Write Pointer
}KEY_FIFO_T;

/** Configuration of each button */
typedef struct st_touch_button_cfg
{
    const uint8_t* p_elem_index;      ///< Element number array used by this button.
    KEY_T       *p_stbtn;
    uint8_t         num_elements;      ///< Number of elements used by this button.
} touch_button_cfg_t;

/** Configuration of each slider */
typedef struct st_touch_slider_cfg
{
    const uint8_t* p_elem_index;      ///< Element number array used by this slider.
    uint8_t  num_elements;      ///< Number of elements used by this slider.
    uint16_t threshold;         ///< Position calculation start threshold value.
    uint16_t decimal_point_percision;
    uint16_t slider_resolution;
    uint16_t *pdata;
} touch_slider_cfg_t;

/** Configuration of each wheel */
typedef struct st_touch_wheel_cfg_t
{
    const uint8_t* p_elem_index;      ///< Element number array used by this wheel.
    uint8_t  num_elements;      ///< Number of elements used by this wheel.
    uint16_t threshold;         ///< Position calculation start threshold value.
    uint16_t decimal_point_percision;
    uint16_t wheel_resolution;
    uint16_t *pdata;
} touch_wheel_cfg_t;

/** Configuration of touch */
typedef struct st_touch_cfg_t
{
    touch_button_cfg_t *touch_button_cfg;
    touch_slider_cfg_t *touch_slider_cfg;
    touch_wheel_cfg_t *touch_wheel_cfg;
} touch_cfg_t;

extern uint8_t wakeupflag; // 0  sleep mode   1  wakeup sta
extern uint32_t tkyPinAll;
extern uint16_t tkyQueueAll;
extern uint8_t wakeUpCount, wakeupflag;

/* Function declarations for external calls */
extern void touch_Init(touch_cfg_t *p);
extern void touch_ScanWakeUp(void);
extern void touch_ScanEnterSleep(void);
extern uint8_t touch_GetKey(void);
extern uint8_t touch_GetKeyState(KEY_ID_E _ucKeyID);
extern void touch_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
extern void touch_ClearKey(void);
extern void touch_Scan(void);
extern void touch_InfoDebug(void);
extern uint16_t touch_GetLineSliderData(void);
extern uint16_t touch_GetWheelSliderData(void);
void touch_GPIOModeCfg (GPIOMode_TypeDef mode, uint32_t channel);
extern void touch_GPIOSleep(void);
#endif
