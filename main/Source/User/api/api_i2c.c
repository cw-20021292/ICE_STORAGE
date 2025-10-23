/**
 * File : api_i2c.c
 * 
 * Application Programming Interface
 * Depend on HAL
*/
#include "hw.h"
#include "r_cg_serial.h"

#include "hal_i2c.h"
#include "api_i2c.h"

#include "util.h"

SI2c_T i2ca0;
SI2c_T i2ca1;

#define CONFIG_USE_I2CA0          (0)
#define CONFIG_USE_I2CA1          (1)

#define I2C_OP_WR                 (0x00)
#define I2C_OP_RD                 (0x01)

#define I2C_DEFAULT_DELAY         (50)
#define I2C_DEFAULT_RETRY_COUNT   (3)

void InitI2c(void)
{
    MEMSET( (void __FAR *)&i2ca0, 0x00, sizeof( SI2c_T ) );
    MEMSET( (void __FAR *)&i2ca1, 0x00, sizeof( SI2c_T ) );
}

/**
 * I2CA0
**/
void I2ca0SetWriteProc(U8 _proc)
{
    i2ca0.WriteProc = _proc;
}


U8	I2ca0GetWriteProc(void)
{
    return i2ca0.WriteProc;
}

static BOOL_T	I2ca0WriteProcComplete(void)
{
    U16	wait = 10000U;

    do
    {
        if( wait > 0U )
            wait--;
        else
            return FALSE;
    }
    while( !(I2ca0GetWriteProc() == I2CA0_PROC_DONE) );

    return TRUE;
}

void I2ca0SetReadProc(U8 _proc)
{
    i2ca0.ReadProc = _proc;
}

U8 I2ca0GetReadProc(void)
{
    return i2ca0.ReadProc;
}

static BOOL_T	I2ca0ReadProcComplete(void)
{
    U16	wait = 10000U;

    do
    {
        if( wait > 0U )
            wait--;
        else
            return FALSE;
    }
    while( !(I2ca0GetReadProc() == I2CA0_PROC_DONE) );

    return TRUE;
}

static void I2ca0CountReadError(void)
{
    i2ca0.ReadFail++;
}

static void I2ca0CountWriteError(void)
{
    i2ca0.WriteFail++;
}

static void I2ca0StopCondition(void)
{
#if( CONFIG_USE_I2CA0 == 1 )
    R_IICA0_StopCondition();
    //Delay_MS( DEFAULT_DELAY );
#endif
}

// Byte Write Addr 8bit / 16bit
static BOOL_T	I2ca0ByteWriteSub8bit(U8 _dev, U8 _addr, U8 _data)
{
    U8 buf[2] = {0U, };

    buf[0] = _addr;
    buf[1] = _data;

    I2ca0SetWriteProc(I2CA0_PROC_START);
    if( i2ca0_master_send(_dev | I2C_OP_WR, buf, 2U, 200U) != MD_OK)
    {
        I2ca0CountWriteError();
        I2ca0StopCondition();
        return FALSE;
    }

    if( !I2ca0WriteProcComplete() )
    {
        I2ca0CountWriteError();
        I2ca0StopCondition();
        return FALSE;
    }

    I2ca0StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    //Delay_MS(100);

    i2ca0.WriteOk++;
    return TRUE;
}

BOOL_T I2ca0ByteWrite8bit(U8 _dev, U8 _addr, U8 _data)
{
    U8 retryCount = I2C_DEFAULT_RETRY_COUNT;

    do
    {
        if( retryCount == 0U )
        {
            return FALSE;
        }
        retryCount--;

        RESET_WDT();

    }
    while( I2ca0ByteWriteSub8bit(_dev, _addr, _data) != TRUE );

    RESET_WDT();
    return TRUE;
}

