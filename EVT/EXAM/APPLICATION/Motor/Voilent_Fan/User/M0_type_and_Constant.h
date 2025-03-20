/********************************** (C) COPYRIGHT *******************************
 * File Name          : Motor_type_and_Constant.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/14
 * Description        : Data type definition, constant definition, state definition.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __M0_TYPE_AND_CONSTANT_H
#define __M0_TYPE_AND_CONSTANT_H

/* Includes -----------------------------------------------------------------*/
#include "stdint.h"
#include "IQmath_RV32.h"

/* Exported types -----------------------------------------------------------*/
//sense of rotation
#define   CW    1
#define   CCW   -1

//System running status enumeration prototype
typedef enum
{
    IDLE, INIT ,PRESTART, POSITION, START , RUN, STOP, BRAKE, WAIT, FAULT
} Runningstatus_Type;

//Enumeration of true and false decision results
typedef enum {FALSE = 0, TRUE = !FALSE} Truth_Verify_Type;//0 is false and 1 is true

//Array parameters define prototypes
typedef struct
{
    _iq24 Inv1;          //Calculation coefficient 1
    _iq24 Inv2;          //Calculation coefficient 2
    uint16_t   Length;   //Length
    uint16_t   Index;    //Index
} BuffPara_Type;

//First order low-pass filter prototype definition
typedef struct
{
    _iq24 Out_Pre;          //Output previous value
    _iq24 Out_New;          //Output value
    _iq24 Input_Coef;       //Input calculation coefficient
    _iq24 Output_Coef;      //Output calculation coefficient
} LPF_First_Order_Type;

//First order high-pass filter prototype definition
typedef struct
{
    _iq24 In_Pre;           //Input previous value
    _iq24 Out_Pre;          //Output previous value
    _iq24 Out_New;          //Output value
    _iq24 Input_Coef;       //Input calculation coefficient
    _iq24 Output_Coef;      //Output calculation coefficient
} HPF_First_Order_Type;

//Observer state structure prototype definition
typedef struct
{
    Truth_Verify_Type Timeout;      //Timeout
    Truth_Verify_Type Stall;        //Stall state;

    _iq24 SpeedstallBand1;          //Stall judgment speed threshold value 1
    _iq24 SpeedstallBand2;          //Stall judgment speed threshold value 2
    _iq24 SpeedstallBand3;          //Stall judgment speed threshold value 3
    _iq24 SpeedstallBand4;          //Stall judgment speed threshold value 4
    _iq24 SpeedstallBand5;          //Stall judgment speed threshold value 5
    _iq24 SpeedstallBand;           //Stall judgment speed threshold value
    _iq24 SpeedstallThresh;         //Stall judgment floor speed threshold value
    uint16_t Speedstall_CNT;        //Stall judgment cnt
    uint16_t SpeedstallCntThresh;   //Stall judgment cnt threshold value

    uint16_t Currinterval_CNT;      //Stall current judgment interval cnt
    uint16_t Currinterval_Set;      //Stall current judgment interval set
    uint16_t Currover_CNT;          //Stall judgment over current cnt
    uint16_t CurrCntThresh;         //Stall judgment over current cnt threshold value


    uint32_t Capbuff[6];                    //Change phase capture buffer
    uint32_t Captotal;                      //Change phase capture buffer sum
    uint32_t Capmax;                        //Maximum value of change phase capture buffer
    uint32_t Capmin;                        //Minimum value of change phase capture buffer
    uint32_t Capave;                        //Average value of change phase capture buffer
    uint32_t CapminSet;                     //Change phase capture minimum threshold value

    uint16_t Speedstallvibra_Calcnt;        //Calculation times of speed fluctuation and abnormal stall
    uint16_t Speedstallvibra_Cnt;           //Times of speed fluctuation and abnormal stall
    uint16_t Speedstallvibra_CntThresh;     //judgment times of speed fluctuation and abnormal stall
    uint16_t Speedstallvibra_Cntinterval;   //Time interval for detecting abnormal rotation jam due to speed fluctuation
    uint16_t Speedstallvibra_Interval_num;  //The effective interval between abnormal rotation jam and speed fluctuation
    uint16_t Speedstallvibra_Interval_set;  //Speed fluctuation Indicates the threshold of effective interval between abnormal rotation jams
    uint8_t Speedstallvibra_Thresh;         //Speed fluctuation Indicates the threshold for abnormal locked-rotor fluctuation
}Observer_Status_Type;

