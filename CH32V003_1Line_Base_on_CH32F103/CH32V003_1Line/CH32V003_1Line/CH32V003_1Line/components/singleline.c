/********************************** (C) COPYRIGHT *******************************
 * File Name          : Singleline.c
 * Author             : RVMaking
 * Version            : V1.0.0
 * Date               : 2022/08/01
 * Description        : single line debug interface
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#include "singleline.h"



//T = 125ns
/* Medium speed mode (2x) - half the maximum value defined in the protocol  */
/* Timer delay and execution time need to be measured while  two waiting */
//Data 1
#define S2D1_L      (4)     // 232ns ≈ 2T      
#define S2D1_H      (2)     // 274ns ≈ 2T
//Data 0
#define S2D0_L      (42)    // 739ns ≈ 6T     
#define S2D0_H      (2)     // 274ns ≈ 2T     
#define S2STOP_H    (18*9)  //      
#define S2RD_H      (2)     // 274ns ≈ 2T    

/* Fast mode (1x) - half the maximum value defined in the protocol  */
//Data 1
#define S1D1_L      (2)     // 152ns     T
#define S1D1_H      (2)     // 274ns     T
//Data 0
#define S1D0_L      (26)    // 489ns     4T
#define S1D0_H      (2)     // 274ns     T
#define S1STOP_H    (10*9)  //      10T
#define S1RD_H      (2)     // 274ns      2T


uint8_t tD0_L=S2D0_L, tD0_H=S2D0_H;
uint8_t tD1_L=S2D1_L, tD1_H=S2D1_H;
uint8_t tRD_H=S2RD_H, tSTOP_H=S2STOP_H;
uint8_t DatBuf[42][2];



/*
0000001c <SHARE1>:
  1c:   7b251073            csrw    dscratch,a0
  20:   7b359073            csrw    0x7b3,a1
  24:   e0000537            lui a0,0xe0000
  28:   0f852583            lw  a1,248(a0) # e00000f8 <_eusrstack+0xbfff80f8>

0000002c <READ>:
  //8bit
  2c:   0005c583            lhu a1,0(a1)
  //16bit
  2c:   0005d583            lhu a1,0(a1)
  //32bit
  2c:   418c                lhu a1,0(a1)

  30:   0eb52a23            sw  a1,244(a0)

00000034 <SHARE2>:
  34:   7b202573            csrr    a0,dscratch
  38:   7b3025f3            csrr    a1,0x7b3
  //32bit
  00100073              ebreak
  //16bit- 8bit
  9002                  ebreak
*/
const uint32_t CH32V003_R_32bit[8]= { 0x7b251073, 0x7b359073, 0xe0000537, 0x0f852583, 0x2a23418c, 0x25730eb5, 0x25f37b20, 0x90027b30};
const uint32_t CH32V003_R_16bit[8]= { 0x7b251073, 0x7b359073, 0xe0000537, 0x0f852583, 0x0005d583, 0x0eb52a23, 0x7b202573, 0x7b3025f3};
const uint32_t CH32V003_R_8bit[8]= { 0x7b251073, 0x7b359073, 0xe0000537, 0x0f852583, 0x0005c583, 0x0eb52a23, 0x7b202573, 0x7b3025f3};

/*
0000001c <SHARE1>:
  1c:   7b251073            csrw    dscratch,a0
  20:   7b359073            csrw    0x7b3,a1
  24:   e0000537            lui a0,0xe0000
  28:   0f852583            lw  a1,248(a0) # e00000f8 <_eusrstack+0xbfff80f8>

0000002c <WRITE>:
  2c:   0f452503            lw  a0,244(a0)
  //32bit
  30:   c188                sw  a0,0(a1)
  //16bit
  30:   00a59023            sh  a0,0(a1)
  //8bit
  30:   00a58023            sb  a0,0(a1)

00000032 <SHARE2>:
  32:   7b202573            csrr    a0,dscratch
  36:   7b3025f3            csrr    a1,0x7b3

*/