static BOOL_T	I2ca0ByteWriteSub16bit(U8 _dev, U16 _addr, U8 _data)
{
    U8 buf[3] = {0U, };

    buf[0] = GET_16_HIGH_BYTE(_addr);
    buf[1] = GET_16_LOW_BYTE(_addr);
    buf[2] = _data;

    I2ca0SetWriteProc(I2CA0_PROC_START);
    if( i2ca0_master_send(_dev | I2C_OP_WR, buf, 3U, 200U) != MD_OK )
    {
        I2ca0CountWriteError();

        I2ca0StopCondition();
        return FALSE;
    }

    if( !I2ca0WriteProcComplete() )
    {
        I2ca0CountWriteError();
        I2ca0StopCondition();
        return FALSE;
    }

    I2ca0StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    //Delay_MS(100);

    i2ca0.WriteOk++;
    return TRUE;
}

BOOL_T I2ca0ByteWrite16bit(U8 _dev, U16 _addr, U8 _data)
{
    U8 retryCount = I2C_DEFAULT_RETRY_COUNT;

    do
    {
        if( retryCount == 0U )
        {
            return FALSE;
        }
        retryCount--;

        RESET_WDT();

    }
    while( I2ca0ByteWriteSub16bit(_dev, _addr, _data) != TRUE );

    RESET_WDT();
    return TRUE;
}

// Page Write Addr 8bit / 16bit
static BOOL_T	I2ca0PageWriteSub8bit(U8 _dev, U8 _addr, U8 *_data, U8 _len)
{
    U8	buf[I2CA0_BUF_SIZE + 1];
    U8	rbuf[I2CA0_BUF_SIZE];

    buf[0] = _addr;

    MEMCPY( (void __FAR *)&buf[1], (const void __FAR *)&_data[0], _len );

    I2ca0SetWriteProc(I2CA0_PROC_START);

    // Write Page..
    if( i2ca0_master_send(_dev | I2C_OP_WR, &buf[0], _len + 1U, 200U) != MD_OK )
    {
        I2ca0CountWriteError();
        I2ca0StopCondition();
        return FALSE;
    }

    if( !I2ca0WriteProcComplete() )
    {
        I2ca0CountWriteError();
        I2ca0StopCondition();
        return FALSE;
    }

    I2ca0StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    //Delay_MS( I2C_DEFAULT_DELAY );

    // Read.. page
    if( !I2ca0SeqRead8bit(_dev, _addr, &rbuf[0], _len) )
    {
        I2ca0CountWriteError();
        I2ca0StopCondition();
        return FALSE;
    }

    // Check data..
    if( memcmp(&buf[1], &rbuf[0], _len) != 0 )
    {
        I2ca0CountWriteError();
        I2ca0StopCondition();
        return FALSE;
    }

    I2ca0StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    //Delay_MS( I2C_DEFAULT_DELAY );
    i2ca0.WriteOk++;
    return TRUE;
}

U8 I2ca0PageWrite8bit(U8 _dev, U8 _addr, U8 *_data, U8 _len)
{
    U8 retryCount = I2C_DEFAULT_RETRY_COUNT;

    do
    {
        if( retryCount == 0U )
        {
            return FALSE;
        }
        retryCount--;

        RESET_WDT();

    }
    while( I2ca0PageWriteSub8bit(_dev, _addr, _data, _len) != TRUE );

    RESET_WDT();
    return TRUE;
}

static BOOL_T	I2ca0PageWriteSub16bit(U8 _dev, U16 _addr, U8 *_data, U8 _len)
{
    U8 buf [I2CA0_BUF_SIZE + 2];
    U8 rbuf[I2CA0_BUF_SIZE];

    buf[0] = GET_16_HIGH_BYTE(_addr);
    buf[1] = GET_16_LOW_BYTE(_addr);

    MEMCPY( (void __FAR *)&buf[2], (const void __FAR *)&_data[0], _len );

    I2ca0SetWriteProc(I2CA0_PROC_START);

    // Write Page..
    if( i2ca0_master_send(_dev | I2C_OP_WR, &buf[0], _len + 2, 200U) != MD_OK )
    {
        I2ca0CountWriteError();
        I2ca0StopCondition();
        return FALSE;
    }

    if( !I2ca0WriteProcComplete() )
    {
        I2ca0CountWriteError();
        I2ca0StopCondition();
        return FALSE;
    }

    I2ca0StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    //Delay_MS( I2C_DEFAULT_DELAY );

    // Read.. page
    if( !I2ca0SeqRead16bit(_dev, _addr, &rbuf[0], _len) )
    {
        I2ca0CountWriteError();
        I2ca0StopCondition();
        return FALSE;
    }

    // Check data..
    if( memcmp(&buf[2], &rbuf[0], _len) != 0 )
    {
        I2ca0CountWriteError();
        I2ca0StopCondition();
        return FALSE;
    }

    I2ca0StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    //Delay_MS( I2C_DEFAULT_DELAY );
    i2ca0.WriteOk++;
    return TRUE;
}

