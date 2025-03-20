/********************************** (C) COPYRIGHT *******************************
 * File Name          : WCH_touch_lite.h
 * Author             : WCH
 * Version            : V1.7
 * Date               : 2025/01/14
 * Description        : Touch Key Libraries
 *******************************************************************************/
#ifndef __WCH_TOUCHKEY_H__
#define __WCH_TOUCHKEY_H__
//------------------------------------------

//---------------filter-----------------
#define FILTER_MODE_3    	      3       //--Filter mode 3, multiple key outputs possible--
#define FILTER_MODE_CS10          2       //--CS10V Specialized Filter Mode-
//---------------single key mode-----------------
#define TKY_SINGLE_KEY_MULTI    0       //--Multi-key output, i.e. any keystroke above the threshold will trigger the
#define TKY_SINGLE_KEY_MAX      1       //--Maximum value single key output, i.e., only the key with the largest change is reported among the keys that exceed the threshold value
#define TKY_SINGLE_KEY_MUTU     2       //--Mutually exclusive single key output, i.e. the next most changed key is reported only after the current key is released,
                                        //  Otherwise, the other buttons are not reported, no matter how much they change.

//--------------lib param-----------------
#define TKY_BUFLEN  			28
//---------------------------------------
typedef struct
{
	uint8_t maxQueueNum;				    //--Number of test queues--
	uint8_t singlePressMod;                	//--Single Button Mode---
	uint8_t shieldEn;                   	//--shielding enable---
	uint8_t filterMode;				        //--Filter Mode--
	uint8_t filterGrade;				    //--Filter Rating--
	uint8_t peakQueueNum;                	//--Maximum key offset queue---
	uint8_t peakQueueOffset;             	//--Offset value of the maximum key offset queue---
	uint8_t baseRefreshOnPress;			    //--Whether the baseline is carried out at key presses--
	uint8_t baseUpRefreshDouble;        	//--Baseline Up Refresh Rate Parameter---
	uint8_t baseDownRefreshSlow;       		//--Baseline downward updating of downspeed parameters---
	uint8_t rfu[2];
    uint32_t baseRefreshSampleNum;     		//--Number of baseline refresh samples--
	uint32_t *tkyBufP;					 		//--Pointer to test channel data buffer--
}TKY_BaseInitTypeDef;

typedef struct
{
	uint8_t queueNum;                 		//--Serial number of the channel in the test queue--
	uint8_t channelNum;               		//--The ADC channel label corresponding to this channel--
	uint16_t chargeTime;		            	//--Charge time for this channel--
	uint16_t disChargeTime;            		//--Discharge time for this channel--
	uint16_t baseLine;  	   	        		//--base line--
	uint16_t threshold;		            	//--thresholds 1--
	uint16_t threshold2;              		//--thresholds 2--
	uint8_t sleepStatus;		          		//--dormant--
}TKY_ChannelInitTypeDef;


/*********************************************************************
 * @fn      TKY_BaseInit
 *
 * @brief   Initialization of TouchKey general parameters.
 *
 * @param   TKY_BaseInitStruct - TKY_BaseInitStruct, parameter for initialization
 *
 * @return  uint8_t - reData, 0 for priming success, others for failure faults.
 */
extern uint8_t TKY_BaseInit(TKY_BaseInitTypeDef TKY_BaseInitStruct);

/*********************************************************************
 * @fn      TKY_CHInit
 *
 * @brief   TouchKey channel parameter initialization.
 *
 * @param   TKY_CHInitStruct - parameters for initialization
 *
 * @return  uint8_t - reData, 0 for priming success, others for failure faults.
 */
extern uint8_t TKY_CHInit(TKY_ChannelInitTypeDef TKY_CHInitStruct);

/*********************************************************************
 * @fn      TKY_GetCurChannelMean
 *
 * @brief   Get the average value of the current channel, mainly used for setting baseline and threshold.
 *
 * @param   TKY_CHInitStruct - parameters for initialization
 *
 * @return  uint16_t - The ADC value of the current test.
 */
