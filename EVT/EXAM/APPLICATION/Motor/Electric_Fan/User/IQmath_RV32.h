/* 2021.09.10 
*****************************************
**   Copyright  (C)  WCH  2001-2021    **
**   Web:      http://wch.cn           **
*****************************************
**  Fixed Point Math lib for RISC-V    **
**  IDE: MounRiver Studio              **
*****************************************
IQMATH:为不支持硬件浮点的RISC-V内核MCU提供一套快速的MATH函数
*/

#ifndef __IQMATH_RV_H__
#define __IQMATH_RV_H__

#define IQMATH_RV_VER		0x10

#ifdef __cplusplus
extern "C"
{
#endif
/* ********************************************************************************************************************* */


/* 选择IQ_MATH基本运算函数来源*/
#define FLOAT_MATH              1   //IQMATH函数将使用标准MATH.LIB内的函数,运算效率会低
#define IQ_MATH                 0   //IQMATH函数所需的基本运算函数将调用IQMATH库内自定义函数进行运算，速度上比FLOAT_MATH要高。


#ifndef MATH_TYPE
#define MATH_TYPE               IQ_MATH
#endif

/* 设定IQMATH库默认精度(Q精度)，范围： 1 <= GLOBAL_Q <= 30 */
#ifndef GLOBAL_Q
#define GLOBAL_Q                24
#endif

/* 快速除法，运算速度较IQDIV提高2倍，但代码会增加2KB左右 */
//#define DIV_FAST                 0
#define DIV_FAST                 1

#if MATH_TYPE == FLOAT_MATH
#include <math.h>
#include <stdlib.h>
#endif
#include <limits.h>

/* 常量宏定义 */

#define Q30                     30
#define Q29                     29
#define Q28                     28
#define Q27                     27
#define Q26                     26
#define Q25                     25
#define Q24                     24
#define Q23                     23
#define Q22                     22
#define Q21                     21
#define Q20                     20
#define Q19                     19
#define Q18                     18
#define Q17                     17
#define Q16                     16
#define Q15                     15
#define Q14                     14
#define Q13                     13
#define Q12                     12
#define Q11                     11
#define Q10                     10
#define Q9                      9
#define Q8                      8
#define Q7                      7
#define Q6                      6
#define Q5                      5
#define Q4                      4
#define Q3                      3
#define Q2                      2
#define Q1                      1
#define QG                      GLOBAL_Q

#define MAX_IQ_POS              LONG_MAX
#define MAX_IQ_NEG              LONG_MIN
#define MIN_IQ_POS              1
#define MIN_IQ_NEG              -1

/* MATH_TYPE选择定点运算 */
#if MATH_TYPE == IQ_MATH

/* 各种IQ格式的定义 */
typedef long _iq30;
typedef long _iq29;
typedef long _iq28;
typedef long _iq27;
typedef long _iq26;
typedef long _iq25;
typedef long _iq24;
typedef long _iq23;
typedef long _iq22;
typedef long _iq21;
typedef long _iq20;
typedef long _iq19;
typedef long _iq18;
typedef long _iq17;
typedef long _iq16;
typedef long _iq15;
typedef long _iq14;
typedef long _iq13;
typedef long _iq12;
typedef long _iq11;
typedef long _iq10;
typedef long _iq9;
typedef long _iq8;
typedef long _iq7;
typedef long _iq6;
typedef long _iq5;
typedef long _iq4;
typedef long _iq3;
typedef long _iq2;
typedef long _iq1;
typedef long _iq;

/* 各种IQ格式的数乘以2的倍数 */
#define _IQmpy2(A)              ((A) << 1)
#define _IQmpy4(A)              ((A) << 2)
#define _IQmpy8(A)              ((A) << 3)
#define _IQmpy16(A)             ((A) << 4)
#define _IQmpy32(A)             ((A) << 5)
#define _IQmpy64(A)             ((A) << 6)
#define _IQmpy128(A)            ((A) << 7)
#define _IQmpy256(A)            ((A) << 8)
#define _IQmpy512(A)            ((A) << 9)
#define _IQdiv2(A)              ((A) >> 1)
#define _IQdiv4(A)              ((A) >> 2)
#define _IQdiv8(A)              ((A) >> 3)
#define _IQdiv16(A)             ((A) >> 4)
#define _IQdiv32(A)             ((A) >> 5)
#define _IQdiv64(A)             ((A) >> 6)
#define _IQdiv128(A)            ((A) >> 7)
#define _IQdiv256(A)            ((A) >> 8)
#define _IQdiv512(A)            ((A) >> 9)

/* 将浮点数转换成各种IQ格式的数 */
#define _IQ30(A)                ((_iq30)((A) * (1 << 30)))
#define _IQ29(A)                ((_iq29)((A) * (1 << 29)))
#define _IQ28(A)                ((_iq28)((A) * (1 << 28)))
#define _IQ27(A)                ((_iq27)((A) * (1 << 27)))
#define _IQ26(A)                ((_iq26)((A) * (1 << 26)))
#define _IQ25(A)                ((_iq25)((A) * (1 << 25)))
#define _IQ24(A)                ((_iq24)((A) * (1 << 24)))
#define _IQ23(A)                ((_iq23)((A) * (1 << 23)))
#define _IQ22(A)                ((_iq22)((A) * (1 << 22)))
#define _IQ21(A)                ((_iq21)((A) * (1 << 21)))
#define _IQ20(A)                ((_iq20)((A) * (1 << 20)))
#define _IQ19(A)                ((_iq19)((A) * (1 << 19)))
#define _IQ18(A)                ((_iq18)((A) * (1 << 18)))
#define _IQ17(A)                ((_iq17)((A) * (1 << 17)))
#define _IQ16(A)                ((_iq16)((A) * (1 << 16)))
#define _IQ15(A)                ((_iq15)((A) * (1 << 15)))
#define _IQ14(A)                ((_iq14)((A) * (1 << 14)))
#define _IQ13(A)                ((_iq13)((A) * (1 << 13)))
#define _IQ12(A)                ((_iq12)((A) * (1 << 12)))
#define _IQ11(A)                ((_iq11)((A) * (1 << 11)))
#define _IQ10(A)                ((_iq10)((A) * (1 << 10)))
#define _IQ9(A)                 ((_iq9)((A) * (1 << 9)))
#define _IQ8(A)                 ((_iq8)((A) * (1 << 8)))
#define _IQ7(A)                 ((_iq7)((A) * (1 << 7)))
#define _IQ6(A)                 ((_iq6)((A) * (1 << 6)))
#define _IQ5(A)                 ((_iq5)((A) * (1 << 5)))
#define _IQ4(A)                 ((_iq4)((A) * (1 << 4)))
#define _IQ3(A)                 ((_iq3)((A) * (1 << 3)))
#define _IQ2(A)                 ((_iq2)((A) * (1 << 2)))
#define _IQ1(A)                 ((_iq1)((A) * (1 << 1)))
/*******************************************************************************
* Function Name  : _IQ
* Description    : 将浮点数转换成IQ格式数
* Input          : float类型数值
* Return         : 返回转换后的IQ格式数，与GLOBAL_Q指定的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 30
#define _IQ(A)                  _IQ30(A)
#endif
#if GLOBAL_Q == 29
#define _IQ(A)                  _IQ29(A)
#endif
#if GLOBAL_Q == 28
#define _IQ(A)                  _IQ28(A)
#endif
#if GLOBAL_Q == 27
#define _IQ(A)                  _IQ27(A)
#endif
#if GLOBAL_Q == 26
#define _IQ(A)                  _IQ26(A)
#endif
#if GLOBAL_Q == 25
#define _IQ(A)                  _IQ25(A)
#endif
#if GLOBAL_Q == 24
#define _IQ(A)                  _IQ24(A)
#endif
#if GLOBAL_Q == 23
#define _IQ(A)                  _IQ23(A)
#endif
#if GLOBAL_Q == 22
#define _IQ(A)                  _IQ22(A)
#endif
#if GLOBAL_Q == 21
#define _IQ(A)                  _IQ21(A)
#endif
#if GLOBAL_Q == 20
#define _IQ(A)                  _IQ20(A)
#endif
#if GLOBAL_Q == 19
#define _IQ(A)                  _IQ19(A)
#endif
#if GLOBAL_Q == 18
#define _IQ(A)                  _IQ18(A)
#endif
#if GLOBAL_Q == 17
#define _IQ(A)                  _IQ17(A)
#endif
#if GLOBAL_Q == 16
#define _IQ(A)                  _IQ16(A)
#endif
#if GLOBAL_Q == 15
#define _IQ(A)                  _IQ15(A)
#endif
#if GLOBAL_Q == 14
#define _IQ(A)                  _IQ14(A)
#endif
#if GLOBAL_Q == 13
#define _IQ(A)                  _IQ13(A)
#endif
#if GLOBAL_Q == 12
#define _IQ(A)                  _IQ12(A)
#endif
#if GLOBAL_Q == 11
#define _IQ(A)                  _IQ11(A)
#endif
#if GLOBAL_Q == 10
#define _IQ(A)                  _IQ10(A)
#endif
#if GLOBAL_Q == 9
#define _IQ(A)                  _IQ9(A)
#endif
#if GLOBAL_Q == 8
#define _IQ(A)                  _IQ8(A)
#endif
#if GLOBAL_Q == 7
#define _IQ(A)                  _IQ7(A)
#endif
#if GLOBAL_Q == 6
#define _IQ(A)                  _IQ6(A)
#endif
#if GLOBAL_Q == 5
#define _IQ(A)                  _IQ5(A)
#endif
#if GLOBAL_Q == 4
#define _IQ(A)                  _IQ4(A)
#endif
#if GLOBAL_Q == 3
#define _IQ(A)                  _IQ3(A)
#endif
#if GLOBAL_Q == 2
#define _IQ(A)                  _IQ2(A)
#endif
#if GLOBAL_Q == 1
#define _IQ(A)                  _IQ1(A)
#endif


/*******************************************************************************
* Function Name  : _IQMathVerStr
* Description    : IQMath版本信息与编译时间的字符串
* Input          : 无
* Return         : 返回IQMath版本信息与编译时间的字符串
*******************************************************************************/
extern char * _IQMathVerStr(void);

extern float _IQ30toF(_iq30 A);
extern float _IQ29toF(_iq29 A);
extern float _IQ28toF(_iq28 A);
extern float _IQ27toF(_iq27 A);
extern float _IQ26toF(_iq26 A);
extern float _IQ25toF(_iq25 A);
extern float _IQ24toF(_iq24 A);
extern float _IQ23toF(_iq23 A);
extern float _IQ22toF(_iq22 A);
extern float _IQ21toF(_iq21 A);
extern float _IQ20toF(_iq20 A);
extern float _IQ19toF(_iq19 A);
extern float _IQ18toF(_iq18 A);
extern float _IQ17toF(_iq17 A);
extern float _IQ16toF(_iq16 A);
extern float _IQ15toF(_iq15 A);
extern float _IQ14toF(_iq14 A);
extern float _IQ13toF(_iq13 A);
extern float _IQ12toF(_iq12 A);
extern float _IQ11toF(_iq11 A);
extern float _IQ10toF(_iq10 A);
extern float _IQ9toF(_iq9 A);
extern float _IQ8toF(_iq8 A);
extern float _IQ7toF(_iq7 A);
extern float _IQ6toF(_iq6 A);
extern float _IQ5toF(_iq5 A);
extern float _IQ4toF(_iq4 A);
extern float _IQ3toF(_iq3 A);
extern float _IQ2toF(_iq2 A);
extern float _IQ1toF(_iq1 A);

/*******************************************************************************
* Function Name  : _IQtoF
* Description    : 将IQ格式的数转换成单精度浮点数
* Input          : IQ类型数据
* Return         : 转换后的float类型的数据
*******************************************************************************/
#if GLOBAL_Q == 30
#define _IQtoF(A)               _IQ30toF(A)
#endif
#if GLOBAL_Q == 29
#define _IQtoF(A)               _IQ29toF(A)
#endif
#if GLOBAL_Q == 28
#define _IQtoF(A)               _IQ28toF(A)
#endif
#if GLOBAL_Q == 27
#define _IQtoF(A)               _IQ27toF(A)
#endif
#if GLOBAL_Q == 26
#define _IQtoF(A)               _IQ26toF(A)
#endif
#if GLOBAL_Q == 25
#define _IQtoF(A)               _IQ25toF(A)
#endif
#if GLOBAL_Q == 24
#define _IQtoF(A)               _IQ24toF(A)
#endif
#if GLOBAL_Q == 23
#define _IQtoF(A)               _IQ23toF(A)
#endif
#if GLOBAL_Q == 22
#define _IQtoF(A)               _IQ22toF(A)
#endif
#if GLOBAL_Q == 21
#define _IQtoF(A)               _IQ21toF(A)
#endif
#if GLOBAL_Q == 20
#define _IQtoF(A)               _IQ20toF(A)
#endif
#if GLOBAL_Q == 19
#define _IQtoF(A)               _IQ19toF(A)
#endif
#if GLOBAL_Q == 18
#define _IQtoF(A)               _IQ18toF(A)
#endif
#if GLOBAL_Q == 17
#define _IQtoF(A)               _IQ17toF(A)
#endif
#if GLOBAL_Q == 16
#define _IQtoF(A)               _IQ16toF(A)
#endif
#if GLOBAL_Q == 15
#define _IQtoF(A)               _IQ15toF(A)
#endif
#if GLOBAL_Q == 14
#define _IQtoF(A)               _IQ14toF(A)
#endif
#if GLOBAL_Q == 13
#define _IQtoF(A)               _IQ13toF(A)
#endif
#if GLOBAL_Q == 12
#define _IQtoF(A)               _IQ12toF(A)
#endif
#if GLOBAL_Q == 11
#define _IQtoF(A)               _IQ11toF(A)
#endif
#if GLOBAL_Q == 10
#define _IQtoF(A)               _IQ10toF(A)
#endif
#if GLOBAL_Q == 9
#define _IQtoF(A)               _IQ9toF(A)
#endif
#if GLOBAL_Q == 8
#define _IQtoF(A)               _IQ8toF(A)
#endif
#if GLOBAL_Q == 7
#define _IQtoF(A)               _IQ7toF(A)
#endif
#if GLOBAL_Q == 6
#define _IQtoF(A)               _IQ6toF(A)
#endif
#if GLOBAL_Q == 5
#define _IQtoF(A)               _IQ5toF(A)
#endif
#if GLOBAL_Q == 4
#define _IQtoF(A)               _IQ4toF(A)
#endif
#if GLOBAL_Q == 3
#define _IQtoF(A)               _IQ3toF(A)
#endif
#if GLOBAL_Q == 2
#define _IQtoF(A)               _IQ2toF(A)
#endif
#if GLOBAL_Q == 1
#define _IQtoF(A)               _IQ1toF(A)
#endif


extern double _IQ30toD(_iq30 A);
extern double _IQ29toD(_iq29 A);
extern double _IQ28toD(_iq28 A);
extern double _IQ27toD(_iq27 A);
extern double _IQ26toD(_iq26 A);
extern double _IQ25toD(_iq25 A);
extern double _IQ24toD(_iq24 A);
extern double _IQ23toD(_iq23 A);
extern double _IQ22toD(_iq22 A);
extern double _IQ21toD(_iq21 A);
extern double _IQ20toD(_iq20 A);
extern double _IQ19toD(_iq19 A);
extern double _IQ18toD(_iq18 A);
extern double _IQ17toD(_iq17 A);
extern double _IQ16toD(_iq16 A);
extern double _IQ15toD(_iq15 A);
extern double _IQ14toD(_iq14 A);
extern double _IQ13toD(_iq13 A);
extern double _IQ12toD(_iq12 A);
extern double _IQ11toD(_iq11 A);
extern double _IQ10toD(_iq10 A);
extern double _IQ9toD(_iq9 A);
extern double _IQ8toD(_iq8 A);
extern double _IQ7toD(_iq7 A);
extern double _IQ6toD(_iq6 A);
extern double _IQ5toD(_iq5 A);
extern double _IQ4toD(_iq4 A);
extern double _IQ3toD(_iq3 A);
extern double _IQ2toD(_iq2 A);
extern double _IQ1toD(_iq1 A);

/*******************************************************************************
* Function Name  : _IQtoD
* Description    : 将IQ格式的数转换成双精度浮点数
* Input          : IQ类型数据
* Return         : 转换后的double类型的数据
*******************************************************************************/
#if GLOBAL_Q == 30
#define _IQtoD(A)               _IQ30toD(A)
#endif
#if GLOBAL_Q == 29
#define _IQtoD(A)               _IQ29toD(A)
#endif
#if GLOBAL_Q == 28
#define _IQtoD(A)               _IQ28toD(A)
#endif
#if GLOBAL_Q == 27
#define _IQtoD(A)               _IQ27toD(A)
#endif
#if GLOBAL_Q == 26
#define _IQtoD(A)               _IQ26toD(A)
#endif
#if GLOBAL_Q == 25
#define _IQtoD(A)               _IQ25toD(A)
#endif
#if GLOBAL_Q == 24
#define _IQtoD(A)               _IQ24toD(A)
#endif
#if GLOBAL_Q == 23
#define _IQtoD(A)               _IQ23toD(A)
#endif
#if GLOBAL_Q == 22
#define _IQtoD(A)               _IQ22toD(A)
#endif
#if GLOBAL_Q == 21
#define _IQtoD(A)               _IQ21toD(A)
#endif
#if GLOBAL_Q == 20
#define _IQtoD(A)               _IQ20toD(A)
#endif
#if GLOBAL_Q == 19
#define _IQtoD(A)               _IQ19toD(A)
#endif
#if GLOBAL_Q == 18
#define _IQtoD(A)               _IQ18toD(A)
#endif
#if GLOBAL_Q == 17
#define _IQtoD(A)               _IQ17toD(A)
#endif
#if GLOBAL_Q == 16
#define _IQtoD(A)               _IQ16toD(A)
#endif
#if GLOBAL_Q == 15
#define _IQtoD(A)               _IQ15toD(A)
#endif
#if GLOBAL_Q == 14
#define _IQtoD(A)               _IQ14toD(A)
#endif
#if GLOBAL_Q == 13
#define _IQtoD(A)               _IQ13toD(A)
#endif
#if GLOBAL_Q == 12
#define _IQtoD(A)               _IQ12toD(A)
#endif
#if GLOBAL_Q == 11
#define _IQtoD(A)               _IQ11toD(A)
#endif
#if GLOBAL_Q == 10
#define _IQtoD(A)               _IQ10toD(A)
#endif
#if GLOBAL_Q == 9
#define _IQtoD(A)               _IQ9toD(A)
#endif
#if GLOBAL_Q == 8
#define _IQtoD(A)               _IQ8toD(A)
#endif
#if GLOBAL_Q == 7
#define _IQtoD(A)               _IQ7toD(A)
#endif
#if GLOBAL_Q == 6
#define _IQtoD(A)               _IQ6toD(A)
#endif
#if GLOBAL_Q == 5
#define _IQtoD(A)               _IQ5toD(A)
#endif
#if GLOBAL_Q == 4
#define _IQtoD(A)               _IQ4toD(A)
#endif
#if GLOBAL_Q == 3
#define _IQtoD(A)               _IQ3toD(A)
#endif
#if GLOBAL_Q == 2
#define _IQtoD(A)               _IQ2toD(A)
#endif
#if GLOBAL_Q == 1
#define _IQtoD(A)               _IQ1toD(A)
#endif


#define _IQsat(A, Pos, Neg)     (((A) > (Pos)) ?                              \
                                 (Pos) :                                      \
                                 (((A) < (Neg)) ? (Neg) : (A)))

/* GLOBAL_Q指定的IQ精度的数与指定IQ精度数之间相互转换函数 */
#define _IQtoIQ30(A)            ((_iq30)(A) << (30 - GLOBAL_Q))
#define _IQ30toIQ(A)            ((_iq30)(A) >> (30 - GLOBAL_Q))

#if (GLOBAL_Q >= 29)
#define _IQtoIQ29(A)            ((_iq29)(A) >> (GLOBAL_Q - 29))
#define _IQ29toIQ(A)            ((_iq29)(A) << (GLOBAL_Q - 29))
#else
#define _IQtoIQ29(A)            ((_iq29)(A) << (29 - GLOBAL_Q))
#define _IQ29toIQ(A)            ((_iq29)(A) >> (29 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 28)
#define _IQtoIQ28(A)            ((_iq28)(A) >> (GLOBAL_Q - 28))
#define _IQ28toIQ(A)            ((_iq28)(A) << (GLOBAL_Q - 28))
#else
#define _IQtoIQ28(A)            ((_iq28)(A) << (28 - GLOBAL_Q))
#define _IQ28toIQ(A)            ((_iq28)(A) >> (28 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 27)
#define _IQtoIQ27(A)            ((_iq27)(A) >> (GLOBAL_Q - 27))
#define _IQ27toIQ(A)            ((_iq27)(A) << (GLOBAL_Q - 27))
#else
#define _IQtoIQ27(A)            ((_iq27)(A) << (27 - GLOBAL_Q))
#define _IQ27toIQ(A)            ((_iq27)(A) >> (27 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 26)
#define _IQtoIQ26(A)            ((_iq26)(A) >> (GLOBAL_Q - 26))
#define _IQ26toIQ(A)            ((_iq26)(A) << (GLOBAL_Q - 26))
#else
#define _IQtoIQ26(A)            ((_iq26)(A) << (26 - GLOBAL_Q))
#define _IQ26toIQ(A)            ((_iq26)(A) >> (26 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 25)
#define _IQtoIQ25(A)            ((_iq25)(A) >> (GLOBAL_Q - 25))
#define _IQ25toIQ(A)            ((_iq25)(A) << (GLOBAL_Q - 25))
#else
#define _IQtoIQ25(A)            ((_iq25)(A) << (25 - GLOBAL_Q))
#define _IQ25toIQ(A)            ((_iq25)(A) >> (25 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 24)
#define _IQtoIQ24(A)            ((_iq24)(A) >> (GLOBAL_Q - 24))
#define _IQ24toIQ(A)            ((_iq24)(A) << (GLOBAL_Q - 24))
#else
#define _IQtoIQ24(A)            ((_iq24)(A) << (24 - GLOBAL_Q))
#define _IQ24toIQ(A)            ((_iq24)(A) >> (24 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 23)
#define _IQtoIQ23(A)            ((_iq23)(A) >> (GLOBAL_Q - 23))
#define _IQ23toIQ(A)            ((_iq23)(A) << (GLOBAL_Q - 23))
#else
#define _IQtoIQ23(A)            ((_iq23)(A) << (23 - GLOBAL_Q))
#define _IQ23toIQ(A)            ((_iq23)(A) >> (23 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 22)
#define _IQtoIQ22(A)            ((_iq22)(A) >> (GLOBAL_Q - 22))
#define _IQ22toIQ(A)            ((_iq22)(A) << (GLOBAL_Q - 22))
#else
#define _IQtoIQ22(A)            ((_iq22)(A) << (22 - GLOBAL_Q))
#define _IQ22toIQ(A)            ((_iq22)(A) >> (22 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 21)
#define _IQtoIQ21(A)            ((_iq21)(A) >> (GLOBAL_Q - 21))
#define _IQ21toIQ(A)            ((_iq21)(A) << (GLOBAL_Q - 21))
#else
#define _IQtoIQ21(A)            ((_iq21)(A) << (21 - GLOBAL_Q))
#define _IQ21toIQ(A)            ((_iq21)(A) >> (21 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 20)
#define _IQtoIQ20(A)            ((_iq20)(A) >> (GLOBAL_Q - 20))
#define _IQ20toIQ(A)            ((_iq20)(A) << (GLOBAL_Q - 20))
#else
#define _IQtoIQ20(A)            ((_iq20)(A) << (20 - GLOBAL_Q))
#define _IQ20toIQ(A)            ((_iq20)(A) >> (20 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 19)
#define _IQtoIQ19(A)            ((_iq19)(A) >> (GLOBAL_Q - 19))
#define _IQ19toIQ(A)            ((_iq19)(A) << (GLOBAL_Q - 19))
#else
#define _IQtoIQ19(A)            ((_iq19)(A) << (19 - GLOBAL_Q))
#define _IQ19toIQ(A)            ((_iq19)(A) >> (19 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 18)
#define _IQtoIQ18(A)            ((_iq18)(A) >> (GLOBAL_Q - 18))
#define _IQ18toIQ(A)            ((_iq18)(A) << (GLOBAL_Q - 18))
#else
#define _IQtoIQ18(A)            ((_iq18)(A) << (18 - GLOBAL_Q))
#define _IQ18toIQ(A)            ((_iq18)(A) >> (18 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 17)
#define _IQtoIQ17(A)            ((_iq17)(A) >> (GLOBAL_Q - 17))
#define _IQ17toIQ(A)            ((_iq17)(A) << (GLOBAL_Q - 17))
#else
#define _IQtoIQ17(A)            ((_iq17)(A) << (17 - GLOBAL_Q))
#define _IQ17toIQ(A)            ((_iq17)(A) >> (17 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 16)
#define _IQtoIQ16(A)            ((_iq16)(A) >> (GLOBAL_Q - 16))
#define _IQ16toIQ(A)            ((_iq16)(A) << (GLOBAL_Q - 16))
#else
#define _IQtoIQ16(A)            ((_iq16)(A) << (16 - GLOBAL_Q))
#define _IQ16toIQ(A)            ((_iq16)(A) >> (16 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 15)
#define _IQtoIQ15(A)            ((_iq15)(A) >> (GLOBAL_Q - 15))
#define _IQ15toIQ(A)            ((_iq15)(A) << (GLOBAL_Q - 15))
#else
#define _IQtoIQ15(A)            ((_iq15)(A) << (15 - GLOBAL_Q))
#define _IQ15toIQ(A)            ((_iq15)(A) >> (15 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 14)
#define _IQtoIQ14(A)            ((_iq14)(A) >> (GLOBAL_Q - 14))
#define _IQ14toIQ(A)            ((_iq14)(A) << (GLOBAL_Q - 14))
#else
#define _IQtoIQ14(A)            ((_iq14)(A) << (14 - GLOBAL_Q))
#define _IQ14toIQ(A)            ((_iq14)(A) >> (14 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 13)
#define _IQtoIQ13(A)            ((_iq13)(A) >> (GLOBAL_Q - 13))
#define _IQ13toIQ(A)            ((_iq13)(A) << (GLOBAL_Q - 13))
#else
#define _IQtoIQ13(A)            ((_iq13)(A) << (13 - GLOBAL_Q))
#define _IQ13toIQ(A)            ((_iq13)(A) >> (13 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 12)
#define _IQtoIQ12(A)            ((_iq12)(A) >> (GLOBAL_Q - 12))
#define _IQ12toIQ(A)            ((_iq12)(A) << (GLOBAL_Q - 12))
#else
#define _IQtoIQ12(A)            ((_iq12)(A) << (12 - GLOBAL_Q))
#define _IQ12toIQ(A)            ((_iq12)(A) >> (12 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 11)
#define _IQtoIQ11(A)            ((_iq11)(A) >> (GLOBAL_Q - 11))
#define _IQ11toIQ(A)            ((_iq11)(A) << (GLOBAL_Q - 11))
#else
#define _IQtoIQ11(A)            ((_iq11)(A) << (11 - GLOBAL_Q))
#define _IQ11toIQ(A)            ((_iq11)(A) >> (11 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 10)
#define _IQtoIQ10(A)            ((_iq10)(A) >> (GLOBAL_Q - 10))
#define _IQ10toIQ(A)            ((_iq10)(A) << (GLOBAL_Q - 10))
#else
#define _IQtoIQ10(A)            ((_iq10)(A) << (10 - GLOBAL_Q))
#define _IQ10toIQ(A)            ((_iq10)(A) >> (10 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 9)
#define _IQtoIQ9(A)             ((_iq9)(A) >> (GLOBAL_Q - 9))
#define _IQ9toIQ(A)             ((_iq9)(A) << (GLOBAL_Q - 9))
#else
#define _IQtoIQ9(A)             ((_iq9)(A) << (9 - GLOBAL_Q))
#define _IQ9toIQ(A)             ((_iq9)(A) >> (9 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 8)
#define _IQtoIQ8(A)             ((_iq8)(A) >> (GLOBAL_Q - 8))
#define _IQ8toIQ(A)             ((_iq8)(A) << (GLOBAL_Q - 8))
#else
#define _IQtoIQ8(A)             ((_iq8)(A) << (8 - GLOBAL_Q))
#define _IQ8toIQ(A)             ((_iq8)(A) >> (8 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 7)
#define _IQtoIQ7(A)             ((_iq7)(A) >> (GLOBAL_Q - 7))
#define _IQ7toIQ(A)             ((_iq7)(A) << (GLOBAL_Q - 7))
#else
#define _IQtoIQ7(A)             ((_iq7)(A) << (7 - GLOBAL_Q))
#define _IQ7toIQ(A)             ((_iq7)(A) >> (7 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 6)
#define _IQtoIQ6(A)             ((_iq6)(A) >> (GLOBAL_Q - 6))
#define _IQ6toIQ(A)             ((_iq6)(A) << (GLOBAL_Q - 6))
#else
#define _IQtoIQ6(A)             ((_iq6)(A) << (6 - GLOBAL_Q))
#define _IQ6toIQ(A)             ((_iq6)(A) >> (6 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 5)
#define _IQtoIQ5(A)             ((_iq5)(A) >> (GLOBAL_Q - 5))
#define _IQ5toIQ(A)             ((_iq5)(A) << (GLOBAL_Q - 5))
#else
#define _IQtoIQ5(A)             ((_iq5)(A) << (5 - GLOBAL_Q))
#define _IQ5toIQ(A)             ((_iq5)(A) >> (5 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 4)
#define _IQtoIQ4(A)             ((_iq4)(A) >> (GLOBAL_Q - 4))
#define _IQ4toIQ(A)             ((_iq4)(A) << (GLOBAL_Q - 4))
#else
#define _IQtoIQ4(A)             ((_iq4)(A) << (4 - GLOBAL_Q))
#define _IQ4toIQ(A)             ((_iq4)(A) >> (4 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 3)
#define _IQtoIQ3(A)             ((_iq3)(A) >> (GLOBAL_Q - 3))
#define _IQ3toIQ(A)             ((_iq3)(A) << (GLOBAL_Q - 3))
#else
#define _IQtoIQ3(A)             ((_iq3)(A) << (3 - GLOBAL_Q))
#define _IQ3toIQ(A)             ((_iq3)(A) >> (3 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 2)
#define _IQtoIQ2(A)             ((_iq2)(A) >> (GLOBAL_Q - 2))
#define _IQ2toIQ(A)             ((_iq2)(A) << (GLOBAL_Q - 2))
#else
#define _IQtoIQ2(A)             ((_iq2)(A) << (2 - GLOBAL_Q))
#define _IQ2toIQ(A)             ((_iq2)(A) >> (2 - GLOBAL_Q))
#endif