const uint32_t CH32V003_W_32bit[8]= { 0x7b251073, 0x7b359073, 0xe0000537, 0x0f852583, 0x0f452503, 0x2573c188, 0x25f37b20, 0x90027b30};
const uint32_t CH32V003_W_16bit[8]= { 0x7b251073, 0x7b359073, 0xe0000537, 0x0f852583, 0x0f452503, 0x00a59023, 0x7b202573, 0x7b3025f3};
const uint32_t CH32V003_W_8bit[8]= { 0x7b251073, 0x7b359073, 0xe0000537, 0x0f852583, 0x0f452503, 0x00a58023, 0x7b202573, 0x7b3025f3};


/*
  1c:   e0000537            lui a0,0xe0000
  20:   0f450513            addi    a0,a0,244 # e00000f4 <_eusrstack+0xbfff80f4>
  24:   414c                    lw  a1,4(a0)
  26:   ffc5f693            andi    a3,a1,-4
  2a:   00d58563            beq a1,a3,34 <read>

0000002e <write>:
  2e:   4110                    lw  a2,0(a0)
  30:   c290                    sw  a2,0(a3)
  32:   a019                    j   38 <Auto_Increment>

00000034 <read>:
  34:   4290                    lw  a2,0(a3)
  36:   c110                    sw  a2,0(a0)

00000038 <Auto_Increment>:
  38:   0591                    addi    a1,a1,4
  3a:   c14c                    sw  a1,4(a0)
*/
/*CH8571 batch read and write only supports 32bit, the lower bit of the read address is 0, and the lower bit of the write address is 1 */
const uint32_t CH32V003_R_W_blk[8]= { 0xe0000537, 0x0f450513, 0xf693414c, 0x8563ffc5, 0x411000d5, 0xa019c290, 0xc1104290, 0xc14c0591};





//Single-line transmission data bit
uint8_t  DataBitSend( uint8_t *pbuf, uint16_t size )
{
    uint8_t  *p8 = pbuf;
    uint16_t  i = size;
    __disable_irq();
    DIO_OUT();
    do
    {
        SWDIO_Out_0();
        TIM1->CNT=0;
        while(TIM1->CNT < *p8);
        p8++;
        SWDIO_Out_1();
        TIM1->CNT=0;
        while(TIM1->CNT < *p8);
        p8++;
    }
    while(--i);
    __enable_irq();
    return 0;
}


//Single line receive data bit
uint32_t  DataBitRecv( uint8_t d1L, uint8_t d1H, uint8_t rdH )
{
    uint32_t   dat=0;  
    uint16_t   i = 32;

    __disable_irq();
    do
    {
        DIO_OUT();
        SWDIO_Out_0();
        dat <<= 1;
        TIM1->CNT=0;
        while( TIM1->CNT < d1L ){};

        DIO_IN();
//        TIM1->CNT=0;
//        while( TIM1->CNT < d1H ){};

        if( SWDIO_In( )==0 )
        {
            TIM1->CNT=0;
            while(TIM1->CNT < rdH);
        }
        else
        {
            dat ++;
        }
    }
    while(--i);

    __enable_irq();

    SWDIO_Out_1(); //Restore bus high
    DIO_OUT();

    return dat;
}


uint8_t StopBit( void )
{
    uint16_t   i = tSTOP_H;
    DIO_IN();
    TIM1->CNT=0;
    while(TIM1->CNT < i);
    return 0;
}


//Continue Bit Send
uint8_t  ContinueBitSend( uint32_t dat )
{
    uint8_t (*pbuf)[][2] = &DatBuf;
    uint8_t  i;

    (*pbuf)[0][0] = tD0_L;     // start bit
    (*pbuf)[0][1] = tD0_H;

    for(i=1; i<33; i++)
    {
        if( dat & 0x80000000 )
        {
            (*pbuf)[i][0] = tD1_L;
            (*pbuf)[i][1] = tD1_H;
        }
        else
        {
            (*pbuf)[i][0] = tD0_L;
            (*pbuf)[i][1] = tD0_H;
        }
        dat <<= 1;
    }

    DataBitSend( (uint8_t *)pbuf, 33 );
    StopBit( );
    return 0;
}