extern uint16_t TKY_GetCurChannelMean(uint8_t curChNum, uint16_t chargeTime, uint16_t disChargeTime, uint16_t averageNum);

/*********************************************************************
 * @fn      TKY_GetCurQueueValue
 *
 * @brief   Gets the value of the specified channel after processing
 *
 * @param   curQueueNum - The current channel to be fetched
 *
 * @return  uint16_t - The current channel's processing value.
 */
extern uint16_t TKY_GetCurQueueValue(uint8_t curQueueNum);

/*********************************************************************
 * @fn      TKY_PollForFilterMode_3
 *
 * @brief   TouchKey main loop polling mode, suitable for filter 3 filtering, execution process blocking
 *
 * @param   none
 *
 * @return  uint16_t - pressData, 0 is not pressed, 1 is pressed.
 */
extern uint16_t TKY_PollForFilterMode_3(void);

/*********************************************************************
 * @fn      TKY_PollForFilterMode_CS10
 *
 * @brief   TouchKey Master Cycle Polling Mode, suitable for CS10V testing
 *
 * @param   none
 *
 * @return  uint16_t - pressData, 0 is not pressed, 1 is pressed.
 */
extern uint16_t TKY_PollForFilterMode_CS10(void);

/*********************************************************************
 * @fn      TKY_ScanForWakeUp
 *
 * @brief   TouchKey sleep detection, timed polling of main loop
 *
 * @param   scanBitValue
 *
 * @return  uint16_t - pressData, 0 is not pressed, 1 is pressed.
 */
extern uint16_t TKY_ScanForWakeUp(uint16_t scanBitValue);

/*********************************************************************
 * @fn      TKY_SetCurQueueSleepStatus
 *
 * @brief   Sets the hibernation state of the specified channel.
 *
 * @param   curQueueNum - current channel to be set,
 *          sleepStatus - sleepStatus is not sleep when 0, non-0 sleep.
 *
 * @return  uint8_t - Returns 0 for successful setup and 1 for exceeding the maximum length of the queue.
 */
extern uint8_t TKY_SetCurQueueSleepStatus(uint8_t curQueueNum, uint8_t sleepStatus);


/*********************************************************************
 * @fn      TKY_SetSleepStatusValue
 *
 * @brief   Sets the hibernation state of multiple specified channels
 *
 * @param   setValue - sets the sleep state in bitwise order of the detection queue, 0: hibernate, 1: sleep.
 *
 * @return  uint8_t - Returns 0 for successful setup and 1 for exceeding the maximum length of the queue.
 */
extern uint8_t TKY_SetSleepStatusValue (uint16_t setValue);

/*********************************************************************
 * @fn      TKY_GetSleepStatusValue
 *
 * @brief   Sets the hibernation state of multiple specified channels.
 *
 * @param   none
 *
 * @return  uint16_t - Returns 0 for successful setup and 1 for exceeding the maximum length of the queue.
 */
extern uint16_t TKY_GetSleepStatusValue(void);

/*********************************************************************
 * @fn      TKY_SetCurQueueChargeTime
 *
 * @brief   Sets the charging parameters for the specified channel.
 *
 * @param   curQueueNum - The current channel to be set.
 *          chargeTime - charge Time
 *          disChargeTime - disCharge Time
 *
 * @return  uint8_t - Returns 0 for successful setup and 1 for exceeding the maximum length of the queue.
 */
extern uint8_t TKY_SetCurQueueChargeTime (uint8_t curQueueNum,
                                          uint16_t chargeTime,
                                          uint16_t disChargeTime);

/*********************************************************************
 * @fn      TKY_SetCurQueueThreshold
 *
 * @brief   Sets the threshold for the specified channel.
 *
 * @param   curQueueNum - The current channel to be set.
 *          threshold - threshold value.
 *          threshold2 - threshold2 value.
 *
 * @return  uint8_t - Returns 0 for successful setup and 1 for exceeding the maximum length of the queue.
 */
extern uint8_t TKY_SetCurQueueThreshold (uint8_t curQueueNum,
                                         uint16_t threshold,
                                         uint16_t threshold2);