#define _IQtoIQ1(A)             ((_iq1)(A) >> (GLOBAL_Q - 1))
#define _IQ1toIQ(A)             ((_iq1)(A) << (GLOBAL_Q - 1))

#if (GLOBAL_Q >= 15)
#define _IQtoQ15(A)             ((long)(A) >> (GLOBAL_Q - 15))
#define _Q15toIQ(A)             ((_iq15)(A) << (GLOBAL_Q - 15))
#else
#define _IQtoQ15(A)             ((long)(A) << (15 - GLOBAL_Q))
#define _Q15toIQ(A)             ((_iq15)(A) >> (15 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 14)
#define _IQtoQ14(A)             ((long)(A) >> (GLOBAL_Q - 14))
#define _Q14toIQ(A)             ((_iq14)(A) << (GLOBAL_Q - 14))
#else
#define _IQtoQ14(A)             ((long)(A) << (14 - GLOBAL_Q))
#define _Q14toIQ(A)             ((_iq14)(A) >> (14 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 13)
#define _IQtoQ13(A)             ((long)(A) >> (GLOBAL_Q - 13))
#define _Q13toIQ(A)             ((_iq13)(A) << (GLOBAL_Q - 13))
#else
#define _IQtoQ13(A)             ((long)(A) << (13 - GLOBAL_Q))
#define _Q13toIQ(A)             ((_iq13)(A) >> (13 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 12)
#define _IQtoQ12(A)             ((long)(A) >> (GLOBAL_Q - 12))
#define _Q12toIQ(A)             ((_iq12)(A) << (GLOBAL_Q - 12))
#else
#define _IQtoQ12(A)             ((long)(A) << (12 - GLOBAL_Q))
#define _Q12toIQ(A)             ((_iq12)(A) >> (12 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 11)
#define _IQtoQ11(A)             ((long)(A) >> (GLOBAL_Q - 11))
#define _Q11toIQ(A)             ((_iq11)(A) << (GLOBAL_Q - 11))
#else
#define _IQtoQ11(A)             ((long)(A) << (11 - GLOBAL_Q))
#define _Q11toIQ(A)             ((_iq11)(A) >> (11 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 10)
#define _IQtoQ10(A)             ((long)(A) >> (GLOBAL_Q - 10))
#define _Q10toIQ(A)             ((_iq10)(A) << (GLOBAL_Q - 10))
#else
#define _IQtoQ10(A)             ((long)(A) << (10 - GLOBAL_Q))
#define _Q10toIQ(A)             ((_iq10)(A) >> (10 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 9)
#define _IQtoQ9(A)              ((long)(A) >> (GLOBAL_Q - 9))
#define _Q9toIQ(A)              ((_iq9)(A) << (GLOBAL_Q - 9))
#else
#define _IQtoQ9(A)              ((long)(A) << (9 - GLOBAL_Q))
#define _Q9toIQ(A)              ((_iq9)(A) >> (9 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 8)
#define _IQtoQ8(A)              ((long)(A) >> (GLOBAL_Q - 8))
#define _Q8toIQ(A)              ((_iq8)(A) << (GLOBAL_Q - 8))
#else
#define _IQtoQ8(A)              ((long)(A) << (8 - GLOBAL_Q))
#define _Q8toIQ(A)              ((_iq8)(A) >> (8 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 7)
#define _IQtoQ7(A)              ((long)(A) >> (GLOBAL_Q - 7))
#define _Q7toIQ(A)              ((_iq7)(A) << (GLOBAL_Q - 7))
#else
#define _IQtoQ7(A)              ((long)(A) << (7 - GLOBAL_Q))
#define _Q7toIQ(A)              ((_iq7)(A) >> (7 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 6)
#define _IQtoQ6(A)              ((long)(A) >> (GLOBAL_Q - 6))
#define _Q6toIQ(A)              ((_iq6)(A) << (GLOBAL_Q - 6))
#else
#define _IQtoQ6(A)              ((long)(A) << (6 - GLOBAL_Q))
#define _Q6toIQ(A)              ((_iq6)(A) >> (6 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 5)
#define _IQtoQ5(A)              ((long)(A) >> (GLOBAL_Q - 5))
#define _Q5toIQ(A)              ((_iq5)(A) << (GLOBAL_Q - 5))
#else
#define _IQtoQ5(A)              ((long)(A) << (5 - GLOBAL_Q))
#define _Q5toIQ(A)              ((_iq5)(A) >> (5 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 4)
#define _IQtoQ4(A)              ((long)(A) >> (GLOBAL_Q - 4))
#define _Q4toIQ(A)              ((_iq4)(A) << (GLOBAL_Q - 4))
#else
#define _IQtoQ4(A)              ((long)(A) << (4 - GLOBAL_Q))
#define _Q4toIQ(A)              ((_iq4)(A) >> (4 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 3)
#define _IQtoQ3(A)              ((long)(A) >> (GLOBAL_Q - 3))
#define _Q3toIQ(A)              ((_iq3)(A) << (GLOBAL_Q - 3))
#else
#define _IQtoQ3(A)              ((long)(A) << (3 - GLOBAL_Q))
#define _Q3toIQ(A)              ((_iq3)(A) >> (3 - GLOBAL_Q))
#endif

#if (GLOBAL_Q >= 2)
#define _IQtoQ2(A)              ((long)(A) >> (GLOBAL_Q - 2))
#define _Q2toIQ(A)              ((_iq2)(A) << (GLOBAL_Q - 2))
#else
#define _IQtoQ2(A)              ((long)(A) << (2 - GLOBAL_Q))
#define _Q2toIQ(A)              ((_iq2)(A) >> (2 - GLOBAL_Q))
#endif

#define _IQtoQ1(A)              ((long)(A) >> (GLOBAL_Q - 1))
#define _Q1toIQ(A)              ((_iq1)(A) << (GLOBAL_Q - 1))

extern _iq30 _IQ30mpy(_iq30 A, _iq30 B);
extern _iq29 _IQ29mpy(_iq29 A, _iq29 B);
extern _iq28 _IQ28mpy(_iq28 A, _iq28 B);
extern _iq27 _IQ27mpy(_iq27 A, _iq27 B);
extern _iq26 _IQ26mpy(_iq26 A, _iq26 B);
extern _iq25 _IQ25mpy(_iq25 A, _iq25 B);
extern _iq24 _IQ24mpy(_iq24 A, _iq24 B);
extern _iq23 _IQ23mpy(_iq23 A, _iq23 B);
extern _iq22 _IQ22mpy(_iq22 A, _iq22 B);
extern _iq21 _IQ21mpy(_iq21 A, _iq21 B);
extern _iq20 _IQ20mpy(_iq20 A, _iq20 B);
extern _iq19 _IQ19mpy(_iq19 A, _iq19 B);
extern _iq18 _IQ18mpy(_iq18 A, _iq18 B);
extern _iq17 _IQ17mpy(_iq17 A, _iq17 B);
extern _iq16 _IQ16mpy(_iq16 A, _iq16 B);
extern _iq15 _IQ15mpy(_iq15 A, _iq15 B);
extern _iq14 _IQ14mpy(_iq14 A, _iq14 B);
extern _iq13 _IQ13mpy(_iq13 A, _iq13 B);
extern _iq12 _IQ12mpy(_iq12 A, _iq12 B);
extern _iq11 _IQ11mpy(_iq11 A, _iq11 B);
extern _iq10 _IQ10mpy(_iq10 A, _iq10 B);
extern _iq9 _IQ9mpy(_iq9 A, _iq9 B);
extern _iq8 _IQ8mpy(_iq8 A, _iq8 B);
extern _iq7 _IQ7mpy(_iq7 A, _iq7 B);
extern _iq6 _IQ6mpy(_iq6 A, _iq6 B);
extern _iq5 _IQ5mpy(_iq5 A, _iq5 B);
extern _iq4 _IQ4mpy(_iq4 A, _iq4 B);
extern _iq3 _IQ3mpy(_iq3 A, _iq3 B);
extern _iq2 _IQ2mpy(_iq2 A, _iq2 B);
extern _iq1 _IQ1mpy(_iq1 A, _iq1 B);
/*******************************************************************************
* Function Name  : _IQmpy
* Description    : IQ格式数的乘法运算:A*B
* Input          : A、B为两个相同IQ格式的乘数
* Return         : A与B的乘积，与参数的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 30
#define _IQmpy(A, B)            _IQ30mpy(A, B)
#endif
#if GLOBAL_Q == 29
#define _IQmpy(A, B)            _IQ29mpy(A, B)
#endif
#if GLOBAL_Q == 28
#define _IQmpy(A, B)            _IQ28mpy(A, B)
#endif
#if GLOBAL_Q == 27
#define _IQmpy(A, B)            _IQ27mpy(A, B)
#endif
#if GLOBAL_Q == 26
#define _IQmpy(A, B)            _IQ26mpy(A, B)
#endif
#if GLOBAL_Q == 25
#define _IQmpy(A, B)            _IQ25mpy(A, B)
#endif
#if GLOBAL_Q == 24
#define _IQmpy(A, B)            _IQ24mpy(A, B)
#endif
#if GLOBAL_Q == 23
#define _IQmpy(A, B)            _IQ23mpy(A, B)
#endif
#if GLOBAL_Q == 22
#define _IQmpy(A, B)            _IQ22mpy(A, B)
#endif
#if GLOBAL_Q == 21
#define _IQmpy(A, B)            _IQ21mpy(A, B)
#endif
#if GLOBAL_Q == 20
#define _IQmpy(A, B)            _IQ20mpy(A, B)
#endif
#if GLOBAL_Q == 19
#define _IQmpy(A, B)            _IQ19mpy(A, B)
#endif
#if GLOBAL_Q == 18
#define _IQmpy(A, B)            _IQ18mpy(A, B)
#endif
#if GLOBAL_Q == 17
#define _IQmpy(A, B)            _IQ17mpy(A, B)
#endif
#if GLOBAL_Q == 16
#define _IQmpy(A, B)            _IQ16mpy(A, B)
#endif
#if GLOBAL_Q == 15
#define _IQmpy(A, B)            _IQ15mpy(A, B)
#endif
#if GLOBAL_Q == 14
#define _IQmpy(A, B)            _IQ14mpy(A, B)
#endif
#if GLOBAL_Q == 13
#define _IQmpy(A, B)            _IQ13mpy(A, B)
#endif
#if GLOBAL_Q == 12
#define _IQmpy(A, B)            _IQ12mpy(A, B)
#endif
#if GLOBAL_Q == 11
#define _IQmpy(A, B)            _IQ11mpy(A, B)
#endif
#if GLOBAL_Q == 10
#define _IQmpy(A, B)            _IQ10mpy(A, B)
#endif
#if GLOBAL_Q == 9
#define _IQmpy(A, B)            _IQ9mpy(A, B)
#endif
#if GLOBAL_Q == 8
#define _IQmpy(A, B)            _IQ8mpy(A, B)
#endif
#if GLOBAL_Q == 7
#define _IQmpy(A, B)            _IQ7mpy(A, B)
#endif
#if GLOBAL_Q == 6
#define _IQmpy(A, B)            _IQ6mpy(A, B)
#endif
#if GLOBAL_Q == 5
#define _IQmpy(A, B)            _IQ5mpy(A, B)
#endif
#if GLOBAL_Q == 4
#define _IQmpy(A, B)            _IQ4mpy(A, B)
#endif
#if GLOBAL_Q == 3
#define _IQmpy(A, B)            _IQ3mpy(A, B)
#endif
#if GLOBAL_Q == 2
#define _IQmpy(A, B)            _IQ2mpy(A, B)
#endif
#if GLOBAL_Q == 1
#define _IQmpy(A, B)            _IQ1mpy(A, B)
#endif


extern _iq30 _IQ30rmpy(_iq30 A, _iq30 B);
extern _iq29 _IQ29rmpy(_iq29 A, _iq29 B);
extern _iq28 _IQ28rmpy(_iq28 A, _iq28 B);
extern _iq27 _IQ27rmpy(_iq27 A, _iq27 B);
extern _iq26 _IQ26rmpy(_iq26 A, _iq26 B);
extern _iq25 _IQ25rmpy(_iq25 A, _iq25 B);
extern _iq24 _IQ24rmpy(_iq24 A, _iq24 B);
extern _iq23 _IQ23rmpy(_iq23 A, _iq23 B);
extern _iq22 _IQ22rmpy(_iq22 A, _iq22 B);
extern _iq21 _IQ21rmpy(_iq21 A, _iq21 B);
extern _iq20 _IQ20rmpy(_iq20 A, _iq20 B);
extern _iq19 _IQ19rmpy(_iq19 A, _iq19 B);
extern _iq18 _IQ18rmpy(_iq18 A, _iq18 B);
extern _iq17 _IQ17rmpy(_iq17 A, _iq17 B);
extern _iq16 _IQ16rmpy(_iq16 A, _iq16 B);
extern _iq15 _IQ15rmpy(_iq15 A, _iq15 B);
extern _iq14 _IQ14rmpy(_iq14 A, _iq14 B);
extern _iq13 _IQ13rmpy(_iq13 A, _iq13 B);
extern _iq12 _IQ12rmpy(_iq12 A, _iq12 B);
extern _iq11 _IQ11rmpy(_iq11 A, _iq11 B);
extern _iq10 _IQ10rmpy(_iq10 A, _iq10 B);
extern _iq9 _IQ9rmpy(_iq9 A, _iq9 B);
extern _iq8 _IQ8rmpy(_iq8 A, _iq8 B);
extern _iq7 _IQ7rmpy(_iq7 A, _iq7 B);
extern _iq6 _IQ6rmpy(_iq6 A, _iq6 B);
extern _iq5 _IQ5rmpy(_iq5 A, _iq5 B);
extern _iq4 _IQ4rmpy(_iq4 A, _iq4 B);
extern _iq3 _IQ3rmpy(_iq3 A, _iq3 B);
extern _iq2 _IQ2rmpy(_iq2 A, _iq2 B);
extern _iq1 _IQ1rmpy(_iq1 A, _iq1 B);
/*******************************************************************************
* Function Name  : _IQrmpy
* Description    : IQ格式数的乘法运算,结果4舍5入: A*B
* Input          : A、B为两个相同IQ格式的乘数
* Return         : A与B的乘积，与参数的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 30
#define _IQrmpy(A, B)           _IQ30rmpy(A, B)
#endif
#if GLOBAL_Q == 29
#define _IQrmpy(A, B)           _IQ29rmpy(A, B)
#endif
#if GLOBAL_Q == 28
#define _IQrmpy(A, B)           _IQ28rmpy(A, B)
#endif
#if GLOBAL_Q == 27
#define _IQrmpy(A, B)           _IQ27rmpy(A, B)
#endif
#if GLOBAL_Q == 26
#define _IQrmpy(A, B)           _IQ26rmpy(A, B)
#endif
#if GLOBAL_Q == 25
#define _IQrmpy(A, B)           _IQ25rmpy(A, B)
#endif
#if GLOBAL_Q == 24
#define _IQrmpy(A, B)           _IQ24rmpy(A, B)
#endif
#if GLOBAL_Q == 23
#define _IQrmpy(A, B)           _IQ23rmpy(A, B)
#endif
#if GLOBAL_Q == 22
#define _IQrmpy(A, B)           _IQ22rmpy(A, B)
#endif
#if GLOBAL_Q == 21
#define _IQrmpy(A, B)           _IQ21rmpy(A, B)
#endif
#if GLOBAL_Q == 20
#define _IQrmpy(A, B)           _IQ20rmpy(A, B)
#endif
#if GLOBAL_Q == 19
#define _IQrmpy(A, B)           _IQ19rmpy(A, B)
#endif
#if GLOBAL_Q == 18
#define _IQrmpy(A, B)           _IQ18rmpy(A, B)
#endif
#if GLOBAL_Q == 17
#define _IQrmpy(A, B)           _IQ17rmpy(A, B)
#endif
#if GLOBAL_Q == 16
#define _IQrmpy(A, B)           _IQ16rmpy(A, B)
#endif
#if GLOBAL_Q == 15
#define _IQrmpy(A, B)           _IQ15rmpy(A, B)
#endif
#if GLOBAL_Q == 14
#define _IQrmpy(A, B)           _IQ14rmpy(A, B)
#endif
#if GLOBAL_Q == 13
#define _IQrmpy(A, B)           _IQ13rmpy(A, B)
#endif
#if GLOBAL_Q == 12
#define _IQrmpy(A, B)           _IQ12rmpy(A, B)
#endif
#if GLOBAL_Q == 11
#define _IQrmpy(A, B)           _IQ11rmpy(A, B)
#endif
#if GLOBAL_Q == 10
#define _IQrmpy(A, B)           _IQ10rmpy(A, B)
#endif
#if GLOBAL_Q == 9
#define _IQrmpy(A, B)           _IQ9rmpy(A, B)
#endif
#if GLOBAL_Q == 8
#define _IQrmpy(A, B)           _IQ8rmpy(A, B)
#endif
#if GLOBAL_Q == 7
#define _IQrmpy(A, B)           _IQ7rmpy(A, B)
#endif
#if GLOBAL_Q == 6
#define _IQrmpy(A, B)           _IQ6rmpy(A, B)
#endif
#if GLOBAL_Q == 5
#define _IQrmpy(A, B)           _IQ5rmpy(A, B)
#endif
#if GLOBAL_Q == 4
#define _IQrmpy(A, B)           _IQ4rmpy(A, B)
#endif
#if GLOBAL_Q == 3
#define _IQrmpy(A, B)           _IQ3rmpy(A, B)
#endif
#if GLOBAL_Q == 2
#define _IQrmpy(A, B)           _IQ2rmpy(A, B)
#endif
#if GLOBAL_Q == 1
#define _IQrmpy(A, B)           _IQ1rmpy(A, B)
#endif


extern _iq30 _IQ30div(_iq30 A, _iq30 B);
extern _iq29 _IQ29div(_iq29 A, _iq29 B);
extern _iq28 _IQ28div(_iq28 A, _iq28 B);
extern _iq27 _IQ27div(_iq27 A, _iq27 B);
extern _iq26 _IQ26div(_iq26 A, _iq26 B);
extern _iq25 _IQ25div(_iq25 A, _iq25 B);
extern _iq24 _IQ24div(_iq24 A, _iq24 B);
extern _iq23 _IQ23div(_iq23 A, _iq23 B);
extern _iq22 _IQ22div(_iq22 A, _iq22 B);
extern _iq21 _IQ21div(_iq21 A, _iq21 B);
extern _iq20 _IQ20div(_iq20 A, _iq20 B);
extern _iq19 _IQ19div(_iq19 A, _iq19 B);
extern _iq18 _IQ18div(_iq18 A, _iq18 B);
extern _iq17 _IQ17div(_iq17 A, _iq17 B);
extern _iq16 _IQ16div(_iq16 A, _iq16 B);
extern _iq15 _IQ15div(_iq15 A, _iq15 B);
extern _iq14 _IQ14div(_iq14 A, _iq14 B);
extern _iq13 _IQ13div(_iq13 A, _iq13 B);
extern _iq12 _IQ12div(_iq12 A, _iq12 B);
extern _iq11 _IQ11div(_iq11 A, _iq11 B);
extern _iq10 _IQ10div(_iq10 A, _iq10 B);
extern _iq9 _IQ9div(_iq9 A, _iq9 B);
extern _iq8 _IQ8div(_iq8 A, _iq8 B);
extern _iq7 _IQ7div(_iq7 A, _iq7 B);
extern _iq6 _IQ6div(_iq6 A, _iq6 B);
extern _iq5 _IQ5div(_iq5 A, _iq5 B);
extern _iq4 _IQ4div(_iq4 A, _iq4 B);
extern _iq3 _IQ3div(_iq3 A, _iq3 B);
extern _iq2 _IQ2div(_iq2 A, _iq2 B);
extern _iq1 _IQ1div(_iq1 A, _iq1 B);

/* 快速计算除法 */
extern _iq30 _IQ30div_FAST(_iq30 A, _iq30 B);
extern _iq29 _IQ29div_FAST(_iq29 A, _iq29 B);
extern _iq28 _IQ28div_FAST(_iq28 A, _iq28 B);
extern _iq27 _IQ27div_FAST(_iq27 A, _iq27 B);
extern _iq26 _IQ26div_FAST(_iq26 A, _iq26 B);
extern _iq25 _IQ25div_FAST(_iq25 A, _iq25 B);
extern _iq24 _IQ24div_FAST(_iq24 A, _iq24 B);
extern _iq23 _IQ23div_FAST(_iq23 A, _iq23 B);
extern _iq22 _IQ22div_FAST(_iq22 A, _iq22 B);
extern _iq21 _IQ21div_FAST(_iq21 A, _iq21 B);
extern _iq20 _IQ20div_FAST(_iq20 A, _iq20 B);
extern _iq19 _IQ19div_FAST(_iq19 A, _iq19 B);
extern _iq18 _IQ18div_FAST(_iq18 A, _iq18 B);
extern _iq17 _IQ17div_FAST(_iq17 A, _iq17 B);
extern _iq16 _IQ16div_FAST(_iq16 A, _iq16 B);
extern _iq15 _IQ15div_FAST(_iq15 A, _iq15 B);
extern _iq14 _IQ14div_FAST(_iq14 A, _iq14 B);
extern _iq13 _IQ13div_FAST(_iq13 A, _iq13 B);
extern _iq12 _IQ12div_FAST(_iq12 A, _iq12 B);
extern _iq11 _IQ11div_FAST(_iq11 A, _iq11 B);
extern _iq10 _IQ10div_FAST(_iq10 A, _iq10 B);
extern _iq9 _IQ9div_FAST(_iq9 A, _iq9 B);
extern _iq8 _IQ8div_FAST(_iq8 A, _iq8 B);
extern _iq7 _IQ7div_FAST(_iq7 A, _iq7 B);
extern _iq6 _IQ6div_FAST(_iq6 A, _iq6 B);
extern _iq5 _IQ5div_FAST(_iq5 A, _iq5 B);
extern _iq4 _IQ4div_FAST(_iq4 A, _iq4 B);
extern _iq3 _IQ3div_FAST(_iq3 A, _iq3 B);
extern _iq2 _IQ2div_FAST(_iq2 A, _iq2 B);
extern _iq1 _IQ1div_FAST(_iq1 A, _iq1 B);

/*******************************************************************************
* Function Name  : _IQdiv
* Description    : IQ格式数的除法运算:A/B
* Input          : A为被除数，B为除数，A与B为相同IQ格式的数
* Return         : A除以B的商，与参数的IQ格式相同
*******************************************************************************/
#if  DIV_FAST == 0
#if GLOBAL_Q == 30
#define _IQdiv(A, B)            _IQ30div(A, B)
#endif
#if GLOBAL_Q == 29
#define _IQdiv(A, B)            _IQ29div(A, B)
#endif
#if GLOBAL_Q == 28
#define _IQdiv(A, B)            _IQ28div(A, B)
#endif
#if GLOBAL_Q == 27
#define _IQdiv(A, B)            _IQ27div(A, B)
#endif
#if GLOBAL_Q == 26
#define _IQdiv(A, B)            _IQ26div(A, B)
#endif
#if GLOBAL_Q == 25
#define _IQdiv(A, B)            _IQ25div(A, B)
#endif
#if GLOBAL_Q == 24
#define _IQdiv(A, B)            _IQ24div(A, B)
#endif
#if GLOBAL_Q == 23
#define _IQdiv(A, B)            _IQ23div(A, B)
#endif
#if GLOBAL_Q == 22
#define _IQdiv(A, B)            _IQ22div(A, B)
#endif
#if GLOBAL_Q == 21
#define _IQdiv(A, B)            _IQ21div(A, B)
#endif
#if GLOBAL_Q == 20
#define _IQdiv(A, B)            _IQ20div(A, B)
#endif
#if GLOBAL_Q == 19
#define _IQdiv(A, B)            _IQ19div(A, B)
#endif
#if GLOBAL_Q == 18
#define _IQdiv(A, B)            _IQ18div(A, B)
#endif
#if GLOBAL_Q == 17
#define _IQdiv(A, B)            _IQ17div(A, B)
#endif
#if GLOBAL_Q == 16
#define _IQdiv(A, B)            _IQ16div(A, B)
#endif
#if GLOBAL_Q == 15
#define _IQdiv(A, B)            _IQ15div(A, B)
#endif
#if GLOBAL_Q == 14
#define _IQdiv(A, B)            _IQ14div(A, B)
#endif
#if GLOBAL_Q == 13
#define _IQdiv(A, B)            _IQ13div(A, B)
#endif
#if GLOBAL_Q == 12
#define _IQdiv(A, B)            _IQ12div(A, B)
#endif
#if GLOBAL_Q == 11
#define _IQdiv(A, B)            _IQ11div(A, B)
#endif
#if GLOBAL_Q == 10
#define _IQdiv(A, B)            _IQ10div(A, B)
#endif
#if GLOBAL_Q == 9
#define _IQdiv(A, B)            _IQ9div(A, B)
#endif
#if GLOBAL_Q == 8
#define _IQdiv(A, B)            _IQ8div(A, B)
#endif
#if GLOBAL_Q == 7
#define _IQdiv(A, B)            _IQ7div(A, B)
#endif
#if GLOBAL_Q == 6
#define _IQdiv(A, B)            _IQ6div(A, B)
#endif
#if GLOBAL_Q == 5
#define _IQdiv(A, B)            _IQ5div(A, B)
#endif
#if GLOBAL_Q == 4
#define _IQdiv(A, B)            _IQ4div(A, B)
#endif
#if GLOBAL_Q == 3
#define _IQdiv(A, B)            _IQ3div(A, B)
#endif
#if GLOBAL_Q == 2
#define _IQdiv(A, B)            _IQ2div(A, B)
#endif
#if GLOBAL_Q == 1
#define _IQdiv(A, B)            _IQ1div(A, B)
#endif

