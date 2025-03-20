/********************************** (C) COPYRIGHT *******************************
 * File Name          : app.h
 * Author             : WCH
 * Version            : V1.1
 * Date               : 2025/01/14
 * Description        : Touch Key Routines
 *******************************************************************************/
#ifndef APP_H_
#define APP_H_

#include <ch32v00X.h>

// #define DEBUG 0

#if(DEBUG)
  #define PRINT(format, ...)    printf(format, ##__VA_ARGS__)
#else
  #define PRINT(X...)
#endif

/************************TOUCH_KEY_DEFINE****************************/
#define TOUCH_KEY_ELEMENTS            	    (2)
#define TOUCH_KEY_CHS                       0,1

/************************WHEEL_SLIDER_DEFINE****************************/
#define TOUCH_DECIMAL_POINT_PRECISION       (100)

#define TOUCH_WHEEL_ELEMENTS            	  (3)
#define TOUCH_WHEEL_RESOLUTION              (120)
#define TOUCH_WHEEL_CHS                     2,3,4

/************************LINE_SLIDER_DEFINE****************************/
#define TOUCH_SLIDER_ELEMENTS            	  (3)
#define TOUCH_SLIDER_RESOLUTION             (120)
#define TOUCH_SLIDER_CHS                    5,6,7
/************************TOUCH_PAD_DEFINE****************************/
#define TKY_THRESHOLD1  60  //upper threshold
#define TKY_THRESHOLD2  30  //lower threshold

#define CBUF_NUM            80
#define TBUF_NUM            40       //Number of touches calculated
#define FILTER_NUM          5        //Number of larger and smaller values filtered out

#define TKY_THRESHOLD_MAX   120      //Touch Threshold Upper Limit
#define TOUCH_WINDOW        30       //touch window
#define TOUCH_DROP_RATE     0.15     //Touch attenuation ratio


#define ENLARGE_FACTOR_X  16		//X-coordinate extension
#define ENLARGE_FACTOR_Y  32		//Y-coordinate extension

#define ROW_NUM         3
#define COL_NUM         3

#define PX_LOW          ENLARGE_FACTOR_X
#define PX_HIGH         ENLARGE_FACTOR_X*ROW_NUM
#define PY_LOW          ENLARGE_FACTOR_Y
#define PY_HIGH         ENLARGE_FACTOR_Y*COL_NUM

#define SAVE_POINT_NUM 12
#define DEAL_POINT_NUM 50

enum touch_state{
    TOUCH_NONE,
    TOUCH_MOVE_LEFT = 0x01,
    TOUCH_MOVE_RIGHT = 0x02,
    TOUCH_MOVE_UP = 0x04,
    TOUCH_MOVE_DOWN = 0x08,
    TOUCH_CLICK_SINGLE = 0x10,
    TOUCH_CLICK_DOUBLE = 0x20,
    TOUCH_CLICK_RELEASE = 0x40,
};

enum click_state
{
    STA_CLICK_RELEASE,
    STA_SINGLE_START,
    STA_SINGLE_DONE,
    STA_DOUBLE_DONE,
};

extern volatile uint8_t timerFlag;

void TKY_Init(void);
void TKY_dataProcess(void);

#endif