U8 I2ca0PageWrite16bit(U8 _dev, U16 _addr, U8 *_data, U8 _len)
{
    U8 retryCount = I2C_DEFAULT_RETRY_COUNT;

    do
    {
        if( retryCount == 0U )
        {
            return FALSE;
        }
        retryCount--;

        RESET_WDT();

    }
    while( I2ca0PageWriteSub16bit(_dev, _addr, _data, _len) != TRUE );

    RESET_WDT();
    return TRUE;
}

static BOOL_T	I2ca0ByteReadSub8bit(U8 _dev, U8 _addr, U8 *_data)
{
    U8 buf = 0U;

    buf = _addr;

    I2ca0SetWriteProc(I2CA0_PROC_START);
    // Write address..
    if( i2ca0_master_send(_dev | I2C_OP_WR, &buf, 1, 200) != MD_OK )
    {
        I2ca0CountReadError();
        I2ca0StopCondition();
        return FALSE;
    }

    if( !I2ca0WriteProcComplete() )
    {
        I2ca0CountReadError();
        I2ca0StopCondition();
        return FALSE;
    }

    I2ca0SetReadProc(I2CA0_PROC_START);

    // Read data..
    if( i2ca0_master_recv(_dev | I2C_OP_RD, _data, 1U, 200U) != MD_OK )
    {
        I2ca0CountReadError();
        I2ca0StopCondition();
        return FALSE;
    }


    if( !I2ca0ReadProcComplete() )
    {
        I2ca0CountReadError();
        I2ca0StopCondition();
        return FALSE;
    }

    I2ca0StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    i2ca0.ReadOk++;
    return TRUE;
}

BOOL_T I2ca0ByteRead8bit(U8 _dev, U8 _addr, U8 *_data)
{
    U8 retryCount = I2C_DEFAULT_RETRY_COUNT;

    do
    {
        if( retryCount == 0U )
        {
            return FALSE;
        }
        retryCount--;

        RESET_WDT();

    }
    while ( I2ca0ByteReadSub8bit(_dev, _addr, _data) != TRUE );

    RESET_WDT();
    return TRUE;
}

static BOOL_T	I2ca0ByteReadSub16bit(U8 _dev, U16 _addr, U8 *_data)
{
    U8 buf[2] = {0, };

    buf[0] = GET_16_HIGH_BYTE(_addr);
    buf[1] = GET_16_LOW_BYTE(_addr);

    I2ca0SetWriteProc(I2CA0_PROC_START);
    // Write address..
    if( i2ca0_master_send(_dev | I2C_OP_WR, buf, 2U, 200U) != MD_OK )
    {
        I2ca0CountReadError();
        I2ca0StopCondition();
        return FALSE;
    }

    if( !I2ca0WriteProcComplete() )
    {
        I2ca0CountReadError();
        I2ca0StopCondition();
        return FALSE;
    }

    I2ca0SetReadProc(I2CA0_PROC_START);

    // Read data..
    if( i2ca0_master_recv(_dev | I2C_OP_RD, _data, 1U, 200U) != MD_OK )
    {
        I2ca0CountReadError();
        I2ca0StopCondition();
        return FALSE;
    }


    if( !I2ca0ReadProcComplete() )
    {
        I2ca0CountReadError();
        I2ca0StopCondition();
        return FALSE;
    }

    I2ca0StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    i2ca0.ReadOk++;
    return TRUE;
}