#else //DIV_FAST == 1

#if GLOBAL_Q == 30
#define _IQdiv(A, B)            _IQ30div_FAST(A, B)
#endif
#if GLOBAL_Q == 29
#define _IQdiv(A, B)            _IQ29div_FAST(A, B)
#endif
#if GLOBAL_Q == 28
#define _IQdiv(A, B)            _IQ28div_FAST(A, B)
#endif
#if GLOBAL_Q == 27
#define _IQdiv(A, B)            _IQ27div_FAST(A, B)
#endif
#if GLOBAL_Q == 26
#define _IQdiv(A, B)            _IQ26div_FAST(A, B)
#endif
#if GLOBAL_Q == 25
#define _IQdiv(A, B)            _IQ25div_FAST(A, B)
#endif
#if GLOBAL_Q == 24
#define _IQdiv(A, B)            _IQ24div_FAST(A, B)
#endif
#if GLOBAL_Q == 23
#define _IQdiv(A, B)            _IQ23div_FAST(A, B)
#endif
#if GLOBAL_Q == 22
#define _IQdiv(A, B)            _IQ22div_FAST(A, B)
#endif
#if GLOBAL_Q == 21
#define _IQdiv(A, B)            _IQ21div_FAST(A, B)
#endif
#if GLOBAL_Q == 20
#define _IQdiv(A, B)            _IQ20div_FAST(A, B)
#endif
#if GLOBAL_Q == 19
#define _IQdiv(A, B)            _IQ19div_FAST(A, B)
#endif
#if GLOBAL_Q == 18
#define _IQdiv(A, B)            _IQ18div_FAST(A, B)
#endif
#if GLOBAL_Q == 17
#define _IQdiv(A, B)            _IQ17div_FAST(A, B)
#endif
#if GLOBAL_Q == 16
#define _IQdiv(A, B)            _IQ16div_FAST(A, B)
#endif
#if GLOBAL_Q == 15
#define _IQdiv(A, B)            _IQ15div_FAST(A, B)
#endif
#if GLOBAL_Q == 14
#define _IQdiv(A, B)            _IQ14div_FAST(A, B)
#endif
#if GLOBAL_Q == 13
#define _IQdiv(A, B)            _IQ13div_FAST(A, B)
#endif
#if GLOBAL_Q == 12
#define _IQdiv(A, B)            _IQ12div_FAST(A, B)
#endif
#if GLOBAL_Q == 11
#define _IQdiv(A, B)            _IQ11div_FAST(A, B)
#endif
#if GLOBAL_Q == 10
#define _IQdiv(A, B)            _IQ10div_FAST(A, B)
#endif
#if GLOBAL_Q == 9
#define _IQdiv(A, B)            _IQ9div_FAST(A, B)
#endif
#if GLOBAL_Q == 8
#define _IQdiv(A, B)            _IQ8div_FAST(A, B)
#endif
#if GLOBAL_Q == 7
#define _IQdiv(A, B)            _IQ7div_FAST(A, B)
#endif
#if GLOBAL_Q == 6
#define _IQdiv(A, B)            _IQ6div_FAST(A, B)
#endif
#if GLOBAL_Q == 5
#define _IQdiv(A, B)            _IQ5div_FAST(A, B)
#endif
#if GLOBAL_Q == 4
#define _IQdiv(A, B)            _IQ4div_FAST(A, B)
#endif
#if GLOBAL_Q == 3
#define _IQdiv(A, B)            _IQ3div_FAST(A, B)
#endif
#if GLOBAL_Q == 2
#define _IQdiv(A, B)            _IQ2div_FAST(A, B)
#endif
#if GLOBAL_Q == 1
#define _IQdiv(A, B)            _IQ1div_FAST(A, B)
#endif

#endif  //end for DIV_FAST == 0


