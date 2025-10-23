/**
 * File : rtc_comm.c
 * 
 * Rtc / Eeprom Comunitcation
*/
#include "rtc_comm.h"

#include "util.h"

#include "api_i2c.h"

/**
  * @brief	: Writing a single byte.
  * @param	: U16 memory address, U8 data.
  * @retval : True or False.
  */
BOOL_T RTC_ByteWrite(U8 _dev, U8 _addr , U8 _data)
{
    return I2ca1ByteWrite8bit(_dev, _addr, _data);
}

/**
  * @brief	: Writing as the page size.
  * @param	: U16 memory address, U8 data.
  * @retval : True or False.
  */
BOOL_T RTC_PageWrite(U8 _dev, U8 _addr , U8 *_data, U8 _len)
{
    return I2ca1PageWrite8bit(_dev, _addr, _data, _len);
}

#if 0
/**
  * @brief	: Read the data from the current address.
  * @param	: U8 data.
  * @retval : True or False.
  */
BOOL_T RTC_CurrentAddrRead(U8 _dev, U8 *_data)
{
    
}
#endif

/**
  * @brief	: Read one byte.
  * @param	: U16 memory address, U8 * read data.
  * @retval : True or False.
  */
BOOL_T RTC_ByteRead(U8 _dev, U8 _addr , U8 *_data)
{
    return I2ca1ByteRead8bit(_dev, _addr, _data);
}

/**
  * @brief	: Read multiple bytes.
  * @param	: U16 memory address, U8 * read data, U8 read size.
  * @retval : True or False.
  */
BOOL_T RTC_SeqRead(U8 _dev, U8 _addr , U8 * _data, U8 _len)
{
    return I2ca1SeqRead8bit(_dev, _addr, _data, _len);
}

/**
  * @brief	: EEPROM all erase.
  * @param	: None.
  * @retval : True or False.
  */
BOOL_T RTC_EEPROM_AllErase(void)
{
    U8 buf[ EEP_PAGE_SIZE + 1 ];
    U8 i;
    U8 addr;

    MEMSET( (void __FAR *)&buf[ 1 ], 0xFF, EEP_PAGE_SIZE );

    for( i=0; i<EEP_PAGE_NUM; i++ )
    {
        addr = (i * EEP_PAGE_SIZE);
        buf[0] = addr;

        I2ca1PageWriteWeak8bit(DEV_ADDR_EEP, &buf[0], EEP_PAGE_SIZE);
    }

    I2ca1PageStopWeak8bit();
    return TRUE;
}