//ADC Sample computation structure
typedef struct
{
    uint8_t AdcChannel1;                   //ADC channel of sample current
    uint8_t AdcChannel2;                   //ADC channel of sample bus voltage
    uint8_t AdcChannel3;                   //ADC channel of sample BEMF

    _iq16 RatedCurr_ConvInv;               //The rated current amplitude corresponds to the reciprocal of the AD sampling integer
    uint16_t IOffsetValue;                 //Current sampling bias
    _iq24 IDCPres;                         //Bus current sampling current value(unit-A_per)
    _iq24 IDCFilter_2;                     //Bus current sampling filter value 2(unit-A_per)
    LPF_First_Order_Type    IDCLPF1st_2;   //Bus current filter 2

    _iq18 DCVoltRatedADC;                  //The rated bus voltage corresponds to the AD sampling integer
    _iq18 RatedDCVoltADCInv;               //The rated bus voltage corresponds to the reciprocal of the AD sampling integer
    _iq24 DCVoltPres;                      //Current value of bus voltage sampling(unit-V_per)
    _iq24 DCVoltHalf;                      //Bus voltage sampling half value(unit-V_per)

    _iq24 DCVoltInv;                       //The reciprocal of the current bus voltage

    uint16_t DCVoltPres_ADvalue;           //Current bus voltage sample ADC value

    _iq18 EMFRatedADC;                    //Back potential corresponding to rated bus voltage ADC sampling integer
    _iq18 EMFRated_ConvInv;               //Reciprocal of the sampling value of the back potential corresponding to the rated bus voltage
    _iq24 BEMFPres;                       //BEMF samples the current value(unit-V_per)
    _iq24 BEMFU;                          //BEMF of U phase(unit-V_per)
    _iq24 BEMFV;                          //BEMF of V phase(unit-V_per)
    _iq24 BEMFW;                          //BEMF of W phase(unit-V_per)
} ADCStruc_Type;

//Acceleration and deceleration control structure
typedef struct
{
    _iq24 Preramp;                  //The motor mechanical angular speed before Speed ramp(unit-rad/s_per)
    _iq24 Aftramp;                  //The motor mechanical angular speed(unit-rad/s_per)
    _iq24 AccStep;                  //AccStep(unit-rad/s^2_per)
    _iq24 DecStep;                  //DecStep(unit-rad/s^2_per)

    uint8_t  Speed_adncnt;          //Speed_adncnt
    uint8_t  Speed_rankset;         //Speed_rankset
    _iq24 Speed1;                   //Speed setting value 1(unit-Rad/s_per)
    _iq24 Speed2;                   //Speed setting value 2(unit-Rad/s_per)
    _iq24 Speed3;                   //Speed setting value 3(unit-Rad/s_per)

    _iq24 Speed_Stop;               //Stop Speed setting(unit-Rad/s_per)

    uint8_t SpeedStopflag;          //Flag of Stop Speed
}Speedrampctr_Type;

//Transient overcurrent protection structure
typedef struct
{
    _iq24 CurrThreshold;        //Overcurrent threshold(unit-A_per)
    uint16_t CntThreshold;      //Threshold of Overcurrent cnt
    uint16_t Cnt;               //Overcurrent cnt
} InstPhaseOC_Type;

//Bus voltage protection structure
typedef struct
{
    _iq24 OverThresh;           //Overvoltage threshold(unit-V_per)
    uint16_t OverCntThresh;     //Threshold of Overvoltage cnt
    uint16_t OverCnt;           //Overvoltage cnt

    _iq24 UnderThresh;          //Undervoltage threshold(unit-V_per)
    uint16_t UnderCntThresh;    //Threshold of Undervoltage cnt
    uint16_t UnderCnt;          //Undervoltage cnt
} DCbusProtection_Type;

//Motor base value structure definition
typedef struct
{
    _iq18 In;
    _iq18 Ibase;
} Motor_Base_Type;