extern _iq29 _IQ29sin(_iq29 A);
extern _iq28 _IQ28sin(_iq28 A);
extern _iq27 _IQ27sin(_iq27 A);
extern _iq26 _IQ26sin(_iq26 A);
extern _iq25 _IQ25sin(_iq25 A);
extern _iq24 _IQ24sin(_iq24 A);
extern _iq23 _IQ23sin(_iq23 A);
extern _iq22 _IQ22sin(_iq22 A);
extern _iq21 _IQ21sin(_iq21 A);
extern _iq20 _IQ20sin(_iq20 A);
extern _iq19 _IQ19sin(_iq19 A);
extern _iq18 _IQ18sin(_iq18 A);
extern _iq17 _IQ17sin(_iq17 A);
extern _iq16 _IQ16sin(_iq16 A);
extern _iq15 _IQ15sin(_iq15 A);
extern _iq14 _IQ14sin(_iq14 A);
extern _iq13 _IQ13sin(_iq13 A);
extern _iq12 _IQ12sin(_iq12 A);
extern _iq11 _IQ11sin(_iq11 A);
extern _iq10 _IQ10sin(_iq10 A);
extern _iq9 _IQ9sin(_iq9 A);
extern _iq8 _IQ8sin(_iq8 A);
extern _iq7 _IQ7sin(_iq7 A);
extern _iq6 _IQ6sin(_iq6 A);
extern _iq5 _IQ5sin(_iq5 A);
extern _iq4 _IQ4sin(_iq4 A);
extern _iq3 _IQ3sin(_iq3 A);
extern _iq2 _IQ2sin(_iq2 A);
extern _iq1 _IQ1sin(_iq1 A);
/*******************************************************************************
* Function Name  : _IQsin
* Description    : IQ格式数的正弦函数:sin(A)
* Input          : 输入参数为IQ格式的弧度值
* Return         : 返回A的正弦值，与参数的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 29
#define _IQsin(A)               _IQ29sin(A)
#endif
#if GLOBAL_Q == 28
#define _IQsin(A)               _IQ28sin(A)
#endif
#if GLOBAL_Q == 27
#define _IQsin(A)               _IQ27sin(A)
#endif
#if GLOBAL_Q == 26
#define _IQsin(A)               _IQ26sin(A)
#endif
#if GLOBAL_Q == 25
#define _IQsin(A)               _IQ25sin(A)
#endif
#if GLOBAL_Q == 24
#define _IQsin(A)               _IQ24sin(A)
#endif
#if GLOBAL_Q == 23
#define _IQsin(A)               _IQ23sin(A)
#endif
#if GLOBAL_Q == 22
#define _IQsin(A)               _IQ22sin(A)
#endif
#if GLOBAL_Q == 21
#define _IQsin(A)               _IQ21sin(A)
#endif
#if GLOBAL_Q == 20
#define _IQsin(A)               _IQ20sin(A)
#endif
#if GLOBAL_Q == 19
#define _IQsin(A)               _IQ19sin(A)
#endif
#if GLOBAL_Q == 18
#define _IQsin(A)               _IQ18sin(A)
#endif
#if GLOBAL_Q == 17
#define _IQsin(A)               _IQ17sin(A)
#endif
#if GLOBAL_Q == 16
#define _IQsin(A)               _IQ16sin(A)
#endif
#if GLOBAL_Q == 15
#define _IQsin(A)               _IQ15sin(A)
#endif
#if GLOBAL_Q == 14
#define _IQsin(A)               _IQ14sin(A)
#endif
#if GLOBAL_Q == 13
#define _IQsin(A)               _IQ13sin(A)
#endif
#if GLOBAL_Q == 12
#define _IQsin(A)               _IQ12sin(A)
#endif
#if GLOBAL_Q == 11
#define _IQsin(A)               _IQ11sin(A)
#endif
#if GLOBAL_Q == 10
#define _IQsin(A)               _IQ10sin(A)
#endif
#if GLOBAL_Q == 9
#define _IQsin(A)               _IQ9sin(A)
#endif
#if GLOBAL_Q == 8
#define _IQsin(A)               _IQ8sin(A)
#endif
#if GLOBAL_Q == 7
#define _IQsin(A)               _IQ7sin(A)
#endif
#if GLOBAL_Q == 6
#define _IQsin(A)               _IQ6sin(A)
#endif
#if GLOBAL_Q == 5
#define _IQsin(A)               _IQ5sin(A)
#endif
#if GLOBAL_Q == 4
#define _IQsin(A)               _IQ4sin(A)
#endif
#if GLOBAL_Q == 3
#define _IQsin(A)               _IQ3sin(A)
#endif
#if GLOBAL_Q == 2
#define _IQsin(A)               _IQ2sin(A)
#endif
#if GLOBAL_Q == 1
#define _IQsin(A)               _IQ1sin(A)
#endif


extern _iq30 _IQ30sinPU(_iq30 A);
extern _iq29 _IQ29sinPU(_iq29 A);
extern _iq28 _IQ28sinPU(_iq28 A);
extern _iq27 _IQ27sinPU(_iq27 A);
extern _iq26 _IQ26sinPU(_iq26 A);
extern _iq25 _IQ25sinPU(_iq25 A);
extern _iq24 _IQ24sinPU(_iq24 A);
extern _iq23 _IQ23sinPU(_iq23 A);
extern _iq22 _IQ22sinPU(_iq22 A);
extern _iq21 _IQ21sinPU(_iq21 A);
extern _iq20 _IQ20sinPU(_iq20 A);
extern _iq19 _IQ19sinPU(_iq19 A);
extern _iq18 _IQ18sinPU(_iq18 A);
extern _iq17 _IQ17sinPU(_iq17 A);
extern _iq16 _IQ16sinPU(_iq16 A);
extern _iq15 _IQ15sinPU(_iq15 A);
extern _iq14 _IQ14sinPU(_iq14 A);
extern _iq13 _IQ13sinPU(_iq13 A);
extern _iq12 _IQ12sinPU(_iq12 A);
extern _iq11 _IQ11sinPU(_iq11 A);
extern _iq10 _IQ10sinPU(_iq10 A);
extern _iq9 _IQ9sinPU(_iq9 A);
extern _iq8 _IQ8sinPU(_iq8 A);
extern _iq7 _IQ7sinPU(_iq7 A);
extern _iq6 _IQ6sinPU(_iq6 A);
extern _iq5 _IQ5sinPU(_iq5 A);
extern _iq4 _IQ4sinPU(_iq4 A);
extern _iq3 _IQ3sinPU(_iq3 A);
extern _iq2 _IQ2sinPU(_iq2 A);
extern _iq1 _IQ1sinPU(_iq1 A);
/*******************************************************************************
* Function Name  : _IQsinPU
* Description    : IQ格式数的正弦函数：sin((2*PI)*A)
* Input          : 输入参数是几分之一个圆
* Return         : 返回A的正弦值，与参数的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 30
#define _IQsinPU(A)             _IQ30sinPU(A)
#endif
#if GLOBAL_Q == 29
#define _IQsinPU(A)             _IQ29sinPU(A)
#endif
#if GLOBAL_Q == 28
#define _IQsinPU(A)             _IQ28sinPU(A)
#endif
#if GLOBAL_Q == 27
#define _IQsinPU(A)             _IQ27sinPU(A)
#endif
#if GLOBAL_Q == 26
#define _IQsinPU(A)             _IQ26sinPU(A)
#endif
#if GLOBAL_Q == 25
#define _IQsinPU(A)             _IQ25sinPU(A)
#endif
#if GLOBAL_Q == 24
#define _IQsinPU(A)             _IQ24sinPU(A)
#endif
#if GLOBAL_Q == 23
#define _IQsinPU(A)             _IQ23sinPU(A)
#endif
#if GLOBAL_Q == 22
#define _IQsinPU(A)             _IQ22sinPU(A)
#endif
#if GLOBAL_Q == 21
#define _IQsinPU(A)             _IQ21sinPU(A)
#endif
#if GLOBAL_Q == 20
#define _IQsinPU(A)             _IQ20sinPU(A)
#endif
#if GLOBAL_Q == 19
#define _IQsinPU(A)             _IQ19sinPU(A)
#endif
#if GLOBAL_Q == 18
#define _IQsinPU(A)             _IQ18sinPU(A)
#endif
#if GLOBAL_Q == 17
#define _IQsinPU(A)             _IQ17sinPU(A)
#endif
#if GLOBAL_Q == 16
#define _IQsinPU(A)             _IQ16sinPU(A)
#endif
#if GLOBAL_Q == 15
#define _IQsinPU(A)             _IQ15sinPU(A)
#endif
#if GLOBAL_Q == 14
#define _IQsinPU(A)             _IQ14sinPU(A)
#endif
#if GLOBAL_Q == 13
#define _IQsinPU(A)             _IQ13sinPU(A)
#endif
#if GLOBAL_Q == 12
#define _IQsinPU(A)             _IQ12sinPU(A)
#endif
#if GLOBAL_Q == 11
#define _IQsinPU(A)             _IQ11sinPU(A)
#endif
#if GLOBAL_Q == 10
#define _IQsinPU(A)             _IQ10sinPU(A)
#endif
#if GLOBAL_Q == 9
#define _IQsinPU(A)             _IQ9sinPU(A)
#endif
#if GLOBAL_Q == 8
#define _IQsinPU(A)             _IQ8sinPU(A)
#endif
#if GLOBAL_Q == 7
#define _IQsinPU(A)             _IQ7sinPU(A)
#endif
#if GLOBAL_Q == 6
#define _IQsinPU(A)             _IQ6sinPU(A)
#endif
#if GLOBAL_Q == 5
#define _IQsinPU(A)             _IQ5sinPU(A)
#endif
#if GLOBAL_Q == 4
#define _IQsinPU(A)             _IQ4sinPU(A)
#endif
#if GLOBAL_Q == 3
#define _IQsinPU(A)             _IQ3sinPU(A)
#endif
#if GLOBAL_Q == 2
#define _IQsinPU(A)             _IQ2sinPU(A)
#endif
#if GLOBAL_Q == 1
#define _IQsinPU(A)             _IQ1sinPU(A)
#endif

extern _iq29 _IQ29asin(_iq29 A);
extern _iq28 _IQ28asin(_iq28 A);
extern _iq27 _IQ27asin(_iq27 A);
extern _iq26 _IQ26asin(_iq26 A);
extern _iq25 _IQ25asin(_iq25 A);
extern _iq24 _IQ24asin(_iq24 A);
extern _iq23 _IQ23asin(_iq23 A);
extern _iq22 _IQ22asin(_iq22 A);
extern _iq21 _IQ21asin(_iq21 A);
extern _iq20 _IQ20asin(_iq20 A);
extern _iq19 _IQ19asin(_iq19 A);
extern _iq18 _IQ18asin(_iq18 A);
extern _iq17 _IQ17asin(_iq17 A);
extern _iq16 _IQ16asin(_iq16 A);
extern _iq15 _IQ15asin(_iq15 A);
extern _iq14 _IQ14asin(_iq14 A);
extern _iq13 _IQ13asin(_iq13 A);
extern _iq12 _IQ12asin(_iq12 A);
extern _iq11 _IQ11asin(_iq11 A);
extern _iq10 _IQ10asin(_iq10 A);
extern _iq9 _IQ9asin(_iq9 A);
extern _iq8 _IQ8asin(_iq8 A);
extern _iq7 _IQ7asin(_iq7 A);
extern _iq6 _IQ6asin(_iq6 A);
extern _iq5 _IQ5asin(_iq5 A);
extern _iq4 _IQ4asin(_iq4 A);
extern _iq3 _IQ3asin(_iq3 A);
extern _iq2 _IQ2asin(_iq2 A);
extern _iq1 _IQ1asin(_iq1 A);
/*******************************************************************************
* Function Name  : _IQasin
* Description    : IQ格式数的反正弦函数:asin(A)
* Input          : 输入参数为IQ格式的弧度值
* Return         : 返回A的反正弦值，与参数的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 29
#define _IQasin(A)              _IQ29asin(A)
#endif
#if GLOBAL_Q == 28
#define _IQasin(A)              _IQ28asin(A)
#endif
#if GLOBAL_Q == 27
#define _IQasin(A)              _IQ27asin(A)
#endif
#if GLOBAL_Q == 26
#define _IQasin(A)              _IQ26asin(A)
#endif
#if GLOBAL_Q == 25
#define _IQasin(A)              _IQ25asin(A)
#endif
#if GLOBAL_Q == 24
#define _IQasin(A)              _IQ24asin(A)
#endif
#if GLOBAL_Q == 23
#define _IQasin(A)              _IQ23asin(A)
#endif
#if GLOBAL_Q == 22
#define _IQasin(A)              _IQ22asin(A)
#endif
#if GLOBAL_Q == 21
#define _IQasin(A)              _IQ21asin(A)
#endif
#if GLOBAL_Q == 20
#define _IQasin(A)              _IQ20asin(A)
#endif
#if GLOBAL_Q == 19
#define _IQasin(A)              _IQ19asin(A)
#endif
#if GLOBAL_Q == 18
#define _IQasin(A)              _IQ18asin(A)
#endif
#if GLOBAL_Q == 17
#define _IQasin(A)              _IQ17asin(A)
#endif
#if GLOBAL_Q == 16
#define _IQasin(A)              _IQ16asin(A)
#endif
#if GLOBAL_Q == 15
#define _IQasin(A)              _IQ15asin(A)
#endif
#if GLOBAL_Q == 14
#define _IQasin(A)              _IQ14asin(A)
#endif
#if GLOBAL_Q == 13
#define _IQasin(A)              _IQ13asin(A)
#endif
#if GLOBAL_Q == 12
#define _IQasin(A)              _IQ12asin(A)
#endif
#if GLOBAL_Q == 11
#define _IQasin(A)              _IQ11asin(A)
#endif
#if GLOBAL_Q == 10
#define _IQasin(A)              _IQ10asin(A)
#endif
#if GLOBAL_Q == 9
#define _IQasin(A)              _IQ9asin(A)
#endif
#if GLOBAL_Q == 8
#define _IQasin(A)              _IQ8asin(A)
#endif
#if GLOBAL_Q == 7
#define _IQasin(A)              _IQ7asin(A)
#endif
#if GLOBAL_Q == 6
#define _IQasin(A)              _IQ6asin(A)
#endif
#if GLOBAL_Q == 5
#define _IQasin(A)              _IQ5asin(A)
#endif
#if GLOBAL_Q == 4
#define _IQasin(A)              _IQ4asin(A)
#endif
#if GLOBAL_Q == 3
#define _IQasin(A)              _IQ3asin(A)
#endif
#if GLOBAL_Q == 2
#define _IQasin(A)              _IQ2asin(A)
#endif
#if GLOBAL_Q == 1
#define _IQasin(A)              _IQ1asin(A)
#endif


extern _iq29 _IQ29cos(_iq29 A);
extern _iq28 _IQ28cos(_iq28 A);
extern _iq27 _IQ27cos(_iq27 A);
extern _iq26 _IQ26cos(_iq26 A);
extern _iq25 _IQ25cos(_iq25 A);
extern _iq24 _IQ24cos(_iq24 A);
extern _iq23 _IQ23cos(_iq23 A);
extern _iq22 _IQ22cos(_iq22 A);
extern _iq21 _IQ21cos(_iq21 A);
extern _iq20 _IQ20cos(_iq20 A);
extern _iq19 _IQ19cos(_iq19 A);
extern _iq18 _IQ18cos(_iq18 A);
extern _iq17 _IQ17cos(_iq17 A);
extern _iq16 _IQ16cos(_iq16 A);
extern _iq15 _IQ15cos(_iq15 A);
extern _iq14 _IQ14cos(_iq14 A);
extern _iq13 _IQ13cos(_iq13 A);
extern _iq12 _IQ12cos(_iq12 A);
extern _iq11 _IQ11cos(_iq11 A);
extern _iq10 _IQ10cos(_iq10 A);
extern _iq9 _IQ9cos(_iq9 A);
extern _iq8 _IQ8cos(_iq8 A);
extern _iq7 _IQ7cos(_iq7 A);
extern _iq6 _IQ6cos(_iq6 A);
extern _iq5 _IQ5cos(_iq5 A);
extern _iq4 _IQ4cos(_iq4 A);
extern _iq3 _IQ3cos(_iq3 A);
extern _iq2 _IQ2cos(_iq2 A);
extern _iq1 _IQ1cos(_iq1 A);
/*******************************************************************************
* Function Name  : _IQcos
* Description    : IQ格式数的余弦函数:cos(A)
* Input          : 输入参数为IQ格式的弧度值
* Return         : 返回A的余弦值，与参数的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 29
#define _IQcos(A)               _IQ29cos(A)
#endif
#if GLOBAL_Q == 28
#define _IQcos(A)               _IQ28cos(A)
#endif
#if GLOBAL_Q == 27
#define _IQcos(A)               _IQ27cos(A)
#endif
#if GLOBAL_Q == 26
#define _IQcos(A)               _IQ26cos(A)
#endif
#if GLOBAL_Q == 25
#define _IQcos(A)               _IQ25cos(A)
#endif
#if GLOBAL_Q == 24
#define _IQcos(A)               _IQ24cos(A)
#endif
#if GLOBAL_Q == 23
#define _IQcos(A)               _IQ23cos(A)
#endif
#if GLOBAL_Q == 22
#define _IQcos(A)               _IQ22cos(A)
#endif
#if GLOBAL_Q == 21
#define _IQcos(A)               _IQ21cos(A)
#endif
#if GLOBAL_Q == 20
#define _IQcos(A)               _IQ20cos(A)
#endif
#if GLOBAL_Q == 19
#define _IQcos(A)               _IQ19cos(A)
#endif
#if GLOBAL_Q == 18
#define _IQcos(A)               _IQ18cos(A)
#endif
#if GLOBAL_Q == 17
#define _IQcos(A)               _IQ17cos(A)
#endif
#if GLOBAL_Q == 16
#define _IQcos(A)               _IQ16cos(A)
#endif
#if GLOBAL_Q == 15
#define _IQcos(A)               _IQ15cos(A)
#endif
#if GLOBAL_Q == 14
#define _IQcos(A)               _IQ14cos(A)
#endif
#if GLOBAL_Q == 13
#define _IQcos(A)               _IQ13cos(A)
#endif
#if GLOBAL_Q == 12
#define _IQcos(A)               _IQ12cos(A)
#endif
#if GLOBAL_Q == 11
#define _IQcos(A)               _IQ11cos(A)
#endif
#if GLOBAL_Q == 10
#define _IQcos(A)               _IQ10cos(A)
#endif
#if GLOBAL_Q == 9
#define _IQcos(A)               _IQ9cos(A)
#endif
#if GLOBAL_Q == 8
#define _IQcos(A)               _IQ8cos(A)
#endif
#if GLOBAL_Q == 7
#define _IQcos(A)               _IQ7cos(A)
#endif
#if GLOBAL_Q == 6
#define _IQcos(A)               _IQ6cos(A)
#endif
#if GLOBAL_Q == 5
#define _IQcos(A)               _IQ5cos(A)
#endif
#if GLOBAL_Q == 4
#define _IQcos(A)               _IQ4cos(A)
#endif
#if GLOBAL_Q == 3
#define _IQcos(A)               _IQ3cos(A)
#endif
#if GLOBAL_Q == 2
#define _IQcos(A)               _IQ2cos(A)
#endif
#if GLOBAL_Q == 1
#define _IQcos(A)               _IQ1cos(A)
#endif


extern _iq30 _IQ30cosPU(_iq30 A);
extern _iq29 _IQ29cosPU(_iq29 A);
extern _iq28 _IQ28cosPU(_iq28 A);
extern _iq27 _IQ27cosPU(_iq27 A);
extern _iq26 _IQ26cosPU(_iq26 A);
extern _iq25 _IQ25cosPU(_iq25 A);
extern _iq24 _IQ24cosPU(_iq24 A);
extern _iq23 _IQ23cosPU(_iq23 A);
extern _iq22 _IQ22cosPU(_iq22 A);
extern _iq21 _IQ21cosPU(_iq21 A);
extern _iq20 _IQ20cosPU(_iq20 A);
extern _iq19 _IQ19cosPU(_iq19 A);
extern _iq18 _IQ18cosPU(_iq18 A);
extern _iq17 _IQ17cosPU(_iq17 A);
extern _iq16 _IQ16cosPU(_iq16 A);
extern _iq15 _IQ15cosPU(_iq15 A);
extern _iq14 _IQ14cosPU(_iq14 A);
extern _iq13 _IQ13cosPU(_iq13 A);
extern _iq12 _IQ12cosPU(_iq12 A);
extern _iq11 _IQ11cosPU(_iq11 A);
extern _iq10 _IQ10cosPU(_iq10 A);
extern _iq9 _IQ9cosPU(_iq9 A);
extern _iq8 _IQ8cosPU(_iq8 A);
extern _iq7 _IQ7cosPU(_iq7 A);
extern _iq6 _IQ6cosPU(_iq6 A);
extern _iq5 _IQ5cosPU(_iq5 A);
extern _iq4 _IQ4cosPU(_iq4 A);
extern _iq3 _IQ3cosPU(_iq3 A);
extern _iq2 _IQ2cosPU(_iq2 A);
extern _iq1 _IQ1cosPU(_iq1 A);
/*******************************************************************************
* Function Name  : _IQcosPU
* Description    : IQ格式数的余弦函数,acos((2*PI)*A)
* Input          : 输入参数是几分之一个圆
* Return         : 返回A的余弦值，与参数的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 30
#define _IQcosPU(A)             _IQ30cosPU(A)
#endif
#if GLOBAL_Q == 29
#define _IQcosPU(A)             _IQ29cosPU(A)
#endif
#if GLOBAL_Q == 28
#define _IQcosPU(A)             _IQ28cosPU(A)
#endif
#if GLOBAL_Q == 27
#define _IQcosPU(A)             _IQ27cosPU(A)
#endif
#if GLOBAL_Q == 26
#define _IQcosPU(A)             _IQ26cosPU(A)
#endif
#if GLOBAL_Q == 25
#define _IQcosPU(A)             _IQ25cosPU(A)
#endif
#if GLOBAL_Q == 24
#define _IQcosPU(A)             _IQ24cosPU(A)
#endif
#if GLOBAL_Q == 23
#define _IQcosPU(A)             _IQ23cosPU(A)
#endif
#if GLOBAL_Q == 22
#define _IQcosPU(A)             _IQ22cosPU(A)
#endif
#if GLOBAL_Q == 21
#define _IQcosPU(A)             _IQ21cosPU(A)
#endif
#if GLOBAL_Q == 20
#define _IQcosPU(A)             _IQ20cosPU(A)
#endif
#if GLOBAL_Q == 19
#define _IQcosPU(A)             _IQ19cosPU(A)
#endif
#if GLOBAL_Q == 18
#define _IQcosPU(A)             _IQ18cosPU(A)
#endif
#if GLOBAL_Q == 17
#define _IQcosPU(A)             _IQ17cosPU(A)
#endif
#if GLOBAL_Q == 16
#define _IQcosPU(A)             _IQ16cosPU(A)
#endif
#if GLOBAL_Q == 15
#define _IQcosPU(A)             _IQ15cosPU(A)
#endif
#if GLOBAL_Q == 14
#define _IQcosPU(A)             _IQ14cosPU(A)
#endif
#if GLOBAL_Q == 13
#define _IQcosPU(A)             _IQ13cosPU(A)
#endif
#if GLOBAL_Q == 12
#define _IQcosPU(A)             _IQ12cosPU(A)
#endif
#if GLOBAL_Q == 11
#define _IQcosPU(A)             _IQ11cosPU(A)
#endif
#if GLOBAL_Q == 10
#define _IQcosPU(A)             _IQ10cosPU(A)
#endif
#if GLOBAL_Q == 9
#define _IQcosPU(A)             _IQ9cosPU(A)
#endif
#if GLOBAL_Q == 8
#define _IQcosPU(A)             _IQ8cosPU(A)
#endif
#if GLOBAL_Q == 7
#define _IQcosPU(A)             _IQ7cosPU(A)
#endif
#if GLOBAL_Q == 6
#define _IQcosPU(A)             _IQ6cosPU(A)
#endif
#if GLOBAL_Q == 5
#define _IQcosPU(A)             _IQ5cosPU(A)
#endif
#if GLOBAL_Q == 4
#define _IQcosPU(A)             _IQ4cosPU(A)
#endif
#if GLOBAL_Q == 3
#define _IQcosPU(A)             _IQ3cosPU(A)
#endif
#if GLOBAL_Q == 2
#define _IQcosPU(A)             _IQ2cosPU(A)
#endif
#if GLOBAL_Q == 1
#define _IQcosPU(A)             _IQ1cosPU(A)
#endif


#define _IQ29acos(A)            (_IQ29(1.570796327) - _IQ29asin(A))
#define _IQ28acos(A)            (_IQ28(1.570796327) - _IQ28asin(A))
#define _IQ27acos(A)            (_IQ27(1.570796327) - _IQ27asin(A))
#define _IQ26acos(A)            (_IQ26(1.570796327) - _IQ26asin(A))
#define _IQ25acos(A)            (_IQ25(1.570796327) - _IQ25asin(A))
#define _IQ24acos(A)            (_IQ24(1.570796327) - _IQ24asin(A))
#define _IQ23acos(A)            (_IQ23(1.570796327) - _IQ23asin(A))
#define _IQ22acos(A)            (_IQ22(1.570796327) - _IQ22asin(A))
#define _IQ21acos(A)            (_IQ21(1.570796327) - _IQ21asin(A))
#define _IQ20acos(A)            (_IQ20(1.570796327) - _IQ20asin(A))
#define _IQ19acos(A)            (_IQ19(1.570796327) - _IQ19asin(A))
#define _IQ18acos(A)            (_IQ18(1.570796327) - _IQ18asin(A))
#define _IQ17acos(A)            (_IQ17(1.570796327) - _IQ17asin(A))
#define _IQ16acos(A)            (_IQ16(1.570796327) - _IQ16asin(A))
#define _IQ15acos(A)            (_IQ15(1.570796327) - _IQ15asin(A))
#define _IQ14acos(A)            (_IQ14(1.570796327) - _IQ14asin(A))
#define _IQ13acos(A)            (_IQ13(1.570796327) - _IQ13asin(A))
#define _IQ12acos(A)            (_IQ12(1.570796327) - _IQ12asin(A))
#define _IQ11acos(A)            (_IQ11(1.570796327) - _IQ11asin(A))
#define _IQ10acos(A)            (_IQ10(1.570796327) - _IQ10asin(A))
#define _IQ9acos(A)             (_IQ9(1.570796327) - _IQ9asin(A))
#define _IQ8acos(A)             (_IQ8(1.570796327) - _IQ8asin(A))
#define _IQ7acos(A)             (_IQ7(1.570796327) - _IQ7asin(A))
#define _IQ6acos(A)             (_IQ6(1.570796327) - _IQ6asin(A))
#define _IQ5acos(A)             (_IQ5(1.570796327) - _IQ5asin(A))
#define _IQ4acos(A)             (_IQ4(1.570796327) - _IQ4asin(A))
#define _IQ3acos(A)             (_IQ3(1.570796327) - _IQ3asin(A))
#define _IQ2acos(A)             (_IQ2(1.570796327) - _IQ2asin(A))
#define _IQ1acos(A)             (_IQ1(1.570796327) - _IQ1asin(A))
/*******************************************************************************
* Function Name  : _IQacos
* Description    : IQ格式数的反余弦函数:acos(A)
* Input          : 输入参数为IQ格式的弧度值
* Return         : 返回A的反余弦值，与参数的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 29
#define _IQacos(A)              _IQ29acos(A)
#endif
#if GLOBAL_Q == 28
#define _IQacos(A)              _IQ28acos(A)
#endif
#if GLOBAL_Q == 27
#define _IQacos(A)              _IQ27acos(A)
#endif
#if GLOBAL_Q == 26
#define _IQacos(A)              _IQ26acos(A)
#endif
#if GLOBAL_Q == 25
#define _IQacos(A)              _IQ25acos(A)
#endif
#if GLOBAL_Q == 24
#define _IQacos(A)              _IQ24acos(A)
#endif
#if GLOBAL_Q == 23
#define _IQacos(A)              _IQ23acos(A)
#endif
#if GLOBAL_Q == 22
#define _IQacos(A)              _IQ22acos(A)
#endif
#if GLOBAL_Q == 21
#define _IQacos(A)              _IQ21acos(A)
#endif
#if GLOBAL_Q == 20
#define _IQacos(A)              _IQ20acos(A)
#endif
#if GLOBAL_Q == 19
#define _IQacos(A)              _IQ19acos(A)
#endif
#if GLOBAL_Q == 18
#define _IQacos(A)              _IQ18acos(A)
#endif
#if GLOBAL_Q == 17
#define _IQacos(A)              _IQ17acos(A)
#endif
#if GLOBAL_Q == 16
#define _IQacos(A)              _IQ16acos(A)
#endif
#if GLOBAL_Q == 15
#define _IQacos(A)              _IQ15acos(A)
#endif
#if GLOBAL_Q == 14
#define _IQacos(A)              _IQ14acos(A)
#endif
#if GLOBAL_Q == 13
#define _IQacos(A)              _IQ13acos(A)
#endif
#if GLOBAL_Q == 12
#define _IQacos(A)              _IQ12acos(A)
#endif
#if GLOBAL_Q == 11
#define _IQacos(A)              _IQ11acos(A)
#endif
#if GLOBAL_Q == 10
#define _IQacos(A)              _IQ10acos(A)
#endif
#if GLOBAL_Q == 9
#define _IQacos(A)              _IQ9acos(A)
#endif
#if GLOBAL_Q == 8
#define _IQacos(A)              _IQ8acos(A)
#endif
#if GLOBAL_Q == 7
#define _IQacos(A)              _IQ7acos(A)
#endif
#if GLOBAL_Q == 6
#define _IQacos(A)              _IQ6acos(A)
#endif
#if GLOBAL_Q == 5
#define _IQacos(A)              _IQ5acos(A)
#endif
#if GLOBAL_Q == 4
#define _IQacos(A)              _IQ4acos(A)
#endif
#if GLOBAL_Q == 3
#define _IQacos(A)              _IQ3acos(A)
#endif
#if GLOBAL_Q == 2
#define _IQacos(A)              _IQ2acos(A)
#endif
#if GLOBAL_Q == 1
#define _IQacos(A)              _IQ1acos(A)
#endif


extern _iq29 _IQ29atan2(_iq29 A, _iq29 B);
extern _iq28 _IQ28atan2(_iq28 A, _iq28 B);
extern _iq27 _IQ27atan2(_iq27 A, _iq27 B);
extern _iq26 _IQ26atan2(_iq26 A, _iq26 B);
extern _iq25 _IQ25atan2(_iq25 A, _iq25 B);
extern _iq24 _IQ24atan2(_iq24 A, _iq24 B);
extern _iq23 _IQ23atan2(_iq23 A, _iq23 B);
extern _iq22 _IQ22atan2(_iq22 A, _iq22 B);
extern _iq21 _IQ21atan2(_iq21 A, _iq21 B);
extern _iq20 _IQ20atan2(_iq20 A, _iq20 B);
extern _iq19 _IQ19atan2(_iq19 A, _iq19 B);
extern _iq18 _IQ18atan2(_iq18 A, _iq18 B);
extern _iq17 _IQ17atan2(_iq17 A, _iq17 B);
extern _iq16 _IQ16atan2(_iq16 A, _iq16 B);
extern _iq15 _IQ15atan2(_iq15 A, _iq15 B);
extern _iq14 _IQ14atan2(_iq14 A, _iq14 B);
extern _iq13 _IQ13atan2(_iq13 A, _iq13 B);
extern _iq12 _IQ12atan2(_iq12 A, _iq12 B);
extern _iq11 _IQ11atan2(_iq11 A, _iq11 B);
extern _iq10 _IQ10atan2(_iq10 A, _iq10 B);
extern _iq9 _IQ9atan2(_iq9 A, _iq9 B);
extern _iq8 _IQ8atan2(_iq8 A, _iq8 B);
extern _iq7 _IQ7atan2(_iq7 A, _iq7 B);
extern _iq6 _IQ6atan2(_iq6 A, _iq6 B);
extern _iq5 _IQ5atan2(_iq5 A, _iq5 B);
extern _iq4 _IQ4atan2(_iq4 A, _iq4 B);
extern _iq3 _IQ3atan2(_iq3 A, _iq3 B);
extern _iq2 _IQ2atan2(_iq2 A, _iq2 B);
extern _iq1 _IQ1atan2(_iq1 A, _iq1 B);
/*******************************************************************************
* Function Name  : _IQatan2
* Description    : IQ格式数的反正切函数，可计算4个象限内的点:atan2(A,B)
* Input          : A为IQ格式的Y轴坐标值，B为IQ格式的X轴坐标值
* Return         : 返回点(B,A)与X轴的夹角的弧度值，与参数的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 29
#define _IQatan2(A, B)          _IQ29atan2(A, B)
#endif
#if GLOBAL_Q == 28
#define _IQatan2(A, B)          _IQ28atan2(A, B)
#endif
#if GLOBAL_Q == 27
#define _IQatan2(A, B)          _IQ27atan2(A, B)
#endif
#if GLOBAL_Q == 26
#define _IQatan2(A, B)          _IQ26atan2(A, B)
#endif
#if GLOBAL_Q == 25
#define _IQatan2(A, B)          _IQ25atan2(A, B)
#endif
#if GLOBAL_Q == 24
#define _IQatan2(A, B)          _IQ24atan2(A, B)
#endif
#if GLOBAL_Q == 23
#define _IQatan2(A, B)          _IQ23atan2(A, B)
#endif
#if GLOBAL_Q == 22
#define _IQatan2(A, B)          _IQ22atan2(A, B)
#endif
#if GLOBAL_Q == 21
#define _IQatan2(A, B)          _IQ21atan2(A, B)
#endif
#if GLOBAL_Q == 20
#define _IQatan2(A, B)          _IQ20atan2(A, B)
#endif
#if GLOBAL_Q == 19
#define _IQatan2(A, B)          _IQ19atan2(A, B)
#endif
#if GLOBAL_Q == 18
#define _IQatan2(A, B)          _IQ18atan2(A, B)
#endif
#if GLOBAL_Q == 17
#define _IQatan2(A, B)          _IQ17atan2(A, B)
#endif
#if GLOBAL_Q == 16
#define _IQatan2(A, B)          _IQ16atan2(A, B)
#endif
#if GLOBAL_Q == 15
#define _IQatan2(A, B)          _IQ15atan2(A, B)
#endif
#if GLOBAL_Q == 14
#define _IQatan2(A, B)          _IQ14atan2(A, B)
#endif
#if GLOBAL_Q == 13
#define _IQatan2(A, B)          _IQ13atan2(A, B)
#endif
#if GLOBAL_Q == 12
#define _IQatan2(A, B)          _IQ12atan2(A, B)
#endif
#if GLOBAL_Q == 11
#define _IQatan2(A, B)          _IQ11atan2(A, B)
#endif
#if GLOBAL_Q == 10
#define _IQatan2(A, B)          _IQ10atan2(A, B)
#endif
#if GLOBAL_Q == 9
#define _IQatan2(A, B)          _IQ9atan2(A, B)
#endif
#if GLOBAL_Q == 8
#define _IQatan2(A, B)          _IQ8atan2(A, B)
#endif
#if GLOBAL_Q == 7
#define _IQatan2(A, B)          _IQ7atan2(A, B)
#endif
#if GLOBAL_Q == 6
#define _IQatan2(A, B)          _IQ6atan2(A, B)
#endif
#if GLOBAL_Q == 5
#define _IQatan2(A, B)          _IQ5atan2(A, B)
#endif
#if GLOBAL_Q == 4
#define _IQatan2(A, B)          _IQ4atan2(A, B)
#endif
#if GLOBAL_Q == 3
#define _IQatan2(A, B)          _IQ3atan2(A, B)
#endif
#if GLOBAL_Q == 2
#define _IQatan2(A, B)          _IQ2atan2(A, B)
#endif
#if GLOBAL_Q == 1
#define _IQatan2(A, B)          _IQ1atan2(A, B)
#endif


extern _iq30 _IQ30atan2PU(_iq30 A, _iq30 B);
extern _iq29 _IQ29atan2PU(_iq29 A, _iq29 B);
extern _iq28 _IQ28atan2PU(_iq28 A, _iq28 B);
extern _iq27 _IQ27atan2PU(_iq27 A, _iq27 B);
extern _iq26 _IQ26atan2PU(_iq26 A, _iq26 B);
extern _iq25 _IQ25atan2PU(_iq25 A, _iq25 B);
extern _iq24 _IQ24atan2PU(_iq24 A, _iq24 B);
extern _iq23 _IQ23atan2PU(_iq23 A, _iq23 B);
extern _iq22 _IQ22atan2PU(_iq22 A, _iq22 B);
extern _iq21 _IQ21atan2PU(_iq21 A, _iq21 B);
extern _iq20 _IQ20atan2PU(_iq20 A, _iq20 B);
extern _iq19 _IQ19atan2PU(_iq19 A, _iq19 B);
extern _iq18 _IQ18atan2PU(_iq18 A, _iq18 B);
extern _iq17 _IQ17atan2PU(_iq17 A, _iq17 B);
extern _iq16 _IQ16atan2PU(_iq16 A, _iq16 B);
extern _iq15 _IQ15atan2PU(_iq15 A, _iq15 B);
extern _iq14 _IQ14atan2PU(_iq14 A, _iq14 B);
extern _iq13 _IQ13atan2PU(_iq13 A, _iq13 B);
extern _iq12 _IQ12atan2PU(_iq12 A, _iq12 B);
extern _iq11 _IQ11atan2PU(_iq11 A, _iq11 B);
extern _iq10 _IQ10atan2PU(_iq10 A, _iq10 B);
extern _iq9 _IQ9atan2PU(_iq9 A, _iq9 B);
extern _iq8 _IQ8atan2PU(_iq8 A, _iq8 B);
extern _iq7 _IQ7atan2PU(_iq7 A, _iq7 B);
extern _iq6 _IQ6atan2PU(_iq6 A, _iq6 B);
extern _iq5 _IQ5atan2PU(_iq5 A, _iq5 B);
extern _iq4 _IQ4atan2PU(_iq4 A, _iq4 B);
extern _iq3 _IQ3atan2PU(_iq3 A, _iq3 B);
extern _iq2 _IQ2atan2PU(_iq2 A, _iq2 B);
extern _iq1 _IQ1atan2PU(_iq1 A, _iq1 B);
/*******************************************************************************
* Function Name  : _IQatan2PU
* Description    : IQ格式数的反正切函数，可计算4个象限内的点
* Input          : A为IQ格式的Y轴坐标值，B为IQ格式的X轴坐标值
* Return         : 返回值是0到1之间的一个小数，代表几分之一个圆，与参数的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 30
#define _IQatan2PU(A, B)        _IQ30atan2PU(A, B)
#endif
#if GLOBAL_Q == 29
#define _IQatan2PU(A, B)        _IQ29atan2PU(A, B)
#endif
#if GLOBAL_Q == 28
#define _IQatan2PU(A, B)        _IQ28atan2PU(A, B)
#endif
#if GLOBAL_Q == 27
#define _IQatan2PU(A, B)        _IQ27atan2PU(A, B)
#endif
#if GLOBAL_Q == 26
#define _IQatan2PU(A, B)        _IQ26atan2PU(A, B)
#endif
#if GLOBAL_Q == 25
#define _IQatan2PU(A, B)        _IQ25atan2PU(A, B)
#endif
#if GLOBAL_Q == 24
#define _IQatan2PU(A, B)        _IQ24atan2PU(A, B)
#endif
#if GLOBAL_Q == 23
#define _IQatan2PU(A, B)        _IQ23atan2PU(A, B)
#endif
#if GLOBAL_Q == 22
#define _IQatan2PU(A, B)        _IQ22atan2PU(A, B)
#endif
#if GLOBAL_Q == 21
#define _IQatan2PU(A, B)        _IQ21atan2PU(A, B)
#endif
#if GLOBAL_Q == 20
#define _IQatan2PU(A, B)        _IQ20atan2PU(A, B)
#endif
#if GLOBAL_Q == 19
#define _IQatan2PU(A, B)        _IQ19atan2PU(A, B)
#endif
#if GLOBAL_Q == 18
#define _IQatan2PU(A, B)        _IQ18atan2PU(A, B)
#endif
#if GLOBAL_Q == 17
#define _IQatan2PU(A, B)        _IQ17atan2PU(A, B)
#endif
#if GLOBAL_Q == 16
#define _IQatan2PU(A, B)        _IQ16atan2PU(A, B)
#endif
#if GLOBAL_Q == 15
#define _IQatan2PU(A, B)        _IQ15atan2PU(A, B)
#endif
#if GLOBAL_Q == 14
#define _IQatan2PU(A, B)        _IQ14atan2PU(A, B)
#endif
#if GLOBAL_Q == 13
#define _IQatan2PU(A, B)        _IQ13atan2PU(A, B)
#endif
#if GLOBAL_Q == 12
#define _IQatan2PU(A, B)        _IQ12atan2PU(A, B)
#endif
#if GLOBAL_Q == 11
#define _IQatan2PU(A, B)        _IQ11atan2PU(A, B)
#endif
#if GLOBAL_Q == 10
#define _IQatan2PU(A, B)        _IQ10atan2PU(A, B)
#endif
#if GLOBAL_Q == 9
#define _IQatan2PU(A, B)        _IQ9atan2PU(A, B)
#endif
#if GLOBAL_Q == 8
#define _IQatan2PU(A, B)        _IQ8atan2PU(A, B)
#endif
#if GLOBAL_Q == 7
#define _IQatan2PU(A, B)        _IQ7atan2PU(A, B)
#endif
#if GLOBAL_Q == 6
#define _IQatan2PU(A, B)        _IQ6atan2PU(A, B)
#endif
#if GLOBAL_Q == 5
#define _IQatan2PU(A, B)        _IQ5atan2PU(A, B)
#endif
#if GLOBAL_Q == 4
#define _IQatan2PU(A, B)        _IQ4atan2PU(A, B)
#endif
#if GLOBAL_Q == 3
#define _IQatan2PU(A, B)        _IQ3atan2PU(A, B)
#endif
#if GLOBAL_Q == 2
#define _IQatan2PU(A, B)        _IQ2atan2PU(A, B)
#endif
#if GLOBAL_Q == 1
#define _IQatan2PU(A, B)        _IQ1atan2PU(A, B)
#endif


#define _IQ29atan(A)            _IQ29atan2(A, _IQ29(1.0))
#define _IQ28atan(A)            _IQ28atan2(A, _IQ28(1.0))
#define _IQ27atan(A)            _IQ27atan2(A, _IQ27(1.0))
#define _IQ26atan(A)            _IQ26atan2(A, _IQ26(1.0))
#define _IQ25atan(A)            _IQ25atan2(A, _IQ25(1.0))
#define _IQ24atan(A)            _IQ24atan2(A, _IQ24(1.0))
#define _IQ23atan(A)            _IQ23atan2(A, _IQ23(1.0))
#define _IQ22atan(A)            _IQ22atan2(A, _IQ22(1.0))
#define _IQ21atan(A)            _IQ21atan2(A, _IQ21(1.0))
#define _IQ20atan(A)            _IQ20atan2(A, _IQ20(1.0))
#define _IQ19atan(A)            _IQ19atan2(A, _IQ19(1.0))
#define _IQ18atan(A)            _IQ18atan2(A, _IQ18(1.0))
#define _IQ17atan(A)            _IQ17atan2(A, _IQ17(1.0))
#define _IQ16atan(A)            _IQ16atan2(A, _IQ16(1.0))
#define _IQ15atan(A)            _IQ15atan2(A, _IQ15(1.0))
#define _IQ14atan(A)            _IQ14atan2(A, _IQ14(1.0))
#define _IQ13atan(A)            _IQ13atan2(A, _IQ13(1.0))
#define _IQ12atan(A)            _IQ12atan2(A, _IQ12(1.0))
#define _IQ11atan(A)            _IQ11atan2(A, _IQ11(1.0))
#define _IQ10atan(A)            _IQ10atan2(A, _IQ10(1.0))
#define _IQ9atan(A)             _IQ9atan2(A, _IQ9(1.0))
#define _IQ8atan(A)             _IQ8atan2(A, _IQ8(1.0))
#define _IQ7atan(A)             _IQ7atan2(A, _IQ7(1.0))
#define _IQ6atan(A)             _IQ6atan2(A, _IQ6(1.0))
#define _IQ5atan(A)             _IQ5atan2(A, _IQ5(1.0))
#define _IQ4atan(A)             _IQ4atan2(A, _IQ4(1.0))
#define _IQ3atan(A)             _IQ3atan2(A, _IQ3(1.0))
#define _IQ2atan(A)             _IQ2atan2(A, _IQ2(1.0))
#define _IQ1atan(A)             _IQ1atan2(A, _IQ1(1.0))
/*******************************************************************************
* Function Name  : _IQatan
* Description    : IQ格式数的反正切函数，计算第1、4象限内的点:atan(A)
* Input          : 参数A为IQ格式的数
* Return         : 返回值介于-pi/2和pi/2之间，与参数的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 29
#define _IQatan(A)              _IQ29atan2(A, _IQ29(1.0))
#endif
#if GLOBAL_Q == 28
#define _IQatan(A)              _IQ28atan2(A, _IQ28(1.0))
#endif
#if GLOBAL_Q == 27
#define _IQatan(A)              _IQ27atan2(A, _IQ27(1.0))
#endif
#if GLOBAL_Q == 26
#define _IQatan(A)              _IQ26atan2(A, _IQ26(1.0))
#endif
#if GLOBAL_Q == 25
#define _IQatan(A)              _IQ25atan2(A, _IQ25(1.0))
#endif
#if GLOBAL_Q == 24
#define _IQatan(A)              _IQ24atan2(A, _IQ24(1.0))
#endif
#if GLOBAL_Q == 23
#define _IQatan(A)              _IQ23atan2(A, _IQ23(1.0))
#endif
#if GLOBAL_Q == 22
#define _IQatan(A)              _IQ22atan2(A, _IQ22(1.0))
#endif
#if GLOBAL_Q == 21
#define _IQatan(A)              _IQ21atan2(A, _IQ21(1.0))
#endif
#if GLOBAL_Q == 20
#define _IQatan(A)              _IQ20atan2(A, _IQ20(1.0))
#endif
#if GLOBAL_Q == 19
#define _IQatan(A)              _IQ19atan2(A, _IQ19(1.0))
#endif
#if GLOBAL_Q == 18
#define _IQatan(A)              _IQ18atan2(A, _IQ18(1.0))
#endif
#if GLOBAL_Q == 17
#define _IQatan(A)              _IQ17atan2(A, _IQ17(1.0))
#endif
#if GLOBAL_Q == 16
#define _IQatan(A)              _IQ16atan2(A, _IQ16(1.0))
#endif
#if GLOBAL_Q == 15
#define _IQatan(A)              _IQ15atan2(A, _IQ15(1.0))
#endif
#if GLOBAL_Q == 14
#define _IQatan(A)              _IQ14atan2(A, _IQ14(1.0))
#endif
#if GLOBAL_Q == 13
#define _IQatan(A)              _IQ13atan2(A, _IQ13(1.0))
#endif
#if GLOBAL_Q == 12
#define _IQatan(A)              _IQ12atan2(A, _IQ12(1.0))
#endif
#if GLOBAL_Q == 11
#define _IQatan(A)              _IQ11atan2(A, _IQ11(1.0))
#endif
#if GLOBAL_Q == 10
#define _IQatan(A)              _IQ10atan2(A, _IQ10(1.0))
#endif
#if GLOBAL_Q == 9
#define _IQatan(A)              _IQ9atan2(A, _IQ9(1.0))
#endif
#if GLOBAL_Q == 8
#define _IQatan(A)              _IQ8atan2(A, _IQ8(1.0))
#endif
#if GLOBAL_Q == 7
#define _IQatan(A)              _IQ7atan2(A, _IQ7(1.0))
#endif
#if GLOBAL_Q == 6
#define _IQatan(A)              _IQ6atan2(A, _IQ6(1.0))
#endif
#if GLOBAL_Q == 5
#define _IQatan(A)              _IQ5atan2(A, _IQ5(1.0))
#endif
#if GLOBAL_Q == 4
#define _IQatan(A)              _IQ4atan2(A, _IQ4(1.0))
#endif
#if GLOBAL_Q == 3
#define _IQatan(A)              _IQ3atan2(A, _IQ3(1.0))
#endif
#if GLOBAL_Q == 2
#define _IQatan(A)              _IQ2atan2(A, _IQ2(1.0))
#endif
#if GLOBAL_Q == 1
#define _IQatan(A)              _IQ1atan2(A, _IQ1(1.0))
#endif


extern _iq30 _IQ30sqrt(_iq30 A);
extern _iq29 _IQ29sqrt(_iq29 A);
extern _iq28 _IQ28sqrt(_iq28 A);
extern _iq27 _IQ27sqrt(_iq27 A);
extern _iq26 _IQ26sqrt(_iq26 A);
extern _iq25 _IQ25sqrt(_iq25 A);
extern _iq24 _IQ24sqrt(_iq24 A);
extern _iq23 _IQ23sqrt(_iq23 A);
extern _iq22 _IQ22sqrt(_iq22 A);
extern _iq21 _IQ21sqrt(_iq21 A);
extern _iq20 _IQ20sqrt(_iq20 A);
extern _iq19 _IQ19sqrt(_iq19 A);
extern _iq18 _IQ18sqrt(_iq18 A);
extern _iq17 _IQ17sqrt(_iq17 A);
extern _iq16 _IQ16sqrt(_iq16 A);
extern _iq15 _IQ15sqrt(_iq15 A);
extern _iq14 _IQ14sqrt(_iq14 A);
extern _iq13 _IQ13sqrt(_iq13 A);
extern _iq12 _IQ12sqrt(_iq12 A);
extern _iq11 _IQ11sqrt(_iq11 A);
extern _iq10 _IQ10sqrt(_iq10 A);
extern _iq9 _IQ9sqrt(_iq9 A);
extern _iq8 _IQ8sqrt(_iq8 A);
extern _iq7 _IQ7sqrt(_iq7 A);
extern _iq6 _IQ6sqrt(_iq6 A);
extern _iq5 _IQ5sqrt(_iq5 A);
extern _iq4 _IQ4sqrt(_iq4 A);
extern _iq3 _IQ3sqrt(_iq3 A);
extern _iq2 _IQ2sqrt(_iq2 A);
extern _iq1 _IQ1sqrt(_iq1 A);
/*******************************************************************************
* Function Name  : _IQsqrt
* Description    : IQ格式数的平方根函数:A^(1/2)
* Input          : 参数A为IQ格式的数
* Return         : 返回A的平方根值，与参数的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 30
#define _IQsqrt(A)              _IQ30sqrt(A)
#endif
#if GLOBAL_Q == 29
#define _IQsqrt(A)              _IQ29sqrt(A)
#endif
#if GLOBAL_Q == 28
#define _IQsqrt(A)              _IQ28sqrt(A)
#endif
#if GLOBAL_Q == 27
#define _IQsqrt(A)              _IQ27sqrt(A)
#endif
#if GLOBAL_Q == 26
#define _IQsqrt(A)              _IQ26sqrt(A)
#endif
#if GLOBAL_Q == 25
#define _IQsqrt(A)              _IQ25sqrt(A)
#endif
#if GLOBAL_Q == 24
#define _IQsqrt(A)              _IQ24sqrt(A)
#endif
#if GLOBAL_Q == 23
#define _IQsqrt(A)              _IQ23sqrt(A)
#endif
#if GLOBAL_Q == 22
#define _IQsqrt(A)              _IQ22sqrt(A)
#endif
#if GLOBAL_Q == 21
#define _IQsqrt(A)              _IQ21sqrt(A)
#endif
#if GLOBAL_Q == 20
#define _IQsqrt(A)              _IQ20sqrt(A)
#endif
#if GLOBAL_Q == 19
#define _IQsqrt(A)              _IQ19sqrt(A)
#endif
#if GLOBAL_Q == 18
#define _IQsqrt(A)              _IQ18sqrt(A)
#endif
#if GLOBAL_Q == 17
#define _IQsqrt(A)              _IQ17sqrt(A)
#endif
#if GLOBAL_Q == 16
#define _IQsqrt(A)              _IQ16sqrt(A)
#endif
#if GLOBAL_Q == 15
#define _IQsqrt(A)              _IQ15sqrt(A)
#endif
#if GLOBAL_Q == 14
#define _IQsqrt(A)              _IQ14sqrt(A)
#endif
#if GLOBAL_Q == 13
#define _IQsqrt(A)              _IQ13sqrt(A)
#endif
#if GLOBAL_Q == 12
#define _IQsqrt(A)              _IQ12sqrt(A)
#endif
#if GLOBAL_Q == 11
#define _IQsqrt(A)              _IQ11sqrt(A)
#endif
#if GLOBAL_Q == 10
#define _IQsqrt(A)              _IQ10sqrt(A)
#endif
#if GLOBAL_Q == 9
#define _IQsqrt(A)              _IQ9sqrt(A)
#endif
#if GLOBAL_Q == 8
#define _IQsqrt(A)              _IQ8sqrt(A)
#endif
#if GLOBAL_Q == 7
#define _IQsqrt(A)              _IQ7sqrt(A)
#endif
#if GLOBAL_Q == 6
#define _IQsqrt(A)              _IQ6sqrt(A)
#endif
#if GLOBAL_Q == 5
#define _IQsqrt(A)              _IQ5sqrt(A)
#endif
#if GLOBAL_Q == 4
#define _IQsqrt(A)              _IQ4sqrt(A)
#endif
#if GLOBAL_Q == 3
#define _IQsqrt(A)              _IQ3sqrt(A)
#endif
#if GLOBAL_Q == 2
#define _IQsqrt(A)              _IQ2sqrt(A)
#endif
#if GLOBAL_Q == 1
#define _IQsqrt(A)              _IQ1sqrt(A)
#endif


extern _iq30 _IQ30isqrt(_iq30 A);
extern _iq29 _IQ29isqrt(_iq29 A);
extern _iq28 _IQ28isqrt(_iq28 A);
extern _iq27 _IQ27isqrt(_iq27 A);
extern _iq26 _IQ26isqrt(_iq26 A);
extern _iq25 _IQ25isqrt(_iq25 A);
extern _iq24 _IQ24isqrt(_iq24 A);
extern _iq23 _IQ23isqrt(_iq23 A);
extern _iq22 _IQ22isqrt(_iq22 A);
extern _iq21 _IQ21isqrt(_iq21 A);
extern _iq20 _IQ20isqrt(_iq20 A);
extern _iq19 _IQ19isqrt(_iq19 A);
extern _iq18 _IQ18isqrt(_iq18 A);
extern _iq17 _IQ17isqrt(_iq17 A);
extern _iq16 _IQ16isqrt(_iq16 A);
extern _iq15 _IQ15isqrt(_iq15 A);
extern _iq14 _IQ14isqrt(_iq14 A);
extern _iq13 _IQ13isqrt(_iq13 A);
extern _iq12 _IQ12isqrt(_iq12 A);
extern _iq11 _IQ11isqrt(_iq11 A);
extern _iq10 _IQ10isqrt(_iq10 A);
extern _iq9 _IQ9isqrt(_iq9 A);
extern _iq8 _IQ8isqrt(_iq8 A);
extern _iq7 _IQ7isqrt(_iq7 A);
extern _iq6 _IQ6isqrt(_iq6 A);
extern _iq5 _IQ5isqrt(_iq5 A);
extern _iq4 _IQ4isqrt(_iq4 A);
extern _iq3 _IQ3isqrt(_iq3 A);
extern _iq2 _IQ2isqrt(_iq2 A);
extern _iq1 _IQ1isqrt(_iq1 A);
/*******************************************************************************
* Function Name  : _IQisqrt
* Description    : IQ格式数的平方根倒数函数:A^(-1/2)
* Input          : 参数A为IQ格式的数
* Return         : 返回A的平方根倒数值，与参数的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 30
#define _IQisqrt(A)             _IQ30isqrt(A)
#endif
#if GLOBAL_Q == 29
#define _IQisqrt(A)             _IQ29isqrt(A)
#endif
#if GLOBAL_Q == 28
#define _IQisqrt(A)             _IQ28isqrt(A)
#endif
#if GLOBAL_Q == 27
#define _IQisqrt(A)             _IQ27isqrt(A)
#endif
#if GLOBAL_Q == 26
#define _IQisqrt(A)             _IQ26isqrt(A)
#endif
#if GLOBAL_Q == 25
#define _IQisqrt(A)             _IQ25isqrt(A)
#endif
#if GLOBAL_Q == 24
#define _IQisqrt(A)             _IQ24isqrt(A)
#endif
#if GLOBAL_Q == 23
#define _IQisqrt(A)             _IQ23isqrt(A)
#endif
#if GLOBAL_Q == 22
#define _IQisqrt(A)             _IQ22isqrt(A)
#endif
#if GLOBAL_Q == 21
#define _IQisqrt(A)             _IQ21isqrt(A)
#endif
#if GLOBAL_Q == 20
#define _IQisqrt(A)             _IQ20isqrt(A)
#endif
#if GLOBAL_Q == 19
#define _IQisqrt(A)             _IQ19isqrt(A)
#endif
#if GLOBAL_Q == 18
#define _IQisqrt(A)             _IQ18isqrt(A)
#endif
#if GLOBAL_Q == 17
#define _IQisqrt(A)             _IQ17isqrt(A)
#endif
#if GLOBAL_Q == 16
#define _IQisqrt(A)             _IQ16isqrt(A)
#endif
#if GLOBAL_Q == 15
#define _IQisqrt(A)             _IQ15isqrt(A)
#endif
#if GLOBAL_Q == 14
#define _IQisqrt(A)             _IQ14isqrt(A)
#endif
#if GLOBAL_Q == 13
#define _IQisqrt(A)             _IQ13isqrt(A)
#endif
#if GLOBAL_Q == 12
#define _IQisqrt(A)             _IQ12isqrt(A)
#endif
#if GLOBAL_Q == 11
#define _IQisqrt(A)             _IQ11isqrt(A)
#endif
#if GLOBAL_Q == 10
#define _IQisqrt(A)             _IQ10isqrt(A)
#endif
#if GLOBAL_Q == 9
#define _IQisqrt(A)             _IQ9isqrt(A)
#endif
#if GLOBAL_Q == 8
#define _IQisqrt(A)             _IQ8isqrt(A)
#endif
#if GLOBAL_Q == 7
#define _IQisqrt(A)             _IQ7isqrt(A)
#endif
#if GLOBAL_Q == 6
#define _IQisqrt(A)             _IQ6isqrt(A)
#endif
#if GLOBAL_Q == 5
#define _IQisqrt(A)             _IQ5isqrt(A)
#endif
#if GLOBAL_Q == 4
#define _IQisqrt(A)             _IQ4isqrt(A)
#endif
#if GLOBAL_Q == 3
#define _IQisqrt(A)             _IQ3isqrt(A)
#endif
#if GLOBAL_Q == 2
#define _IQisqrt(A)             _IQ2isqrt(A)
#endif
#if GLOBAL_Q == 1
#define _IQisqrt(A)             _IQ1isqrt(A)
#endif


extern _iq30 _IQ30exp(_iq30 A);
extern _iq29 _IQ29exp(_iq29 A);
extern _iq28 _IQ28exp(_iq28 A);
extern _iq27 _IQ27exp(_iq27 A);
extern _iq26 _IQ26exp(_iq26 A);
extern _iq25 _IQ25exp(_iq25 A);
extern _iq24 _IQ24exp(_iq24 A);
extern _iq23 _IQ23exp(_iq23 A);
extern _iq22 _IQ22exp(_iq22 A);
extern _iq21 _IQ21exp(_iq21 A);
extern _iq20 _IQ20exp(_iq20 A);
extern _iq19 _IQ19exp(_iq19 A);
extern _iq18 _IQ18exp(_iq18 A);
extern _iq17 _IQ17exp(_iq17 A);
extern _iq16 _IQ16exp(_iq16 A);
extern _iq15 _IQ15exp(_iq15 A);
extern _iq14 _IQ14exp(_iq14 A);
extern _iq13 _IQ13exp(_iq13 A);
extern _iq12 _IQ12exp(_iq12 A);
extern _iq11 _IQ11exp(_iq11 A);
extern _iq10 _IQ10exp(_iq10 A);
extern _iq9 _IQ9exp(_iq9 A);
extern _iq8 _IQ8exp(_iq8 A);
extern _iq7 _IQ7exp(_iq7 A);
extern _iq6 _IQ6exp(_iq6 A);
extern _iq5 _IQ5exp(_iq5 A);
extern _iq4 _IQ4exp(_iq4 A);
extern _iq3 _IQ3exp(_iq3 A);
extern _iq2 _IQ2exp(_iq2 A);
extern _iq1 _IQ1exp(_iq1 A);
/*******************************************************************************
* Function Name  : _IQexp
* Description    : IQ格式数以e为底的指数函数:e^A
* Input          : 参数A为IQ格式的数
* Return         : 返回A以e为底的指数值，与参数的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 30
#define _IQexp(A)               _IQ30exp(A)
#endif
#if GLOBAL_Q == 29
#define _IQexp(A)               _IQ29exp(A)
#endif
#if GLOBAL_Q == 28
#define _IQexp(A)               _IQ28exp(A)
#endif
#if GLOBAL_Q == 27
#define _IQexp(A)               _IQ27exp(A)
#endif
#if GLOBAL_Q == 26
#define _IQexp(A)               _IQ26exp(A)
#endif
#if GLOBAL_Q == 25
#define _IQexp(A)               _IQ25exp(A)
#endif
#if GLOBAL_Q == 24
#define _IQexp(A)               _IQ24exp(A)
#endif
#if GLOBAL_Q == 23
#define _IQexp(A)               _IQ23exp(A)
#endif
#if GLOBAL_Q == 22
#define _IQexp(A)               _IQ22exp(A)
#endif
#if GLOBAL_Q == 21
#define _IQexp(A)               _IQ21exp(A)
#endif
#if GLOBAL_Q == 20
#define _IQexp(A)               _IQ20exp(A)
#endif
#if GLOBAL_Q == 19
#define _IQexp(A)               _IQ19exp(A)
#endif
#if GLOBAL_Q == 18
#define _IQexp(A)               _IQ18exp(A)
#endif
#if GLOBAL_Q == 17
#define _IQexp(A)               _IQ17exp(A)
#endif
#if GLOBAL_Q == 16
#define _IQexp(A)               _IQ16exp(A)
#endif
#if GLOBAL_Q == 15
#define _IQexp(A)               _IQ15exp(A)
#endif
#if GLOBAL_Q == 14
#define _IQexp(A)               _IQ14exp(A)
#endif
#if GLOBAL_Q == 13
#define _IQexp(A)               _IQ13exp(A)
#endif
#if GLOBAL_Q == 12
#define _IQexp(A)               _IQ12exp(A)
#endif
#if GLOBAL_Q == 11
#define _IQexp(A)               _IQ11exp(A)
#endif
#if GLOBAL_Q == 10
#define _IQexp(A)               _IQ10exp(A)
#endif
#if GLOBAL_Q == 9
#define _IQexp(A)               _IQ9exp(A)
#endif
#if GLOBAL_Q == 8
#define _IQexp(A)               _IQ8exp(A)
#endif
#if GLOBAL_Q == 7
#define _IQexp(A)               _IQ7exp(A)
#endif
#if GLOBAL_Q == 6
#define _IQexp(A)               _IQ6exp(A)
#endif
#if GLOBAL_Q == 5
#define _IQexp(A)               _IQ5exp(A)
#endif
#if GLOBAL_Q == 4
#define _IQexp(A)               _IQ4exp(A)
#endif
#if GLOBAL_Q == 3
#define _IQexp(A)               _IQ3exp(A)
#endif
#if GLOBAL_Q == 2
#define _IQexp(A)               _IQ2exp(A)
#endif
#if GLOBAL_Q == 1
#define _IQexp(A)               _IQ1exp(A)
#endif


extern _iq30 _IQ30exp2(_iq30 A);
extern _iq29 _IQ29exp2(_iq29 A);
extern _iq28 _IQ28exp2(_iq28 A);
extern _iq27 _IQ27exp2(_iq27 A);
extern _iq26 _IQ26exp2(_iq26 A);
extern _iq25 _IQ25exp2(_iq25 A);
extern _iq24 _IQ24exp2(_iq24 A);
extern _iq23 _IQ23exp2(_iq23 A);
extern _iq22 _IQ22exp2(_iq22 A);
extern _iq21 _IQ21exp2(_iq21 A);
extern _iq20 _IQ20exp2(_iq20 A);
extern _iq19 _IQ19exp2(_iq19 A);
extern _iq18 _IQ18exp2(_iq18 A);
extern _iq17 _IQ17exp2(_iq17 A);
extern _iq16 _IQ16exp2(_iq16 A);
extern _iq15 _IQ15exp2(_iq15 A);
extern _iq14 _IQ14exp2(_iq14 A);
extern _iq13 _IQ13exp2(_iq13 A);
extern _iq12 _IQ12exp2(_iq12 A);
extern _iq11 _IQ11exp2(_iq11 A);
extern _iq10 _IQ10exp2(_iq10 A);
extern _iq9 _IQ9exp2(_iq9 A);
extern _iq8 _IQ8exp2(_iq8 A);
extern _iq7 _IQ7exp2(_iq7 A);
extern _iq6 _IQ6exp2(_iq6 A);
extern _iq5 _IQ5exp2(_iq5 A);
extern _iq4 _IQ4exp2(_iq4 A);
extern _iq3 _IQ3exp2(_iq3 A);
extern _iq2 _IQ2exp2(_iq2 A);
extern _iq1 _IQ1exp2(_iq1 A);
/*******************************************************************************
* Function Name  : _IQexp2
* Description    : IQ格式数以2为底的指数函数:2^A
* Input          : 参数A为IQ格式的数
* Return         : 返回A以2为底的指数值，与参数的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 30
#define _IQexp2(A)              _IQ30exp2(A)
#endif
#if GLOBAL_Q == 29
#define _IQexp2(A)              _IQ29exp2(A)
#endif
#if GLOBAL_Q == 28
#define _IQexp2(A)              _IQ28exp2(A)
#endif
#if GLOBAL_Q == 27
#define _IQexp2(A)              _IQ27exp2(A)
#endif
#if GLOBAL_Q == 26
#define _IQexp2(A)              _IQ26exp2(A)
#endif
#if GLOBAL_Q == 25
#define _IQexp2(A)              _IQ25exp2(A)
#endif
#if GLOBAL_Q == 24
#define _IQexp2(A)              _IQ24exp2(A)
#endif
#if GLOBAL_Q == 23
#define _IQexp2(A)              _IQ23exp2(A)
#endif
#if GLOBAL_Q == 22
#define _IQexp2(A)              _IQ22exp2(A)
#endif
#if GLOBAL_Q == 21
#define _IQexp2(A)              _IQ21exp2(A)
#endif
#if GLOBAL_Q == 20
#define _IQexp2(A)              _IQ20exp2(A)
#endif
#if GLOBAL_Q == 19
#define _IQexp2(A)              _IQ19exp2(A)
#endif
#if GLOBAL_Q == 18
#define _IQexp2(A)              _IQ18exp2(A)
#endif
#if GLOBAL_Q == 17
#define _IQexp2(A)              _IQ17exp2(A)
#endif
#if GLOBAL_Q == 16
#define _IQexp2(A)              _IQ16exp2(A)
#endif
#if GLOBAL_Q == 15
#define _IQexp2(A)              _IQ15exp2(A)
#endif
#if GLOBAL_Q == 14
#define _IQexp2(A)              _IQ14exp2(A)
#endif
#if GLOBAL_Q == 13
#define _IQexp2(A)              _IQ13exp2(A)
#endif
#if GLOBAL_Q == 12
#define _IQexp2(A)              _IQ12exp2(A)
#endif
#if GLOBAL_Q == 11
#define _IQexp2(A)              _IQ11exp2(A)
#endif
#if GLOBAL_Q == 10
#define _IQexp2(A)              _IQ10exp2(A)
#endif
#if GLOBAL_Q == 9
#define _IQexp2(A)              _IQ9exp2(A)
#endif
#if GLOBAL_Q == 8
#define _IQexp2(A)              _IQ8exp2(A)
#endif
#if GLOBAL_Q == 7
#define _IQexp2(A)              _IQ7exp2(A)
#endif
#if GLOBAL_Q == 6
#define _IQexp2(A)              _IQ6exp2(A)
#endif
#if GLOBAL_Q == 5
#define _IQexp2(A)              _IQ5exp2(A)
#endif
#if GLOBAL_Q == 4
#define _IQexp2(A)              _IQ4exp2(A)
#endif
#if GLOBAL_Q == 3
#define _IQexp2(A)              _IQ3exp2(A)
#endif
#if GLOBAL_Q == 2
#define _IQexp2(A)              _IQ2exp2(A)
#endif
#if GLOBAL_Q == 1
#define _IQexp2(A)              _IQ1exp2(A)
#endif


#define _IQ30int(A)             ((A) >> 30)
#define _IQ29int(A)             ((A) >> 29)
#define _IQ28int(A)             ((A) >> 28)
#define _IQ27int(A)             ((A) >> 27)
#define _IQ26int(A)             ((A) >> 26)
#define _IQ25int(A)             ((A) >> 25)
#define _IQ24int(A)             ((A) >> 24)
#define _IQ23int(A)             ((A) >> 23)
#define _IQ22int(A)             ((A) >> 22)
#define _IQ21int(A)             ((A) >> 21)
#define _IQ20int(A)             ((A) >> 20)
#define _IQ19int(A)             ((A) >> 19)
#define _IQ18int(A)             ((A) >> 18)
#define _IQ17int(A)             ((A) >> 17)
#define _IQ16int(A)             ((A) >> 16)
#define _IQ15int(A)             ((A) >> 15)
#define _IQ14int(A)             ((A) >> 14)
#define _IQ13int(A)             ((A) >> 13)
#define _IQ12int(A)             ((A) >> 12)
#define _IQ11int(A)             ((A) >> 11)
#define _IQ10int(A)             ((A) >> 10)
#define _IQ9int(A)              ((A) >> 9)
#define _IQ8int(A)              ((A) >> 8)
#define _IQ7int(A)              ((A) >> 7)
#define _IQ6int(A)              ((A) >> 6)
#define _IQ5int(A)              ((A) >> 5)
#define _IQ4int(A)              ((A) >> 4)
#define _IQ3int(A)              ((A) >> 3)
#define _IQ2int(A)              ((A) >> 2)
#define _IQ1int(A)              ((A) >> 1)
/*******************************************************************************
* Function Name  : _IQint
* Description    : 计算IQ格式数的整数部分
* Input          : 参数A为IQ格式的数
* Return         : 返回A整数部分
*******************************************************************************/
#define _IQint(A)               ((A) >> GLOBAL_Q)