//Continue Bit Receive
uint8_t  ContinueBitRecv( uint32_t *pdat )
{
    uint8_t (*pbuf)[][2] = &DatBuf;

    (*pbuf)[0][0] = tD0_L;     // start bit
    (*pbuf)[0][1] = tD0_H;


    DataBitSend( (uint8_t *)pbuf, 1 );
    *pdat = DataBitRecv( tD1_L, tD1_H, tRD_H );
    StopBit( );
    return 0;
}

//Transfer one word at a time
//addr:DM register address
//dat:transmitted data
uint8_t DWordTransm( uint8_t addr, uint32_t dat )
{
    uint8_t (*pbuf)[][2] = &DatBuf;
    uint8_t  i;

    (*pbuf)[0][0] = tD1_L;     // start bit
    (*pbuf)[0][1] = tD1_H;

    addr = addr | 1;
    for(i=1; i<9; i++)
    {
        if( addr & 0x80 )
        {
            (*pbuf)[i][0] = tD1_L;
            (*pbuf)[i][1] = tD1_H;
        }
        else
        {
            (*pbuf)[i][0] = tD0_L;
            (*pbuf)[i][1] = tD0_H;
        }
        addr <<= 1;
    }
    for(i=9; i<41; i++)
    {
        if( dat & 0x80000000 )
        {
            (*pbuf)[i][0] = tD1_L;
            (*pbuf)[i][1] = tD1_H;
        }
        else
        {
            (*pbuf)[i][0] = tD0_L;
            (*pbuf)[i][1] = tD0_H;
        }
        dat <<= 1;
    }

    DataBitSend( (uint8_t *)pbuf, 41 );
    StopBit( );
    return 0;
}

//Receive one word at a time
//addr:DM register address
//*pdat:Pointer of input data
uint8_t DWordRecv( uint8_t addr, uint32_t *pdat )
{
    uint8_t (*pbuf)[][2] = &DatBuf;
    uint8_t  i;

    (*pbuf)[0][0] = tD1_L;     // start bit
    (*pbuf)[0][1] = tD1_H;

    for(i=1; i<9; i++)
    {
        if( addr & 0x80 )
        {
            (*pbuf)[i][0] = tD1_L;
            (*pbuf)[i][1] = tD1_H;
        }
        else
        {
            (*pbuf)[i][0] = tD0_L;
            (*pbuf)[i][1] = tD0_H;
        }
        addr <<= 1;
    }

    DataBitSend( (uint8_t *)pbuf, 9 );
    *pdat = DataBitRecv( tD1_L, tD1_H, tRD_H );
    StopBit( );

    return 0;
}

// Transfer a buffer
//addr:DM register address
//*databuf:Pointer of data
//*psize: data length
uint8_t BlockTransm( uint8_t addr, uint32_t *datbuf, uint16_t *psize )
{
    uint16_t  i, len = *psize;
    uint32_t *p32 = datbuf;

    DWordTransm( addr, *p32++ );
    *psize = 1;
    for( i=1; i<len; i++ )  ContinueBitSend( *p32++ );
    *psize = i;

    return 0;
}

//Receive and save datas in a buffer
//addr:DM register address
//*databuf:Pointer of  receive data
//*psize:length of receive data
uint8_t BlockRecv( uint8_t addr,  uint32_t *datbuf, uint16_t *psize )
{
    uint16_t  i, len = *psize;
    uint32_t *p32 = datbuf;

    DWordRecv( addr, p32++ );
    *psize = 1;
    for( i=1; i<len; i++)   ContinueBitRecv( p32++ );
    *psize = i;

    return 0;
}