// PWM chopper control structure
typedef struct
{
    int16_t    T;                   //PWM period value
    uint8_t    Sector;              //PWM chopper sector
    uint8_t    SectorLast;          //Last PWM chopper sector
    int16_t    Duty;                //Duty of PWM
    int16_t    Dutymax;             //Maximum duty of PWM
    int16_t    Dutymin;             //Minimum duty of PWM
    _iq24      Dutylimcurr;         //Current limit of duty of PWM

    int16_t    DutyU;               //PWM duty of U phase
    int16_t    DutyV;               //PWM duty of V phase
    int16_t    DutyW;               //PWM duty of W phase

    uint16_t UHpin;                 //Upper pin of U phase
    uint16_t ULpin;                 //Lower pin of U phase

    uint16_t VHpin;                 //Upper pin of V phase
    uint16_t VLpin;                 //Lower pin of V phase

    uint16_t WHpin;                 //Upper pin of W phase
    uint16_t WLpin;                 //Lower pin of W phase

    uint8_t  EMFChanelU;            //Sample channel of U phase BEMF
    uint8_t  EMFChanelV;            //Sample channel of V phase BEMF
    uint8_t  EMFChanelW;            //Sample channel of W phase BEMF

    uint16_t BCRApin;               //Reset pins of GPIOA
    uint16_t BSHRApin;              //Set pins of GPIOA
    uint16_t BCRDpin;               //Reset pins of GPIOD
    uint16_t BSHRDpin;              //Set pins of GPIOD

    uint8_t  EMFchannelselect;      //BEMF sample channel of observe

}PWM_CHOPPER_TYPE;

//EMF observation result structure prototype
typedef struct
{
    _iq24 Speed_Inst;                   //Observed real time value of angular velocity, after limiting
    _iq24 Speed_Inst2;                  //Observed angular velocity real time value, unrestricted
    _iq24 Speed_Aver1;                  //Observe the average angular velocity value used by the rotational speed ring
    _iq24 Speed_Aver2;                  //Observe the average angular velocity value, and determine the use of stall

    _iq24 Speed_Singlesamp;             //Speed of change single sample BEMF

    BuffPara_Type SpeedBuffPara1;       //Calculation parameter of the mean speed is used to speed loop
    BuffPara_Type SpeedBuffPara2;       //Calculation parameter of the mean speed is used to determine the blocked rotation
    int8_t  Directioncmd;               //Direction setting

    _iq24 Upper_Lim ;                   //Upper limit value of observed speed
    _iq24 Lower_Lim ;                   //Lower limit value of observed speed
    _iq24 Owe_Speed ;                   //Minimum value of observed speed
    _iq24 Dspeedmax ;                   //Maximum value of observed speed

    uint16_t Timerperiod;               //Capture timer period
    uint16_t Timercnt;                  //Cnt of timer
    uint16_t Cap_Rated;                 //Capture value of rated speed
    uint16_t Cap_Lowerlim;              //Capture value of lower speed
    uint16_t Cap_Upperlim;              //Capture value of upper speed
    uint16_t Cap_Value_Origin;          //Capture value of origin speed
    uint32_t Cap_Value_Total;           //Capture value total
    uint32_t Cap_Value_Revise;          //Capture value revise
    _iq16 Speed_Divider;                //Calculate the reduction of the speed relative to the rated speed
    uint16_t Timer_Overflow_Cnt;        //Timer overflow Cnt
    uint16_t Timer_Overflow_thres;      //Timer overflow threshold

    uint8_t Sector;                     //Sector
    Truth_Verify_Type Sprechangemark;   //Prechange phase mark
    Truth_Verify_Type Schangemark;      //Change phase mark
    uint16_t Schangetimer;              //Delay time of change phase
    uint16_t Schange_cnt;               //Change phase cnt
    uint16_t Schange_cnt_Thre;          //Change phase cnt threshold
    uint16_t Schangedelayset;           //Value of delay change phase detection
    uint16_t Schangedelaycnt;           //Cnt of delay change phase detection
    uint16_t Schangedelay;              //Value of delay change phase detection (Actual value)
    _iq12   Sprechangecoef;             //Coefficient of delay change phase

    uint16_t Sampledelay_H;             //Value of delay change phase detection (Actual value)

    uint8_t Sprechangeflag;             //Flag of prechange phase

    uint8_t changerightflag;            //Flag of change phase rightnow
    _iq12   compensate_cof;             //Coefficient of delay change phase

    uint8_t Singlesampleflag;           //Flag of single sample BEMF

    uint16_t Compulcnt;                 //Cnt of force change phase
    uint16_t Compulcntthrsh;            //Threshold of force change phase

    uint16_t Ctrlswitchcntthre;         //Threshold of change phase cnt
    uint16_t HighSpeedswitchcntthre;    //Threshold of high speed change phase cnt
}EMF_OBSERVER_TYPE;

//Operation structure prototype definition
typedef struct
{
    uint16_t StopwaitCnt;               //Cnt of stop
    uint16_t StopwaitSet;               //Cnt of stop setting

    uint16_t StartbuttonSet;            //Cnt of start button setting
    uint16_t SpeedbuttonCnt;            //Cnt of speed button
    uint16_t SpeedbuttonSetdown;        //Lower cnt of speed button

    Truth_Verify_Type Startmark;        //Start mark
    uint8_t  Startfirstmark;            //First start mark
    Truth_Verify_Type Restartmark;      //Restart mark
    uint8_t Checkmode;                  //Check mode

    uint32_t RuntimeSet;                //Run time set
    uint32_t RuntimeCnt;                //Run time cnt

    uint16_t FaultHoldingSet;           //Holding set of fault state
    uint16_t FaultHoldingCnt;           //Cnt of fault state holding
} Operation_Type;

