/********************************** (C) COPYRIGHT *******************************
* File Name          : TouchKey_CFG.h
* Author             : WCH
* Version            : V1.1
* Date               : 2025/01/14
* Description        : Touch button parameter configuration header file
* ********************************************************************************
* Copyright(c) 2023 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention : This software(modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#ifndef __TOUCH_KEY_CFG_H__
#define __TOUCH_KEY_CFG_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef enum _TKY_QUEUE_ID
{
TKY_QUEUE_0 = 0,
TKY_QUEUE_1,
TKY_QUEUE_2,
TKY_QUEUE_3,
TKY_QUEUE_4,
TKY_QUEUE_5,
TKY_QUEUE_6,
TKY_QUEUE_7,
/* Add new above this */
TKY_QUEUE_END
} TKY_QUEUE_ID;

#define TKY_FILTER_MODE     			              3
#define TKY_FILTER_GRADE     			              4
#define TKY_BASE_REFRESH_ON_PRESS     			    0
#define TKY_BASE_UP_REFRESH_DOUBLE     			    0
#define TKY_BASE_DOWN_REFRESH_SLOW     			    0
#define TKY_BASE_REFRESH_SAMPLE_NUM     			  1000
#define TKY_SHIELD_EN     			                1
#define TKY_SINGLE_PRESS_MODE     			        0
#define TKY_MAX_QUEUE_NUM     			            8                   

#define GEN_TKY_CH_INIT(qNum,chNum,chTime,disChTime,slpmode,chBaseline,trs,trs2) \
    {\
     .queueNum=qNum,.channelNum=chNum,.chargeTime=chTime,.disChargeTime=disChTime,\
     .sleepStatus=slpmode,\
     .baseLine = chBaseline,\
     .threshold=trs,\
     .threshold2=trs2\
    }

#define TKY_CHS_INIT \
				GEN_TKY_CH_INIT(TKY_QUEUE_0,0,0,0,0,3382,120,100),\
				GEN_TKY_CH_INIT(TKY_QUEUE_1,2,0,0,0,3223,120,100),\
				GEN_TKY_CH_INIT(TKY_QUEUE_2,4,0,0,0,3621,100,80),\
				GEN_TKY_CH_INIT(TKY_QUEUE_3,6,0,0,0,3600,120,100),\
				GEN_TKY_CH_INIT(TKY_QUEUE_4,1,0,0,0,3360,120,100),\
				GEN_TKY_CH_INIT(TKY_QUEUE_5,3,0,0,0,3233,120,100),\
				GEN_TKY_CH_INIT(TKY_QUEUE_6,5,0,0,0,3604,120,100),\
				GEN_TKY_CH_INIT(TKY_QUEUE_7,7,0,0,0,3214,120,100)

        /* Add new above this */

//***********************************************************
#ifdef __cplusplus
}
#endif

#endif /* __TOUCH_KEY_CFG_H__ */