BOOL_T I2ca0ByteRead16bit(U8 _dev, U16 _addr, U8 *_data)
{
    U8 retryCount = I2C_DEFAULT_RETRY_COUNT;

    do
    {
        if( retryCount == 0U )
        {
            return FALSE;
        }
        retryCount--;

        RESET_WDT();

    }
    while ( I2ca0ByteReadSub16bit(_dev, _addr, _data) != TRUE );

    RESET_WDT();
    return TRUE;
}

static BOOL_T I2ca0SeqReadSub8bit(U8 _dev, U8 _addr, U8 *_data, U8 _len)
{
    U8 buf = 0;

    buf = _addr;
    I2ca0SetWriteProc(I2CA0_PROC_START);

    /* Send  Address */
    if( i2ca0_master_send(_dev | I2C_OP_WR, &buf, 1U, 200U) != MD_OK )
    {
        I2ca0CountReadError();
        I2ca0StopCondition();
        return FALSE;
    }

    /* Wait */
    if( !I2ca0WriteProcComplete() )
    {
        I2ca0CountReadError();
        I2ca0StopCondition();
        return FALSE;
    }

    I2ca0SetReadProc(I2CA0_PROC_START);

    /* Recv Data */
    if( i2ca0_master_recv(_dev | I2C_OP_RD, _data, _len, 200U) != MD_OK )
    {
        I2ca0CountReadError();
        I2ca0StopCondition();
        return FALSE;
    }

    if( !I2ca0ReadProcComplete() )
    {
        I2ca0CountReadError();
        I2ca0StopCondition();
        return FALSE;
    }


    I2ca0StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    i2ca0.ReadOk++;
    return TRUE;
}

BOOL_T	I2ca0SeqRead8bit(U8 _dev, U8 _addr, U8 *_data, U8 _len)
{
    U8 retryCount = I2C_DEFAULT_RETRY_COUNT;

    do
    {
        if( retryCount == 0U )
        {
            return FALSE;
        }
        retryCount--;

        RESET_WDT();

    }
    while( I2ca0SeqRead8bit(_dev, _addr , _data, _len) != TRUE );

    RESET_WDT();
    return TRUE;
}

static BOOL_T	I2ca0SeqReadSub16bit(U8 _dev, U16 _addr, U8 *_data, U8 _len)
{
    U8 buf[2] = {0, };

    buf[0] = GET_16_HIGH_BYTE(_addr);
    buf[1] = GET_16_LOW_BYTE(_addr);
    I2ca0SetWriteProc(I2CA0_PROC_START);

    /* Send  Address */
    if( i2ca0_master_send(_dev | I2C_OP_WR, buf, 2U, 200U) != MD_OK )
    {
        I2ca0CountReadError();
        I2ca0StopCondition();
        return FALSE;
    }

    /* Wait */
    if( !I2ca0WriteProcComplete() )
    {
        I2ca0CountReadError();
        I2ca0StopCondition();
        return FALSE;
    }

    I2ca0SetReadProc(I2CA0_PROC_START);

    /* Recv Data */
    if( i2ca0_master_recv(_dev | I2C_OP_RD, _data, _len, 200) != MD_OK )
    {
        I2ca0CountReadError();
        I2ca0StopCondition();
        return FALSE;
    }

    if( !I2ca0ReadProcComplete() )
    {
        I2ca0CountReadError();
        I2ca0StopCondition();
        return FALSE;
    }

    I2ca0StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    i2ca0.ReadOk++;
    return TRUE;
}

BOOL_T I2ca0SeqRead16bit(U8 _dev, U16 _addr, U8 * _data, U8 _len)
{
    U8 retryCount = I2C_DEFAULT_RETRY_COUNT;

    do
    {
        if( retryCount == 0U )
        {
            return FALSE;
        }
        retryCount--;

        RESET_WDT();

    }
    while( I2ca0SeqRead16bit(_dev, _addr, _data, _len) != TRUE );

    RESET_WDT();
    return TRUE;
}

