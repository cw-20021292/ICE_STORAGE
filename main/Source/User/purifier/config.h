/**
 * File : config.h
 *
 * Project Config Define
**/
#ifndef __CONFIG_H__
#define __CONFIG_H__

/**
 * Version Infomation
 * 
 * 프로그램 버전 표시
 * 1) 첫번째 자리 : 1 - 양산 적용, 0 - 테스트 버전 (DPP, LPP)
 * 2) 두번째 자리 : 0 - 양산 적용, 1 - DPP, 2 - LPP
 * 3) 네번째 자리 : 버전 표시 0-16
*/
#define PROGRAM_VERSION    "V0.2.0.2"

/**
 * Model Infomation
*/
/// For PTA
#define COUNTRY_KOR   (0)
#define COUNTRY_US    (7)

#define COUNTRY_CODE  COUNTRY_KOR

/// For CHP / HP
#define CONFIG_SYSTEM_HP     (0)
#define CONFIG_SYSTEM_CHP    (1)
#define CONFIG_SYSTEM_AUTO   (2)
#define CONFIG_SYSTEM_TYPE   CONFIG_SYSTEM_AUTO

/// 수위 센서 에러 카운트 초기화를 위한 컴파일 옵션
#define TANK_RO              (0)
#define TANK_NT              (1)
#define CONFIG_TANK_TYPE     TANK_RO

/**
 * Feature
**/
/* Fota */
#define  CONFIG_FOTA_LIB          (1)    /// FOTA 기능 활성화( 표준 라이브러리 xxx_f 사용) 

/* Flushing */
#define CONFIG_CANCEL_FLUSHING    (1)    /// Flushing 취소후 일반모드로 진입 활성화 ( 0 : Disable, 1: Enable )
#define CONFIG_ENABLE_ERROR_SOUND (0)    /// Flushing 상태에서 비활성화된 버튼 push 시 에러음 유무 (0 : Disable, 1: Enable)

/* Error */
#define CONFIG_USE_ERROR            (1)  /// Error 판단 로직 실행 유무 ( 0 : Disable, 1 : Enable )

#define CONFIG_LEAK                 (0)  /// 0: 해제, 1: 설정
#define CONFIG_GAS_SW               (0)  /// 0: 해제, 1: 설정
#define CONFIG_CIRCULATE_PUMP       (1)  /// 0: 해제, 1: 설정

/* Voice */
#define CONFIG_VOICE                (1)  /// 0 : BUZZER, 1 : VOICE 

/* RTC 테스트 - 1sec */
#define  CONFIG_RTC_TEST        (0)   /// 0: 10초 주기 제어 1: 1초 주기 제어

/* Debug */
#define CONFIG_DEBUG_COMM       (0)   /// Communication Debug Mode ( 0 : Normal, 1: Debug )

/* Ster */
#define CONFIG_STER                     (0)  /// 0: 해제, 1: 설정
#define CONFIG_IMPROVED_FLOW_PATH_STER  (0)  /// 0 : LPP, 1 : 변경 제어
#define CONFIG_STER_TIME_COUNT          (0)  /// 0: 살균 시간 카운 Disable, 1 : 살균 시간 카운트 Enable

#if CONFIG_STER 
   #define CONFIG_USE_CIRCULATE_PUMP   (1)   /// 배수시 펌프 동작
   #define CONFIG_USE_DRAIN_TIMEOUT    (1)   /// 배수 최대 시간, timeout되면 배수 스텝 강제 종료
#else
   #define CONFIG_USE_CIRCULATE_PUMP   (1)   /// 배수시 펌프 동작
   #define CONFIG_USE_DRAIN_TIMEOUT    (1)   /// 배수 최대 시간, timeout되면 배수 스텝 강제 종료
#endif

