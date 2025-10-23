/**
 * File : hal_serial.c
 * 
 * Hardware Abstraction Layer
 * Depend on Renesas MCU Chip
*/

/* UART0 : FRONT */
#pragma interrupt INTST0    r_uart0_interrupt_send
#pragma interrupt INTSR0    r_uart0_interrupt_receive

/* UART1 : WIFI */
//#pragma interrupt INTST1    r_uart1_interrupt_send
//#pragma interrupt INTSR1    r_uart1_interrupt_receive

/* UART2 : BLDC COMP */
#pragma interrupt INTST2    r_uart2_interrupt_send
#pragma interrupt INTSR2    r_uart2_interrupt_receive

/* UART3 : EOL */
#pragma interrupt INTST3    r_uart3_interrupt_send
#pragma interrupt INTSR3    r_uart3_interrupt_receive

#include "hw.h"

#include "hal_serial.h"

#include "timer.h"
#include "comm.h"

// TimeStamp ( @ms )
#define UART0_RX_TIME_STAMP   (10)
#define UART1_RX_TIME_STAMP   (10)
#define UART2_RX_TIME_STAMP   (100)
#define UART3_RX_TIME_STAMP   (10)


#if ( CONFIG_USE_UART_0 == 1 )

#define UART0_TXD             TXD0
#define UART0_RXD             RXD0
#define UART0_STATUS          SSR01
#define UART0_FLAG_CLEAR      SIR01
#define UART0_TX_INTERRUPT    STMK0

/**
 * @ brief     Function For Start UART0
 * @ detail    none
 * @ param   none
 * @ return   none
**/
void HalUart0Start(void)
{
    R_UART0_Start();
}

/**
 * @ brief     Function For Stop UART0
 * @ detail    none
 * @ param   none
 * @ return   none
**/
void HalUart0Stop(void)
{
    R_UART0_Stop();
}

/**
 * @ brief          Function For Receive Data of UART0
 * @ detail         none
 * @ param   xPtrRxData   Received Data
 * @ return         none
**/
void HalUart0ReceiveByte( U8 *xPtrRxData )
{
    *xPtrRxData = UART0_RXD;
}

/**
 * @ brief              Function For Send Data of UART0
 * @ detail             none
 * @ param   xTxData    none
 * @ return             none
**/
void HalUart0SendByte( U8 xTxData )
{
    UART0_TXD = xTxData;
}

/**
 * @ brief               Function For Enable Interrupt of UART1
 * @ detail              none
 * @ param    xUenable   Interrupt Enable(1), Disable(0)
 * @ return              none
**/
void HalUart0EnableInterrupt( U8 xUenable )
{
    if( xUenable == TRUE )
    {
        /* Enable INTST2 Interrupt */
        UART0_TX_INTERRUPT = 0;
    }
    else
    {
        /* Disable INTST2 Interrupt */
        UART0_TX_INTERRUPT = 1;
    }
}

#endif

#if ( CONFIG_USE_UART_1 == 1 )

#define UART1_TXD             TXD1
#define UART1_RXD             RXD1
#define UART1_STATUS          SSR03
#define UART1_FLAG_CLEAR      SIR03
#define UART1_TX_INTERRUPT    STMK1

/**
 * @ brief     Function For Start UART1
 * @ detail    none
 * @ param   none
 * @ return   none
**/
void HalUart1Start(void)
{
    R_UART1_Start();
}

/**
 * @ brief     Function For Stop UART1
 * @ detail    none
 * @ param   none
 * @ return   none
**/
void HalUart1Stop(void)
{
    R_UART1_Stop();
}

/**
 * @ brief          Function For Receive Data of UART1
 * @ detail         none
 * @ param   xPtrRxData   Received Data
 * @ return         none
**/
void HalUart1ReceiveByte( U8 *xPtrRxData )
{
    *xPtrRxData = UART1_RXD;
}

/**
 * @ brief              Function For Send Data of UART1
 * @ detail             none
 * @ param   xTxData    none
 * @ return             none
**/
void HalUart1SendByte( U8 xTxData )
{
    UART1_TXD = xTxData;
}

/**
 * @ brief               Function For Enable Interrupt of UART1
 * @ detail              none
 * @ param    xUenable   Interrupt Enable(1), Disable(0)
 * @ return              none
**/
void HalUart1EnableInterrupt( U8 xUenable )
{
    if( xUenable == TRUE )
    {
        /* Enable INTST2 Interrupt */
        UART1_TX_INTERRUPT = 0;
    }
    else
    {
        /* Disable INTST2 Interrupt */
        UART1_TX_INTERRUPT = 1;
    }
}

#endif