extern _iq30 _IQ30frac(_iq30 A);
extern _iq29 _IQ29frac(_iq29 A);
extern _iq28 _IQ28frac(_iq28 A);
extern _iq27 _IQ27frac(_iq27 A);
extern _iq26 _IQ26frac(_iq26 A);
extern _iq25 _IQ25frac(_iq25 A);
extern _iq24 _IQ24frac(_iq24 A);
extern _iq23 _IQ23frac(_iq23 A);
extern _iq22 _IQ22frac(_iq22 A);
extern _iq21 _IQ21frac(_iq21 A);
extern _iq20 _IQ20frac(_iq20 A);
extern _iq19 _IQ19frac(_iq19 A);
extern _iq18 _IQ18frac(_iq18 A);
extern _iq17 _IQ17frac(_iq17 A);
extern _iq16 _IQ16frac(_iq16 A);
extern _iq15 _IQ15frac(_iq15 A);
extern _iq14 _IQ14frac(_iq14 A);
extern _iq13 _IQ13frac(_iq13 A);
extern _iq12 _IQ12frac(_iq12 A);
extern _iq11 _IQ11frac(_iq11 A);
extern _iq10 _IQ10frac(_iq10 A);
extern _iq9 _IQ9frac(_iq9 A);
extern _iq8 _IQ8frac(_iq8 A);
extern _iq7 _IQ7frac(_iq7 A);
extern _iq6 _IQ6frac(_iq6 A);
extern _iq5 _IQ5frac(_iq5 A);
extern _iq4 _IQ4frac(_iq4 A);
extern _iq3 _IQ3frac(_iq3 A);
extern _iq2 _IQ2frac(_iq2 A);
extern _iq1 _IQ1frac(_iq1 A);
/*******************************************************************************
* Function Name  : _IQfrac
* Description    : 计算IQ格式数的小数部分
* Input          : 参数A为IQ格式的数
* Return         : 返回A小数部分，与参数的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 30
#define _IQfrac(A)              _IQ30frac(A)
#endif
#if GLOBAL_Q == 29
#define _IQfrac(A)              _IQ29frac(A)
#endif
#if GLOBAL_Q == 28
#define _IQfrac(A)              _IQ28frac(A)
#endif
#if GLOBAL_Q == 27
#define _IQfrac(A)              _IQ27frac(A)
#endif
#if GLOBAL_Q == 26
#define _IQfrac(A)              _IQ26frac(A)
#endif
#if GLOBAL_Q == 25
#define _IQfrac(A)              _IQ25frac(A)
#endif
#if GLOBAL_Q == 24
#define _IQfrac(A)              _IQ24frac(A)
#endif
#if GLOBAL_Q == 23
#define _IQfrac(A)              _IQ23frac(A)
#endif
#if GLOBAL_Q == 22
#define _IQfrac(A)              _IQ22frac(A)
#endif
#if GLOBAL_Q == 21
#define _IQfrac(A)              _IQ21frac(A)
#endif
#if GLOBAL_Q == 20
#define _IQfrac(A)              _IQ20frac(A)
#endif
#if GLOBAL_Q == 19
#define _IQfrac(A)              _IQ19frac(A)
#endif
#if GLOBAL_Q == 18
#define _IQfrac(A)              _IQ18frac(A)
#endif
#if GLOBAL_Q == 17
#define _IQfrac(A)              _IQ17frac(A)
#endif
#if GLOBAL_Q == 16
#define _IQfrac(A)              _IQ16frac(A)
#endif
#if GLOBAL_Q == 15
#define _IQfrac(A)              _IQ15frac(A)
#endif
#if GLOBAL_Q == 14
#define _IQfrac(A)              _IQ14frac(A)
#endif
#if GLOBAL_Q == 13
#define _IQfrac(A)              _IQ13frac(A)
#endif
#if GLOBAL_Q == 12
#define _IQfrac(A)              _IQ12frac(A)
#endif
#if GLOBAL_Q == 11
#define _IQfrac(A)              _IQ11frac(A)
#endif
#if GLOBAL_Q == 10
#define _IQfrac(A)              _IQ10frac(A)
#endif
#if GLOBAL_Q == 9
#define _IQfrac(A)              _IQ9frac(A)
#endif
#if GLOBAL_Q == 8
#define _IQfrac(A)              _IQ8frac(A)
#endif
#if GLOBAL_Q == 7
#define _IQfrac(A)              _IQ7frac(A)
#endif
#if GLOBAL_Q == 6
#define _IQfrac(A)              _IQ6frac(A)
#endif
#if GLOBAL_Q == 5
#define _IQfrac(A)              _IQ5frac(A)
#endif
#if GLOBAL_Q == 4
#define _IQfrac(A)              _IQ4frac(A)
#endif
#if GLOBAL_Q == 3
#define _IQfrac(A)              _IQ3frac(A)
#endif
#if GLOBAL_Q == 2
#define _IQfrac(A)              _IQ2frac(A)
#endif
#if GLOBAL_Q == 1
#define _IQfrac(A)              _IQ1frac(A)
#endif

extern long __IQxmpy(long A, long B, long S);
#define _IQ30mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 30 + 32 - (IQA) - (IQB))
#define _IQ29mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 29 + 32 - (IQA) - (IQB))
#define _IQ28mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 28 + 32 - (IQA) - (IQB))
#define _IQ27mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 27 + 32 - (IQA) - (IQB))
#define _IQ26mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 26 + 32 - (IQA) - (IQB))
#define _IQ25mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 25 + 32 - (IQA) - (IQB))
#define _IQ24mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 24 + 32 - (IQA) - (IQB))
#define _IQ23mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 23 + 32 - (IQA) - (IQB))
#define _IQ22mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 22 + 32 - (IQA) - (IQB))
#define _IQ21mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 21 + 32 - (IQA) - (IQB))
#define _IQ20mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 20 + 32 - (IQA) - (IQB))
#define _IQ19mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 19 + 32 - (IQA) - (IQB))
#define _IQ18mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 18 + 32 - (IQA) - (IQB))
#define _IQ17mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 17 + 32 - (IQA) - (IQB))
#define _IQ16mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 16 + 32 - (IQA) - (IQB))
#define _IQ15mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 15 + 32 - (IQA) - (IQB))
#define _IQ14mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 14 + 32 - (IQA) - (IQB))
#define _IQ13mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 13 + 32 - (IQA) - (IQB))
#define _IQ12mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 12 + 32 - (IQA) - (IQB))
#define _IQ11mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 11 + 32 - (IQA) - (IQB))
#define _IQ10mpyIQX(A, IQA, B, IQB) __IQxmpy(A, B, 10 + 32 - (IQA) - (IQB))
#define _IQ9mpyIQX(A, IQA, B, IQB)  __IQxmpy(A, B, 9 + 32 - (IQA) - (IQB))
#define _IQ8mpyIQX(A, IQA, B, IQB)  __IQxmpy(A, B, 8 + 32 - (IQA) - (IQB))
#define _IQ7mpyIQX(A, IQA, B, IQB)  __IQxmpy(A, B, 7 + 32 - (IQA) - (IQB))
#define _IQ6mpyIQX(A, IQA, B, IQB)  __IQxmpy(A, B, 6 + 32 - (IQA) - (IQB))
#define _IQ5mpyIQX(A, IQA, B, IQB)  __IQxmpy(A, B, 5 + 32 - (IQA) - (IQB))
#define _IQ4mpyIQX(A, IQA, B, IQB)  __IQxmpy(A, B, 4 + 32 - (IQA) - (IQB))
#define _IQ3mpyIQX(A, IQA, B, IQB)  __IQxmpy(A, B, 3 + 32 - (IQA) - (IQB))
#define _IQ2mpyIQX(A, IQA, B, IQB)  __IQxmpy(A, B, 2 + 32 - (IQA) - (IQB))
#define _IQ1mpyIQX(A, IQA, B, IQB)  __IQxmpy(A, B, 1 + 32 - (IQA) - (IQB))
/*******************************************************************************
* Function Name  : _IQmpyIQX
* Description    : 2个任意IQ格式数乘积
* Input          : A:IQ格式数。IQA:A的IQ精度
* Input          : B:IQ格式数。IQB:B的IQ精度
* Return         : 返回A与B的乘积，与GLOBAL_Q指定的IQ格式相同
*******************************************************************************/
#define _IQmpyIQX(A, IQA, B, IQB)   __IQxmpy(A, B,                            \
                                             (GLOBAL_Q) + 32 - (IQA) - (IQB))


