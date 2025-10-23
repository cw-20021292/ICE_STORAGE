
#ifndef _WIFI_SetFunctionList_H_
#define _WIFI_SetFunctionList_H_

/******************************************************************************/
/***** Header *****************************************************************/
/******************************************************************************/
#include "WIFI_Config.h"

/******************************************************************************/
/***** Function ***************************************************************/
/******************************************************************************/
void* GetWifiFuncDataEventList ( void );
U16 GetWifiFuncEventListSize ( void );

/******************************************************************************/
/***** Define *****************************************************************/
/******************************************************************************/

typedef enum
{
    WIFI_FUNC_0001_POWER_SEL = 0x0001,	//	���� ����
    WIFI_FUNC_0002_COLD_SEL,    		//	�ü� ���� (��)
    WIFI_FUNC_0003_HOT_LOCK_SEL,		//	�¼� ��� ���� (��)
    WIFI_FUNC_0004_SAVE_SEL,    		//	���� ����
    WIFI_FUNC_0005_LOCK_SEL,    		//	��� ���� (��)
    WIFI_FUNC_0006_MUTE_SEL,    		//	���� ���� (��)
    WIFI_FUNC_0007_STERILIZE_SEL,   	//	��� ���� ����
    WIFI_FUNC_0008_ICE_SEL,     		//	���� ����
    WIFI_FUNC_0009_HOT_TEMP,    		//	�¼� �µ� ǥ��
    WIFI_FUNC_000A_COLD_TEMP,   		//	�ü� �µ� ǥ�� Ȯ��
    WIFI_FUNC_000B_WATER_SEL,   		//	�� ���� (��)
    WIFI_FUNC_000C_USER_SEL,    		//	�����
    WIFI_FUNC_000D_WATER_OUTQUANTITY,	//	���� ���� (��)
    WIFI_FUNC_000E_TIME_YEAR,       	//	���� �ð� - ��
    WIFI_FUNC_000F_TIME_MONTHDAY,   	//	���� �ð� - ����
    WIFI_FUNC_0010_TIME_MINSEC,     	//	���� �ð� - �ú�
    WIFI_FUNC_0011_STERILIZE_CYCLE, 	//	��� �ֱ�
    WIFI_FUNC_0012_STERILIZE_MODE,  	//	��� �ֱ� ���
    WIFI_FUNC_0013_STERILIZE_TIME,  	//	��� �ð�����-�ú�
    WIFI_FUNC_0014_STERILIZE_STATUS, 	//	��� ���� ����
    WIFI_FUNC_0015_TDS_IN,  			//	�Լ� TDS
    WIFI_FUNC_0016_TDS_OUT, 			//	��� TDS
    WIFI_FUNC_0017_WATER_PRESSURE,  	//	�����
    WIFI_FUNC_0018_TDS_INTEMP,  		//	TDS �Լ� �µ�
    WIFI_FUNC_0019_TDS_OUTTEMP, 		//	TDS ��� �µ�
    WIFI_FUNC_001A_HOT_INTEMP,  		//	���� �¼� ���� �µ� (��)
    WIFI_FUNC_001B_HOT_MEANTEMP,		//	���� �¼� ��� �µ� (��)
    WIFI_FUNC_001C_HOT_SELTEMP, 		//	�¼� ���� �µ� (��)
    WIFI_FUNC_001D_WATER_INQUANTITY,	//	�Լ� ����
    WIFI_FUNC_001E_AUTODRAIN_SEL,   	//	�ڵ� ��� ����
    WIFI_FUNC_001F_DRAIN_STATUS,     	//	��� ����
    WIFI_FUNC_0020_WATER_REQUANTITY,	//	�� ���� ����
    WIFI_FUNC_0021_HEART_TIME1, 		//	��Ʈ ���񽺽ð� 1
    WIFI_FUNC_0022_HEART_TIME2, 		//	��Ʈ ���񽺽ð� 2
    WIFI_FUNC_0023_HEART_TIME3, 		//	��Ʈ ���񽺽ð� 3
    WIFI_FUNC_0024_HEART_TIME4, 		//	��Ʈ ���񽺽ð� 4 (��)
    WIFI_FUNC_0025_FILTER_NOTICE1,  	//	����1 ��ȯ�˸�
    WIFI_FUNC_0026_FILTER_NOTICE2,		//	����2 ��ȯ�˸�
    WIFI_FUNC_0027_FILTER_NOTICE3,		//	����3 ��ȯ�˸�
    WIFI_FUNC_0028_FILTER_NOTICE4,		//	����4 ��ȯ�˸�
    WIFI_FUNC_0029_SERVICE_TIME,    	//	���� �����
    WIFI_FUNC_002A_REMOVE_STATE,    	//	������ ����
    WIFI_FUNC_002B_SAVE_DETECT,     	//	���� ���ԡ�
    WIFI_FUNC_002C_SILVER_CARE,     	//	�ǹ� �ɾ� (��)
    WIFI_FUNC_002D_DRAIN_QUANTITY,  	//	��� ����
    WIFI_FUNC_002E_STERILIZE_QUANTITY,  //	��� ����
    WIFI_FUNC_002F_SPARKLING_SEL,   	//	ź��� ����
    WIFI_FUNC_0030_ICE_MAKETIME,    	//	���� ���� �ð�
    WIFI_FUNC_0031_VOICE_SEL,   		//	���� �ȳ� ����
    WIFI_FUNC_0032_ICELOCK_SEL, 		//	���� ��� ����
    WIFI_FUNC_0033_VOICE_VOL_SEL,    	//	���� ���� ����
    WIFI_FUNC_0034_LIGHT_SEL,   		//	����Ʈ ����
    WIFI_FUNC_0035_COVER1_OPEN, 		//	Ŀ��1 ���� (��)
    WIFI_FUNC_0036_COVER2_OPEN, 		//	Ŀ��2 ����
    WIFI_FUNC_0037_SLEEP_SEL,  			//	���� ���� (��)
    WIFI_FUNC_0038_MASH_CLEAN,  		//	�޽� ���� ��ô �˸�
    WIFI_FUNC_0039_HOT_SEL,     		//	�¼� ����
    WIFI_FUNC_003A_FREEZE_BURST,		//	���� ���� �˸�
    WIFI_FUNC_003B_HOT_TEMP_SEL,    	//	���� �µ� ��� ����
    WIFI_FUNC_003C_COLD_STRENGTH_SEL,   //	�ü� �� ����
    WIFI_FUNC_003D_HEIGHT_SEL,          // 	�� ����
    WIFI_FUNC_003E_WATER_COURSE_STATUS, // 	���� ��� ���� ���� (��)
    WIFI_FUNC_003F_WATER_COURSE_RESET,  // 	���� ��� ���� (��)
    WIFI_FUNC_0040_SMART_CHECK_RQST,    // 	����Ʈ ���� ��� ���� (��)
    WIFI_FUNC_0041_SMART_CHECK_STATUS,  //	����Ʈ ���� ���� (��)
    WIFI_FUNC_0042_SMART_CHECK_PROGRESS,//	����Ʈ ���� ����� (��)
    WIFI_FUNC_0043_FILTER_STATUS1,      //	����1 ���� ���� (��)
    WIFI_FUNC_0044_FILTER_STATUS2,		//	����2 ���� ����
    WIFI_FUNC_0045_FILTER_STATUS3,		//	����3 ���� ����
    WIFI_FUNC_0046_FILTER_STATUS4,		//	����4 ���� ����
    WIFI_FUNC_0047_WATER_QUANTITY_SEL,  // 	���� ���ⷮ
    WIFI_FUNC_0048_HOT_TEMP_SEL,		//	���� �¼� �µ� ����
    WIFI_FUNC_0049_OUT_BUTTON_LIGHT,	//	����� (��)
    WIFI_FUNC_004A_WELCOME_LIGHT,		//	���� ����Ʈ
    WIFI_FUNC_004B_HIDDEN_LCD_SEL,		//	���� ȭ�� ����
    WIFI_FUNC_004C_FAUCET_MOVE_SEL,		//	�Ŀ�� ���� ����
    WIFI_FUNC_004D_FAUCET_MOVE_STATUS,	//	�Ŀ�� ���� ����
    WIFI_FUNC_004E_CUP_SENSING_SEL,		//	�� ���� ����
    WIFI_FUNC_004F_SOUND_VOL_SEL,		//	ȿ���� ���� ����
    WIFI_FUNC_0050_UV_STERILIZE_STATUS,	//	UV ��� ���� ���� - ����
    WIFI_FUNC_0051_QUANTITY_USE_SEL,	//	���� ���ⷮ ��� ����
    WIFI_FUNC_0052_VOICE_MODE_SEL,		//	���� ���
    WIFI_FUNC_0053_FILTER_RESET1,		//	����1 ���� (��)
    WIFI_FUNC_0054_FILTER_RESET2,		//	����2 ����
    WIFI_FUNC_0055_FILTER_RESET3,		//	����3 ����
    WIFI_FUNC_0056_FILTER_RESET4,		//	����4 ����
    WIFI_FUNC_0057_CUP_SENSING_SENSITIVITY,		//	�� ���� �ΰ��� ����
    WIFI_FUNC_0058_ICE_SIZE_SEL,				//	���̽� ������
    WIFI_FUNC_0059_ICE_ONOFF_SEL,				//	���� On/Off
    WIFI_FUNC_005A_UV_ICE_TANK_STATUS,			//	���� ��ũ ��� ���� ����
    WIFI_FUNC_005B_UV_ICE_TRAY_STATUS,			//	���� Ʈ���� ��� ���� ����
    WIFI_FUNC_005C_SMART_CHECK_RESULT,			//	����Ʈ ���� ��� ���� (��)
    WIFI_FUNC_005D_UV_ICE_FAUCET_STATUS,		//	���� �Ŀ�� ��� ���� ����
    WIFI_FUNC_005E_SLEEP_MODE_SEL,				//	��ħ��� ����
    WIFI_FUNC_005F_SLEEP_START_HOUR,			//	��ħ��� ���� �ð�
    WIFI_FUNC_0060_SLEEP_START_MIN,				//	��ħ��� ���� ��
    WIFI_FUNC_0061_SLEEP_END_HOUR,				//	��ħ��� ���� �ð�
    WIFI_FUNC_0062_SLEEP_END_MIN,				//	��ħ��� ���� ��
    WIFI_FUNC_0063_DEFAULT_QUANTITY_ML_SEL,		//	�⺻ �뷮 ����
    WIFI_FUNC_0064_STERILIZE_EXPECTED_TIME,		//	EW ��� �ҿ� ���� �ð�
    WIFI_FUNC_0065_DEFAULT_TEMP_SEL,			//	�⺻ �¼� �µ� ���� ���� (��)
    WIFI_FUNC_0066_DEFAULT_QUANTITY_SEL,		//	�⺻ ���� ���� ���� ���� (��)
    WIFI_FUNC_0067_WATER_QUANTITY_USE_SEL0,		//	4oz ���� ��� ���� ���� (��)
    WIFI_FUNC_0068_WATER_QUANTITY_USE_SEL1,		//	6oz ���� ��� ���� ���� (��)
    WIFI_FUNC_0069_WATER_QUANTITY_USE_SEL2, 	//	8oz ���� ��� ���� ���� (��)
    WIFI_FUNC_006A_WATER_QUANTITY_USE_SEL3, 	//	10oz ���� ��� ���� ���� (��)
    WIFI_FUNC_006B_WATER_QUANTITY_USE_SEL4, 	//	12oz ���� ��� ���� ���� (��)
    WIFI_FUNC_006C_WATER_QUANTITY_USE_SEL5, 	//	14oz ���� ��� ���� ���� (��)
    WIFI_FUNC_006D_WATER_QUANTITY_USE_SEL6, 	//	16oz ���� ��� ���� ���� (��)
    WIFI_FUNC_006E_WATER_QUANTITY_USE_SEL7, 	//	20oz ���� ��� ���� ���� (��)
    WIFI_FUNC_006F_WATER_QUANTITY_USE_SEL8, 	//	32oz ���� ��� ���� ���� (��)
    WIFI_FUNC_0070_WATER_QUANTITY_USE_SEL9,		//	���� ���� ��� ���� ���� (��)
    WIFI_FUNC_0071_WATER_QUANTITY_USE_SEL10,	//	reserve2 ���� ��� ���� ����
    WIFI_FUNC_0072_WATER_QUANTITY_SORT_SEL,		//	���� ���� ���� ���� (��)
    WIFI_FUNC_0073_WATER_TEMP_SORT_SEL,			//	�¼� �µ� ���� ���� ���� (��)
    WIFI_FUNC_0074_UNUSE_POWER_SAVING_SEL,		//	�̻�� ���� ����
    WIFI_FUNC_0075_TEMP_UNIT_SEL,				//	�µ� ���� ǥ�� ���� ���� (��)
    WIFI_FUNC_0076_WATER_QUANTITY_UNIT_SEL,		//	���� ���� ���� ǥ�� ���� ���� (��)
    WIFI_FUNC_0077_HOT_TARGET_TEMP_TIME,        // 	�¼� ��ǥ �µ� ���� �ð�
    WIFI_FUNC_0078_COLD_TARGET_TEMP_TIME,       // 	�ü� ��ǥ �µ� ���� �ð�
    WIFI_FUNC_0079_MIXED_WATER_AVE_TEMP,		//	ȥ�� ���� ��� �µ�
    WIFI_FUNC_007A_MIXED_WATER_MIN_TEMP,		//	ȥ�� ���� �ּ� �µ�
    WIFI_FUNC_007B_MIXED_WATER_MAX_TEMP,		//	ȥ�� ���� �ִ� �µ�
    WIFI_FUNC_007C_COLDTANK_LOW_WATERLEVEL,		//	�ü� ��ũ ������ ����
    WIFI_FUNC_007D_COLDTANK_HIGH_WATERLEVEL,	//	�ü� ��ũ ������ ����
    WIFI_FUNC_007E_HIGH_TEMP_STL_SEL,			//	��¼� ��� ����
    // WIFI_FUNC_007F_HIGH_TEMP_STL_STATUS,		//	��¼� ��� ����
    WIFI_FUNC_007F_DEFAULT_TEMP_SET,            //  �⺻�¼��µ�����(�µ���)

    WIFI_FUNC_0080_HIGH_TEMP_STL_TIME,			//	��¼� ��� ���� �ð�
    WIFI_FUNC_0081_HIGH_TEMP_STL_TIME_M,		//	��¼� ��� ���� �ð� (��)
    WIFI_FUNC_0082_UNUSE_POW_SAVING_STATUS,		//	�̻�� ���� ���� ����
    WIFI_FUNC_0083_POWER_SAVING_STATUS,			//	���� ���� ����
    WIFI_FUNC_0084_HOT_RESTARTING_TEMP,			//	�¼� ��⵿ �µ�
    WIFI_FUNC_0085_HOT_STOP_TEMP,				//	�¼� ���� �µ�
    WIFI_FUNC_0086_COLD_RESTARTING_TEMP,		//	�ü� ��⵿ �µ�
    WIFI_FUNC_0087_COLD_STOP_TEMP,				//	�ü� ���� �µ�
    WIFI_FUNC_0088_COLD_ADDITIONAL_TIME,    	//	�ü� �ð� �߰� �ð�
    WIFI_FUNC_0089_BOOST_PUMP_STATUS,			//	�ν�Ʈ ���� ���� ����
    WIFI_FUNC_008A_COLDTANK_UV_STL_STATUS,		//	�ü� ��ũ ��� ���� ���� (UV)
    WIFI_FUNC_008B_HEATER_STATUS,				//	���� ���� ���� (������)
    WIFI_FUNC_008C_COMP_STATUS,					//	COMP ���� ����
    WIFI_FUNC_008D_FILTER_WATER_USAGE,			//	���� �� ��뷮
    WIFI_FUNC_008E_FILTER_REMAINING_DAY,		//	���� �ܿ� ����� (�ð�)
    WIFI_FUNC_008F_ICE_FULL_STATUS,				//	���� ���� ����

    WIFI_FUNC_0090_ICE_TRAY_POSITION_STATUS,	//	���� Ʈ���� ��ġ ����
    WIFI_FUNC_0091_REFRIGERANT_SWC_STATUS,		//	�ø� ��ȯ ��� ����
    WIFI_FUNC_0092_DEICING_STAUTS,				//	Ż�� ��� ����
    WIFI_FUNC_0093_ICEMAKING_STAUTS,			//	���� ���� ����
    WIFI_FUNC_0094_DUMMY_DEICING_STAUTS,		//	���� Ż�� ���� ����
    WIFI_FUNC_0095_ICEMAKING_COMPLET_TIME,  	// 	���� �Ϸ� �ð�
    WIFI_FUNC_0096_DEICING_COMPLET_TIME,    	// 	Ż�� �Ϸ� �ð�
    WIFI_FUNC_0097_TARY_WATER_COMPLET_TIME, 	// 	Ʈ���� �Լ� �Ϸ� �ð�
    WIFI_FUNC_0098_TARY_WATER_QUANTITY,     	//	Ʈ���� �Լ� ����
    WIFI_FUNC_0099_COMP_TARGET_RPS,				//	����� ��ǥ RPS
    WIFI_FUNC_009A_COMP_CURRENT_RPS,			//	����� ���� RPS
    WIFI_FUNC_009B_DRAINTANK_LOW_WLEVEL,		//	�巹�� ��ũ ������ ���� ����
    WIFI_FUNC_009C_DRAINTANK_HIGH_WLEVEL,		//	�巹�� ��ũ ������ ���� ����
    WIFI_FUNC_009D_DRAINTANK_DRAIN_STAUTS,		//	�巹�� ��� ���� ����
    WIFI_FUNC_009E_COLD_EVA_TEMP,				//	�� ���� ���߱� �µ� ����
    WIFI_FUNC_009F_ICE_EVA_TEMP,				//	���� ���߱� �µ� ����

    WIFI_FUNC_00A0_COLD_TANK_TEMP,				//	�ü� ��ũ �µ�
    WIFI_FUNC_00A1_HOT_TANK_TEMP,				//	�¼� ��ũ �µ�
    WIFI_FUNC_00A2_CHILD_LOCK,					//	��ü ��ư �ڵ� ��� (��)
    WIFI_FUNC_00A3_FLOWPATH_EMPTY,				//	���� ��� ����
    WIFI_FUNC_00A4_HOTTANK_LOW_W_LEVEL,			//	�¼� ��ũ ������ ����
    WIFI_FUNC_00A5_HOTTANK_HIGH_W_LEVEL,		//	�¼� ��ũ ������ ����
    WIFI_FUNC_00A6_PURETANK_LOW_W_LEVEL,		//	���� ��ũ ������ ����
    WIFI_FUNC_00A7_PURETANK_HIGH_W_LEVEL,		//	���� ��ũ ������ ����
	WIFI_FUNC_00A8_COLDTANK_MID_W_LEVEL,		//	�ü� ��ũ �߼��� ����
	WIFI_FUNC_00A9_UNUSED_TIME,					//	��ǰ �̻�� �ð�
	WIFI_FUNC_00AA_SMART_SAVING_SET_TIME,		//	����Ʈ ���� �̻�� �����ð�
	WIFI_FUNC_00AB_SAMRT_SAVING_STATE,			//	����Ʈ ���� ����
	WIFI_FUNC_00AC_FILTER1_WATER_USAGE,			//	����1 ���� �� ��뷮 (��)
	WIFI_FUNC_00AD_FILTER1_UASGE_DAY,			//	����1 �ܿ� ��� �ð� (��)
	WIFI_FUNC_00AE_FILTER2_WATER_USAGE,			//	����2 ���� �� ��뷮
	WIFI_FUNC_00AF_FILTER2_UASGE_DAY,			//	����2 �ܿ� ��� �ð�
	WIFI_FUNC_00B0_FILTER3_WATER_USAGE,			//	����3 ���� �� ��뷮
	WIFI_FUNC_00B1_FILTER3_UASGE_DAY,			//	����3 �ܿ� ��� �ð�
	WIFI_FUNC_00B2_FILTER_NOTICE1,				//	���� ��ü �˸� (��)
	WIFI_FUNC_00B3_FILTER_NOTICE2,				//	���� ��ü �˸� (��)
	WIFI_FUNC_00B4_FILTER_NOTICE3,				//	���� ��ü �˸� (��)
    WIFI_FUNC_00B5_FILTER_FLUSHING,             //  ���� �÷��� ����
    WIFI_FUNC_00B6_VOICE_LANGUAGE,              //  ��������
    WIFI_FUNC_00B7_FILTER_SEQ,                  //  ���ͱ�ü����
    WIFI_FUNC_00B8_FILTER1_DDAY,                //  ����1 ��ü ������
    WIFI_FUNC_00B9_FILTER2_DDAY,                //  ����2 ��ü ������
    WIFI_FUNC_00BA_FILTER3_DDAY,                //  ����3 ��ü ������
    WIFI_FUNC_00BB_FLOWPATH_STL_STATUS,         //  ���� ��¼� ��ջ���
    WIFI_FUNC_00BC_FAUCE_STL_STATUS,            //  �Ŀ�� ��¼� ��ջ���
    WIFI_FUNC_00BD_ICETANK_STL_STATUS,          //  ������ũ ��¼� ��ջ���
    WIFI_FUNC_00BE_QUANTITY_HALFCUP,            //  ���� �뷮
    WIFI_FUNC_00BF_QUANTITY_ONECUP,             //  ���� �뷮

    WIFI_FUNC_00C0_QUANTITY_TWOCUPS,            //  ���� �뷮
    WIFI_FUNC_00C1_HOTTEMP_USE_SEL0,            //  �¼��µ� ��� ���� 0 (45)
    WIFI_FUNC_00C2_HOTTEMP_USE_SEL1,            //  �¼��µ� ��� ���� 1 (50)
    WIFI_FUNC_00C3_HOTTEMP_USE_SEL2,            //  �¼��µ� ��� ���� 2 (55)
    WIFI_FUNC_00C4_HOTTEMP_USE_SEL3,            //  �¼��µ� ��� ���� 3 (60)
    WIFI_FUNC_00C5_HOTTEMP_USE_SEL4,            //  �¼��µ� ��� ���� 4 (65)
    WIFI_FUNC_00C6_HOTTEMP_USE_SEL5,            //  �¼��µ� ��� ���� 5 (70)
    WIFI_FUNC_00C7_HOTTEMP_USE_SEL6,            //  �¼��µ� ��� ���� 6 (75)
    WIFI_FUNC_00C8_HOTTEMP_USE_SEL7,            //  �¼��µ� ��� ���� 7 (80)
    WIFI_FUNC_00C9_HOTTEMP_USE_SEL8,            //  �¼��µ� ��� ���� 8 (85)
    WIFI_FUNC_00CA_HOTTEMP_USE_SEL9,            //  �¼��µ� ��� ���� 9 (100)
    WIFI_FUNC_00CB_WATER_EXTRACT_STATUS,        //  �� �������
    WIFI_FUNC_00CC_ICE_EXTRACT_STATUS,          //  ���� �������
    WIFI_FUNC_00CD_SETUP_MODE_STATUS,           //  ��ǰ ���� ����

    WIFI_FUNC_00CE_HOTTEMP_USE_SEL10,           //  �¼��µ� ��� ���� 10
    WIFI_FUNC_00CF_HOTTEMP_USE_SEL11,           //  �¼��µ� ��� ���� 11
    
    WIFI_FUNC_00D0_UV_ICE_TANK_STATUS2,			//	���� ��ũ ��� ���� ����2
    WIFI_FUNC_00D1_ALTITUDE_AUTO,               //  ����� �ڵ� ����
    WIFI_FUNC_00D2_ALARM_1_SET,                  //  �˶��ð� 1 ����
    WIFI_FUNC_00D3_ALARM_2_SET,                  //  �˶��ð� 2 ����
    WIFI_FUNC_00D4_ALARM_3_SET,                  //  �˶��ð� 3 ����
    WIFI_FUNC_00D5_ALARM_1_START_HOUR,           //  �˶��ð� 1 ���۽ð� �� ����
    WIFI_FUNC_00D6_ALARM_2_START_HOUR,           //  �˶��ð� 2 ���۽ð� �� ����
    WIFI_FUNC_00D7_ALARM_3_START_HOUR,           //  �˶��ð� 3 ���۽ð� �� ����
    WIFI_FUNC_00D8_ALARM_1_START_MIN,            //  �˶��ð� 1 ���۽ð� �� ����
    WIFI_FUNC_00D9_ALARM_2_START_MIN,            //  �˶��ð� 2 ���۽ð� �� ����
    WIFI_FUNC_00DA_ALARM_3_START_MIN,            //  �˶��ð� 3 ���۽ð� �� ����
    WIFI_FUNC_00DB_ALARM_1_SOUND,                //  �˶��ð� 1 �˶��� ����
    WIFI_FUNC_00DC_ALARM_2_SOUND,                //  �˶��ð� 2 �˶��� ����
    WIFI_FUNC_00DD_ALARM_3_SOUND,                //  �˶��ð� 3 �˶��� ����
    WIFI_FUNC_00DE_HOME_SCREEN_MODE,             //  �⺻Ȩ ��� ���� (���� / ������ / ���̿���)
    WIFI_FUNC_00DF_STANBY_SCREEN,                //  ���ȭ�� (����, �ð�, ���X)

    WIFI_FUNC_00E0_SCREEN_BRIGHTNESS,            //  ��� ����
    WIFI_FUNC_00E1_MYWATER_TEMP_1,               //  ���̿���1 �µ�
    WIFI_FUNC_00E2_MYWATER_QUANTITY_1,           //  ���̿���1 �뷮
    WIFI_FUNC_00E3_MYWATER_RECIPE_1,             //  ���̿���1 ������
    WIFI_FUNC_00E4_MYWATER_TEMP_2,               //  ���̿���2 �µ�
    WIFI_FUNC_00E5_MYWATER_QUANTITY_2,           //  ���̿���2 �뷮
    WIFI_FUNC_00E6_MYWATER_RECIPE_2,             //  ���̿���2 ������
    WIFI_FUNC_00E7_MYWATER_TEMP_3,               //  ���̿���3 �µ�
    WIFI_FUNC_00E8_MYWATER_QUANTITY_3,           //  ���̿���3 �뷮
    WIFI_FUNC_00E9_MYWATER_RECIPE_3,             //  ���̿���3 ������
    WIFI_FUNC_00EA_MYWATER_TEMP_4,               //  ���̿���4 �µ�
    WIFI_FUNC_00EB_MYWATER_QUANTITY_4,           //  ���̿���4 �뷮
    WIFI_FUNC_00EC_MYWATER_RECIPE_4,             //  ���̿���4 ������
    WIFI_FUNC_00ED_MYWATER_TEMP_5,               //  ���̿���5 �µ�
    WIFI_FUNC_00EE_MYWATER_QUANTITY_5,           //  ���̿���5 �뷮
    WIFI_FUNC_00EF_MYWATER_RECIPE_5,             //  ���̿���5 ������

    WIFI_FUNC_00F0_MYWATER_TEMP_6,               //  ���̿���6 �µ�
    WIFI_FUNC_00F1_MYWATER_QUANTITY_6,           //  ���̿���6 �뷮
    WIFI_FUNC_00F2_MYWATER_RECIPE_6,             //  ���̿���6 ������
    WIFI_FUNC_00F3_MYWATER_TEMP_7,               //  ���̿���7 �µ�
    WIFI_FUNC_00F4_MYWATER_QUANTITY_7,           //  ���̿���7 �뷮
    WIFI_FUNC_00F5_MYWATER_RECIPE_7,             //  ���̿���7 ������
    WIFI_FUNC_00F6_MYWATER_TEMP_8,               //  ���̿���8 �µ�
    WIFI_FUNC_00F7_MYWATER_QUANTITY_8,           //  ���̿���8 �뷮
    WIFI_FUNC_00F8_MYWATER_RECIPE_8,             //  ���̿���8 ������
    WIFI_FUNC_00F9_MYWATER_TEMP_9,               //  ���̿���9 �µ�
    WIFI_FUNC_00FA_MYWATER_QUANTITY_9,           //  ���̿���9 �뷮
    WIFI_FUNC_00FB_MYWATER_RECIPE_9,             //  ���̿���9 ������
    WIFI_FUNC_00FC_MYWATER_TEMP_10,               //  ���̿���10 �µ�
    WIFI_FUNC_00FD_MYWATER_QUANTITY_10,           //  ���̿���10 �뷮
    WIFI_FUNC_00FE_MYWATER_RECIPE_10,             //  ���̿���10 ������

    WIFI_FUNC_NUM_1

}E_API_A1010_T;