#if ( CONFIG_USE_UART_2 == 1 )

#define UART2_TXD             TXD2
#define UART2_RXD             RXD2
#define UART2_STATUS          SSR11
#define UART2_FLAG_CLEAR      SIR11
#define UART2_TX_INTERRUPT    STMK2

/**
 * @ brief     Function For Start UART2
 * @ detail    none
 * @ param   none
 * @ return   none
**/
void HalUart2Start(void)
{
    R_UART2_Start();
}

/**
 * @ brief     Function For Stop UART2
 * @ detail    none
 * @ param   none
 * @ return   none
**/
void HalUart2Stop(void)
{
    R_UART2_Stop();
}

/**
 * @ brief          Function For Receive Data of UART2
 * @ detail         none
 * @ param   xPtrRxData   Received Data
 * @ return         none
**/
void HalUart2ReceiveByte( U8 *xPtrRxData )
{
    *xPtrRxData = UART2_RXD;
}


/**
 * @ brief              Function For Send Data of UART2
 * @ detail             none
 * @ param   xTxData    none
 * @ return             none
**/
void HalUart2SendByte( U8 xTxData )
{
    UART2_TXD = xTxData;
}

/**
 * @ brief               Function For Enable Interrupt of UART2
 * @ detail              none
 * @ param    xUenable   Interrupt Enable(1), Disable(0)
 * @ return              none
**/
void HalUart2EnableInterrupt( U8 xUenable )
{
    if( xUenable == TRUE )
    {
        /* Enable INTST2 Interrupt */
        UART2_TX_INTERRUPT = 0;
    }
    else
    {
        /* Disable INTST2 Interrupt */
        UART2_TX_INTERRUPT = 1;
    }
}

#endif

#if ( CONFIG_USE_UART_3 == 1 )

#define UART3_TXD                TXD3
#define UART3_RXD                RXD3
#define UART3_STATUS             SSR13
#define UART3_FLAG_CLEAR         SIR13
#define UART3_TX_INTERRUPT       STMK3

/**
 * @ brief     Function For Start UART3
 * @ detail    none
 * @ param   none
 * @ return   none
**/
void HalUart3Start(void)
{
    R_UART3_Start();
}

/**
 * @ brief     Function For Stop UART3
 * @ detail    none
 * @ param   none
 * @ return   none
**/
void HalUart3Stop(void)
{
    R_UART2_Stop();
}

/**
 * @ brief          Function For Receive Data of UART3
 * @ detail         none
 * @ param   xPtrRxData   Received Data
 * @ return         none
**/
void HalUart3ReceiveByte( U8 *xPtrRxData )
{
    *xPtrRxData = UART3_RXD;
}

/**
 * @ brief              Function For Send Data of UART3
 * @ detail             none
 * @ param   xTxData    none
 * @ return             none
**/
void HalUart3SendByte( U8 xTxData )
{
    UART3_TXD = xTxData;
}

/**
 * @ brief               Function For Enable Interrupt of UART3
 * @ detail              none
 * @ param    xUenable   Interrupt Enable(1), Disable(0)
 * @ return              none
**/
void HalUart3EnableInterrupt( U8 xUenable )
{
    if( xUenable == TRUE )
    {
        /* Enable INTST3 Interrupt */
        UART3_TX_INTERRUPT = 0;
    }
    else
    {
        /* Disable INTST3 Interrupt */
        UART3_TX_INTERRUPT = 1;
    }
}

#endif

/* INTERRUPT */
/* UART 0 */
#if ( CONFIG_USE_UART_0 == 1 )
U16 the_rx0_err_count = 0;
/**
 * @ brief       Function For Process Interrupt about Reception End
 * @ detail      Uart0 Reception Interrput Service Routine
 * @ param       none
 * @ return      none
*/
__interrupt static void r_uart0_interrupt_receive(void)
{
    volatile U8 rxData = 0;
    volatile U8 errType = 0;

    errType = (uint8_t)(UART0_STATUS & 0x0007U);
    UART0_FLAG_CLEAR = (uint16_t)errType;

    rxData = UART0_RXD;

    EI();

    if( errType == 0 )
    {
        if( IsFullRecvBuffer(COMM_ID_FRONT) == FALSE )
        {
            SetRecvBuffer(COMM_ID_FRONT, rxData);
        }
        else
        {
            InitCommId(COMM_ID_FRONT);
        }

        StartTimer(TIMER_USER, TIMER_USER_ID_COMM_FRONT_RX, UART0_RX_TIME_STAMP);
    }
    else
    {
        the_rx0_err_count++;

        if( the_rx0_err_count >= 0xFFFF )
        {
            the_rx0_err_count = 0;
        }
    }
}

