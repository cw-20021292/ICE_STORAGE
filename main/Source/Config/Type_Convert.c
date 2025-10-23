/// @file     Type_Convert.c
/// @date     2025/03/19
/// @author   Jaejin Ham
/// @brief    데이터 형태의 변환  함수  File


#include "Global_Header.h"


/// @brief      HEX to Decimal Type Convert
/// @details    HEX값을 Decimal 형태의 값으로 변경시킨다(예 : 0x11 -> 11)
/// @param      mu8HexData : HEX Data
/// @return     mu8ReturnData : DEC Data
U8 HEX2DEC(U8 mu8HexData)
{
  U8 mu8ReturnData = 0;

  mu8ReturnData = (((mu8HexData >> 4) & 0x0F) * 10) + (mu8HexData & 0x0F);

  return  mu8ReturnData;
}


/// @brief      Decimal to HEX Type Convert
/// @details    Decimal값을 HEX 형태의 값으로 변경시킨다(예 : 11 -> 0x11)
/// @param      mu8DecData : DEC Data
/// @return     mu8ReturnData : HEX Data
U8 DEC2HEX(U8 mu8DecData)
{
  U8 mu8ReturnData = 0;

  mu8ReturnData = (((mu8DecData / 10) << 4) & 0xF0) + (mu8DecData % 10);

  return  mu8ReturnData;
}


/// @brief      HEX to Decimal Type Convert
/// @details    HEX값을 Decimal 형태의 값으로 변경시킨다(예 : 0x1111 -> 1111)
/// @param      mu16HexData : HEX Data
/// @return     mu16ReturnData : DEC Data
U16 Bit16HEX2DEC(U16 mu16HexData)
{
  U16 mu16ReturnData = 0;

  mu16ReturnData = ((mu16HexData >> 12) & 0x000F) * 1000;
  mu16ReturnData += ((mu16HexData >> 8) & 0x000F) * 100;
  mu16ReturnData += ((mu16HexData >> 4) & 0x000F) * 10;
  mu16ReturnData += mu16HexData & 0x000F;

  return  mu16ReturnData;
}


/// @brief      Decimal to HEX Type Convert
/// @details    Decimal값을 HEX 형태의 값으로 변경시킨다(예 : 1111 -> 0x1111)
/// @param      mu16DecData : DEC Data
/// @return     mu16ReturnData : HEX Data
U16 DEC2Bit16HEX(U16 mu16DecData)
{
  U16 mu16ReturnData = 0;

  mu16ReturnData = ((mu16DecData / 1000) << 12) & 0xF000;
  mu16ReturnData += (((mu16DecData % 1000) / 100) << 8) & 0x0F00;
  mu16ReturnData += (((mu16DecData % 100) / 10) << 4) & 0x00F0;
  mu16ReturnData += (mu16DecData % 10) & 0x000F;

  return  mu16ReturnData;
}


/// @brief      Ascii to HEX Type Convert
/// @details    HEX값을 아스키 코드값으로 변경한다(예 : 0x01 -> 0x31)
/// @param      mu8HexData : Hex Data
/// @return     mu8ReturnData : Ascii Data
U8 HEX2ASCII(U8 mu8HexData)
{
  U8 mu8ReturnData = 0;

  if (mu8HexData < 0x0A)
  {
    mu8ReturnData = mu8HexData | 0x30;
  }
  else
  {
    mu8ReturnData = (mu8HexData - 9) | 0x40;
  }

  return  mu8ReturnData;
}


/// @brief      Ascii to HEX Type Convert
/// @details    아스키 코드값을 HEX값으로 변경한다(예 : 0x31 -> 0x01)
/// @param      mu8AsciiData : Ascii Data
/// @return     mu8ReturnData : Hex Data
U8 ASCII2HEX(U8 mu8AsciiData)
{
  U8 mu8ReturnData = 0;

  if (mu8AsciiData >= 0x41)
  {
    mu8ReturnData = (mu8AsciiData & ~0x40) + 9;
  }
  else
  {
    mu8ReturnData = mu8AsciiData & ~0x30;
  }

  return  mu8ReturnData;
}