/**
 * I2CA1
**/
void I2ca1SetWriteProc(U8 _proc)
{
    i2ca1.WriteProc = _proc;
}


U8	I2ca1GetWriteProc( void )
{
    return i2ca1.WriteProc;
}

static BOOL_T	I2ca1WriteProcComplete(void)
{
    U16	wait = 10000U;

    do
    {
        if( wait > 0U )
            wait--;
        else
            return FALSE;
    }
    while( !(I2ca1GetWriteProc() == I2CA1_PROC_DONE) );

    return TRUE;
}

void I2ca1SetReadProc(U8 _proc )
{
    i2ca1.ReadProc = _proc;
}

U8 I2ca1GetReadProc(void)
{
    return i2ca1.ReadProc;
}

static BOOL_T	I2ca1ReadProcComplete(void)
{
    U16	wait = 10000U;

    do
    {
        if( wait > 0U )
            wait--;
        else
            return FALSE;
    }
    while( !(I2ca1GetReadProc() == I2CA1_PROC_DONE) );

    return TRUE;
}

static void I2ca1CountReadError(void)
{
    i2ca1.ReadFail++;
}

static void I2ca1CountWriteError(void)
{
    i2ca1.WriteFail++;
}

static void I2ca1StopCondition(void)
{
#if( CONFIG_USE_I2CA1 == 1 )
    R_IICA1_StopCondition();
    //Delay_MS( DEFAULT_DELAY );
#endif
}

// Byte Write Addr 8bit / 16bit
static BOOL_T	I2ca1ByteWriteSub8bit(U8 _dev, U8 _addr, U8 _data)
{
    U8 buf[2] = {0U, };

    buf[0] = _addr;
    buf[1] = _data;

    I2ca1SetWriteProc(I2CA1_PROC_START);
    if( i2ca1_master_send(_dev | I2C_OP_WR, buf, 2U, 200U) != MD_OK )
    {
        I2ca1CountWriteError();

        I2ca1StopCondition();
        return FALSE;
    }

    if( !I2ca1WriteProcComplete() )
    {
        I2ca1CountWriteError();
        I2ca1StopCondition();
        return FALSE;
    }

    I2ca1StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    //Delay_MS(100);

    i2ca1.WriteOk++;
    return TRUE;
}

BOOL_T I2ca1ByteWrite8bit(U8 _dev, U8 _addr, U8 _data)
{
    U8 retryCount = I2C_DEFAULT_RETRY_COUNT;

    do
    {
        if( retryCount == 0U )
        {
            return FALSE;
        }
        retryCount--;

        RESET_WDT();

    }
    while( I2ca1ByteWriteSub8bit(_dev, _addr, _data) != TRUE );

    RESET_WDT();
    return TRUE;
}

static BOOL_T	I2ca1ByteWriteSub16bit(U8 _dev, U16 _addr, U8 _data)
{
    U8 buf[3] = {0U, };

    buf[0] = GET_16_HIGH_BYTE(_addr);
    buf[1] = GET_16_LOW_BYTE(_addr);
    buf[2] = _data;

    I2ca1SetWriteProc(I2CA1_PROC_START);
    if( i2ca1_master_send(_dev | I2C_OP_WR, buf, 3U, 200U) != MD_OK )
    {
        I2ca1CountWriteError();

        I2ca1StopCondition();
        return FALSE;
    }

    if( !I2ca1WriteProcComplete() )
    {
        I2ca1CountWriteError();
        I2ca1StopCondition();
        return FALSE;
    }

    I2ca1StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    //Delay_MS(100);

    i2ca1.WriteOk++;
    return TRUE;
}

BOOL_T I2ca1ByteWrite16bit(U8 _dev, U16 _addr, U8 _data)
{
    U8 retryCount = I2C_DEFAULT_RETRY_COUNT;

    do
    {
        if( retryCount == 0U )
        {
            return FALSE;
        }
        retryCount--;

        RESET_WDT();

    }
    while( I2ca1ByteWriteSub16bit(_dev, _addr, _data) != TRUE );

    RESET_WDT();
    return TRUE;
}