/**
 * @ brief       Function For Process Interrupt about Transmission End
 * @ detail      Uart0 Transmission Interrupt Service Routine
 * @ param       none
 * @ return      none
**/
__interrupt static void r_uart0_interrupt_send(void)
{
    EI();

    if( IsCompleteTx(COMM_ID_FRONT) == FALSE )
    {
        SendByte(COMM_ID_FRONT);
    }
    else
    {
        InitCommId(COMM_ID_FRONT);
    }
}
#endif

/* UART 1 */
#if ( CONFIG_USE_UART_1 == 1 )
U16 the_rx1_err_count = 0;
/**
 * @ brief       Function For Process Interrupt about Reception End
 * @ detail      Uart1 Reception Interrput Service Routine
 * @ param       none
 * @ return      none
**/
__interrupt static void r_uart1_interrupt_receive(void)
{
    volatile U8 rx_data;

    rx_data = UART1_RXD;

    InterruptUartWifiError();
    InterruptUartWifiRx( rx_data );
}

/**
 * @ brief       Function For Process Interrupt about Transmission End
 * @ detail      Uart1 Transmission Interrupt Service Routine
 * @ param       none
 * @ return      none
**/
__interrupt static void r_uart1_interrupt_send(void)
{
    InterruptUartWifiTx();
}
#endif

/* UART 2 */
#if ( CONFIG_USE_UART_2 == 1 )
U16 the_rx2_err_count = 0;
/**
 * @ brief       Function For Process Interrupt about Reception End
 * @ detail      Uart2 Reception Interrput Service Routine
 * @ param       none
 * @ return      none
**/
__interrupt static void r_uart2_interrupt_receive(void)
{
    volatile U8 rxData = 0;
    volatile U8 errType = 0;

    errType = (uint8_t)(UART2_STATUS & 0x0007U);
    UART2_FLAG_CLEAR = (uint16_t)errType;

    rxData = UART2_RXD;

    if( errType == 0 )
    {
        if( IsFullRecvBuffer(COMM_ID_COMP) == FALSE )
        {
            SetRecvBuffer(COMM_ID_COMP, rxData);
        }
        else
        {
            InitCommId(COMM_ID_COMP);
        }

        StartTimer(TIMER_USER, TIMER_USER_ID_COMM_COMP_RX, UART2_RX_TIME_STAMP);
    }
    else
    {
        the_rx2_err_count++;

        if( the_rx2_err_count >= 0xFFFF )
        {
            the_rx2_err_count = 0;
        }
    }
}

/**
 * @ brief       Function For Process Interrupt about Transmission End
 * @ detail      Uart2 Transmission Interrupt Service Routine
 * @ param       none
 * @ return      none
**/
__interrupt static void r_uart2_interrupt_send(void)
{
    if( IsCompleteTx(COMM_ID_COMP) == FALSE )
    {
        SendByte(COMM_ID_COMP);
    }
    else
    {
        InitCommId(COMM_ID_COMP);
    }
}
#endif


/* UART 3 */
#if ( CONFIG_USE_UART_3 == 1 )
U16 the_rx3_err_count = 0;
/**
 * @ brief       Function For Process Interrupt about Reception End
 * @ detail      Uart3 Reception Interrput Service Routine
 * @ param       none
 * @ return      none
**/
__interrupt static void r_uart3_interrupt_receive(void)
{
    volatile U8 rxData = 0;
    volatile U8 errType = 0;

    errType = (uint8_t)(UART3_STATUS & 0x0007U);
    UART3_FLAG_CLEAR = (uint16_t)errType;

    rxData = UART3_RXD;

    if( errType == 0 )
    {
        if( IsFullRecvBuffer(COMM_ID_EOL) == FALSE )
        {
            SetRecvBuffer(COMM_ID_EOL, rxData);
        }
        else
        {
            InitCommId(COMM_ID_EOL);
        }

        StartTimer(TIMER_USER, TIMER_USER_ID_COMM_EOL_RX, UART3_RX_TIME_STAMP);
    }
    else
    {
        the_rx3_err_count++;

        if( the_rx3_err_count >= 0xFFFF )
        {
            the_rx3_err_count = 0;
        }
    }
}

/**
 * @ brief       Function For Process Interrupt about Transmission End
 * @ detail      Uart3 Transmission Interrupt Service Routine
 * @ param       none
 * @ return      none
**/
__interrupt static void r_uart3_interrupt_send(void)
{
    if( IsCompleteTx(COMM_ID_EOL) == FALSE )
    {
        SendByte(COMM_ID_EOL);
    }
    else
    {
        InitCommId(COMM_ID_EOL);
    }
}
#endif