typedef struct
{
    uint8_t Incstep1;                   //Increase step 1
    uint8_t Incstep2;                   //Increase step 2
    uint8_t Decstep1;                   //Decrease step 1
    uint8_t Decstep2;                   //Decrease step 2
    uint8_t OCDecstep;                  //Decrease step of over current
    uint8_t OSDecstep;                  //Decrease step of over speed
} Speed_SContrl_Type;

/* Exported constants -------------------------------------------------------*/
#define SPEED_CONTROL_M            0x0001      //Control  0£ºtorque control; 1£ºspeed control
#define FIRST_START_M              0x0002      //First start mark
#define STARTUP_FAILURE_M          0x0004      //Failed to start
#define SPEED_ABNORMAL_M           0x0008      //Speed feedback Abnormal
#define BRAKE_RES_ON_M             0x0010      //Brake trigger
#define MOTOR_OVER_HEAT_M          0x0020      //Motor over heat
#define MOTOR_OVER_CURR_SOFT_M     0x0040      //Motor over current
#define DC_OVER_CURR_SOFT_M        0x0080      //Motor direct current over current of software
#define DC_OVER_CURR_HARD_M        0x0100      //Motor direct current over current of hardware
#define MOTOR_STALL_M              0x0200      //Motor stall
#define DC_OVER_VOLT               0x0400      //Bus overvoltage
#define DC_UNDER_VOLT              0x0800      //Bus undervoltage
#define MOTOR_REVERSE_M            0x1000      //Motor reversal
#define MOTOR_OVERLOAD_M           0x2000      //Motor overload
#define POSITION_FAILUE            0x4000      //Location failure

#define PI_2f                    6.283185           //2*PI
#define PI_1_Q24                _IQ(3.141593)       //PI
#define PI_2_Q24                _IQ(6.283185)       //2*PI
#define PI_2_Q18                _IQ18(6.283185)     //2*PI
#define DIV_PI_2_Q24            _IQ(0.1591549)      //1/(2*PI)
#define PI_05_Q24               _IQ(1.570796)       //0.5*PI
#define Three_Q24               _IQ(3.0)            //3.0
#define Three_Q18               _IQ18(3.0)          //3.0
#define SQRT3_Q24               _IQ(1.732051)       //SQRT(3)
#define SQRT3_Q18               _IQ18(1.732051)     //SQRT(3)
#define SQRT_3                   1.732              //SQRT(3)
#define SQRT2_Q24               _IQ(1.414214)       //SQRT(2)
#define UNIT_Q24                _IQ(1.0)            //1.0
#define HALF_Q24                _IQ(0.5)            //0.5
#define QUARD_Q24               _IQ(0.25)           //0.5
#define DIV_SQRT3_Q24           _IQ(0.577350)       //1/SQRT(3)
#define SQRT3_DIV2_Q24          _IQ(0.866)          //SQRT(3)/2
#define SQRT_2DIV3_Q24          _IQ(0.816497)       //SQRT(2/3)
#define ONE_HALF_Q24            _IQ(1.50)           //1.5
#define DIV_MIN_Q24             _IQ(0.000001)       //Minimum divisor limit

#define U8_MAX     ((uint8_t)255)
#define S8_MAX     ((int8_t)127)
#define S8_MIN     ((int8_t)-128)
#define U16_MAX    ((uint16_t)65535u)
#define S16_MAX    ((int16_t)32767)
#define S16_MIN    ((int16_t)-32768)
#define U32_MAX    ((uint32_t)4294967295uL)
#define S32_MAX    ((int32_t)2147483647)
#define S32_MIN    ((int32_t)-2147483648)


/* Exported macro ------------------------------------------------------------*/
#define _IQMPY_MACRO(A,B)     (signed long)( ((signed long long)A *B )>>24)
#define _IQ18MPY_MACRO(A,B)   (signed long)( ((signed long long)A *B )>>18)
#define _IQ16MPY_MACRO(A,B)   (signed long)( ((signed long long)A *B )>>16)
#define _IQ12MPY_MACRO(A,B)   (signed long)( ((signed long long)A *B )>>12)
/* Exported functions ------------------------------------------------------- */
#endif
