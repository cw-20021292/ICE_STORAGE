/**
 * File : rtc_comm.h
 * 
 * Rtc / Eeprom Comunitcation
*/
#ifndef __RTC_COMM_H__
#define	__RTC_COMM_H__

#include "prj_type.h"

/* Exported Definitions --------------------------------------------------------*/
#define DEV_ADDR_EEP	      (0xA0)  /* Device EEPROM - FM24C02C */
#define DEV_ADDR_RTC	      (0xDE)  /* Device RTC - ISL12026 */

#define EEP_MAX_SIZE		    (256)   /* bytes */
#define EEP_PAGE_SIZE	      (8)     /* 8-byte page write */
#define EEP_PAGE_NUM	      (32)    /* 256 bytes / 8 page per bytes = 32 pages */
#define RTC_PAGE_SIZE       (8)     /* 8-byte page write for rtc */

/**
  * @brief	: Writing a single byte.
  * @param	: U16 memory address, U8 data.
  * @retval : True or False.
  */
BOOL_T RTC_ByteWrite(U8 _dev, U8 _addr , U8 _data);

/**
  * @brief	: Writing as the page size.
  * @param	: U16 memory address, U8 data.
  * @retval : True or False.
  */
BOOL_T RTC_PageWrite(U8 _dev, U8 _addr , U8 *_data, U8 _len);

#if 0
/**
  * @brief	: Read the data from the current address.
  * @param	: U8 data.
  * @retval : True or False.
  */
BOOL_T RTC_CurrentAddrRead(U8 _dev, U8 *_data);
#endif

/**
  * @brief	: Read one byte.
  * @param	: U16 memory address, U8 * read data.
  * @retval : True or False.
  */
BOOL_T RTC_ByteRead(U8 _dev, U8 _addr , U8 *_data);

/**
  * @brief	: Read multiple bytes.
  * @param	: U16 memory address, U8 * read data, U8 read size.
  * @retval : True or False.
  */
BOOL_T RTC_SeqRead(U8 _dev, U8 _addr , U8 * _data, U8 _len);

/**
  * @brief	: EEPROM all erase.
  * @param	: None.
  * @retval : True or False.
  */
BOOL_T RTC_EEPROM_AllErase(void);

#endif /* __RTC_COMM_H__ */
