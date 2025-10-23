/**
 * File : util.c
 * 
 * Util Function
*/
#include "mcu.h"
#include "util.h"
#include "hw.h"

/**
 * @brief              Function For Change Hex to Decimal
 * @detail             none
 * @param     xUhex    Hex Data
 * @return             Decimal Value
**/
U8 Hex2Dec(U8 hex)
{
    U8 uDec;

    uDec = (hex & 0x0F);
    uDec += ((hex & 0xF0) >> 4) * 10;;

    return uDec;
}

/**
 * @brief              Function For Change Decimal to Hex
 * @detail             none
 * @param     xUdec    Decimal Data
 * @return             Hex Value
**/
U8 Dec2Hex(U8 dec)
{
    U8 uHex;

    uHex = dec % 10;
    uHex += (dec / 10) * 16;

    return uHex;
}

/**
 * @brief                Function For Change Ascil to Byte
 * @detail               none
 * @param     xUupper    Ascil Upper Data
 * @param     xUlower    Ascil Lower Data
 * @return               Byte Value
**/
U8 ConvAsc2Byte(U8 upper, U8 lower)
{
    U8 temp = 0;
    U8 val = 0;

    // Conv upper byte
    if( upper > '9' )
    {
        temp = upper - '7';
    }
    else 
    {
        temp = upper - '0';
    }
    val = temp * 16;

    // Conv lower byte
    if( lower > '9' )
    {
        temp = lower - '7';
    }
    else 
    {
        temp = lower - '0';
    }
    val += temp;

    return val;
}

/**
 * @brief             Function For Delay System
 * @detail            System Shut Down During Time
 * @param     xUus    Time ( Unit : us )
 * @return            none
**/
void Delay_US(U8 us)
{
    while( us-- )
    {
        NOP();
        NOP();
        NOP();
    }
}

/**
 * @brief             Function For Delay System
 * @detail            System Shut Down During Time
 * @param     xUus    Time ( Unit : ms )
 * @return            none
**/
void Delay_MS(U16 ms)
{
    U16 ui;
    U8  uj;

    for( ui = 0 ; ui < ms ; ui++ )
    {
        for( uj = 0; uj < 4 ; uj++ )   // 1ms loop delay
        {
            Delay_US(250);
            R_WDT_Restart();
        }
    }
}


/**
 * @brief             Function For ByteOrder
 * @detail            none
 * @param     xUsrc   Source Data
 * @return            Changed Data
**/
U16 ConvByteOrder(U16 src)
{
    U16 h_to_l = src >> 8;
    U16 l_to_h = ( src & 0x00FF ) << 8;

    return ( l_to_h | h_to_l );
}

/**
 * @brief        Function For Reset System
 * @detail       System Reset using WatchDog Reset
 * @param        none
 * @return       none
**/
void Reset(void)
{
    while(1)
    {
        ; // wait.. watchdog reset
    }
}

/**
 * @brief             Function For Limit Value
 * @detail            min, max�?초과?�는 경우 Init Value�?반환?�다. 그렇지 ?��? 경우 mu8Val�?return?�로 반환?�다.
 * @param     xUval   none
 * @param     xUmin   none
 * @param     xUmax   none
 * @param     xUinit  none
 * @return            Original Value or Inital Value
**/
U8 CheckLimitValue(U8 val, U8 min, U8 max, U8 init)
{
    if( val < min || val > max  )
    {
        return init;
    }

    return val;
}

/**
 * @brief              Function For Get Min Value
 * @detail             none
 * @param     xDataX   Data1
 * @param     xDataY   Data2
 * @return             Min Value
**/
I16 GetMin(I16 dataX, I16 dataY)
{
    if( dataX > dataY )
    {
        return dataY;
    }

    return dataX;
}

/**
 * @brief              Function For Get Min Value
 * @detail             none
 * @param     xDataX   Data1
 * @param     xDataY   Data2
 * @param     xDataZ   Data3
 * @return             Min Value
**/
I16 GetMin3(I16 dataX, I16 dataY, I16 dataZ)
{
    I16 min = 0;

    min = dataX;

    if( min > dataY )
    {
        min = dataY;
    }

    if( min > dataZ )
    {
        min = dataZ;
    }

    return min;
}


/**
 * @brief              Function For Get Max Value 
 * @detail             none
 * @param     xDataX   Data1
 * @param     xDataY   Data2
 * @return             Max Value
**/
I16	GetMax(I16 dataX, I16 dataY)
{
    if( dataX > dataY )
    {
        return dataX;
    }

    return dataY;
}

/**
 * @brief             Function For 
 * @detail            Min or Max �?벗어?�면 Min or Max 값을 취함
 * @param     xUval   none
 * @param     xUmin   none
 * @param     xUmax   none
 * @return            Value
**/
U8 SetMinMax8(U8 val, U8 min, U8 max)
{
    if( val <= min )
    {
        val = min;
    }
    else if( val >= max )
    {
        val = max;
    }

    return val;
}

/**
 * @brief             Function For 
 * @detail            Min or Max �?벗어?�면 Min or Max 값을 취함
 * @param     xUval   none
 * @param     xUmin   none
 * @param     xUmax   none
 * @return            Value
**/
U16 SetMinMax16(U16 val, U16 min, U16 max)
{
    if( val <= min )
    {
        val = min;
    }
    else if( val >= max )
    {
        val = max;
    }

    return val;
}

/**
 * @brief             Function For 
 * @detail            Min or Max �?벗어?�면 Min or Max 값을 취함
 * @param     xVal    none
 * @param     xVal    none
 * @param     xMax    none
 * @return            Value
**/
I16 SetMinMaxI16(I16 val, I16 min, I16 max)
{
    if( val <= min )
    {
        val = min;
    }
    else if( val >= max )
    {
        val = max;
    }

    return val;
}

/**
 * @brief             Function For 
 * @detail            Min or Max �?벗어?�면 Min or Max 값을 취함
 * @param     xUval   none
 * @param     xUmin   none
 * @param     xUmax   none
 * @return            Value
**/
U32 SetMinMax32(U32 val, U32 min, U32 max)
{
    if( val <= min )
    {
        val = min;
    }
    else if( val >= max )
    {
        val = max;
    }

    return val;
}

/**
 * @brief             Function For 
 * @detail            Min or Max �?벗어?�면 Min or Max 값을 취함
 * @param     xFval   none
 * @param     xFmin   none
 * @param     xFmax   none
 * @return            Value
**/
F32 SetMinMaxF32(F32 val, F32 min, F32 max)
{
    if( val <= min )
    {
        val = min;
    }
    else if( val >= max )
    {
        val = max;
    }

    return val;
}

I32 Absolute(I32 num)
{
    I32 data = 0;

    if ( num < 0 )
    {  
        data = -num;
    }
    else
    {
        data = num;
    }

    return data;
}

void SetBit(U8 *pBitData, U8 pos)
{
    *pBitData |= ((U8)(1U)<<pos); 
}

void ClearBit(U8 *pBitData, U8 pos)
{
    *pBitData &= ~((U8)(1U)<<pos); 
}

U8 GetBit(U8 *pBitData, U8 pos)
{
    U8 uData = 0U;

    uData = *pBitData & ((U8)(1U)<<pos);
    uData = uData >> pos;

    return uData;
}
