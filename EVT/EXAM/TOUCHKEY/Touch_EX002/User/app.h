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


#if(DEBUG)
  #define PRINT(format, ...)    printf(format, ##__VA_ARGS__)
#else
  #define PRINT(X...)
#endif

/************************TOUCH_KEY_DEFINE****************************/
#define TOUCH_KEY_ELEMENTS            	    (TKY_MAX_QUEUE_NUM)
#define TOUCH_KEY_CHS                       0,1,2,3,4,5,6,7

extern volatile uint8_t timerFlag;

void TKY_Init(void);
void TKY_dataProcess(void);

#endif