#define _IQ30mpyI32(A, B)       ((A) * (B))
#define _IQ29mpyI32(A, B)       ((A) * (B))
#define _IQ28mpyI32(A, B)       ((A) * (B))
#define _IQ27mpyI32(A, B)       ((A) * (B))
#define _IQ26mpyI32(A, B)       ((A) * (B))
#define _IQ25mpyI32(A, B)       ((A) * (B))
#define _IQ24mpyI32(A, B)       ((A) * (B))
#define _IQ23mpyI32(A, B)       ((A) * (B))
#define _IQ22mpyI32(A, B)       ((A) * (B))
#define _IQ21mpyI32(A, B)       ((A) * (B))
#define _IQ20mpyI32(A, B)       ((A) * (B))
#define _IQ19mpyI32(A, B)       ((A) * (B))
#define _IQ18mpyI32(A, B)       ((A) * (B))
#define _IQ17mpyI32(A, B)       ((A) * (B))
#define _IQ16mpyI32(A, B)       ((A) * (B))
#define _IQ15mpyI32(A, B)       ((A) * (B))
#define _IQ14mpyI32(A, B)       ((A) * (B))
#define _IQ13mpyI32(A, B)       ((A) * (B))
#define _IQ12mpyI32(A, B)       ((A) * (B))
#define _IQ11mpyI32(A, B)       ((A) * (B))
#define _IQ10mpyI32(A, B)       ((A) * (B))
#define _IQ9mpyI32(A, B)        ((A) * (B))
#define _IQ8mpyI32(A, B)        ((A) * (B))
#define _IQ7mpyI32(A, B)        ((A) * (B))
#define _IQ6mpyI32(A, B)        ((A) * (B))
#define _IQ5mpyI32(A, B)        ((A) * (B))
#define _IQ4mpyI32(A, B)        ((A) * (B))
#define _IQ3mpyI32(A, B)        ((A) * (B))
#define _IQ2mpyI32(A, B)        ((A) * (B))
#define _IQ1mpyI32(A, B)        ((A) * (B))
/*******************************************************************************
* Function Name  : _IQmpyI32
* Description    : IQ格式数乘以整数
* Input          : A:IQ格式数。B:整数
* Return         : 返回A与B的乘积,与参数A的IQ格式相同
*******************************************************************************/
#define _IQmpyI32(A, B)         ((A) * (B))

extern _iq30 _IQ30mpyI32int(_iq30 A, long B);
extern _iq29 _IQ29mpyI32int(_iq29 A, long B);
extern _iq28 _IQ28mpyI32int(_iq28 A, long B);
extern _iq27 _IQ27mpyI32int(_iq27 A, long B);
extern _iq26 _IQ26mpyI32int(_iq26 A, long B);
extern _iq25 _IQ25mpyI32int(_iq25 A, long B);
extern _iq24 _IQ24mpyI32int(_iq24 A, long B);
extern _iq23 _IQ23mpyI32int(_iq23 A, long B);
extern _iq22 _IQ22mpyI32int(_iq22 A, long B);
extern _iq21 _IQ21mpyI32int(_iq21 A, long B);
extern _iq20 _IQ20mpyI32int(_iq20 A, long B);
extern _iq19 _IQ19mpyI32int(_iq19 A, long B);
extern _iq18 _IQ18mpyI32int(_iq18 A, long B);
extern _iq17 _IQ17mpyI32int(_iq17 A, long B);
extern _iq16 _IQ16mpyI32int(_iq16 A, long B);
extern _iq15 _IQ15mpyI32int(_iq15 A, long B);
extern _iq14 _IQ14mpyI32int(_iq14 A, long B);
extern _iq13 _IQ13mpyI32int(_iq13 A, long B);
extern _iq12 _IQ12mpyI32int(_iq12 A, long B);
extern _iq11 _IQ11mpyI32int(_iq11 A, long B);
extern _iq10 _IQ10mpyI32int(_iq10 A, long B);
extern _iq9 _IQ9mpyI32int(_iq9 A, long B);
extern _iq8 _IQ8mpyI32int(_iq8 A, long B);
extern _iq7 _IQ7mpyI32int(_iq7 A, long B);
extern _iq6 _IQ6mpyI32int(_iq6 A, long B);
extern _iq5 _IQ5mpyI32int(_iq5 A, long B);
extern _iq4 _IQ4mpyI32int(_iq4 A, long B);
extern _iq3 _IQ3mpyI32int(_iq3 A, long B);
extern _iq2 _IQ2mpyI32int(_iq2 A, long B);
extern _iq1 _IQ1mpyI32int(_iq1 A, long B);
/*******************************************************************************
* Function Name  : _IQmpyI32int
* Description    : IQ格式数乘以整数
* Input          : A:IQ格式数。B:整数
* Return         : 返回A与B乘积的整数部分
*******************************************************************************/
#if GLOBAL_Q == 30
#define _IQmpyI32int(A, B)      _IQ30mpyI32int(A, B)
#endif
#if GLOBAL_Q == 29
#define _IQmpyI32int(A, B)      _IQ29mpyI32int(A, B)
#endif
#if GLOBAL_Q == 28
#define _IQmpyI32int(A, B)      _IQ28mpyI32int(A, B)
#endif
#if GLOBAL_Q == 27
#define _IQmpyI32int(A, B)      _IQ27mpyI32int(A, B)
#endif
#if GLOBAL_Q == 26
#define _IQmpyI32int(A, B)      _IQ26mpyI32int(A, B)
#endif
#if GLOBAL_Q == 25
#define _IQmpyI32int(A, B)      _IQ25mpyI32int(A, B)
#endif
#if GLOBAL_Q == 24
#define _IQmpyI32int(A, B)      _IQ24mpyI32int(A, B)
#endif
#if GLOBAL_Q == 23
#define _IQmpyI32int(A, B)      _IQ23mpyI32int(A, B)
#endif
#if GLOBAL_Q == 22
#define _IQmpyI32int(A, B)      _IQ22mpyI32int(A, B)
#endif
#if GLOBAL_Q == 21
#define _IQmpyI32int(A, B)      _IQ21mpyI32int(A, B)
#endif
#if GLOBAL_Q == 20
#define _IQmpyI32int(A, B)      _IQ20mpyI32int(A, B)
#endif
#if GLOBAL_Q == 19
#define _IQmpyI32int(A, B)      _IQ19mpyI32int(A, B)
#endif
#if GLOBAL_Q == 18
#define _IQmpyI32int(A, B)      _IQ18mpyI32int(A, B)
#endif
#if GLOBAL_Q == 17
#define _IQmpyI32int(A, B)      _IQ17mpyI32int(A, B)
#endif
#if GLOBAL_Q == 16
#define _IQmpyI32int(A, B)      _IQ16mpyI32int(A, B)
#endif
#if GLOBAL_Q == 15
#define _IQmpyI32int(A, B)      _IQ15mpyI32int(A, B)
#endif
#if GLOBAL_Q == 14
#define _IQmpyI32int(A, B)      _IQ14mpyI32int(A, B)
#endif
#if GLOBAL_Q == 13
#define _IQmpyI32int(A, B)      _IQ13mpyI32int(A, B)
#endif
#if GLOBAL_Q == 12
#define _IQmpyI32int(A, B)      _IQ12mpyI32int(A, B)
#endif
#if GLOBAL_Q == 11
#define _IQmpyI32int(A, B)      _IQ11mpyI32int(A, B)
#endif
#if GLOBAL_Q == 10
#define _IQmpyI32int(A, B)      _IQ10mpyI32int(A, B)
#endif
#if GLOBAL_Q == 9
#define _IQmpyI32int(A, B)      _IQ9mpyI32int(A, B)
#endif
#if GLOBAL_Q == 8
#define _IQmpyI32int(A, B)      _IQ8mpyI32int(A, B)
#endif
#if GLOBAL_Q == 7
#define _IQmpyI32int(A, B)      _IQ7mpyI32int(A, B)
#endif
#if GLOBAL_Q == 6
#define _IQmpyI32int(A, B)      _IQ6mpyI32int(A, B)
#endif
#if GLOBAL_Q == 5
#define _IQmpyI32int(A, B)      _IQ5mpyI32int(A, B)
#endif
#if GLOBAL_Q == 4
#define _IQmpyI32int(A, B)      _IQ4mpyI32int(A, B)
#endif
#if GLOBAL_Q == 3
#define _IQmpyI32int(A, B)      _IQ3mpyI32int(A, B)
#endif
#if GLOBAL_Q == 2
#define _IQmpyI32int(A, B)      _IQ2mpyI32int(A, B)
#endif
#if GLOBAL_Q == 1
#define _IQmpyI32int(A, B)      _IQ1mpyI32int(A, B)
#endif

extern _iq30 _IQ30mpyI32frac(_iq30 A, long B);
extern _iq29 _IQ29mpyI32frac(_iq29 A, long B);
extern _iq28 _IQ28mpyI32frac(_iq28 A, long B);
extern _iq27 _IQ27mpyI32frac(_iq27 A, long B);
extern _iq26 _IQ26mpyI32frac(_iq26 A, long B);
extern _iq25 _IQ25mpyI32frac(_iq25 A, long B);
extern _iq24 _IQ24mpyI32frac(_iq24 A, long B);
extern _iq23 _IQ23mpyI32frac(_iq23 A, long B);
extern _iq22 _IQ22mpyI32frac(_iq22 A, long B);
extern _iq21 _IQ21mpyI32frac(_iq21 A, long B);
extern _iq20 _IQ20mpyI32frac(_iq20 A, long B);
extern _iq19 _IQ19mpyI32frac(_iq19 A, long B);
extern _iq18 _IQ18mpyI32frac(_iq18 A, long B);
extern _iq17 _IQ17mpyI32frac(_iq17 A, long B);
extern _iq16 _IQ16mpyI32frac(_iq16 A, long B);
extern _iq15 _IQ15mpyI32frac(_iq15 A, long B);
extern _iq14 _IQ14mpyI32frac(_iq14 A, long B);
extern _iq13 _IQ13mpyI32frac(_iq13 A, long B);
extern _iq12 _IQ12mpyI32frac(_iq12 A, long B);
extern _iq11 _IQ11mpyI32frac(_iq11 A, long B);
extern _iq10 _IQ10mpyI32frac(_iq10 A, long B);
extern _iq9 _IQ9mpyI32frac(_iq9 A, long B);
extern _iq8 _IQ8mpyI32frac(_iq8 A, long B);
extern _iq7 _IQ7mpyI32frac(_iq7 A, long B);
extern _iq6 _IQ6mpyI32frac(_iq6 A, long B);
extern _iq5 _IQ5mpyI32frac(_iq5 A, long B);
extern _iq4 _IQ4mpyI32frac(_iq4 A, long B);
extern _iq3 _IQ3mpyI32frac(_iq3 A, long B);
extern _iq2 _IQ2mpyI32frac(_iq2 A, long B);
extern _iq1 _IQ1mpyI32frac(_iq1 A, long B);
/*******************************************************************************
* Function Name  : _IQmpyI32frac
* Description    : IQ格式数乘以整数
* Input          : A:IQ格式数。B:整数
* Return         : 返回乘积的小数部分，与参数A的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 30
#define _IQmpyI32frac(A, B)     _IQ30mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 29
#define _IQmpyI32frac(A, B)     _IQ29mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 28
#define _IQmpyI32frac(A, B)     _IQ28mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 27
#define _IQmpyI32frac(A, B)     _IQ27mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 26
#define _IQmpyI32frac(A, B)     _IQ26mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 25
#define _IQmpyI32frac(A, B)     _IQ25mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 24
#define _IQmpyI32frac(A, B)     _IQ24mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 23
#define _IQmpyI32frac(A, B)     _IQ23mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 22
#define _IQmpyI32frac(A, B)     _IQ22mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 21
#define _IQmpyI32frac(A, B)     _IQ21mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 20
#define _IQmpyI32frac(A, B)     _IQ20mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 19
#define _IQmpyI32frac(A, B)     _IQ19mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 18
#define _IQmpyI32frac(A, B)     _IQ18mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 17
#define _IQmpyI32frac(A, B)     _IQ17mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 16
#define _IQmpyI32frac(A, B)     _IQ16mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 15
#define _IQmpyI32frac(A, B)     _IQ15mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 14
#define _IQmpyI32frac(A, B)     _IQ14mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 13
#define _IQmpyI32frac(A, B)     _IQ13mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 12
#define _IQmpyI32frac(A, B)     _IQ12mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 11
#define _IQmpyI32frac(A, B)     _IQ11mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 10
#define _IQmpyI32frac(A, B)     _IQ10mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 9
#define _IQmpyI32frac(A, B)     _IQ9mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 8
#define _IQmpyI32frac(A, B)     _IQ8mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 7
#define _IQmpyI32frac(A, B)     _IQ7mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 6
#define _IQmpyI32frac(A, B)     _IQ6mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 5
#define _IQmpyI32frac(A, B)     _IQ5mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 4
#define _IQmpyI32frac(A, B)     _IQ4mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 3
#define _IQmpyI32frac(A, B)     _IQ3mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 2
#define _IQmpyI32frac(A, B)     _IQ2mpyI32frac(A, B)
#endif
#if GLOBAL_Q == 1
#define _IQmpyI32frac(A, B)     _IQ1mpyI32frac(A, B)
#endif

extern _iq30 _IQ30mag(_iq30 A, _iq30 B);
extern _iq29 _IQ29mag(_iq29 A, _iq29 B);
extern _iq28 _IQ28mag(_iq28 A, _iq28 B);
extern _iq27 _IQ27mag(_iq27 A, _iq27 B);
extern _iq26 _IQ26mag(_iq26 A, _iq26 B);
extern _iq25 _IQ25mag(_iq25 A, _iq25 B);
extern _iq24 _IQ24mag(_iq24 A, _iq24 B);
extern _iq23 _IQ23mag(_iq23 A, _iq23 B);
extern _iq22 _IQ22mag(_iq22 A, _iq22 B);
extern _iq21 _IQ21mag(_iq21 A, _iq21 B);
extern _iq20 _IQ20mag(_iq20 A, _iq20 B);
extern _iq19 _IQ19mag(_iq19 A, _iq19 B);
extern _iq18 _IQ18mag(_iq18 A, _iq18 B);
extern _iq17 _IQ17mag(_iq17 A, _iq17 B);
extern _iq16 _IQ16mag(_iq16 A, _iq16 B);
extern _iq15 _IQ15mag(_iq15 A, _iq15 B);
extern _iq14 _IQ14mag(_iq14 A, _iq14 B);
extern _iq13 _IQ13mag(_iq13 A, _iq13 B);
extern _iq12 _IQ12mag(_iq12 A, _iq12 B);
extern _iq11 _IQ11mag(_iq11 A, _iq11 B);
extern _iq10 _IQ10mag(_iq10 A, _iq10 B);
extern _iq9 _IQ9mag(_iq9 A, _iq9 B);
extern _iq8 _IQ8mag(_iq8 A, _iq8 B);
extern _iq7 _IQ7mag(_iq7 A, _iq7 B);
extern _iq6 _IQ6mag(_iq6 A, _iq6 B);
extern _iq5 _IQ5mag(_iq5 A, _iq5 B);
extern _iq4 _IQ4mag(_iq4 A, _iq4 B);
extern _iq3 _IQ3mag(_iq3 A, _iq3 B);
extern _iq2 _IQ2mag(_iq2 A, _iq2 B);
extern _iq1 _IQ1mag(_iq1 A, _iq1 B);
/*******************************************************************************
* Function Name  : _IQmag
* Description    : IQ格式数的的求模函数:(A*A+B*B)^(1/2)
* Input          : 参数A、B为IQ格式的数
* Return         : 返回原点与点(A,B)之间的长度值，与参数的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 30
#define _IQmag(A, B)            _IQ30mag(A, B)
#endif
#if GLOBAL_Q == 29
#define _IQmag(A, B)            _IQ29mag(A, B)
#endif
#if GLOBAL_Q == 28
#define _IQmag(A, B)            _IQ28mag(A, B)
#endif
#if GLOBAL_Q == 27
#define _IQmag(A, B)            _IQ27mag(A, B)
#endif
#if GLOBAL_Q == 26
#define _IQmag(A, B)            _IQ26mag(A, B)
#endif
#if GLOBAL_Q == 25
#define _IQmag(A, B)            _IQ25mag(A, B)
#endif
#if GLOBAL_Q == 24
#define _IQmag(A, B)            _IQ24mag(A, B)
#endif
#if GLOBAL_Q == 23
#define _IQmag(A, B)            _IQ23mag(A, B)
#endif
#if GLOBAL_Q == 22
#define _IQmag(A, B)            _IQ22mag(A, B)
#endif
#if GLOBAL_Q == 21
#define _IQmag(A, B)            _IQ21mag(A, B)
#endif
#if GLOBAL_Q == 20
#define _IQmag(A, B)            _IQ20mag(A, B)
#endif
#if GLOBAL_Q == 19
#define _IQmag(A, B)            _IQ19mag(A, B)
#endif
#if GLOBAL_Q == 18
#define _IQmag(A, B)            _IQ18mag(A, B)
#endif
#if GLOBAL_Q == 17
#define _IQmag(A, B)            _IQ17mag(A, B)
#endif
#if GLOBAL_Q == 16
#define _IQmag(A, B)            _IQ16mag(A, B)
#endif
#if GLOBAL_Q == 15
#define _IQmag(A, B)            _IQ15mag(A, B)
#endif
#if GLOBAL_Q == 14
#define _IQmag(A, B)            _IQ14mag(A, B)
#endif
#if GLOBAL_Q == 13
#define _IQmag(A, B)            _IQ13mag(A, B)
#endif
#if GLOBAL_Q == 12
#define _IQmag(A, B)            _IQ12mag(A, B)
#endif
#if GLOBAL_Q == 11
#define _IQmag(A, B)            _IQ11mag(A, B)
#endif
#if GLOBAL_Q == 10
#define _IQmag(A, B)            _IQ10mag(A, B)
#endif
#if GLOBAL_Q == 9
#define _IQmag(A, B)            _IQ9mag(A, B)
#endif
#if GLOBAL_Q == 8
#define _IQmag(A, B)            _IQ8mag(A, B)
#endif
#if GLOBAL_Q == 7
#define _IQmag(A, B)            _IQ7mag(A, B)
#endif
#if GLOBAL_Q == 6
#define _IQmag(A, B)            _IQ6mag(A, B)
#endif
#if GLOBAL_Q == 5
#define _IQmag(A, B)            _IQ5mag(A, B)
#endif
#if GLOBAL_Q == 4
#define _IQmag(A, B)            _IQ4mag(A, B)
#endif
#if GLOBAL_Q == 3
#define _IQmag(A, B)            _IQ3mag(A, B)
#endif
#if GLOBAL_Q == 2
#define _IQmag(A, B)            _IQ2mag(A, B)
#endif
#if GLOBAL_Q == 1
#define _IQmag(A, B)            _IQ1mag(A, B)
#endif


extern _iq _atoIQN(const char *A, long B);
#define _atoIQ30(A)             _atoIQN(A, 30)
#define _atoIQ29(A)             _atoIQN(A, 29)
#define _atoIQ28(A)             _atoIQN(A, 28)
#define _atoIQ27(A)             _atoIQN(A, 27)
#define _atoIQ26(A)             _atoIQN(A, 26)
#define _atoIQ25(A)             _atoIQN(A, 25)
#define _atoIQ24(A)             _atoIQN(A, 24)
#define _atoIQ23(A)             _atoIQN(A, 23)
#define _atoIQ22(A)             _atoIQN(A, 22)
#define _atoIQ21(A)             _atoIQN(A, 21)
#define _atoIQ20(A)             _atoIQN(A, 20)
#define _atoIQ19(A)             _atoIQN(A, 19)
#define _atoIQ18(A)             _atoIQN(A, 18)
#define _atoIQ17(A)             _atoIQN(A, 17)
#define _atoIQ16(A)             _atoIQN(A, 16)
#define _atoIQ15(A)             _atoIQN(A, 15)
#define _atoIQ14(A)             _atoIQN(A, 14)
#define _atoIQ13(A)             _atoIQN(A, 13)
#define _atoIQ12(A)             _atoIQN(A, 12)
#define _atoIQ11(A)             _atoIQN(A, 11)
#define _atoIQ10(A)             _atoIQN(A, 10)
#define _atoIQ9(A)              _atoIQN(A, 9)
#define _atoIQ8(A)              _atoIQN(A, 8)
#define _atoIQ7(A)              _atoIQN(A, 7)
#define _atoIQ6(A)              _atoIQN(A, 6)
#define _atoIQ5(A)              _atoIQN(A, 5)
#define _atoIQ4(A)              _atoIQN(A, 4)
#define _atoIQ3(A)              _atoIQN(A, 3)
#define _atoIQ2(A)              _atoIQN(A, 2)
#define _atoIQ1(A)              _atoIQN(A, 1)
/*******************************************************************************
* Function Name  : _atoIQ
* Description    : 将一个字符串转换成IQ格式数。
* Input          : 参数A为一个字符串
* Return         : 返回IQ格式数
*******************************************************************************/
#define _atoIQ(A)               _atoIQN(A, GLOBAL_Q)

extern int _IQNtoa(char *A, const char *B, _iq C, int D);
#define _IQ30toa(A, B, C)       _IQNtoa(A, B, C, 30);
#define _IQ29toa(A, B, C)       _IQNtoa(A, B, C, 29);
#define _IQ28toa(A, B, C)       _IQNtoa(A, B, C, 28);
#define _IQ27toa(A, B, C)       _IQNtoa(A, B, C, 27);
#define _IQ26toa(A, B, C)       _IQNtoa(A, B, C, 26);
#define _IQ25toa(A, B, C)       _IQNtoa(A, B, C, 25);
#define _IQ24toa(A, B, C)       _IQNtoa(A, B, C, 24);
#define _IQ23toa(A, B, C)       _IQNtoa(A, B, C, 23);
#define _IQ22toa(A, B, C)       _IQNtoa(A, B, C, 22);
#define _IQ21toa(A, B, C)       _IQNtoa(A, B, C, 21);
#define _IQ20toa(A, B, C)       _IQNtoa(A, B, C, 20);
#define _IQ19toa(A, B, C)       _IQNtoa(A, B, C, 19);
#define _IQ18toa(A, B, C)       _IQNtoa(A, B, C, 18);
#define _IQ17toa(A, B, C)       _IQNtoa(A, B, C, 17);
#define _IQ16toa(A, B, C)       _IQNtoa(A, B, C, 16);
#define _IQ15toa(A, B, C)       _IQNtoa(A, B, C, 15);
#define _IQ14toa(A, B, C)       _IQNtoa(A, B, C, 14);
#define _IQ13toa(A, B, C)       _IQNtoa(A, B, C, 13);
#define _IQ12toa(A, B, C)       _IQNtoa(A, B, C, 12);
#define _IQ11toa(A, B, C)       _IQNtoa(A, B, C, 11);
#define _IQ10toa(A, B, C)       _IQNtoa(A, B, C, 10);
#define _IQ9toa(A, B, C)        _IQNtoa(A, B, C, 9);
#define _IQ8toa(A, B, C)        _IQNtoa(A, B, C, 8);
#define _IQ7toa(A, B, C)        _IQNtoa(A, B, C, 7);
#define _IQ6toa(A, B, C)        _IQNtoa(A, B, C, 6);
#define _IQ5toa(A, B, C)        _IQNtoa(A, B, C, 5);
#define _IQ4toa(A, B, C)        _IQNtoa(A, B, C, 4);
#define _IQ3toa(A, B, C)        _IQNtoa(A, B, C, 3);
#define _IQ2toa(A, B, C)        _IQNtoa(A, B, C, 2);
#define _IQ1toa(A, B, C)        _IQNtoa(A, B, C, 1);
/*******************************************************************************
* Function Name  : _IQtoa
* Description    : 将IQ格式数转换成一个字符串。
* Input          : A:存放转换结果的缓冲区。
*                : B:格式化输出参数"%I.Qf",I和Q分别代表整数、小数部分的10进制个数。
*                :   例如，"%7.8f"打印整数部分7个10进制数，小数部分打印8个10进制数。
*                : C:要转换的IQ格式数。
* Return         : 返回C的字符串形式。
*******************************************************************************/
#define _IQtoa(A, B, C)         _IQNtoa(A, B, C, GLOBAL_Q)


#define _IQ30abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ29abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ28abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ27abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ26abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ25abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ24abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ23abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ22abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ21abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ20abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ19abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ18abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ17abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ16abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ15abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ14abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ13abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ12abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ11abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ10abs(A)             (((A) < 0) ? - (A) : (A))
#define _IQ9abs(A)              (((A) < 0) ? - (A) : (A))
#define _IQ8abs(A)              (((A) < 0) ? - (A) : (A))
#define _IQ7abs(A)              (((A) < 0) ? - (A) : (A))
#define _IQ6abs(A)              (((A) < 0) ? - (A) : (A))
#define _IQ5abs(A)              (((A) < 0) ? - (A) : (A))
#define _IQ4abs(A)              (((A) < 0) ? - (A) : (A))
#define _IQ3abs(A)              (((A) < 0) ? - (A) : (A))
#define _IQ2abs(A)              (((A) < 0) ? - (A) : (A))
#define _IQ1abs(A)              (((A) < 0) ? - (A) : (A))
/*******************************************************************************
* Function Name  : _IQabs
* Description    : IQ格式数的绝对值函数:|A|
* Input          : 参数A为IQ格式的数
* Return         : 返回A的绝对值，与参数的IQ格式相同
*******************************************************************************/
#define _IQabs(A)               (((A) < 0) ? - (A) : (A))

extern _iq30 _IQ30log10(_iq30 A);
extern _iq29 _IQ29log10(_iq29 A);
extern _iq28 _IQ28log10(_iq28 A);
extern _iq27 _IQ27log10(_iq27 A);
extern _iq26 _IQ26log10(_iq26 A);
extern _iq25 _IQ25log10(_iq25 A);
extern _iq24 _IQ24log10(_iq24 A);
extern _iq23 _IQ23log10(_iq23 A);
extern _iq22 _IQ22log10(_iq22 A);
extern _iq21 _IQ21log10(_iq21 A);
extern _iq20 _IQ20log10(_iq20 A);
extern _iq19 _IQ19log10(_iq19 A);
extern _iq18 _IQ18log10(_iq18 A);
extern _iq17 _IQ17log10(_iq17 A);
extern _iq16 _IQ16log10(_iq16 A);
extern _iq15 _IQ15log10(_iq15 A);
extern _iq14 _IQ14log10(_iq14 A);
extern _iq13 _IQ13log10(_iq13 A);
extern _iq12 _IQ12log10(_iq12 A);
extern _iq11 _IQ11log10(_iq11 A);
extern _iq10 _IQ10log10(_iq10 A);
extern _iq9 _IQ9log10(_iq9 A);
extern _iq8 _IQ8log10(_iq8 A);
extern _iq7 _IQ7log10(_iq7 A);
extern _iq6 _IQ6log10(_iq6 A);
extern _iq5 _IQ5log10(_iq5 A);
extern _iq4 _IQ4log10(_iq4 A);
extern _iq3 _IQ3log10(_iq3 A);
extern _iq2 _IQ2log10(_iq2 A);