/* Ice / Cold */
#define CONFIG_ENABLE_DEICING_COOLING        (0)  /// 얼음 우선 모드에서 탈빙 중 냉각 기능 활성화, 0: 해제, 1: 설정
#define CONFIG_ENABLE_ICON_AIS_ICETRAY_TEST  (0)  /// 아이스 트레이 점검 기능 사양 스위칭 0: IGLOO, 1:조합키
#define CONFIG_ICETRAY_ICING_AT_KEYPBA_TEST  (0)  /// KEY PBA 공정 검사 시, 제빙위치로 이동,  0 : Disable, 1 : Enable */

#define CONFIG_SET_ICE_MODE                  (0)  /// 제빙 모드 설정

#define CONFIG_BACK_FEEDER                   (0)  /// 피더모터 역회전

/* 제빙 -> 냉각 전환 시 Comp Off 후 진행 설정 ( 냉각 -> 제빙 전환 시 TurnOffComp(COMP_TYPE_COLD); 하므로 항상 OFF 됨) */
#define CONFIG_ICE_COLD_COMP                 (1)  /// 제빙 -> 냉각 전환 시 Comp Off 후 진행하도록 함

#define CONFIG_USE_ICING_MAX_COUNT           (1)  /// 제빙 때는 MAX COUNT 10회 시에만 냉수 입수 시킴

#define CONFIG_MAKE_MORE                     (1)  /// 얼음 x판 더 만들기

/* Iceout */
#define CONFIG_USE_CONTROL_ICE_FEEDER        (0)  /// 얼음 추출 시 피더 제어 함수 활용 ( 0 : Disable, 1 : Enable )

/* Waterout */
#define CONFIG_HOT_WATER_AIRFLOW             (0)  /// 온수 에어 빼기 기능 활성화
#define CONFIG_USE_FLOW_METER_AT_ICE_WATER   (1)  /// 얼음물 추출 시 유량센서 활용 ( 0 ; Disable, 1 : Enable )

/* Test */
#define CONFIG_TEST                  (0)  /// 냉동, Skip 압축기, 더미탈빙 기능 활성화
#define CONFIG_TEST_8585             (0)  /// 내구성 - 결로프로그램 - 신뢰성 내구성 테스트 ( 냉수 추출 기능 )
#define CONFIG_TEST_8585_2           (0)  /// 내구성 - 정수 / 냉수 / 온수 추출 기능
#define CONFIG_TEST_8585_ICE         (0)  /// 내구성 - 결로프로그램 - 얼음 추출 기능
#define CONFIG_TEST_8585_ICE_WATER   (0)  /// 내구성 - 결로프로그램 - 얼음물 추출 기능
#define CONFIG_TEST_8585_FEEDER      (0)  /// 내구성 - 피더 스크류 내구성 ( 정회전 )
#define CONFIG_TEST_8585_BACK_FEEDER (0)  /// 내구성 - 피더 스크류 내구성 ( 역회전 )
#define CONFIG_TEST_8585_HOT_REPEAT  (0)  /// 내구성 - 온수 반복 추출 기능

#define CONFIG_TEST_HOT_WATER       (0)  /// 신뢰성, 온수 연속 추출 테스트 프로그램 
#define CONFIG_TEST_GAS_SWITCH      (0)  /// 냉매 전환 밸브 이동 반복 테스트 
#define CONFIG_TEST_HOTGAS_SWITCH   (0)  /// 핫가스 냉매 전환 밸브 이동 반복 테스트 
#define CONFIG_TEST_MMI             (0)  /// MMI 기능 활성화
#define CONFIG_TEST_TEMP_READ       (0)  /// 온도 센서 상시 읽기 모드 ( 테스트 모드 )
#define CONFIG_TEST_UV_MANUAL       (0)  /// UV 수동 테스트

#define CONFIG_TEST_CHECK_FRONT     (0)


#define CONFIG_TEST_FLOW_METER      (0)   /// Flow meter Auto Counting

/* Jig */
#define CONFIG_JIG_RBK_FRONT        (0)   // FRONT JIG PROGRAM( FOR RBK )
#define CONFIG_JIG_WOORIM_FRONT     (0)   // FRONT JIG PROFRAM( FOR WOORIM )

#endif  /* __CONFIG_H__ */
