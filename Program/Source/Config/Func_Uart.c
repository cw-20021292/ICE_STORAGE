/// @file     Func_Uart.c
/// @date     2025/04/25
/// @author   Jaejin Ham, Hyunho Kang
/// @brief    Uart ���� �Լ�  File


#include "Global_Header.h"


#if (UART0_USE == USE)
/// @brief      r_Config_UART0_interrupt_send() �Լ� ���� �ڵ带 ��� �����ϰ� ��ſ� �߰��� �Լ�
/// @details    UART0 Tx ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void UART0_Transmit_Interrupt(void)
{
    UART0_TX_INTERRUPT_FUNCTION();
}


/// @brief      r_Config_UART0_interrupt_receive() �Լ� ���� �ڵ带 ��� �����ϰ� ��ſ� �߰��� �Լ�
/// @details    UART0 Rx ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void UART0_Recieve_Interrupt(void)
{
    UART0_RX_INTERRUPT_FUNCTION();
}


/// @brief      r_Config_UART0_interrupt_error() �Լ� �ȿ� �߰��� �Լ�
/// @details    UART0 Error ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void UART0_Error_Interrupt(void)
{
    UART0_RX_ERROR_INTERRUPT_FUNCTION();
}
#endif


#if (UART1_USE == USE)
/// @brief      r_Config_UART1_interrupt_send() �Լ� ���� �ڵ带 ��� �����ϰ� ��ſ� �߰��� �Լ�
/// @details    UART1 Tx ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void UART1_Transmit_Interrupt(void)
{
    UART1_TX_INTERRUPT_FUNCTION();
}


/// @brief      r_Config_UART1_interrupt_receive() �Լ� ���� �ڵ带 ��� �����ϰ� ��ſ� �߰��� �Լ�
/// @details    UART1 Rx ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void UART1_Recieve_Interrupt(void)
{
    UART1_RX_INTERRUPT_FUNCTION();
}


/// @brief      r_Config_UART1_interrupt_error() �Լ� �ȿ� �߰��� �Լ�
/// @details    UART1 Error ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void UART1_Error_Interrupt(void)
{
    UART1_RX_ERROR_INTERRUPT_FUNCTION();
}
#endif


#if (UART2_USE == USE)
/// @brief      r_Config_UART2_interrupt_send() �Լ� ���� �ڵ带 ��� �����ϰ� ��ſ� �߰��� �Լ�
/// @details    UART2 Tx ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void UART2_Transmit_Interrupt(void)
{
    UART2_TX_INTERRUPT_FUNCTION();
}


/// @brief      r_Config_UART2_interrupt_receive() �Լ� ���� �ڵ带 ��� �����ϰ� ��ſ� �߰��� �Լ�
/// @details    UART2 Rx ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void UART2_Recieve_Interrupt(void)
{
    UART2_RX_INTERRUPT_FUNCTION();
}


/// @brief      r_Config_UART2_interrupt_error() �Լ� �ȿ� �߰��� �Լ�
/// @details    UART2 Error ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void UART2_Error_Interrupt(void)
{
    UART2_RX_ERROR_INTERRUPT_FUNCTION();
}

#endif


#if (UART3_USE == USE)
/// @brief      r_Config_UART3_interrupt_send() �Լ� ���� �ڵ带 ��� �����ϰ� ��ſ� �߰��� �Լ�
/// @details    UART3 Tx ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void UART3_Transmit_Interrupt(void)
{
    UART3_TX_INTERRUPT_FUNCTION();
}


/// @brief      r_Config_UART3_interrupt_receive() �Լ� ���� �ڵ带 ��� �����ϰ� ��ſ� �߰��� �Լ�
/// @details    UART3 Rx ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void UART3_Recieve_Interrupt(void)
{
    UART3_RX_INTERRUPT_FUNCTION();
}


/// @brief      r_Config_UART3_interrupt_error() �Լ� �ȿ� �߰��� �Լ�
/// @details    UART3 Error ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void UART3_Error_Interrupt(void)
{
    UART3_RX_ERROR_INTERRUPT_FUNCTION();
}
#endif


#if (UARTA0_USE == USE)
/// @brief      r_Config_UARTA0_interrupt_send() �Լ� ���� �ڵ带 ��� �����ϰ� ��ſ� �߰��� �Լ�
/// @details    UARTA0 Tx ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void UARTA0_Transmit_Interrupt(void)
{
    UARTA0_TX_INTERRUPT_FUNCTION();
}


/// @brief      r_Config_UARTA0_interrupt_receive() �Լ� ���� �ڵ带 ��� �����ϰ� ��ſ� �߰��� �Լ�
/// @details    UARTA0 Rx ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void UARTA0_Recieve_Interrupt(void)
{
    UARTA0_RX_INTERRUPT_FUNCTION();
}


/// @brief      r_Config_UARTA0_interrupt_error() �Լ� �ȿ� �߰��� �Լ�
/// @details    UARTA0 Error ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void UARTA0_Error_Interrupt(void)
{
    UARTA0_RX_ERROR_INTERRUPT_FUNCTION();
}
#endif


#if (UARTA1_USE == USE)
/// @brief      r_Config_UARTA1_interrupt_send() �Լ� ���� �ڵ带 ��� �����ϰ� ��ſ� �߰��� �Լ�
/// @details    UARTA1 Tx ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void UARTA1_Transmit_Interrupt(void)
{
    UARTA1_TX_INTERRUPT_FUNCTION();
}


/// @brief      r_Config_UARTA1_interrupt_receive() �Լ� ���� �ڵ带 ��� �����ϰ� ��ſ� �߰��� �Լ�
/// @details    UARTA1 Rx ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void UARTA1_Recieve_Interrupt(void)
{
    UARTA1_RX_INTERRUPT_FUNCTION();
}


/// @brief      r_Config_UARTA1_interrupt_error() �Լ� �ȿ� �߰��� �Լ�
/// @details    UARTA1 Error ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void UARTA1_Error_Interrupt(void)
{
    UARTA1_RX_ERROR_INTERRUPT_FUNCTION();
}
#endif