/*********************************************************************
 * @fn      TKY_GetCurIdleStatus
 *
 * @brief   Get Idle State.
 *
 * @param   none
 *
 * @return  uint8_t - Returns whether or not it is idle
 */
extern uint8_t TKY_GetCurIdleStatus(void);

/*********************************************************************
 * @fn      TKY_GetCurVersion
 *
 * @brief   Get the current version number.
 *
 * @param   none
 *
 * @return  uint16_t - Get the current version number.
 */
extern uint16_t TKY_GetCurVersion(void);

/*********************************************************************
 * @fn      TKY_GetCurQueueBaseLine
 *
 * @brief   Gets the specified channel baseline value.
 *
 * @param   curQueueNum -The current channel to be fetched.
 *
 * @return  uint16_t - Processing value for the current channel.
 */
extern uint16_t TKY_GetCurQueueBaseLine(uint8_t curQueueNum);

/*********************************************************************
 * @fn      TKY_GetCurQueueRealVal
 *
 * @brief   Gets the raw measured value of the specified channel.
 *
 * @param   curQueueNum - The current channel to be fetched.
 *
 * @return  uint16_t - The raw measured value of the current channel.
 */
extern uint16_t TKY_GetCurQueueRealVal(uint8_t curQueueNum);

/*********************************************************************
 * @fn      TKY_SetCurQueueBaseLine
 *
 * @brief   Sets the specified channel baseline value.
 *
 * @param   curQueueNum - the current channel to set the value for.
 *          baseLineValue - the current channel to set the value for.
 *
 * @return  none
 */
extern void TKY_SetCurQueueBaseLine(uint8_t curQueueNum, uint16_t baseLineValue);

/*********************************************************************
 * @fn      TKY_SetBaseRefreshSampleNum
 *
 * @brief   The baseline refreshes the multiplier parameter upwards.
 *
 * @param   newValue - newValue is the filter mode
 *
 * @return  none
 */
extern void TKY_SetBaseRefreshSampleNum(uint32_t newValue);

/*********************************************************************
 * @fn      TKY_SetBaseUpRefreshDouble
 *
 * @brief   The baseline refreshes the multiplier parameter upwards.
 *
 * @param   newValue - newValue is the filter mode
 *
 * @return  none
 */
extern void TKY_SetBaseUpRefreshDouble(uint8_t newValue);

/*********************************************************************
 * @fn      TKY_SetBaseDownRefreshSlow
 *
 * @brief   Sets the baseline down to update the downspeed parameter.
 *
 * @param   newValue - newValue is the filter mode
 *
 * @return  none
 */
extern void TKY_SetBaseDownRefreshSlow(uint8_t newValue);

/*********************************************************************
 * @fn      TKY_SetFilterMode
 *
 * @brief   Setting the current operating filter mode
 *
 * @param   newValue - newValue is the filter mode
 *
 * @return  none
 */
extern void TKY_SetFilterMode(uint8_t newValue);

/*********************************************************************
 * @fn      TKY_ClearHistoryData
 *
 * @brief   Clear the historical data.
 *
 * @param   curFilterMode - current filter mode
 *
 * @return  none
 */
extern void TKY_ClearHistoryData(uint8_t curFilterMode);

/*********************************************************************
 * @fn      TKY_SaveCfgReg
 *
 * @brief   CH32 series chips are saved after externally calibrating the configuration parameters of the library
 *
 * @param   none
 *
 * @return  none
 */
extern void TKY_SaveCfgReg(void);

/*********************************************************************
 * @fn      TKY_SaveAndStop
 *
 * @brief   TouchKey's tky's cfg register value is saved and stops running
 *
 * @param   none
 *
 * @return  none
 */
extern void TKY_SaveAndStop(void);

/*********************************************************************
 * @fn      TKY_LoadAndRun
 *
 * @brief   TouchKey's tky's cfg register value is loaded and resumed.
 *
 * @param   none
 *
 * @return  none
 */
extern void TKY_LoadAndRun(void);

#endif