// Page Write Addr 8bit / 16bit
static BOOL_T	I2ca1PageWriteSub8bit(U8 _dev, U8 _addr, U8 *_data, U8 _len)
{
    U8	buf [I2CA1_BUF_SIZE + 1];
    U8	rbuf[I2CA1_BUF_SIZE];

    buf[0] = _addr;

    MEMCPY( (void __FAR *)&buf[1], (const void __FAR *)&_data[0], _len );

    I2ca1SetWriteProc(I2CA1_PROC_START);

    // Write Page..
    if( i2ca1_master_send(_dev | I2C_OP_WR, &buf[0], _len + 1, 200U) != MD_OK )
    {
        I2ca1CountWriteError();
        I2ca1StopCondition();
        return FALSE;
    }

    if( !I2ca1WriteProcComplete() )
    {
        I2ca1CountWriteError();
        I2ca1StopCondition();
        return FALSE;
    }

    I2ca1StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    //Delay_MS( I2C_DEFAULT_DELAY );

    // Read.. page
    if( !I2ca1SeqRead8bit(_dev, _addr, &rbuf[0], _len) )
    {
        I2ca1CountWriteError();
        I2ca1StopCondition();
        return FALSE;
    }

    // Check data..
    if( memcmp(&buf[1], &rbuf[0], _len) != 0 )
    {
        I2ca1CountWriteError();
        I2ca1StopCondition();
        return FALSE;
    }

    I2ca1StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    //Delay_MS( I2C_DEFAULT_DELAY );
    i2ca1.WriteOk++;
    return TRUE;
}

U8 I2ca1PageWrite8bit(U8 _dev, U8 _addr, U8 *_data, U8 _len)
{
    U8 retryCount = I2C_DEFAULT_RETRY_COUNT;

    do
    {
        if( retryCount == 0U )
        {
            return FALSE;
        }
        retryCount--;

        RESET_WDT();

    }
    while( I2ca1PageWriteSub8bit(_dev, _addr, _data, _len) != TRUE );

    RESET_WDT();
    return TRUE;
}

static BOOL_T	I2ca1PageWriteSub16bit(U8 _dev, U16 _addr, U8 *_data, U8 _len)
{
    U8	buf [I2CA1_BUF_SIZE + 2];
    U8	rbuf[I2CA1_BUF_SIZE];

    buf[0] = GET_16_HIGH_BYTE(_addr);
    buf[1] = GET_16_LOW_BYTE(_addr);

    MEMCPY( (void __FAR *)&buf[2], (const void __FAR *)&_data[0], _len );

    I2ca1SetWriteProc(I2CA1_PROC_START);

    // Write Page..
    if( i2ca1_master_send(_dev | I2C_OP_WR, &buf[0], _len + 2, 200U) != MD_OK )
    {
        I2ca1CountWriteError();
        I2ca1StopCondition();
        return FALSE;
    }

    if( !I2ca1WriteProcComplete() )
    {
        I2ca1CountWriteError();
        I2ca1StopCondition();
        return FALSE;
    }

    I2ca1StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    //Delay_MS( I2C_DEFAULT_DELAY );

    // Read.. page
    if( !I2ca1SeqRead16bit(_dev, _addr, &rbuf[0], _len) )
    {
        I2ca1CountWriteError();
        I2ca1StopCondition();
        return FALSE;
    }

    // Check data..
    if( memcmp(&buf[2], &rbuf[0], _len) != 0 )
    {
        I2ca1CountWriteError();
        I2ca1StopCondition();
        return FALSE;
    }

    I2ca1StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    //Delay_MS( I2C_DEFAULT_DELAY );
    i2ca1.WriteOk++;
    return TRUE;
}