/*************************************************************************************/
//Debugging interface timing selection: after debugging interface initialization, it defaults to 2x mode.
//When you need to switch the speed, first set the speed in 2x mode, and then switch to other corresponding speeds.
//It can not be modified normally
void DebugSpeedMode( uint8_t speed )
{
    switch( speed )
    {
    case 0:
        tD0_L = S1D0_L;
        tD0_H = S1D0_H;
        tD1_L = S1D1_L;
        tD1_H = S1D1_H;
        tRD_H = S1D1_H;
        tSTOP_H = S1RD_H;
        break;

    case 1:
        tD0_L = S2D0_L;
        tD0_H = S2D0_H;
        tD1_L = S2D1_L;
        tD1_H = S2D1_H;
        tSTOP_H = S2RD_H;
        tSTOP_H = S2STOP_H;
        break;

    default:
        tD0_L = S2D0_L;
        tD0_H = S2D0_H;
        tD1_L = S2D1_L;
        tD1_H = S2D1_H;
        tSTOP_H = S2RD_H;
        tSTOP_H = S2STOP_H;
        break;
    }
}


//Interface initialization
void ResetDeviceSWD( void )
{
    GPIO_InitTypeDef  GPIO_InitStructure= {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );

    //DIO
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	SWDIO_Out_1();
	Delay_Ms(5);
    SWDIO_Out_0();
    Delay_Ms(20);
    SWDIO_Out_1();
}


//note debug timebase is 8M
void TimeBase_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure={0};
    TIM_DeInit(TIM1);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 1-1;  //72M
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    TIM_Cmd(TIM1, ENABLE);
}


//Single-line initialization and timing setting
uint8_t SingleLineDebugIint(void)
{
    uint32_t  d;
    ResetDeviceSWD( );
	
    /* Enable slave output */
    DWordTransm( ONLINE_CFGR_SHAD, 0x5AA50400 );
    DWordTransm( ONLINE_CFGR_MASK, 0x5AA50400 );
    DWordRecv( ONLINE_CAPR_STA, &d);

    debug_log("CAPR_STA0:%08x\r\n", d);

    if( d != 0x00010403 )   return 1;
    return 0;
}


//Single-line query DMstatus register
//field:A field of DM status registers
//val:Judgment value
uint8_t SingleLineCheckDMSTA( uint32_t field,  uint32_t val)
{
    uint8_t    ack, i;
    uint32_t   dat;

    for(i=0; i<200; i++)
    {
        ack = DWordRecv( DEG_Module_STA, &dat );
        if(ack)  return ack;
        if( (dat!=0xFFFFFFFF)&&((dat & field) == val) )    break;
        Delay_Ms( 3 );
    }
    debug_log("DMSTATS:%08x \r\n",dat);
    if(i==200)   return 0xef;
    return 0;
}


//Single-line query abstract command status
//Check the abstract command status register to detect whether there are errors during the execution of the abstract command
uint8_t SingleLineCheckAbstSTA( void )
{
    uint8_t    ack, i;
    uint32_t   dat=0;

    for(i=0; i<200; i++)
    {
        ack = DWordRecv( DEG_Abst_STA, &dat );
        if(ack)  return ack;
        if( (dat&(1<<12))==0 )
        {
            dat = (dat>>8)&0x07;      // check[10:8]&[12]
            if( dat != 0 )
            {
                ack = DWordTransm( DEG_Abst_STA, 0xffffffff );       // Clear error flag
                return  dat;
            }
            else  break;
        }
        Delay_Ms( 1 );
    }
    if(i==200)   return 0xef;
    return 0;
}


// SingleLine Check the core pause bit of the dmstatus register until the pause or timeout
uint8_t SingleLineCheckPauseContinue(void)
{
    uint8_t    i=0;
    uint32_t   dat;
    do
    {
        i++;
        DWordRecv( DEG_Module_STA, &dat );
        Delay_Ms(4);
        debug_log("exit:%08x\n",dat);
    }
    while(((dat&(0x3<<8)) != (0x3<<8)) && (i<200)  );   //Until Pause
    if(i>=200) return 1;

    return 0;
}


//Single-line read status
//Read DM status register value
uint32_t  SingleLineReadPauseS(void)
{
    uint32_t   dat=0;
    DWordRecv( DEG_Module_STA, &dat );
    return dat;
}


