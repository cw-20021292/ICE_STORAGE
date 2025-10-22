
#ifndef __ICE_MAKE_H__
#define __ICE_MAKE_H__

#include "r_smc_entry.h"
#include "Type_Define.h"

typedef enum {
    ICE_MODE_DUMMY_DETACH = 0x01,
    ICE_MODE_INIT = 0x02,
    ICE_MODE_PREHEAT = 0x04,
    ICE_MODE_READY = 0x08,
    ICE_MODE_MAKE = 0x10,
    ICE_MODE_DETACH = 0x20,
    ICE_MODE_TRAY_RETRY = 0x40,     // �Ⱦ�����

    // ICE_MODE_INIT = 0x40,
    // ICE_MODE_INIT = 0x80,
} ICE_MODE;

typedef enum {
    ICE_TRAY_DOWN,          // Ʈ���� Ż�� �̵�
    ICE_TRAY_DOWN_CHECK,    // Ʈ���� Ż�� �̵��Ϸ� Ȯ�� (Ʈ���� �̵����� üũ)
    COMP_STATUS_CHECK,      // �������� ���� ���� ���� Ȯ��
    ICE_SWITCH,             // �ø� ���� ��ġ �̵�
    ICE_MELTING,            // ���� ���̱� �ð�����
    HOT_GAS_SWITCH,         // �ø� �ְ����� �̵�
    HOT_GAS_HZ_SET,         // �ְ��� HZ ����
    HOT_GAS_ON,             // �ְ��� ON (�ð� ����)
    DUMMY_DETACH_COMPLETE   // ����Ż�� ��    
} ICE_DUMMY_DETACH_STEP;

/* INIT */
typedef enum
{
    STEP_INIT_TRAY_MOVE,        // Ʈ���� ���� ��ġ�� �̵�
    STEP_INIT_TRAY_DEICING,     // Ʈ���� ���� ��ġ üũ (Ʈ���� �̵����� üũ)
    STEP_INIT_TRAY_DONE         // Ʈ���� �̵� �Ϸ�
} ICE_INIT_STEP;

void SetIceMode(U8 u8_p_mode);
void ClearIceMode(U8 u8_p_mode);
U8 GetIceMode(void);

void IceInit(void);
U8 ProcessIceInit(void);

#endif