U8 I2ca1PageWrite16bit(U8 _dev, U16 _addr, U8 *_data, U8 _len)
{
    U8 retryCount = I2C_DEFAULT_RETRY_COUNT;

    do
    {
        if( retryCount == 0U )
        {
            return FALSE;
        }
        retryCount--;

        RESET_WDT();

    }
    while( I2ca1PageWriteSub16bit(_dev, _addr, _data, _len) != TRUE );

    RESET_WDT();
    return TRUE;
}

BOOL_T I2ca1PageWriteWeak8bit(U8 _dev, U8 *buf, U8 _len)
{
    I2ca1SetWriteProc(I2CA1_PROC_START);

    // Write Page..
    if( i2ca1_master_send(_dev | I2C_OP_WR, buf, _len + 2, 200U) != MD_OK )
    {
        I2ca1CountWriteError();
        I2ca1StopCondition();
        return FALSE;
    }

    if( !I2ca1WriteProcComplete() )
    {
        I2ca1CountWriteError();
        I2ca1StopCondition();
        return FALSE;
    }

    I2ca1StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
}

void I2ca1PageStopWeak8bit(void)
{
    I2ca1StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    i2ca1.WriteOk++;
}

static BOOL_T	I2ca1ByteReadSub8bit(U8 _dev, U8 _addr, U8 *_data)
{
    U8 buf = 0U;

    buf = _addr;

    I2ca1SetWriteProc(I2CA1_PROC_START);
    // Write address..
    if( i2ca1_master_send(_dev | I2C_OP_WR, &buf, 1U, 200U) != MD_OK )
    {
        I2ca1CountReadError();
        I2ca1StopCondition();
        return FALSE;
    }

    if( !I2ca1WriteProcComplete() )
    {
        I2ca1CountReadError();
        I2ca1StopCondition();
        return FALSE;
    }

    I2ca1SetReadProc(I2CA1_PROC_START);

    // Read data..
    if( i2ca1_master_recv(_dev | I2C_OP_RD, _data, 1U, 200U) != MD_OK )
    {
        I2ca1CountReadError();
        I2ca1StopCondition();
        return FALSE;
    }

    if( !I2ca1ReadProcComplete() )
    {
        I2ca1CountReadError();
        I2ca1StopCondition();
        return FALSE;
    }

    I2ca1StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    i2ca1.ReadOk++;
    return TRUE;
}

BOOL_T I2ca1ByteRead8bit(U8 _dev, U8 _addr, U8 *_data)
{
    U8 retryCount = I2C_DEFAULT_RETRY_COUNT;

    do
    {
        if( retryCount == 0U )
        {
            return FALSE;
        }
        retryCount--;

        RESET_WDT();

    }
    while ( I2ca1ByteReadSub8bit(_dev, _addr, _data) != TRUE );

    RESET_WDT();
    return TRUE;
}

static BOOL_T	I2ca1ByteReadSub16bit(U8 _dev, U16 _addr, U8 *_data)
{
    U8 buf[2] = {0U, };

    buf[0] = GET_16_HIGH_BYTE(_addr);
    buf[1] = GET_16_LOW_BYTE(_addr);

    I2ca1SetWriteProc(I2CA1_PROC_START);
    // Write address..
    if( i2ca1_master_send(_dev | I2C_OP_WR, buf, 2U, 200U) != MD_OK )
    {
        I2ca1CountReadError();
        I2ca1StopCondition();
        return FALSE;
    }

    if( !I2ca1WriteProcComplete() )
    {
        I2ca1CountReadError();
        I2ca1StopCondition();
        return FALSE;
    }

    I2ca1SetReadProc(I2CA1_PROC_START);

    // Read data..
    if( i2ca1_master_recv(_dev | I2C_OP_RD, _data, 1U, 200U) != MD_OK )
    {
        I2ca1CountReadError();
        I2ca1StopCondition();
        return FALSE;
    }


    if( !I2ca1ReadProcComplete() )
    {
        I2ca1CountReadError();
        I2ca1StopCondition();
        return FALSE;
    }

    I2ca1StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    i2ca1.ReadOk++;
    return TRUE;
}

