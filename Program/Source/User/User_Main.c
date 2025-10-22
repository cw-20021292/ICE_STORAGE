/// @encoding euc-kr
/// @file   User_Main.c
/// @date   2025/02/24
/// @author Jaejin Ham
/// @brief  Program Main File

/// @mainpage 프로그램 메인 페이지
/// @section intro 소개
/// - 소개 : 르네사스 MCU RL78/G23 Starndad Function C Code
/// @section Program 프로그램명
/// - 프로그램명  : 르네사스 MCU RL78/G23 Starndad Function Code
/// - 프로그램 내용 : 르네사스 G23 라인업 마이컴을 이용한 표준 SW 코드
/// @section info 개발 목적
/// - 라인업 통합 SW 프레임 워크(SW 표준) 구축
/// @section advenced 사용 CPU
/// - RL78/G23 - R7F100GSL (RAM 48K, ROM 512K, Dataflash 8K)
/// @section CREATEINFO 작성정보
/// - 작성자      :   함재진, 강현호, 신현민
/// - 작성일      :   2025.08.29
/// @section MODIFYINFO 수정정보
/// - 2025.08.29 : 공용, 정수기 표준 코드 구성


#include "Global_header.h"


/// @brief    delay 함수
/// @details  설정된 시간만큼 Delay 동작을 한다
/// @param    mu8Time : delay 시간(약 8us단위)
/// @return   void
void Delay(U16 mu8Time)
{
  U16 mu8i;

  for (mu8i = 0 ; mu8i <= mu8Time ; mu8i++)
  {
    R_Config_WDT_Restart();
  }
}


/// @brief    delay ms 함수
/// @details  설정된 시간(약 1ms단위)만큼 Delay 동작을 한다
/// @param    mu8Time : delay 시간(약 1ms단위)
/// @return   void
void Delay_ms(U16 mu16Time)
{
  U16 mu16i = 0;
  U16 mu16j = 0;

  for (mu16i = 0 ; mu16i <= (mu16Time*2) ; mu16i++)
  {
    for (mu16j = 0 ; mu16j <= 830 ; mu16j++)
    {   // constant value for 1msec counter = 830
      R_Config_WDT_Restart();
    }
  }
}


// ******************************************************************************
// *                                                                            *
// *                        Program Main Fuction                                *
// *                                                                            *
// ******************************************************************************
/// @brief    Main function
/// @details  Main 함수 안에 배치하여 사용하여 동작시킨다
/// @param    void
/// @return   void
void User_Main(void)
{
    DI();

    Interrupt_Start();      // 각종 Interrupt들 시작 수행

    Delay_ms(1000);         // 전압 안정화 시간 확보. While문 진입 이후에는 Delay 함수 사용 자제(Time Scheduler 밀릴 수 있음)

    Base_Initialize();

    EI();

    Serial_Start();

    WifiControlProcess(WIFI_TIME_INI);    // Wi-Fi Initialize. EEPROM Initialize 이전에 실행

    /* --- EEPROM Initialize START --- */
#if ( (EEPROM_USE == USE) || (RTC_USE == USE) )
    EEPROM_PRIMARY();     // EEPROM Initialize
    /* --- EEPROM Initialize END--- */
#endif

    WifiControlProcess(WIFI_TIME_SET);    // Wi-Fi WifiOnOffSetting. EEPROM Initialize 이후에 실행
    ChangeOperationMode(OPERATION_MODE_ID_NORMAL);     // 일반 모드로 동작 시작 할 수 있도록 세팅


    while (1)
    {
        WDT_Restart();
        OperationModeSetup();       // 동작 모드 설정
        GoTimeScheduler();          // Time Scheduler 제어 동작
        FunctionProcess_In_WhileLoop();     // 부하 제어 Loop
        WifiControlProcess(WIFI_TIME_WHILE);    // Wi-Fi While 동작

        
    }
}





