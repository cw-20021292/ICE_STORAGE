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
 * ���α׷� ���� ǥ��
 * 1) ù��° �ڸ� : 1 - ��� ����, 0 - �׽�Ʈ ���� (DPP, LPP)
 * 2) �ι�° �ڸ� : 0 - ��� ����, 1 - DPP, 2 - LPP
 * 3) �׹�° �ڸ� : ���� ǥ�� 0-16
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

/// ���� ���� ���� ī��Ʈ �ʱ�ȭ�� ���� ������ �ɼ�
#define TANK_RO              (0)
#define TANK_NT              (1)
#define CONFIG_TANK_TYPE     TANK_RO

/**
 * Feature
**/
/* Fota */
#define  CONFIG_FOTA_LIB          (1)    /// FOTA ��� Ȱ��ȭ( ǥ�� ���̺귯�� xxx_f ���) 

/* Flushing */
#define CONFIG_CANCEL_FLUSHING    (1)    /// Flushing ����� �Ϲݸ��� ���� Ȱ��ȭ ( 0 : Disable, 1: Enable )
#define CONFIG_ENABLE_ERROR_SOUND (0)    /// Flushing ���¿��� ��Ȱ��ȭ�� ��ư push �� ������ ���� (0 : Disable, 1: Enable)

/* Error */
#define CONFIG_USE_ERROR            (1)  /// Error �Ǵ� ���� ���� ���� ( 0 : Disable, 1 : Enable )

#define CONFIG_LEAK                 (0)  /// 0: ����, 1: ����
#define CONFIG_GAS_SW               (0)  /// 0: ����, 1: ����
#define CONFIG_CIRCULATE_PUMP       (1)  /// 0: ����, 1: ����

/* Voice */
#define CONFIG_VOICE                (1)  /// 0 : BUZZER, 1 : VOICE 

/* RTC �׽�Ʈ - 1sec */
#define  CONFIG_RTC_TEST        (0)   /// 0: 10�� �ֱ� ���� 1: 1�� �ֱ� ����

/* Debug */
#define CONFIG_DEBUG_COMM       (0)   /// Communication Debug Mode ( 0 : Normal, 1: Debug )

/* Ster */
#define CONFIG_STER                     (0)  /// 0: ����, 1: ����
#define CONFIG_IMPROVED_FLOW_PATH_STER  (0)  /// 0 : LPP, 1 : ���� ����
#define CONFIG_STER_TIME_COUNT          (0)  /// 0: ��� �ð� ī�� Disable, 1 : ��� �ð� ī��Ʈ Enable

#if CONFIG_STER 
   #define CONFIG_USE_CIRCULATE_PUMP   (1)   /// ����� ���� ����
   #define CONFIG_USE_DRAIN_TIMEOUT    (1)   /// ��� �ִ� �ð�, timeout�Ǹ� ��� ���� ���� ����
#else
   #define CONFIG_USE_CIRCULATE_PUMP   (1)   /// ����� ���� ����
   #define CONFIG_USE_DRAIN_TIMEOUT    (1)   /// ��� �ִ� �ð�, timeout�Ǹ� ��� ���� ���� ����
#endif

/* Ice / Cold */
#define CONFIG_ENABLE_DEICING_COOLING        (0)  /// ���� �켱 ��忡�� Ż�� �� �ð� ��� Ȱ��ȭ, 0: ����, 1: ����
#define CONFIG_ENABLE_ICON_AIS_ICETRAY_TEST  (0)  /// ���̽� Ʈ���� ���� ��� ��� ����Ī 0: IGLOO, 1:����Ű
#define CONFIG_ICETRAY_ICING_AT_KEYPBA_TEST  (0)  /// KEY PBA ���� �˻� ��, ������ġ�� �̵�,  0 : Disable, 1 : Enable */

#define CONFIG_SET_ICE_MODE                  (0)  /// ���� ��� ����

#define CONFIG_BACK_FEEDER                   (0)  /// �Ǵ����� ��ȸ��

/* ���� -> �ð� ��ȯ �� Comp Off �� ���� ���� ( �ð� -> ���� ��ȯ �� TurnOffComp(COMP_TYPE_COLD); �ϹǷ� �׻� OFF ��) */
#define CONFIG_ICE_COLD_COMP                 (1)  /// ���� -> �ð� ��ȯ �� Comp Off �� �����ϵ��� ��

#define CONFIG_USE_ICING_MAX_COUNT           (1)  /// ���� ���� MAX COUNT 10ȸ �ÿ��� �ü� �Լ� ��Ŵ

#define CONFIG_MAKE_MORE                     (1)  /// ���� x�� �� �����

/* Iceout */
#define CONFIG_USE_CONTROL_ICE_FEEDER        (0)  /// ���� ���� �� �Ǵ� ���� �Լ� Ȱ�� ( 0 : Disable, 1 : Enable )

/* Waterout */
#define CONFIG_HOT_WATER_AIRFLOW             (0)  /// �¼� ���� ���� ��� Ȱ��ȭ
#define CONFIG_USE_FLOW_METER_AT_ICE_WATER   (1)  /// ������ ���� �� �������� Ȱ�� ( 0 ; Disable, 1 : Enable )

/* Test */
#define CONFIG_TEST                  (0)  /// �õ�, Skip �����, ����Ż�� ��� Ȱ��ȭ
#define CONFIG_TEST_8585             (0)  /// ������ - ������α׷� - �ŷڼ� ������ �׽�Ʈ ( �ü� ���� ��� )
#define CONFIG_TEST_8585_2           (0)  /// ������ - ���� / �ü� / �¼� ���� ���
#define CONFIG_TEST_8585_ICE         (0)  /// ������ - ������α׷� - ���� ���� ���
#define CONFIG_TEST_8585_ICE_WATER   (0)  /// ������ - ������α׷� - ������ ���� ���
#define CONFIG_TEST_8585_FEEDER      (0)  /// ������ - �Ǵ� ��ũ�� ������ ( ��ȸ�� )
#define CONFIG_TEST_8585_BACK_FEEDER (0)  /// ������ - �Ǵ� ��ũ�� ������ ( ��ȸ�� )
#define CONFIG_TEST_8585_HOT_REPEAT  (0)  /// ������ - �¼� �ݺ� ���� ���

#define CONFIG_TEST_HOT_WATER       (0)  /// �ŷڼ�, �¼� ���� ���� �׽�Ʈ ���α׷� 
#define CONFIG_TEST_GAS_SWITCH      (0)  /// �ø� ��ȯ ��� �̵� �ݺ� �׽�Ʈ 
#define CONFIG_TEST_HOTGAS_SWITCH   (0)  /// �ְ��� �ø� ��ȯ ��� �̵� �ݺ� �׽�Ʈ 
#define CONFIG_TEST_MMI             (0)  /// MMI ��� Ȱ��ȭ
#define CONFIG_TEST_TEMP_READ       (0)  /// �µ� ���� ��� �б� ��� ( �׽�Ʈ ��� )
#define CONFIG_TEST_UV_MANUAL       (0)  /// UV ���� �׽�Ʈ

#define CONFIG_TEST_CHECK_FRONT     (0)


#define CONFIG_TEST_FLOW_METER      (0)   /// Flow meter Auto Counting

/* Jig */
#define CONFIG_JIG_RBK_FRONT        (0)   // FRONT JIG PROGRAM( FOR RBK )
#define CONFIG_JIG_WOORIM_FRONT     (0)   // FRONT JIG PROFRAM( FOR WOORIM )

#endif  /* __CONFIG_H__ */