BOOL_T I2ca1ByteRead16bit(U8 _dev, U16 _addr, U8 *_data)
{
    U8 retryCount = I2C_DEFAULT_RETRY_COUNT;

    do
    {
        if( retryCount == 0U )
        {
            return FALSE;
        }
        retryCount--;

        RESET_WDT();

    }
    while ( I2ca1ByteReadSub16bit(_dev, _addr, _data) != TRUE );

    RESET_WDT();
    return TRUE;
}

static BOOL_T	I2ca1SeqReadSub8bit(U8 _dev, U8 _addr, U8 *_data, U8 _len)
{
    U8 buf = 0U;

    buf = _addr;
    I2ca1SetWriteProc(I2CA1_PROC_START);

    /* Send  Address */
    if( i2ca1_master_send(_dev | I2C_OP_WR, &buf, 1U, 200U) != MD_OK )
    {
        I2ca1CountReadError();
        I2ca1StopCondition();
        return FALSE;
    }

    /* Wait */
    if( !I2ca1WriteProcComplete() )
    {
        I2ca1CountReadError();
        I2ca1StopCondition();
        return FALSE;
    }

    I2ca1SetReadProc(I2CA1_PROC_START);

    /* Recv Data */
    if( i2ca1_master_recv(_dev | I2C_OP_RD, _data, _len, 200U) != MD_OK )
    {
        I2ca1CountReadError();
        I2ca1StopCondition();
        return FALSE;
    }

    if( !I2ca1ReadProcComplete() )
    {
        I2ca1CountReadError();
        I2ca1StopCondition();
        return FALSE;
    }


    I2ca1StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    i2ca1.ReadOk++;
    return TRUE;
}

BOOL_T	I2ca1SeqRead8bit(U8 _dev, U8 _addr, U8 *_data, U8 _len)
{
    U8 retryCount = I2C_DEFAULT_RETRY_COUNT;

    do
    {
        if( retryCount == 0U )
        {
            return FALSE;
        }
        retryCount--;

        RESET_WDT();

    }
    while( I2ca1SeqReadSub8bit(_dev, _addr, _data, _len) != TRUE );

    RESET_WDT();
    return TRUE;
}

static BOOL_T	I2ca1SeqReadSub16bit(U8 _dev, U16 _addr, U8 *_data, U8 _len)
{
    U8 buf[2] = {0U, };

    buf[0] = GET_16_HIGH_BYTE(_addr);
    buf[1] = GET_16_LOW_BYTE(_addr);
    I2ca1SetWriteProc(I2CA1_PROC_START);

    /* Send  Address */
    if( i2ca1_master_send(_dev | I2C_OP_WR, buf, 2U, 200U) != MD_OK )
    {
        I2ca1CountReadError();
        I2ca1StopCondition();
        return FALSE;
    }

    /* Wait */
    if( !I2ca1WriteProcComplete() )
    {
        I2ca1CountReadError();
        I2ca1StopCondition();
        return FALSE;
    }

    I2ca1SetReadProc(I2CA1_PROC_START);

    /* Recv Data */
    if( i2ca1_master_recv(_dev | I2C_OP_RD, _data, _len, 200U) != MD_OK )
    {
        I2ca1CountReadError();
        I2ca1StopCondition();
        return FALSE;
    }

    if( !I2ca1ReadProcComplete() )
    {
        I2ca1CountReadError();
        I2ca1StopCondition();
        return FALSE;
    }

    I2ca1StopCondition();
    Delay_MS(I2C_DEFAULT_DELAY);
    i2ca1.ReadOk++;
    return TRUE;
}

BOOL_T	I2ca1SeqRead16bit(U8 _dev, U16 _addr, U8 *_data, U8 _len)
{
    U8 retryCount = I2C_DEFAULT_RETRY_COUNT;

    do
    {
        if( retryCount == 0U )
        {
            return FALSE;
        }
        retryCount--;

        RESET_WDT();

    }
    while( I2ca1SeqReadSub16bit(_dev, _addr, _data, _len) != TRUE );

    RESET_WDT();
    return TRUE;
}