//Set Core pause Mode
//return: 0:success
//        other：failure
uint8_t SingleLineGoToPauseMode(void)
{
    uint8_t    ack;
    ack = SingleLineDebugIint();
    if(ack)   return ack;
    ack = DWordTransm( DEG_Module_CTRL, 0x80000001 );
    if(ack)  return ack;
    ack = DWordTransm( DEG_Module_CTRL, 0x80000001 );
    if(ack)  return ack;
    ack = SingleLineCheckDMSTA( (3<<8),  (3<<8));  // check[9:8]
    if(ack)  return ack;
    ack = DWordTransm( DEG_Module_CTRL, 0x00000001 );
    return ack;
}



// SingleLine Set Core exit pause Mode
//return: 0:success
//        other：failure
uint8_t SingleLineExitPauseMode(void)
{
    uint8_t    ack;


    ack = DWordTransm( DEG_Module_CTRL, 0x80000001 );
    if(ack)
    {
      return ack;
    }
    ack = DWordTransm( DEG_Module_CTRL, 0x80000001 );

    if(ack)
    {
        return ack;
    }
    ack = DWordTransm( DEG_Module_CTRL, 0x00000001 );

    if(ack)
    {
        return ack;
    }
    ack = DWordTransm( DEG_Module_CTRL, 0x40000001 );
    if(ack)
    {
       return ack;
    }
    ack = SingleLineCheckDMSTA( (3<<10),  (3<<10)); // check[17:16]
    return ack;
}


//SingleLine Set Core Reset
//return: 0:success
//        other：failure
uint8_t SingleLineCoreReset(void)
{
    uint8_t    ack;

    ack = DWordTransm( DEG_Module_CTRL, 0x00000003 );
    if(ack)  return ack;
    ack = SingleLineCheckDMSTA( (3<<18),  (3<<18));  // check[19:18]
    if(ack)  return ack;
    ack = DWordTransm( DEG_Module_CTRL, 0x00000001 );
    if(ack)  return ack;
    ack = DWordTransm( DEG_Module_CTRL, 0x10000001 );
    if(ack)  return ack;
    ack = SingleLineCheckDMSTA( (3<<18),  0);  // check[19:18]
    return ack;
}

//SingleLine set DebugM reset
//return: 0:success
//        other：failure
uint8_t SingleLineDebugMReset(void)
{
    uint8_t    ack;
    uint32_t   dat;

    ack = DWordTransm( DEG_Module_CTRL, 0x00000003 );
    if(ack)  return ack;
    ack = DWordRecv( DEG_Module_CTRL, &dat );
    if(ack)  return ack;
    if(dat!=0x00000003)     return 1;
    ack = DWordTransm( DEG_Module_CTRL, 0x00000002 );
    if(ack)  return ack;
    ack = DWordRecv( DEG_Module_CTRL, &dat );
    if(ack)  return ack;
    if((dat&0x02)!=0)       return 2;
    return ack;
}

//SingleLine Read the value of core general register 
//gpregnum:general register number 0-31 : x0-x31
//*pd：the read value of general register
//ack: 0:success other：failure
uint8_t SingleLineReadGPR( uint8_t gpregnum, uint32_t *pd )
{
    uint8_t    ack;

    ack = DWordTransm( DEG_Abst_CMD, 0x00221000|gpregnum );
    if(ack)  return ack;
    ack = SingleLineCheckAbstSTA();
    if(ack)  return ack;
    ack = DWordRecv( DEG_Abst_DATA0, pd );
    return ack;
}

//SingleLine write value to core general register
//gpregnum:general register number:0-31 : x0-x31
//d：the write value of general register
//ack: 0:success other：failure
uint8_t SingleLineWriteGPR( uint8_t gpregnum, uint32_t d )
{
    uint8_t    ack;

    ack = DWordTransm( DEG_Abst_DATA0, d );
    if(ack)  return ack;
    ack = DWordTransm( DEG_Abst_CMD, 0x00231000|gpregnum );
    if(ack)  return ack;
    ack = SingleLineCheckAbstSTA();
    return ack;
}


