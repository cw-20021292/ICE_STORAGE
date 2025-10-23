/**
 * File : api_i2c.h
 * 
 * Application Programming Interface
 * Depend on HAL
**/
#ifndef __API_I2C_H__
#define __API_I2C_H__

#include "prj_type.h"

typedef struct _i2c_
{
    /* READ/WRITE STATUS */
    U8 WriteProc; 
    U8 ReadProc;

    U32 ReadFail;
    U32 ReadOk;

    U32 WriteFail;
    U32 WriteOk;
} SI2c_T;

/* I2CA1 */
// Buf
#define I2CA0_BUF_SIZE      (16)    // 16byte For Eeeprom/rtc ( Page Unit )

// Status
#define I2CA0_PROC_START		(0U)
#define I2CA0_PROC_DONE		  (1U)



/* I2CA1 */
#define I2CA1_BUF_SIZE      (8)    // 8byte For Eeeprom/rtc ( Page Unit )

// Status
#define I2CA1_PROC_START		(0U)
#define I2CA1_PROC_DONE		  (1U)

void InitI2c(void);

void I2ca0SetWriteProc(U8 _proc);
U8	I2ca0GetWriteProc(void);
void I2ca0SetReadProc(U8 _proc );
U8 I2ca0GetReadProc(void);

BOOL_T I2ca0ByteWrite8bit(U8 _dev, U8 _addr, U8 _data);
BOOL_T I2ca0ByteWrite16bit(U8 _dev, U16 _addr, U8 _data);
U8 I2ca0PageWrite8bit(U8 _dev, U8 _addr, U8 *_data, U8 _len);
U8 I2ca0PageWrite16bit(U8 _dev, U16 _addr, U8 *_data, U8 _len);
BOOL_T I2ca0ByteRead8bit(U8 _dev, U8 _addr, U8 *_data);
BOOL_T I2ca0ByteRead16bit(U8 _dev, U16 _addr, U8 *_data);
BOOL_T	I2ca0SeqRead8bit(U8 _dev, U8 _addr, U8 *_data, U8 _len);
BOOL_T	I2ca0SeqRead16bit(U8 _dev, U16 _addr, U8 * _data, U8 _len);

void I2ca1SetWriteProc(U8 _proc);
U8	I2ca1GetWriteProc(void);
void I2ca1SetReadProc(U8 _proc);
U8 I2ca1GetReadProc(void);

BOOL_T I2ca1ByteWrite8bit(U8 _dev, U8 _addr, U8 _data);
BOOL_T I2ca1ByteWrite16bit(U8 _dev, U16 _addr, U8 _data);
U8 I2ca1PageWrite8bit(U8 _dev, U8 _addr, U8 *_data, U8 _len);
U8 I2ca1PageWrite16bit(U8 _dev, U16 _addr, U8 *_data, U8 _len);
BOOL_T I2ca1PageWriteWeak8bit(U8 _dev, U8 *buf, U8 _len);
void	I2ca1PageStopWeak8bit(void);
BOOL_T I2ca1ByteRead8bit(U8 _dev, U8 _addr, U8 *_data);
BOOL_T I2ca1ByteRead16bit(U8 _dev, U16 _addr, U8 *_data);
BOOL_T	I2ca1SeqRead8bit(U8 _dev, U8 _addr, U8 *_data, U8 _len);
BOOL_T	I2ca1SeqRead16bit(U8 _dev, U16 _addr, U8 * _data, U8 _len);

#endif  /* __API_I2C_H__ */