/*******************************************************************************
* Function Name  : _IQlog10
* Description    : IQ格式数的以10为底的对数:log10
* Input          : 参数A为IQ格式的数
* Return         : 返回A的以10为底的对数，与参数的IQ格式相同
*******************************************************************************/
#if GLOBAL_Q == 30
#define _IQlog10(A)            _IQ30log10(A)
#endif
#if GLOBAL_Q == 29
#define _IQlog10(A)            _IQ29log10(A)
#endif
#if GLOBAL_Q == 28
#define _IQlog10(A)            _IQ28log10(A)
#endif
#if GLOBAL_Q == 27
#define _IQlog10(A)            _IQ27log10(A)
#endif
#if GLOBAL_Q == 26
#define _IQlog10(A)            _IQ26log10(A)
#endif
#if GLOBAL_Q == 25
#define _IQlog10(A)            _IQ25log10(A)
#endif
#if GLOBAL_Q == 24
#define _IQlog10(A)            _IQ24log10(A)
#endif
#if GLOBAL_Q == 23
#define _IQlog10(A)            _IQ23log10(A)
#endif
#if GLOBAL_Q == 22
#define _IQlog10(A)            _IQ22log10(A)
#endif
#if GLOBAL_Q == 21
#define _IQlog10(A)            _IQ21log10(A)
#endif
#if GLOBAL_Q == 20
#define _IQlog10(A)            _IQ20log10(A)
#endif
#if GLOBAL_Q == 19
#define _IQlog10(A)            _IQ19log10(A)
#endif
#if GLOBAL_Q == 18
#define _IQlog10(A)            _IQ18log10(A)
#endif
#if GLOBAL_Q == 17
#define _IQlog10(A)            _IQ17log10(A)
#endif
#if GLOBAL_Q == 16
#define _IQlog10(A)            _IQ16log10(A)
#endif
#if GLOBAL_Q == 15
#define _IQlog10(A)            _IQ15log10(A)
#endif
#if GLOBAL_Q == 14
#define _IQlog10(A)            _IQ14log10(A)
#endif
#if GLOBAL_Q == 13
#define _IQlog10(A)            _IQ13log10(A)
#endif
#if GLOBAL_Q == 12
#define _IQlog10(A)            _IQ12log10(A)
#endif
#if GLOBAL_Q == 11
#define _IQlog10(A)            _IQ11log10(A)
#endif
#if GLOBAL_Q == 10
#define _IQlog10(A)            _IQ10log10(A)
#endif
#if GLOBAL_Q == 9
#define _IQlog10(A)            _IQ9log10(A)
#endif
#if GLOBAL_Q == 8
#define _IQlog10(A)            _IQ8log10(A)
#endif
#if GLOBAL_Q == 7
#define _IQlog10(A)            _IQ7log10(A)
#endif
#if GLOBAL_Q == 6
#define _IQlog10(A)            _IQ6log10(A)
#endif
#if GLOBAL_Q == 5
#define _IQlog10(A)            _IQ5log10(A)
#endif
#if GLOBAL_Q == 4
#define _IQlog10(A)            _IQ4log10(A)
#endif
#if GLOBAL_Q == 3
#define _IQlog10(A)            _IQ3log10(A)
#endif
#if GLOBAL_Q == 2
#define _IQlog10(A)            _IQ2log10(A)
#endif


/*******************************************************************************
* Function Name  : _IQlog10Float
* Description    : 以定点的方式求浮点数log10，最大误差3.8893104843e-6
* Input          : 参数A为浮点格式数
* Return         : 以浮点形式返回A的以10为底的对数。
*******************************************************************************/
extern float _IQlog10Float(float A);

#else // MATH_TYPE == FLOAT_MATH


typedef float _iq30;
typedef float _iq29;
typedef float _iq28;
typedef float _iq27;
typedef float _iq26;
typedef float _iq25;
typedef float _iq24;
typedef float _iq23;
typedef float _iq22;
typedef float _iq21;
typedef float _iq20;
typedef float _iq19;
typedef float _iq18;
typedef float _iq17;
typedef float _iq16;
typedef float _iq15;
typedef float _iq14;
typedef float _iq13;
typedef float _iq12;
typedef float _iq11;
typedef float _iq10;
typedef float _iq9;
typedef float _iq8;
typedef float _iq7;
typedef float _iq6;
typedef float _iq5;
typedef float _iq4;
typedef float _iq3;
typedef float _iq2;
typedef float _iq1;
typedef float _iq;


#define _IQmpy2(A)              ((A) * 2.0)
#define _IQmpy4(A)              ((A) * 4.0)
#define _IQmpy8(A)              ((A) * 8.0)
#define _IQmpy16(A)             ((A) * 16.0)
#define _IQmpy32(A)             ((A) * 32.0)
#define _IQmpy64(A)             ((A) * 64.0)
#define _IQmpy128(A)            ((A) * 128.0)
#define _IQmpy256(A)            ((A) * 256.0)
#define _IQmpy512(A)            ((A) * 512.0)
#define _IQdiv2(A)              ((A) / 2.0)
#define _IQdiv4(A)              ((A) / 4.0)
#define _IQdiv8(A)              ((A) / 8.0)
#define _IQdiv16(A)             ((A) / 16.0)
#define _IQdiv32(A)             ((A) / 32.0)
#define _IQdiv64(A)             ((A) / 64.0)
#define _IQdiv128(A)            ((A) / 128.0)
#define _IQdiv256(A)            ((A) / 256.0)
#define _IQdiv512(A)            ((A) / 512.0)

#define _IQ30(A)                (A)
#define _IQ29(A)                (A)
#define _IQ28(A)                (A)
#define _IQ27(A)                (A)
#define _IQ26(A)                (A)
#define _IQ25(A)                (A)
#define _IQ24(A)                (A)
#define _IQ23(A)                (A)
#define _IQ22(A)                (A)
#define _IQ21(A)                (A)
#define _IQ20(A)                (A)
#define _IQ19(A)                (A)
#define _IQ18(A)                (A)
#define _IQ17(A)                (A)
#define _IQ16(A)                (A)
#define _IQ15(A)                (A)
#define _IQ14(A)                (A)
#define _IQ13(A)                (A)
#define _IQ12(A)                (A)
#define _IQ11(A)                (A)
#define _IQ10(A)                (A)
#define _IQ9(A)                 (A)
#define _IQ8(A)                 (A)
#define _IQ7(A)                 (A)
#define _IQ6(A)                 (A)
#define _IQ5(A)                 (A)
#define _IQ4(A)                 (A)
#define _IQ3(A)                 (A)
#define _IQ2(A)                 (A)
#define _IQ1(A)                 (A)
#define _IQ(A)                  (A)


#define _IQ30toF(A)             (A)
#define _IQ29toF(A)             (A)
#define _IQ28toF(A)             (A)
#define _IQ27toF(A)             (A)
#define _IQ26toF(A)             (A)
#define _IQ25toF(A)             (A)
#define _IQ24toF(A)             (A)
#define _IQ23toF(A)             (A)
#define _IQ22toF(A)             (A)
#define _IQ21toF(A)             (A)
#define _IQ20toF(A)             (A)
#define _IQ19toF(A)             (A)
#define _IQ18toF(A)             (A)
#define _IQ17toF(A)             (A)
#define _IQ16toF(A)             (A)
#define _IQ15toF(A)             (A)
#define _IQ14toF(A)             (A)
#define _IQ13toF(A)             (A)
#define _IQ12toF(A)             (A)
#define _IQ11toF(A)             (A)
#define _IQ10toF(A)             (A)
#define _IQ9toF(A)              (A)
#define _IQ8toF(A)              (A)
#define _IQ7toF(A)              (A)
#define _IQ6toF(A)              (A)
#define _IQ5toF(A)              (A)
#define _IQ4toF(A)              (A)
#define _IQ3toF(A)              (A)
#define _IQ2toF(A)              (A)
#define _IQ1toF(A)              (A)
#define _IQtoF(A)               (A)


#define _IQ30toD(A)             (A)
#define _IQ29toD(A)             (A)
#define _IQ28toD(A)             (A)
#define _IQ27toD(A)             (A)
#define _IQ26toD(A)             (A)
#define _IQ25toD(A)             (A)
#define _IQ24toD(A)             (A)
#define _IQ23toD(A)             (A)
#define _IQ22toD(A)             (A)
#define _IQ21toD(A)             (A)
#define _IQ20toD(A)             (A)
#define _IQ19toD(A)             (A)
#define _IQ18toD(A)             (A)
#define _IQ17toD(A)             (A)
#define _IQ16toD(A)             (A)
#define _IQ15toD(A)             (A)
#define _IQ14toD(A)             (A)
#define _IQ13toD(A)             (A)
#define _IQ12toD(A)             (A)
#define _IQ11toD(A)             (A)
#define _IQ10toD(A)             (A)
#define _IQ9toD(A)              (A)
#define _IQ8toD(A)              (A)
#define _IQ7toD(A)              (A)
#define _IQ6toD(A)              (A)
#define _IQ5toD(A)              (A)
#define _IQ4toD(A)              (A)
#define _IQ3toD(A)              (A)
#define _IQ2toD(A)              (A)
#define _IQ1toD(A)              (A)
#define _IQtoD(A)               (A)


#define _IQsat(A, Pos, Neg)     (((A) > (Pos)) ?                              \
                                 (Pos) :                                      \
                                 (((A) < (Neg)) ? (Neg) : (A)))


#define _IQtoIQ30(A)            (A)
#define _IQ30toIQ(A)            (A)
#define _IQtoIQ29(A)            (A)
#define _IQ29toIQ(A)            (A)
#define _IQtoIQ28(A)            (A)
#define _IQ28toIQ(A)            (A)
#define _IQtoIQ27(A)            (A)
#define _IQ27toIQ(A)            (A)
#define _IQtoIQ26(A)            (A)
#define _IQ26toIQ(A)            (A)
#define _IQtoIQ25(A)            (A)
#define _IQ25toIQ(A)            (A)
#define _IQtoIQ24(A)            (A)
#define _IQ24toIQ(A)            (A)
#define _IQtoIQ23(A)            (A)
#define _IQ23toIQ(A)            (A)
#define _IQtoIQ22(A)            (A)
#define _IQ22toIQ(A)            (A)
#define _IQtoIQ21(A)            (A)
#define _IQ21toIQ(A)            (A)
#define _IQtoIQ20(A)            (A)
#define _IQ20toIQ(A)            (A)
#define _IQtoIQ19(A)            (A)
#define _IQ19toIQ(A)            (A)
#define _IQtoIQ18(A)            (A)
#define _IQ18toIQ(A)            (A)
#define _IQtoIQ17(A)            (A)
#define _IQ17toIQ(A)            (A)
#define _IQtoIQ16(A)            (A)
#define _IQ16toIQ(A)            (A)
#define _IQtoIQ15(A)            (A)
#define _IQ15toIQ(A)            (A)
#define _IQtoIQ14(A)            (A)
#define _IQ14toIQ(A)            (A)
#define _IQtoIQ13(A)            (A)
#define _IQ13toIQ(A)            (A)
#define _IQtoIQ12(A)            (A)
#define _IQ12toIQ(A)            (A)
#define _IQtoIQ11(A)            (A)
#define _IQ11toIQ(A)            (A)
#define _IQtoIQ10(A)            (A)
#define _IQ10toIQ(A)            (A)
#define _IQtoIQ9(A)             (A)
#define _IQ9toIQ(A)             (A)
#define _IQtoIQ8(A)             (A)
#define _IQ8toIQ(A)             (A)
#define _IQtoIQ7(A)             (A)
#define _IQ7toIQ(A)             (A)
#define _IQtoIQ6(A)             (A)
#define _IQ6toIQ(A)             (A)
#define _IQtoIQ5(A)             (A)
#define _IQ5toIQ(A)             (A)
#define _IQtoIQ4(A)             (A)
#define _IQ4toIQ(A)             (A)
#define _IQtoIQ3(A)             (A)
#define _IQ3toIQ(A)             (A)
#define _IQtoIQ2(A)             (A)
#define _IQ2toIQ(A)             (A)
#define _IQtoIQ1(A)             (A)
#define _IQ1toIQ(A)             (A)

#define _IQtoQ15(A)             ((short)((long)((A) * (1 << 15))))
#define _Q15toIQ(A)             (((float)(A)) * (1.0 / (1 << 15)))
#define _IQtoQ14(A)             ((short)((long)((A) * (1 << 14))))
#define _Q14toIQ(A)             (((float)(A)) * (1.0 / (1 << 14)))
#define _IQtoQ13(A)             ((short)((long)((A) * (1 << 13))))
#define _Q13toIQ(A)             (((float)(A)) * (1.0 / (1 << 13)))
#define _IQtoQ12(A)             ((short)((long)((A) * (1 << 12))))
#define _Q12toIQ(A)             (((float)(A)) * (1.0 / (1 << 12)))
#define _IQtoQ11(A)             ((short)((long)((A) * (1 << 11))))
#define _Q11toIQ(A)             (((float)(A)) * (1.0 / (1 << 11)))
#define _IQtoQ10(A)             ((short)((long)((A) * (1 << 10))))
#define _Q10toIQ(A)             (((float)(A)) * (1.0 / (1 << 10)))
#define _IQtoQ9(A)              ((short)((long)((A) * (1 << 9))))
#define _Q9toIQ(A)              (((float)(A)) * (1.0 / (1 << 9)))
#define _IQtoQ8(A)              ((short)((long)((A) * (1 << 8))))
#define _Q8toIQ(A)              (((float)(A)) * (1.0 / (1 << 8)))
#define _IQtoQ7(A)              ((short)((long)((A) * (1 << 7))))
#define _Q7toIQ(A)              (((float)(A)) * (1.0 / (1 << 7)))
#define _IQtoQ6(A)              ((short)((long)((A) * (1 << 6))))
#define _Q6toIQ(A)              (((float)(A)) * (1.0 / (1 << 6)))
#define _IQtoQ5(A)              ((short)((long)((A) * (1 << 5))))
#define _Q5toIQ(A)              (((float)(A)) * (1.0 / (1 << 5)))
#define _IQtoQ4(A)              ((short)((long)((A) * (1 << 4))))
#define _Q4toIQ(A)              (((float)(A)) * (1.0 / (1 << 4)))
#define _IQtoQ3(A)              ((short)((long)((A) * (1 << 3))))
#define _Q3toIQ(A)              (((float)(A)) * (1.0 / (1 << 3)))
#define _IQtoQ2(A)              ((short)((long)((A) * (1 << 2))))
#define _Q2toIQ(A)              (((float)(A)) * (1.0 / (1 << 2)))
#define _IQtoQ1(A)              ((short)((long)((A) * (1 << 1))))
#define _Q1toIQ(A)              (((float)(A)) * (1.0 / (1 << 1)))

#define _IQ30mpy(A, B)          ((A) * (B))
#define _IQ29mpy(A, B)          ((A) * (B))
#define _IQ28mpy(A, B)          ((A) * (B))
#define _IQ27mpy(A, B)          ((A) * (B))
#define _IQ26mpy(A, B)          ((A) * (B))
#define _IQ25mpy(A, B)          ((A) * (B))
#define _IQ24mpy(A, B)          ((A) * (B))
#define _IQ23mpy(A, B)          ((A) * (B))
#define _IQ22mpy(A, B)          ((A) * (B))
#define _IQ21mpy(A, B)          ((A) * (B))
#define _IQ20mpy(A, B)          ((A) * (B))
#define _IQ19mpy(A, B)          ((A) * (B))
#define _IQ18mpy(A, B)          ((A) * (B))
#define _IQ17mpy(A, B)          ((A) * (B))
#define _IQ16mpy(A, B)          ((A) * (B))
#define _IQ15mpy(A, B)          ((A) * (B))
#define _IQ14mpy(A, B)          ((A) * (B))
#define _IQ13mpy(A, B)          ((A) * (B))
#define _IQ12mpy(A, B)          ((A) * (B))
#define _IQ11mpy(A, B)          ((A) * (B))
#define _IQ10mpy(A, B)          ((A) * (B))
#define _IQ9mpy(A, B)           ((A) * (B))
#define _IQ8mpy(A, B)           ((A) * (B))
#define _IQ7mpy(A, B)           ((A) * (B))
#define _IQ6mpy(A, B)           ((A) * (B))
#define _IQ5mpy(A, B)           ((A) * (B))
#define _IQ4mpy(A, B)           ((A) * (B))
#define _IQ3mpy(A, B)           ((A) * (B))
#define _IQ2mpy(A, B)           ((A) * (B))
#define _IQ1mpy(A, B)           ((A) * (B))
#define _IQmpy(A, B)            ((A) * (B))


#define _IQ30rmpy(A, B)         ((A) * (B))
#define _IQ29rmpy(A, B)         ((A) * (B))
#define _IQ28rmpy(A, B)         ((A) * (B))
#define _IQ27rmpy(A, B)         ((A) * (B))
#define _IQ26rmpy(A, B)         ((A) * (B))
#define _IQ25rmpy(A, B)         ((A) * (B))
#define _IQ24rmpy(A, B)         ((A) * (B))
#define _IQ23rmpy(A, B)         ((A) * (B))
#define _IQ22rmpy(A, B)         ((A) * (B))
#define _IQ21rmpy(A, B)         ((A) * (B))
#define _IQ20rmpy(A, B)         ((A) * (B))
#define _IQ19rmpy(A, B)         ((A) * (B))
#define _IQ18rmpy(A, B)         ((A) * (B))
#define _IQ17rmpy(A, B)         ((A) * (B))
#define _IQ16rmpy(A, B)         ((A) * (B))
#define _IQ15rmpy(A, B)         ((A) * (B))
#define _IQ14rmpy(A, B)         ((A) * (B))
#define _IQ13rmpy(A, B)         ((A) * (B))
#define _IQ12rmpy(A, B)         ((A) * (B))
#define _IQ11rmpy(A, B)         ((A) * (B))
#define _IQ10rmpy(A, B)         ((A) * (B))
#define _IQ9rmpy(A, B)          ((A) * (B))
#define _IQ8rmpy(A, B)          ((A) * (B))
#define _IQ7rmpy(A, B)          ((A) * (B))
#define _IQ6rmpy(A, B)          ((A) * (B))
#define _IQ5rmpy(A, B)          ((A) * (B))
#define _IQ4rmpy(A, B)          ((A) * (B))
#define _IQ3rmpy(A, B)          ((A) * (B))
#define _IQ2rmpy(A, B)          ((A) * (B))
#define _IQ1rmpy(A, B)          ((A) * (B))
#define _IQrmpy(A, B)           ((A) * (B))


#define _IQ30rsmpy(A, B)        ((A) * (B))
#define _IQ29rsmpy(A, B)        ((A) * (B))
#define _IQ28rsmpy(A, B)        ((A) * (B))
#define _IQ27rsmpy(A, B)        ((A) * (B))
#define _IQ26rsmpy(A, B)        ((A) * (B))
#define _IQ25rsmpy(A, B)        ((A) * (B))
#define _IQ24rsmpy(A, B)        ((A) * (B))
#define _IQ23rsmpy(A, B)        ((A) * (B))
#define _IQ22rsmpy(A, B)        ((A) * (B))
#define _IQ21rsmpy(A, B)        ((A) * (B))
#define _IQ20rsmpy(A, B)        ((A) * (B))
#define _IQ19rsmpy(A, B)        ((A) * (B))
#define _IQ18rsmpy(A, B)        ((A) * (B))
#define _IQ17rsmpy(A, B)        ((A) * (B))
#define _IQ16rsmpy(A, B)        ((A) * (B))
#define _IQ15rsmpy(A, B)        ((A) * (B))
#define _IQ14rsmpy(A, B)        ((A) * (B))
#define _IQ13rsmpy(A, B)        ((A) * (B))
#define _IQ12rsmpy(A, B)        ((A) * (B))
#define _IQ11rsmpy(A, B)        ((A) * (B))
#define _IQ10rsmpy(A, B)        ((A) * (B))
#define _IQ9rsmpy(A, B)         ((A) * (B))
#define _IQ8rsmpy(A, B)         ((A) * (B))
#define _IQ7rsmpy(A, B)         ((A) * (B))
#define _IQ6rsmpy(A, B)         ((A) * (B))
#define _IQ5rsmpy(A, B)         ((A) * (B))
#define _IQ4rsmpy(A, B)         ((A) * (B))
#define _IQ3rsmpy(A, B)         ((A) * (B))
#define _IQ2rsmpy(A, B)         ((A) * (B))
#define _IQ1rsmpy(A, B)         ((A) * (B))
#define _IQrsmpy(A, B)          ((A) * (B))


#define _IQ30div(A, B)          ((A) / (B))
#define _IQ29div(A, B)          ((A) / (B))
#define _IQ28div(A, B)          ((A) / (B))
#define _IQ27div(A, B)          ((A) / (B))
#define _IQ26div(A, B)          ((A) / (B))
#define _IQ25div(A, B)          ((A) / (B))
#define _IQ24div(A, B)          ((A) / (B))
#define _IQ23div(A, B)          ((A) / (B))
#define _IQ22div(A, B)          ((A) / (B))
#define _IQ21div(A, B)          ((A) / (B))
#define _IQ20div(A, B)          ((A) / (B))
#define _IQ19div(A, B)          ((A) / (B))
#define _IQ18div(A, B)          ((A) / (B))
#define _IQ17div(A, B)          ((A) / (B))
#define _IQ16div(A, B)          ((A) / (B))
#define _IQ15div(A, B)          ((A) / (B))
#define _IQ14div(A, B)          ((A) / (B))
#define _IQ13div(A, B)          ((A) / (B))
#define _IQ12div(A, B)          ((A) / (B))
#define _IQ11div(A, B)          ((A) / (B))
#define _IQ10div(A, B)          ((A) / (B))
#define _IQ9div(A, B)           ((A) / (B))
#define _IQ8div(A, B)           ((A) / (B))
#define _IQ7div(A, B)           ((A) / (B))
#define _IQ6div(A, B)           ((A) / (B))
#define _IQ5div(A, B)           ((A) / (B))
#define _IQ4div(A, B)           ((A) / (B))
#define _IQ3div(A, B)           ((A) / (B))
#define _IQ2div(A, B)           ((A) / (B))
#define _IQ1div(A, B)           ((A) / (B))
#define _IQdiv(A, B)            ((A) / (B))


#define _IQ29sin(A)             sin(A)
#define _IQ28sin(A)             sin(A)
#define _IQ27sin(A)             sin(A)
#define _IQ26sin(A)             sin(A)
#define _IQ25sin(A)             sin(A)
#define _IQ24sin(A)             sin(A)
#define _IQ23sin(A)             sin(A)
#define _IQ22sin(A)             sin(A)
#define _IQ21sin(A)             sin(A)
#define _IQ20sin(A)             sin(A)
#define _IQ19sin(A)             sin(A)
#define _IQ18sin(A)             sin(A)
#define _IQ17sin(A)             sin(A)
#define _IQ16sin(A)             sin(A)
#define _IQ15sin(A)             sin(A)
#define _IQ14sin(A)             sin(A)
#define _IQ13sin(A)             sin(A)
#define _IQ12sin(A)             sin(A)
#define _IQ11sin(A)             sin(A)
#define _IQ10sin(A)             sin(A)
#define _IQ9sin(A)              sin(A)
#define _IQ8sin(A)              sin(A)
#define _IQ7sin(A)              sin(A)
#define _IQ6sin(A)              sin(A)
#define _IQ5sin(A)              sin(A)
#define _IQ4sin(A)              sin(A)
#define _IQ3sin(A)              sin(A)
#define _IQ2sin(A)              sin(A)
#define _IQ1sin(A)              sin(A)
#define _IQsin(A)               sin(A)


#define _IQ30sinPU(A)           sin((A) * 6.283185307)
#define _IQ29sinPU(A)           sin((A) * 6.283185307)
#define _IQ28sinPU(A)           sin((A) * 6.283185307)
#define _IQ27sinPU(A)           sin((A) * 6.283185307)
#define _IQ26sinPU(A)           sin((A) * 6.283185307)
#define _IQ25sinPU(A)           sin((A) * 6.283185307)
#define _IQ24sinPU(A)           sin((A) * 6.283185307)
#define _IQ23sinPU(A)           sin((A) * 6.283185307)
#define _IQ22sinPU(A)           sin((A) * 6.283185307)
#define _IQ21sinPU(A)           sin((A) * 6.283185307)
#define _IQ20sinPU(A)           sin((A) * 6.283185307)
#define _IQ19sinPU(A)           sin((A) * 6.283185307)
#define _IQ18sinPU(A)           sin((A) * 6.283185307)
#define _IQ17sinPU(A)           sin((A) * 6.283185307)
#define _IQ16sinPU(A)           sin((A) * 6.283185307)
#define _IQ15sinPU(A)           sin((A) * 6.283185307)
#define _IQ14sinPU(A)           sin((A) * 6.283185307)
#define _IQ13sinPU(A)           sin((A) * 6.283185307)
#define _IQ12sinPU(A)           sin((A) * 6.283185307)
#define _IQ11sinPU(A)           sin((A) * 6.283185307)
#define _IQ10sinPU(A)           sin((A) * 6.283185307)
#define _IQ9sinPU(A)            sin((A) * 6.283185307)
#define _IQ8sinPU(A)            sin((A) * 6.283185307)
#define _IQ7sinPU(A)            sin((A) * 6.283185307)
#define _IQ6sinPU(A)            sin((A) * 6.283185307)
#define _IQ5sinPU(A)            sin((A) * 6.283185307)
#define _IQ4sinPU(A)            sin((A) * 6.283185307)
#define _IQ3sinPU(A)            sin((A) * 6.283185307)
#define _IQ2sinPU(A)            sin((A) * 6.283185307)
#define _IQ1sinPU(A)            sin((A) * 6.283185307)
#define _IQsinPU(A)             sin((A) * 6.283185307)


#define _IQ29asin(A)            asin(A)
#define _IQ28asin(A)            asin(A)
#define _IQ27asin(A)            asin(A)
#define _IQ26asin(A)            asin(A)
#define _IQ25asin(A)            asin(A)
#define _IQ24asin(A)            asin(A)
#define _IQ23asin(A)            asin(A)
#define _IQ22asin(A)            asin(A)
#define _IQ21asin(A)            asin(A)
#define _IQ20asin(A)            asin(A)
#define _IQ19asin(A)            asin(A)
#define _IQ18asin(A)            asin(A)
#define _IQ17asin(A)            asin(A)
#define _IQ16asin(A)            asin(A)
#define _IQ15asin(A)            asin(A)
#define _IQ14asin(A)            asin(A)
#define _IQ13asin(A)            asin(A)
#define _IQ12asin(A)            asin(A)
#define _IQ11asin(A)            asin(A)
#define _IQ10asin(A)            asin(A)
#define _IQ9asin(A)             asin(A)
#define _IQ8asin(A)             asin(A)
#define _IQ7asin(A)             asin(A)
#define _IQ6asin(A)             asin(A)
#define _IQ5asin(A)             asin(A)
#define _IQ4asin(A)             asin(A)
#define _IQ3asin(A)             asin(A)
#define _IQ2asin(A)             asin(A)
#define _IQ1asin(A)             asin(A)
#define _IQasin(A)              asin(A)


#define _IQ29cos(A)             cos(A)
#define _IQ28cos(A)             cos(A)
#define _IQ27cos(A)             cos(A)
#define _IQ26cos(A)             cos(A)
#define _IQ25cos(A)             cos(A)
#define _IQ24cos(A)             cos(A)
#define _IQ23cos(A)             cos(A)
#define _IQ22cos(A)             cos(A)
#define _IQ21cos(A)             cos(A)
#define _IQ20cos(A)             cos(A)
#define _IQ19cos(A)             cos(A)
#define _IQ18cos(A)             cos(A)
#define _IQ17cos(A)             cos(A)
#define _IQ16cos(A)             cos(A)
#define _IQ15cos(A)             cos(A)
#define _IQ14cos(A)             cos(A)
#define _IQ13cos(A)             cos(A)
#define _IQ12cos(A)             cos(A)
#define _IQ11cos(A)             cos(A)
#define _IQ10cos(A)             cos(A)
#define _IQ9cos(A)              cos(A)
#define _IQ8cos(A)              cos(A)
#define _IQ7cos(A)              cos(A)
#define _IQ6cos(A)              cos(A)
#define _IQ5cos(A)              cos(A)
#define _IQ4cos(A)              cos(A)
#define _IQ3cos(A)              cos(A)
#define _IQ2cos(A)              cos(A)
#define _IQ1cos(A)              cos(A)
#define _IQcos(A)               cos(A)