typedef enum
{
    WIFI_FUNC_00FF_CLEANING_STATUS = WIFI_FUNC_NUM_1,              //  ������ �������� 
    
    WIFI_FUNC_0100_CLEANING_SW_STATUS,              //  ������ ���� ����ġ ���� ����
    WIFI_FUNC_0101_STERILIZE_REMAINING_DAY,         //  ��ս��� �ܿ���
    WIFI_FUNC_0102_SCREEN_SAVER,                    //  ��ũ�����̹� ���� ON/OFF
    WIFI_FUNC_0103_SCREEN_SAVER_START,              //  ��ũ�����̹� ���� �ð� (��)
    WIFI_FUNC_0104_SCREEN_SAVER_END,                //  ��ũ�����̹� ���� �ð� (��)
    WIFI_FUNC_0105_MAIN_THEME,                      //  ����ȭ�� �׸� 0 Ŭ���� / 1 ����ó
    WIFI_FUNC_0106_ADJUSTMENT_THEME,                //  �̼����� �׸� 0 ���� / 1 ��ư��
    WIFI_FUNC_0107_MYWATER_ENABLE_1,                //  MYWATER ��뿩�� 0(�̻��) / 1(���)
    WIFI_FUNC_0108_MYWATER_ENABLE_2,                //  MYWATER ��뿩�� 0(�̻��) / 1(���)
    WIFI_FUNC_0109_MYWATER_ENABLE_3,                //  MYWATER ��뿩�� 0(�̻��) / 1(���)
    WIFI_FUNC_010A_MYWATER_ENABLE_4,                //  MYWATER ��뿩�� 0(�̻��) / 1(���)
    WIFI_FUNC_010B_MYWATER_ENABLE_5,                //  MYWATER ��뿩�� 0(�̻��) / 1(���)
    WIFI_FUNC_010C_MYWATER_ENABLE_6,                //  MYWATER ��뿩�� 0(�̻��) / 1(���)
    WIFI_FUNC_010D_MYWATER_ENABLE_7,                //  MYWATER ��뿩�� 0(�̻��) / 1(���)
    WIFI_FUNC_010E_MYWATER_ENABLE_8,                //  MYWATER ��뿩�� 0(�̻��) / 1(���)
    WIFI_FUNC_010F_MYWATER_ENABLE_9,                //  MYWATER ��뿩�� 0(�̻��) / 1(���)
    WIFI_FUNC_0110_MYWATER_ENABLE_10,               //  MYWATER ��뿩�� 0(�̻��) / 1(���)

    WIFI_FUNC_0111_WATER_COURSE_UASGE_DAY,          //  ������ �ܿ������ (12����)

    WIFI_FUNC_NUM_2

}E_API_A1010_T_2;

#endif