//SingleLine read core's CSR register
//csr:address of CSR register，eg:Address of dpc register is 0x7b1
//*pd：Pointer  of  returned Data 
//ack: 0:success other：failure
uint8_t SingleLineReadCSR( uint16_t csr, uint32_t *pd )
{
    uint8_t    ack;

    ack = DWordTransm( DEG_Abst_CMD, 0x00220000|csr );
    if(ack)  return ack;
    ack = SingleLineCheckAbstSTA();
    if(ack)  return ack;
    ack = DWordRecv( DEG_Abst_DATA0, pd );
    return ack;
}

// SingleLine Write core CSR register
//csr:Address of CSR register ,eg:Address of dpc register is0x7b1
//d：the value of write in CSR register 
//ack: 0:succcess other：failure
uint8_t SingleLineWriteCSR( uint16_t csr, uint32_t d )
{
    uint8_t    ack;

    ack = DWordTransm( DEG_Abst_DATA0, d );
    if(ack)  return ack;
    ack = DWordTransm( DEG_Abst_CMD, 0x00230000|csr );
    if(ack)  return ack;
    ack = SingleLineCheckAbstSTA();
    return ack;
}


// SingleLine Single read memory or MCU register 
//addr:Address to read data
//*pd:Point of read data
//size:Bit width of read operation    DEG_RWSize_32：32bit of read operation，DEG_RWSize_16：16bit of read operation，AHB_CH5871_R_8bit：8bit of read operation
//ack: 0:success other：failure
uint8_t SingleLineReadMEMDirect( uint32_t addr, uint32_t *pd, uint32_t size )
{
    uint32_t const *p32;
    uint8_t    ack, i;

SigRD_Retry:
    ack = DWordTransm( DEG_Abst_DATA1, addr );
    if(ack)  return ack;
    if( size == DEG_RWSize_32 ) p32 = CH32V003_R_32bit;
    else if( size == DEG_RWSize_16 )    p32 = CH32V003_R_16bit;
    else    p32 = CH32V003_R_8bit;
    for ( i=0; i<8; i++ )   ack = DWordTransm( DEG_Prog_BUF0 + (i<<1), *p32++ );
    ack = DWordTransm( DEG_Abst_CMD, (1<<18) );/* execute pro buf */
    if(ack)  return ack;

    ack = SingleLineCheckAbstSTA();
    i=0;
    if(ack)
    {
        if(i<5)
        {
            i++;
            goto SigRD_Retry;
        }
        else
        {
            return ack;
        }
    }

    ack = DWordRecv( DEG_Abst_DATA0, pd );
    return ack;
}


//SingleLine continuously reads * psize length data from the addr start address in words to the buffer
//addr:Start address of reading data  4-byte alignment
//*pseze: Data Length of read
//*pdbuf:Buffer pointer
//ack: 0:success other：failure
uint8_t SingleLineReadMEMDirectBlock( uint32_t addr, uint16_t *psize, uint32_t *pdbuf)
{
    uint32_t const *p32;
    uint8_t    ack, i;
    uint16_t   len = *psize-1;

SigRDBlock_Retry:
    ack = DWordTransm( DEG_Abst_DATA1, addr );
    if(ack)  return ack;
    if( len == 0 )
    {
        p32 = CH32V003_R_32bit;
        for ( i=0; i<8; i++ )   ack = DWordTransm( DEG_Prog_BUF0 + (i<<1), *p32++ );
        ack = DWordTransm( DEG_Abst_CMD, (1<<18) );/* execute pro buf */
        if(ack)  return ack;

        ack = SingleLineCheckAbstSTA();
        i=0;
        if(ack)
        {
            if(i<5)
            {
                i++;
                goto SigRDBlock_Retry;
            }
            else
            {
                return ack;
            }
        }

        ack = DWordRecv( DEG_Abst_DATA0, pdbuf );
        return ack;
    }
    else
    {
        ack = DWordTransm( DEG_Abst_CMDAUTO, 0x00000001 );
        if(ack)  return ack;
        p32 = CH32V003_R_W_blk;
        for ( i=0; i<8; i++ )   DWordTransm( DEG_Prog_BUF0 + (i<<1), *p32++ );
        ack = DWordTransm( DEG_Abst_CMD, (1<<18) );/* execute pro buf */
        if(ack)  return ack;
        ack = BlockRecv( DEG_Abst_DATA0, pdbuf, &len );
        if(ack)  return ack;
        *psize = len;
        ack = DWordTransm( DEG_Abst_CMDAUTO, 0x00000000 );
        if(ack)  return ack;


        ack = SingleLineCheckAbstSTA();
        i=0;
        if(ack)
        {
            if(i<5)
            {
                i++;
                goto SigRDBlock_Retry;
            }
            else
            {
                return ack;
            }
        }


        ack = DWordRecv( DEG_Abst_DATA0, pdbuf+len );
        if(ack)  return ack;
        *psize = len+1;
        return ack;
    }
}

