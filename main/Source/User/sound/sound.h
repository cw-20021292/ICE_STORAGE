#ifndef __SOUND_H__
#define __SOUND_H__

#include "prj_type.h"


typedef enum
{
    SOUND_POWER_OFF,          	/* Power Off (��ǰ ���� ) */
    SOUND_POWER_ON,             	/* Power On (��ǰ ����) */

    SOUND_EFFLUENT,             	/* ���� ���� */
    SOUND_EFFLUENT_END,         	/* ���� �Ϸ� */

    SOUND_CONFIG_SETUP,         	/* �ð�/�¼� on */
    SOUND_CONFIG_CANCEL,        	/* �ð�/�¼� off */

    SOUND_SELECT,               	/* ���� (��/��/���� �� ���ⷮ ���� �� ) */
    SOUND_ERROR,                	/* ȿ���� _ ��� */

    SOUND_SETUP,                	/* �ΰ� ��� ���� */
    SOUND_CANCEL,               	/* �ΰ� ��� ���� */

    SOUND_LOCK_SETUP,           	/* ��� ���� */
    SOUND_LOCK_CANCEL,          	/* ��� Ǯ�� */

    SOUND_EFFLUENT_CONTINUE, 		/* ���� ���� */
    SOUND_STERILIZE_START,   		/* ��� ��� ������ */
    SOUND_STERILIZE_END,     		/* ��� ��� �Ϸ��� */

    SOUND_MEMENTO_1,         		/* �޸��� 1 */
    SOUND_MEMENTO_2,         		/* �޸��� 2 */
    SOUND_MEMENTO_3,         		/* �޸��� 3 */
    SOUND_MEMENTO_4,         		/* �޸��� 4 */
    SOUND_MEMENTO_5,         		/* �޸��� 5 */

    SOUND_WIFI_SELECT,          /* WIFI �� */
    SOUND_WIFI_SETUP,           /* WIFI ON */
    SOUND_WIFI_ERROR,           /* WIFI �Ұ��� */
    SOUND_WIFI_CONNECTING,      /* WIFI ���� �� */
    SOUND_WIFI_CONNECTED,       /* WIFI ���� �Ϸ� */
    SOUND_WIFI_FAIL_CONNECTING, /* WFI ���� ���� */
    SOUND_WIFI_DISCONNECTED,    /* WIFI ���� ���� */

    //SOUND_BLE_START_CONNECTING, /* BLE ���� �� */
    //SOUND_BLE_CONNTED,          /* BLE ���� �Ϸ� */
    SOUND_BLE_FAIL,             /* BLE ���� ���� */

    SOUND_MUTE,                 /* ������ �¾� ���� */
    SOUND_NONE,					        /* ������ �߻� ���� */

    MAX_SOUND_NUM              
}SoundId_T;


/* ���� ���� Ȯ�� */
U8	IsSetSoundMute(void);

/* ���� ����  */
void	SetSoundMute(void);
U8    GetSoundMute(void);

/* ���� ���� */
void	ClearSoundMute(void);

/* �Ҹ� ��� */
void  Sound(SoundId_T mId);

/* ���� �׽�Ʈ */
void TEST_Sound(void);

#endif /* _SOUND_H_ */
