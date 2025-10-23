#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "prj_type.h"

typedef struct _service_check_
{
    U8   TankOpen;   /* ��ũ ž Ŀ���� OPEN ���� */

    U8   Status;     /* ���� ���� ǥ�� ���� */
    U32  Time;       /* ���� ���� Time out �ð� */
    U32  SaveTime;   /* ���� ���� Time EEPROM ���� �ֱ� */

    U16  OpenTime;   /* ���� ���� TANK OPEN �ð� @sec */
} Service_T;


void InitServiceCheck(void);

void GetServiceCheckData(Service_T *pData );

U8 GetServiceCheckTankOpen(void);

U8 GetServiceCheckStatus(void);

#define	SERVICE_TIME_MIN ( 129600UL )	/* 60min x 24hour x 90days = 129600 min */
void SetServiceTime(U32 mu32Time);
U32 GetServiceTime(void);

void SetServiceCheckDay(U8 mu8Days);
U8 GetServiceCheckDay(void);

void SetServiceOpenTime(U16 mu16Time);
U16 GetServiceOpenTime(void);

// @100ms
void ProcessCheckTankOpen(void); 

// @1min
void ProcessServiceCheck(void);
#endif /* __SERVICE_H__ */
