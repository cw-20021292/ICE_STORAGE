/// @file     Func_Uart.h
/// @date     2025/04/25
/// @author   Jaejin Ham, Hyunho Kang
/// @brief    Uart ���� �Լ�  File


#ifndef _FUNC_UART_H_
#define _FUNC_UART_H_


/*  User�� Uart ���� �߰��� �Լ���
    => UART0_Transmit_Interrupt();              // r_Config_UART0_interrupt_send() �Լ� ���� �ڵ带 ��� �����ϰ� ��ſ� �߰�
    => UART0_Recieve_Interrupt();               // r_Config_UART0_interrupt_receive() �Լ� ���� �ڵ带 ��� �����ϰ� ��ſ� �߰�
    => UART0_Error_Interrupt();                 // r_Config_UART0_interrupt_error() �Լ� ���� �ڵ带 ��� �����ϰ� ��ſ� �߰�

    => UART1, UART2, UART3, UARTA0, UARTA1�� ���� ���� ������ ���·� ���
*/

#define UART0_USE                               NO_USE     // UART0 ��� ���� - 0(No Use), 1(Use)
#define UART1_USE                               NO_USE  // UART1 ��� ���� - 0(No Use), 1(Use)
#define UART2_USE                               NO_USE  // UART2 ��� ���� - 0(No Use), 1(Use)
#define UART3_USE                               NO_USE  // UART3 ��� ���� - 0(No Use), 1(Use)
#define UARTA0_USE                              NO_USE  // UARTA0 ��� ���� - 0(No Use), 1(Use)
#define UARTA1_USE                              NO_USE  // UARTA1 ��� ���� - 0(No Use), 1(Use)

/* �ϱ⿡ �� UART�� Tx �� Rx ���ͷ�Ʈ ��ƾ �ȿ��� ������ �Լ��� UART Start, Stop �Լ� ��ġ ��Ų�� */
#if (UART0_USE == USE)
#define UART0_TX_INTERRUPT_FUNCTION()           UART_ISR_BLDC_Comp_Tx();
#define UART0_RX_INTERRUPT_FUNCTION()           UART_ISR_BLDC_Comp_Rx();
#define UART0_RX_ERROR_INTERRUPT_FUNCTION()     UART_ISR_BLDC_Comp_Rx_Error();
#define UART0_START()                           R_Config_UART0_Start();
#define UART0_STOP()                            R_Config_UART0_Stop();

void UART0_Transmit_Interrupt(void);
void UART0_Recieve_Interrupt(void);
void UART0_Error_Interrupt(void);
#endif


#if (UART1_USE == USE)
#define UART1_TX_INTERRUPT_FUNCTION()           UART_ISR_BLDC_Comp_Tx();
#define UART1_RX_INTERRUPT_FUNCTION()           UART_ISR_BLDC_Comp_Rx();
#define UART1_RX_ERROR_INTERRUPT_FUNCTION()     UART_ISR_BLDC_Comp_Rx_Error();
#define UART1_START()                           R_Config_UART1_Start();
#define UART1_STOP()                            R_Config_UART1_Stop();

void UART1_Transmit_Interrupt(void);
void UART1_Recieve_Interrupt(void);
void UART1_Error_Interrupt(void);
#endif


#if (UART2_USE == USE)
#define UART2_TX_INTERRUPT_FUNCTION()           UART_ISR_BLDC_Comp_Tx();
#define UART2_RX_INTERRUPT_FUNCTION()           UART_ISR_BLDC_Comp_Rx();
#define UART2_RX_ERROR_INTERRUPT_FUNCTION()     UART_ISR_BLDC_Comp_Rx_Error();
#define UART2_START()                           R_Config_UART2_Start();
#define UART2_STOP()                            R_Config_UART2_Stop();

void UART2_Transmit_Interrupt(void);
void UART2_Recieve_Interrupt(void);
void UART2_Error_Interrupt(void);
#endif


#if (UART3_USE == USE)
#define UART3_TX_INTERRUPT_FUNCTION()           UART_ISR_BLDC_Comp_Tx();
#define UART3_RX_INTERRUPT_FUNCTION()           UART_ISR_BLDC_Comp_Rx();
#define UART3_RX_ERROR_INTERRUPT_FUNCTION()     UART_ISR_BLDC_Comp_Rx_Error();
#define UART3_START()                           R_Config_UART3_Start();
#define UART3_STOP()                            R_Config_UART3_Stop();

void UART3_Transmit_Interrupt(void);
void UART3_Recieve_Interrupt(void);
void UART3_Error_Interrupt(void);
#endif


#if (UARTA0_USE == USE)
#define UARTA0_TX_INTERRUPT_FUNCTION()          UART_ISR_BLDC_Comp_Tx();
#define UARTA0_RX_INTERRUPT_FUNCTION()          UART_ISR_BLDC_Comp_Rx();
#define UARTA0_RX_ERROR_INTERRUPT_FUNCTION()    UART_ISR_BLDC_Comp_Rx_Error();
#define UARTA0_START()                          R_Config_UARTA0_Start();
#define UARTA0_STOP()                           R_Config_UARTA0_Stop();

void UARTA0_Transmit_Interrupt(void);
void UARTA0_Recieve_Interrupt(void);
void UARTA0_Error_Interrupt(void);
#endif


#if (UARTA1_USE == USE)
#define UARTA1_TX_INTERRUPT_FUNCTION()          UART_ISR_BLDC_Comp_Tx();
#define UARTA1_RX_INTERRUPT_FUNCTION()          UART_ISR_BLDC_Comp_Rx();
#define UARTA1_RX_ERROR_INTERRUPT_FUNCTION()    UART_ISR_BLDC_Comp_Rx_Error();
#define UARTA1_START()                          R_Config_UARTA1_Start();
#define UARTA1_STOP()                           R_Config_UARTA1_Stop();

void UARTA1_Transmit_Interrupt(void);
void UARTA1_Recieve_Interrupt(void);
void UARTA1_Error_Interrupt(void);
#endif


#endif