#define _IQ30cosPU(A)           cos((A) * 6.283185307)
#define _IQ29cosPU(A)           cos((A) * 6.283185307)
#define _IQ28cosPU(A)           cos((A) * 6.283185307)
#define _IQ27cosPU(A)           cos((A) * 6.283185307)
#define _IQ26cosPU(A)           cos((A) * 6.283185307)
#define _IQ25cosPU(A)           cos((A) * 6.283185307)
#define _IQ24cosPU(A)           cos((A) * 6.283185307)
#define _IQ23cosPU(A)           cos((A) * 6.283185307)
#define _IQ22cosPU(A)           cos((A) * 6.283185307)
#define _IQ21cosPU(A)           cos((A) * 6.283185307)
#define _IQ20cosPU(A)           cos((A) * 6.283185307)
#define _IQ19cosPU(A)           cos((A) * 6.283185307)
#define _IQ18cosPU(A)           cos((A) * 6.283185307)
#define _IQ17cosPU(A)           cos((A) * 6.283185307)
#define _IQ16cosPU(A)           cos((A) * 6.283185307)
#define _IQ15cosPU(A)           cos((A) * 6.283185307)
#define _IQ14cosPU(A)           cos((A) * 6.283185307)
#define _IQ13cosPU(A)           cos((A) * 6.283185307)
#define _IQ12cosPU(A)           cos((A) * 6.283185307)
#define _IQ11cosPU(A)           cos((A) * 6.283185307)
#define _IQ10cosPU(A)           cos((A) * 6.283185307)
#define _IQ9cosPU(A)            cos((A) * 6.283185307)
#define _IQ8cosPU(A)            cos((A) * 6.283185307)
#define _IQ7cosPU(A)            cos((A) * 6.283185307)
#define _IQ6cosPU(A)            cos((A) * 6.283185307)
#define _IQ5cosPU(A)            cos((A) * 6.283185307)
#define _IQ4cosPU(A)            cos((A) * 6.283185307)
#define _IQ3cosPU(A)            cos((A) * 6.283185307)
#define _IQ2cosPU(A)            cos((A) * 6.283185307)
#define _IQ1cosPU(A)            cos((A) * 6.283185307)
#define _IQcosPU(A)             cos((A) * 6.283185307)


#define _IQ29acos(A)            acos(A)
#define _IQ28acos(A)            acos(A)
#define _IQ27acos(A)            acos(A)
#define _IQ26acos(A)            acos(A)
#define _IQ25acos(A)            acos(A)
#define _IQ24acos(A)            acos(A)
#define _IQ23acos(A)            acos(A)
#define _IQ22acos(A)            acos(A)
#define _IQ21acos(A)            acos(A)
#define _IQ20acos(A)            acos(A)
#define _IQ19acos(A)            acos(A)
#define _IQ18acos(A)            acos(A)
#define _IQ17acos(A)            acos(A)
#define _IQ16acos(A)            acos(A)
#define _IQ15acos(A)            acos(A)
#define _IQ14acos(A)            acos(A)
#define _IQ13acos(A)            acos(A)
#define _IQ12acos(A)            acos(A)
#define _IQ11acos(A)            acos(A)
#define _IQ10acos(A)            acos(A)
#define _IQ9acos(A)             acos(A)
#define _IQ8acos(A)             acos(A)
#define _IQ7acos(A)             acos(A)
#define _IQ6acos(A)             acos(A)
#define _IQ5acos(A)             acos(A)
#define _IQ4acos(A)             acos(A)
#define _IQ3acos(A)             acos(A)
#define _IQ2acos(A)             acos(A)
#define _IQ1acos(A)             acos(A)
#define _IQacos(A)              acos(A)


#define _IQ29atan(A)            atan(A)
#define _IQ28atan(A)            atan(A)
#define _IQ27atan(A)            atan(A)
#define _IQ26atan(A)            atan(A)
#define _IQ25atan(A)            atan(A)
#define _IQ24atan(A)            atan(A)
#define _IQ23atan(A)            atan(A)
#define _IQ22atan(A)            atan(A)
#define _IQ21atan(A)            atan(A)
#define _IQ20atan(A)            atan(A)
#define _IQ19atan(A)            atan(A)
#define _IQ18atan(A)            atan(A)
#define _IQ17atan(A)            atan(A)
#define _IQ16atan(A)            atan(A)
#define _IQ15atan(A)            atan(A)
#define _IQ14atan(A)            atan(A)
#define _IQ13atan(A)            atan(A)
#define _IQ12atan(A)            atan(A)
#define _IQ11atan(A)            atan(A)
#define _IQ10atan(A)            atan(A)
#define _IQ9atan(A)             atan(A)
#define _IQ8atan(A)             atan(A)
#define _IQ7atan(A)             atan(A)
#define _IQ6atan(A)             atan(A)
#define _IQ5atan(A)             atan(A)
#define _IQ4atan(A)             atan(A)
#define _IQ3atan(A)             atan(A)
#define _IQ2atan(A)             atan(A)
#define _IQ1atan(A)             atan(A)
#define _IQatan(A)              atan(A)


#define _IQ30atan2(A, B)        atan2(A, B)
#define _IQ29atan2(A, B)        atan2(A, B)
#define _IQ28atan2(A, B)        atan2(A, B)
#define _IQ27atan2(A, B)        atan2(A, B)
#define _IQ26atan2(A, B)        atan2(A, B)
#define _IQ25atan2(A, B)        atan2(A, B)
#define _IQ24atan2(A, B)        atan2(A, B)
#define _IQ23atan2(A, B)        atan2(A, B)
#define _IQ22atan2(A, B)        atan2(A, B)
#define _IQ21atan2(A, B)        atan2(A, B)
#define _IQ20atan2(A, B)        atan2(A, B)
#define _IQ19atan2(A, B)        atan2(A, B)
#define _IQ18atan2(A, B)        atan2(A, B)
#define _IQ17atan2(A, B)        atan2(A, B)
#define _IQ16atan2(A, B)        atan2(A, B)
#define _IQ15atan2(A, B)        atan2(A, B)
#define _IQ14atan2(A, B)        atan2(A, B)
#define _IQ13atan2(A, B)        atan2(A, B)
#define _IQ12atan2(A, B)        atan2(A, B)
#define _IQ11atan2(A, B)        atan2(A, B)
#define _IQ10atan2(A, B)        atan2(A, B)
#define _IQ9atan2(A, B)         atan2(A, B)
#define _IQ8atan2(A, B)         atan2(A, B)
#define _IQ7atan2(A, B)         atan2(A, B)
#define _IQ6atan2(A, B)         atan2(A, B)
#define _IQ5atan2(A, B)         atan2(A, B)
#define _IQ4atan2(A, B)         atan2(A, B)
#define _IQ3atan2(A, B)         atan2(A, B)
#define _IQ2atan2(A, B)         atan2(A, B)
#define _IQ1atan2(A, B)         atan2(A, B)
#define _IQatan2(A, B)          atan2(A, B)


#define _IQ30atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ29atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ28atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ27atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ26atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ25atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ24atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ23atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ22atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ21atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ20atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ19atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ18atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ17atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ16atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ15atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ14atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ13atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ12atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ11atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ10atan2PU(A, B)      (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ9atan2PU(A, B)       (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ8atan2PU(A, B)       (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ7atan2PU(A, B)       (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ6atan2PU(A, B)       (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ5atan2PU(A, B)       (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ4atan2PU(A, B)       (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ3atan2PU(A, B)       (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ2atan2PU(A, B)       (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQ1atan2PU(A, B)       (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))
#define _IQatan2PU(A, B)        (((atan2(A, B) * (1.0 / 6.283185307)) >=      \
                                  0.0) ?                                      \
                                 (atan2(A, B) * (1.0 / 6.283185307)) :        \
                                 (atan2(A, B) * (1.0 / 6.283185307) + 1.0))


#define _IQ30sqrt(A)            sqrt(A)
#define _IQ29sqrt(A)            sqrt(A)
#define _IQ28sqrt(A)            sqrt(A)
#define _IQ27sqrt(A)            sqrt(A)
#define _IQ26sqrt(A)            sqrt(A)
#define _IQ25sqrt(A)            sqrt(A)
#define _IQ24sqrt(A)            sqrt(A)
#define _IQ23sqrt(A)            sqrt(A)
#define _IQ22sqrt(A)            sqrt(A)
#define _IQ21sqrt(A)            sqrt(A)
#define _IQ20sqrt(A)            sqrt(A)
#define _IQ19sqrt(A)            sqrt(A)
#define _IQ18sqrt(A)            sqrt(A)
#define _IQ17sqrt(A)            sqrt(A)
#define _IQ16sqrt(A)            sqrt(A)
#define _IQ15sqrt(A)            sqrt(A)
#define _IQ14sqrt(A)            sqrt(A)
#define _IQ13sqrt(A)            sqrt(A)
#define _IQ12sqrt(A)            sqrt(A)
#define _IQ11sqrt(A)            sqrt(A)
#define _IQ10sqrt(A)            sqrt(A)
#define _IQ9sqrt(A)             sqrt(A)
#define _IQ8sqrt(A)             sqrt(A)
#define _IQ7sqrt(A)             sqrt(A)
#define _IQ6sqrt(A)             sqrt(A)
#define _IQ5sqrt(A)             sqrt(A)
#define _IQ4sqrt(A)             sqrt(A)
#define _IQ3sqrt(A)             sqrt(A)
#define _IQ2sqrt(A)             sqrt(A)
#define _IQ1sqrt(A)             sqrt(A)
#define _IQsqrt(A)              sqrt(A)


#define _IQ30isqrt(A)           (1.0 / sqrt(A))
#define _IQ29isqrt(A)           (1.0 / sqrt(A))
#define _IQ28isqrt(A)           (1.0 / sqrt(A))
#define _IQ27isqrt(A)           (1.0 / sqrt(A))
#define _IQ26isqrt(A)           (1.0 / sqrt(A))
#define _IQ25isqrt(A)           (1.0 / sqrt(A))
#define _IQ24isqrt(A)           (1.0 / sqrt(A))
#define _IQ23isqrt(A)           (1.0 / sqrt(A))
#define _IQ22isqrt(A)           (1.0 / sqrt(A))
#define _IQ21isqrt(A)           (1.0 / sqrt(A))
#define _IQ20isqrt(A)           (1.0 / sqrt(A))
#define _IQ19isqrt(A)           (1.0 / sqrt(A))
#define _IQ18isqrt(A)           (1.0 / sqrt(A))
#define _IQ17isqrt(A)           (1.0 / sqrt(A))
#define _IQ16isqrt(A)           (1.0 / sqrt(A))
#define _IQ15isqrt(A)           (1.0 / sqrt(A))
#define _IQ14isqrt(A)           (1.0 / sqrt(A))
#define _IQ13isqrt(A)           (1.0 / sqrt(A))
#define _IQ12isqrt(A)           (1.0 / sqrt(A))
#define _IQ11isqrt(A)           (1.0 / sqrt(A))
#define _IQ10isqrt(A)           (1.0 / sqrt(A))
#define _IQ9isqrt(A)            (1.0 / sqrt(A))
#define _IQ8isqrt(A)            (1.0 / sqrt(A))
#define _IQ7isqrt(A)            (1.0 / sqrt(A))
#define _IQ6isqrt(A)            (1.0 / sqrt(A))
#define _IQ5isqrt(A)            (1.0 / sqrt(A))
#define _IQ4isqrt(A)            (1.0 / sqrt(A))
#define _IQ3isqrt(A)            (1.0 / sqrt(A))
#define _IQ2isqrt(A)            (1.0 / sqrt(A))
#define _IQ1isqrt(A)            (1.0 / sqrt(A))
#define _IQisqrt(A)             (1.0 / sqrt(A))


#define _IQ30exp(A)             exp(A)
#define _IQ29exp(A)             exp(A)
#define _IQ28exp(A)             exp(A)
#define _IQ27exp(A)             exp(A)
#define _IQ26exp(A)             exp(A)
#define _IQ25exp(A)             exp(A)
#define _IQ24exp(A)             exp(A)
#define _IQ23exp(A)             exp(A)
#define _IQ22exp(A)             exp(A)
#define _IQ21exp(A)             exp(A)
#define _IQ20exp(A)             exp(A)
#define _IQ19exp(A)             exp(A)
#define _IQ18exp(A)             exp(A)
#define _IQ17exp(A)             exp(A)
#define _IQ16exp(A)             exp(A)
#define _IQ15exp(A)             exp(A)
#define _IQ14exp(A)             exp(A)
#define _IQ13exp(A)             exp(A)
#define _IQ12exp(A)             exp(A)
#define _IQ11exp(A)             exp(A)
#define _IQ10exp(A)             exp(A)
#define _IQ9exp(A)              exp(A)
#define _IQ8exp(A)              exp(A)
#define _IQ7exp(A)              exp(A)
#define _IQ6exp(A)              exp(A)
#define _IQ5exp(A)              exp(A)
#define _IQ4exp(A)              exp(A)
#define _IQ3exp(A)              exp(A)
#define _IQ2exp(A)              exp(A)
#define _IQ1exp(A)              exp(A)
#define _IQexp(A)               exp(A)


#define _IQ30exp2(A)            exp2(A)
#define _IQ29exp2(A)            exp2(A)
#define _IQ28exp2(A)            exp2(A)
#define _IQ27exp2(A)            exp2(A)
#define _IQ26exp2(A)            exp2(A)
#define _IQ25exp2(A)            exp2(A)
#define _IQ24exp2(A)            exp2(A)
#define _IQ23exp2(A)            exp2(A)
#define _IQ22exp2(A)            exp2(A)
#define _IQ21exp2(A)            exp2(A)
#define _IQ20exp2(A)            exp2(A)
#define _IQ19exp2(A)            exp2(A)
#define _IQ18exp2(A)            exp2(A)
#define _IQ17exp2(A)            exp2(A)
#define _IQ16exp2(A)            exp2(A)
#define _IQ15exp2(A)            exp2(A)
#define _IQ14exp2(A)            exp2(A)
#define _IQ13exp2(A)            exp2(A)
#define _IQ12exp2(A)            exp2(A)
#define _IQ11exp2(A)            exp2(A)
#define _IQ10exp2(A)            exp2(A)
#define _IQ9exp2(A)             exp2(A)
#define _IQ8exp2(A)             exp2(A)
#define _IQ7exp2(A)             exp2(A)
#define _IQ6exp2(A)             exp2(A)
#define _IQ5exp2(A)             exp2(A)
#define _IQ4exp2(A)             exp2(A)
#define _IQ3exp2(A)             exp2(A)
#define _IQ2exp2(A)             exp2(A)
#define _IQ1exp2(A)             exp2(A)
#define _IQexp2(A)              exp2(A)


#define _IQ30int(A)             ((long)(A))
#define _IQ29int(A)             ((long)(A))
#define _IQ28int(A)             ((long)(A))
#define _IQ27int(A)             ((long)(A))
#define _IQ26int(A)             ((long)(A))
#define _IQ25int(A)             ((long)(A))
#define _IQ24int(A)             ((long)(A))
#define _IQ23int(A)             ((long)(A))
#define _IQ22int(A)             ((long)(A))
#define _IQ21int(A)             ((long)(A))
#define _IQ20int(A)             ((long)(A))
#define _IQ19int(A)             ((long)(A))
#define _IQ18int(A)             ((long)(A))
#define _IQ17int(A)             ((long)(A))
#define _IQ16int(A)             ((long)(A))
#define _IQ15int(A)             ((long)(A))
#define _IQ14int(A)             ((long)(A))
#define _IQ13int(A)             ((long)(A))
#define _IQ12int(A)             ((long)(A))
#define _IQ11int(A)             ((long)(A))
#define _IQ10int(A)             ((long)(A))
#define _IQ9int(A)              ((long)(A))
#define _IQ8int(A)              ((long)(A))
#define _IQ7int(A)              ((long)(A))
#define _IQ6int(A)              ((long)(A))
#define _IQ5int(A)              ((long)(A))
#define _IQ4int(A)              ((long)(A))
#define _IQ3int(A)              ((long)(A))
#define _IQ2int(A)              ((long)(A))
#define _IQ1int(A)              ((long)(A))
#define _IQint(A)               ((long)(A))


#define _IQ30frac(A)            ((A) - (float)((long)(A)))
#define _IQ29frac(A)            ((A) - (float)((long)(A)))
#define _IQ28frac(A)            ((A) - (float)((long)(A)))
#define _IQ27frac(A)            ((A) - (float)((long)(A)))
#define _IQ26frac(A)            ((A) - (float)((long)(A)))
#define _IQ25frac(A)            ((A) - (float)((long)(A)))
#define _IQ24frac(A)            ((A) - (float)((long)(A)))
#define _IQ23frac(A)            ((A) - (float)((long)(A)))
#define _IQ22frac(A)            ((A) - (float)((long)(A)))
#define _IQ21frac(A)            ((A) - (float)((long)(A)))
#define _IQ20frac(A)            ((A) - (float)((long)(A)))
#define _IQ19frac(A)            ((A) - (float)((long)(A)))
#define _IQ18frac(A)            ((A) - (float)((long)(A)))
#define _IQ17frac(A)            ((A) - (float)((long)(A)))
#define _IQ16frac(A)            ((A) - (float)((long)(A)))
#define _IQ15frac(A)            ((A) - (float)((long)(A)))
#define _IQ14frac(A)            ((A) - (float)((long)(A)))
#define _IQ13frac(A)            ((A) - (float)((long)(A)))
#define _IQ12frac(A)            ((A) - (float)((long)(A)))
#define _IQ11frac(A)            ((A) - (float)((long)(A)))
#define _IQ10frac(A)            ((A) - (float)((long)(A)))
#define _IQ9frac(A)             ((A) - (float)((long)(A)))
#define _IQ8frac(A)             ((A) - (float)((long)(A)))
#define _IQ7frac(A)             ((A) - (float)((long)(A)))
#define _IQ6frac(A)             ((A) - (float)((long)(A)))
#define _IQ5frac(A)             ((A) - (float)((long)(A)))
#define _IQ4frac(A)             ((A) - (float)((long)(A)))
#define _IQ3frac(A)             ((A) - (float)((long)(A)))
#define _IQ2frac(A)             ((A) - (float)((long)(A)))
#define _IQ1frac(A)             ((A) - (float)((long)(A)))
#define _IQfrac(A)              ((A) - (float)((long)(A)))


#define _IQ30mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ29mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ28mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ27mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ26mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ25mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ24mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ23mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ22mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ21mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ20mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ19mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ18mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ17mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ16mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ15mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ14mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ13mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ12mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ11mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ10mpyIQX(A, IQA, B, IQB) ((A) * (B))
#define _IQ9mpyIQX(A, IQA, B, IQB)  ((A) * (B))
#define _IQ8mpyIQX(A, IQA, B, IQB)  ((A) * (B))
#define _IQ7mpyIQX(A, IQA, B, IQB)  ((A) * (B))
#define _IQ6mpyIQX(A, IQA, B, IQB)  ((A) * (B))
#define _IQ5mpyIQX(A, IQA, B, IQB)  ((A) * (B))
#define _IQ4mpyIQX(A, IQA, B, IQB)  ((A) * (B))
#define _IQ3mpyIQX(A, IQA, B, IQB)  ((A) * (B))
#define _IQ2mpyIQX(A, IQA, B, IQB)  ((A) * (B))
#define _IQ1mpyIQX(A, IQA, B, IQB)  ((A) * (B))
#define _IQmpyIQX(A, IQA, B, IQB)   ((A) * (B))


#define _IQ30mpyI32(A, B)       ((A) * (float)(B))
#define _IQ29mpyI32(A, B)       ((A) * (float)(B))
#define _IQ28mpyI32(A, B)       ((A) * (float)(B))
#define _IQ27mpyI32(A, B)       ((A) * (float)(B))
#define _IQ26mpyI32(A, B)       ((A) * (float)(B))
#define _IQ25mpyI32(A, B)       ((A) * (float)(B))
#define _IQ24mpyI32(A, B)       ((A) * (float)(B))
#define _IQ23mpyI32(A, B)       ((A) * (float)(B))
#define _IQ22mpyI32(A, B)       ((A) * (float)(B))
#define _IQ21mpyI32(A, B)       ((A) * (float)(B))
#define _IQ20mpyI32(A, B)       ((A) * (float)(B))
#define _IQ19mpyI32(A, B)       ((A) * (float)(B))
#define _IQ18mpyI32(A, B)       ((A) * (float)(B))
#define _IQ17mpyI32(A, B)       ((A) * (float)(B))
#define _IQ16mpyI32(A, B)       ((A) * (float)(B))
#define _IQ15mpyI32(A, B)       ((A) * (float)(B))
#define _IQ14mpyI32(A, B)       ((A) * (float)(B))
#define _IQ13mpyI32(A, B)       ((A) * (float)(B))
#define _IQ12mpyI32(A, B)       ((A) * (float)(B))
#define _IQ11mpyI32(A, B)       ((A) * (float)(B))
#define _IQ10mpyI32(A, B)       ((A) * (float)(B))
#define _IQ9mpyI32(A, B)        ((A) * (float)(B))
#define _IQ8mpyI32(A, B)        ((A) * (float)(B))
#define _IQ7mpyI32(A, B)        ((A) * (float)(B))
#define _IQ6mpyI32(A, B)        ((A) * (float)(B))
#define _IQ5mpyI32(A, B)        ((A) * (float)(B))
#define _IQ4mpyI32(A, B)        ((A) * (float)(B))
#define _IQ3mpyI32(A, B)        ((A) * (float)(B))
#define _IQ2mpyI32(A, B)        ((A) * (float)(B))
#define _IQ1mpyI32(A, B)        ((A) * (float)(B))
#define _IQmpyI32(A, B)         ((A) * (float)(B))


#define _IQ30mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ29mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ28mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ27mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ26mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ25mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ24mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ23mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ22mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ21mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ20mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ19mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ18mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ17mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ16mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ15mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ14mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ13mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ12mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ11mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ10mpyI32int(A, B)    ((long)((A) * (float)(B)))
#define _IQ9mpyI32int(A, B)     ((long)((A) * (float)(B)))
#define _IQ8mpyI32int(A, B)     ((long)((A) * (float)(B)))
#define _IQ7mpyI32int(A, B)     ((long)((A) * (float)(B)))
#define _IQ6mpyI32int(A, B)     ((long)((A) * (float)(B)))
#define _IQ5mpyI32int(A, B)     ((long)((A) * (float)(B)))
#define _IQ4mpyI32int(A, B)     ((long)((A) * (float)(B)))
#define _IQ3mpyI32int(A, B)     ((long)((A) * (float)(B)))
#define _IQ2mpyI32int(A, B)     ((long)((A) * (float)(B)))
#define _IQ1mpyI32int(A, B)     ((long)((A) * (float)(B)))
#define _IQmpyI32int(A, B)      ((long)((A) * (float)(B)))


#define _IQ30mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ29mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ28mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ27mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ26mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ25mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ24mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ23mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ22mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ21mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ20mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ19mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ18mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ17mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ16mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ15mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ14mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ13mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ12mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ11mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ10mpyI32frac(A, B)   (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ9mpyI32frac(A, B)    (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ8mpyI32frac(A, B)    (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ7mpyI32frac(A, B)    (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ6mpyI32frac(A, B)    (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ5mpyI32frac(A, B)    (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ4mpyI32frac(A, B)    (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ3mpyI32frac(A, B)    (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ2mpyI32frac(A, B)    (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQ1mpyI32frac(A, B)    (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))
#define _IQmpyI32frac(A, B)     (((A) * (B)) -                                \
                                 (float)((long)((A) * (float)(B))))


#define _IQ30mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ29mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ28mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ27mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ26mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ25mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ24mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ23mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ22mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ21mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ20mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ19mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ18mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ17mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ16mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ15mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ14mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ13mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ12mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ11mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ10mag(A, B)          sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ9mag(A, B)           sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ8mag(A, B)           sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ7mag(A, B)           sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ6mag(A, B)           sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ5mag(A, B)           sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ4mag(A, B)           sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ3mag(A, B)           sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ2mag(A, B)           sqrt(((A) * (A)) + ((B) * (B)))
#define _IQ1mag(A, B)           sqrt(((A) * (A)) + ((B) * (B)))
#define _IQmag(A, B)            sqrt(((A) * (A)) + ((B) * (B)))


#define _atoIQ30(A)             atof(A)
#define _atoIQ29(A)             atof(A)
#define _atoIQ28(A)             atof(A)
#define _atoIQ27(A)             atof(A)
#define _atoIQ26(A)             atof(A)
#define _atoIQ25(A)             atof(A)
#define _atoIQ24(A)             atof(A)
#define _atoIQ23(A)             atof(A)
#define _atoIQ22(A)             atof(A)
#define _atoIQ21(A)             atof(A)
#define _atoIQ20(A)             atof(A)
#define _atoIQ19(A)             atof(A)
#define _atoIQ18(A)             atof(A)
#define _atoIQ17(A)             atof(A)
#define _atoIQ16(A)             atof(A)
#define _atoIQ15(A)             atof(A)
#define _atoIQ14(A)             atof(A)
#define _atoIQ13(A)             atof(A)
#define _atoIQ12(A)             atof(A)
#define _atoIQ11(A)             atof(A)
#define _atoIQ10(A)             atof(A)
#define _atoIQ9(A)              atof(A)
#define _atoIQ8(A)              atof(A)
#define _atoIQ7(A)              atof(A)
#define _atoIQ6(A)              atof(A)
#define _atoIQ5(A)              atof(A)
#define _atoIQ4(A)              atof(A)
#define _atoIQ3(A)              atof(A)
#define _atoIQ2(A)              atof(A)
#define _atoIQ1(A)              atof(A)
#define _atoIQ(A)               atof(A)


#define _IQ30toa(A, B, C)       sprintf(A, B, C)
#define _IQ29toa(A, B, C)       sprintf(A, B, C)
#define _IQ28toa(A, B, C)       sprintf(A, B, C)
#define _IQ27toa(A, B, C)       sprintf(A, B, C)
#define _IQ26toa(A, B, C)       sprintf(A, B, C)
#define _IQ25toa(A, B, C)       sprintf(A, B, C)
#define _IQ24toa(A, B, C)       sprintf(A, B, C)
#define _IQ23toa(A, B, C)       sprintf(A, B, C)
#define _IQ22toa(A, B, C)       sprintf(A, B, C)
#define _IQ21toa(A, B, C)       sprintf(A, B, C)
#define _IQ20toa(A, B, C)       sprintf(A, B, C)
#define _IQ19toa(A, B, C)       sprintf(A, B, C)
#define _IQ18toa(A, B, C)       sprintf(A, B, C)
#define _IQ17toa(A, B, C)       sprintf(A, B, C)
#define _IQ16toa(A, B, C)       sprintf(A, B, C)
#define _IQ15toa(A, B, C)       sprintf(A, B, C)
#define _IQ14toa(A, B, C)       sprintf(A, B, C)
#define _IQ13toa(A, B, C)       sprintf(A, B, C)
#define _IQ12toa(A, B, C)       sprintf(A, B, C)
#define _IQ11toa(A, B, C)       sprintf(A, B, C)
#define _IQ10toa(A, B, C)       sprintf(A, B, C)
#define _IQ9toa(A, B, C)        sprintf(A, B, C)
#define _IQ8toa(A, B, C)        sprintf(A, B, C)
#define _IQ7toa(A, B, C)        sprintf(A, B, C)
#define _IQ6toa(A, B, C)        sprintf(A, B, C)
#define _IQ5toa(A, B, C)        sprintf(A, B, C)
#define _IQ4toa(A, B, C)        sprintf(A, B, C)
#define _IQ3toa(A, B, C)        sprintf(A, B, C)
#define _IQ2toa(A, B, C)        sprintf(A, B, C)
#define _IQ1toa(A, B, C)        sprintf(A, B, C)
#define _IQtoa(A, B, C)         sprintf(A, B, C)


#define _IQ30abs(A)             fabs(A)
#define _IQ29abs(A)             fabs(A)
#define _IQ28abs(A)             fabs(A)
#define _IQ27abs(A)             fabs(A)
#define _IQ26abs(A)             fabs(A)
#define _IQ25abs(A)             fabs(A)
#define _IQ24abs(A)             fabs(A)
#define _IQ23abs(A)             fabs(A)
#define _IQ22abs(A)             fabs(A)
#define _IQ21abs(A)             fabs(A)
#define _IQ20abs(A)             fabs(A)
#define _IQ19abs(A)             fabs(A)
#define _IQ18abs(A)             fabs(A)
#define _IQ17abs(A)             fabs(A)
#define _IQ16abs(A)             fabs(A)
#define _IQ15abs(A)             fabs(A)
#define _IQ14abs(A)             fabs(A)
#define _IQ13abs(A)             fabs(A)
#define _IQ12abs(A)             fabs(A)
#define _IQ11abs(A)             fabs(A)
#define _IQ10abs(A)             fabs(A)
#define _IQ9abs(A)              fabs(A)
#define _IQ8abs(A)              fabs(A)
#define _IQ7abs(A)              fabs(A)
#define _IQ6abs(A)              fabs(A)
#define _IQ5abs(A)              fabs(A)
#define _IQ4abs(A)              fabs(A)
#define _IQ3abs(A)              fabs(A)
#define _IQ2abs(A)              fabs(A)
#define _IQ1abs(A)              fabs(A)
#define _IQabs(A)               fabs(A)

#endif // MATH_TYPE == IQ_MATH


#ifdef __cplusplus
}
#endif

#endif // __IQMATH_RV_H__