//Single Line Write memory or MCU register once
//addr:Address of write data
//d：Data of Write
//size:Bit width of read operation    DEG_RWSize_32：32 bit of read operation，DEG_RWSize_16：16bit of read operation，AHB_CH5871_R_8bit：8bit of read operation
//ack: 0:success other：failure
uint8_t SingleLineWriteMEMDirect( uint32_t addr, uint32_t d, uint32_t size )
{
    uint32_t const *p32;
    uint8_t    ack, i;

    ack = DWordTransm( DEG_Abst_DATA0, d );
    if(ack)  return ack;
    ack = DWordTransm( DEG_Abst_DATA1, addr );
    if(ack)  return ack;
    if( size == DEG_RWSize_32 ) p32 = CH32V003_W_32bit;
    else if( size == DEG_RWSize_16 )    p32 = CH32V003_W_16bit;
    else    p32 = CH32V003_W_8bit;
    for ( i=0; i<8; i++ )   ack = DWordTransm( DEG_Prog_BUF0 + (i<<1), *p32++ );
    ack = DWordTransm( DEG_Abst_CMD, (1<<18) );/* execute pro buf */
    if(ack)  return ack;
    ack = SingleLineCheckAbstSTA();
    if(ack)  return ack;


    return ack;



}


//SingleLine Write pdbuf data into memory or register address by word, and the address increases automatically
//addr: Address of write start  4-byte alignment
//*pseze:data length 
//*pdbuf:Pointer of Buffer 
//ack: 0:success other：failure
uint8_t SingleLineWriteMEMDirectBlock( uint32_t addr, uint16_t *psize, uint32_t *pdbuf )
{
    uint32_t const *p32;
    uint8_t    ack, i;
    uint16_t   len = *psize-1;



    ack = DWordTransm( DEG_Abst_DATA0, *pdbuf );
    if(ack)  return ack;
    *psize = 1;
    if(len == 0)
    {
        ack = DWordTransm( DEG_Abst_DATA1, addr );
        if(ack)  return ack;
        p32 = CH32V003_W_32bit;
        for ( i=0; i<8; i++ )   ack = DWordTransm( DEG_Prog_BUF0 + (i<<1), *p32++ );
        ack = DWordTransm( DEG_Abst_CMD, (1<<18) );/* execute pro buf */
        if(ack)  return ack;
        ack = SingleLineCheckAbstSTA();
        if(ack)  return ack;
    }
    else
    {
        ack = DWordTransm( DEG_Abst_DATA1, addr|1 );
        if(ack)  return ack;
        p32 = CH32V003_R_W_blk;
        for ( i=0; i<8; i++ )   ack = DWordTransm( DEG_Prog_BUF0 + (i<<1), *p32++ );
        ack = DWordTransm( DEG_Abst_CMD, (1<<18) );/* execute pro buf */
        if(ack)  return ack;

        ack = SingleLineCheckAbstSTA();
        if(ack)  return ack;

        ack = DWordTransm( DEG_Abst_CMDAUTO, 0x00000001 );
        if(ack)  return ack;
        ack = BlockTransm( DEG_Abst_DATA0, pdbuf+1, &len );
        if(ack)  return ack;
        *psize = len+1;
        ack = DWordTransm( DEG_Abst_CMDAUTO, 0x00000000 );
        if(ack)  return ack;

    }
    return ack;
}




















