/**
 * File : level.c
 * 
 * Level
*/
#include "level.h"

#define  DETECT_TIME       (20U)  /// 2sec @ 100ms

typedef U8(*FPAction_T)(void);
typedef struct _level_
{
    U8 PreLevel;                  // ��ũ ���� ���� ���� ��
    U8 CurLevel;                  // ��ũ ���� ���� ���� ��
    U8 SetDetectTime;             // ���� ���� �ð� ���� ��
    U8 DetectTime;                // ���� ���� �ð�
    U8 ErrCount[ERR_TYPE_MAX];    // ���� �߻� Ƚ�� 
    FPAction_T pGetLevel;          // ��ũ�� ���� ���� ��ȯ �Լ�
} Level_T;

Level_T LevelList[LEVEL_ID_MAX] = 
{
    { 0, 0, DETECT_TIME, DETECT_TIME, { 0, 0, 0, 0 }, GetLevelRoomTank  },
    { 0, 0, DETECT_TIME, DETECT_TIME, { 0, 0, 0, 0 }, GetLevelColdTank  },
    { 0, 0, DETECT_TIME, DETECT_TIME, { 0, 0, 0, 0 }, GetLevelHotTank   },
    { 0, 0, DETECT_TIME, DETECT_TIME, { 0, 0, 0, 0 }, GetLevelDrainTank },
    { 0, 0, DETECT_TIME, DETECT_TIME, { 0, 0, 0, 0 }, GetLevelIceTray   }
};


// ���� ���� ���� �ð� �ʱ�ȭ
void InitTankConfDetectTime(void)
{
    SetTankConfDetectTime(LEVEL_ID_ROOM ,    DETECT_TIME);   // @100ms
    SetTankConfDetectTime(LEVEL_ID_COLD,     DETECT_TIME);   // @100ms
    SetTankConfDetectTime(LEVEL_ID_HOT,      DETECT_TIME);   // @100ms
    SetTankConfDetectTime(LEVEL_ID_DRAIN,    DETECT_TIME);   // @100ms
    SetTankConfDetectTime(LEVEL_ID_ICE_TRAY, DETECT_TIME);   // @100ms
}

// ���� ���� ���� �ð� ����
void SetTankConfDetectTime(U8 id, U8 detectTime)
{
    LevelList[id].SetDetectTime = detectTime; 
}

// ���� ���� ���� �ð� ��ȯ
U8 GetTankConfDetectTime(U8 id)
{
    return LevelList[id].DetectTime; 
}

// ��ũ ���� ���� ���� ��ȯ
U8 GetTankLevel(U8 id)
{
    return LevelList[id].CurLevel; 
}

// ��ũ ���� ���� ���� ��ȯ
U8 IsDetectTankLevel(U8 id, U8 level)
{
   if( (LevelList[id].CurLevel & level) == level )
   {
       return TRUE;
   }

   return FALSE;
}

U8 IsTankLevelFull(U8 id)
{
    U8 ret = FALSE;
    U8 level;

    level = GetTankLevel(id);
    switch( level )
    {
        // FULL 
        case LEVEL_HIGH:       
        case LEVEL_ERR_LOW:    
        case LEVEL_ERR_HIGH:   
        case LEVEL_ERR_COMPLEX:
        case LEVEL_ERR_OVF_LOW:
        case LEVEL_OVF:        
            ret = TRUE;
            break;

        // NOT FULL TANK
        case LEVEL_LOW:        
        case LEVEL_MID:        
        default:
            ret = FALSE;
            break;
    }

    return ret;
}


U8 IsErrorTankLevel(U8 level)
{
    if( level == LEVEL_OVF
        || level == LEVEL_ERR_OVF_LOW
        || level == LEVEL_ERR_HIGH 
        || level == LEVEL_ERR_COMPLEX 
        || level == LEVEL_ERR_LOW  
      )
    {
        return TRUE;
    }

    return FALSE;
}


U8 GetTankLevelErrorCount(U8 id, U8 errType)
{
    return LevelList[id].ErrCount[errType];
}

void SetTankLevelErrorCount(U8 id, U8 errType, U8 count)
{
    LevelList[id].ErrCount[errType] = count;
}

void ClearTankLevelErrorCount(void)
{
    U8 count = 0U;

    count = GetTankLevelErrorCount(LEVEL_ID_ROOM, ERR_TYPE_OVF);
    if( count < 3 )
    {
        SetTankLevelErrorCount(LEVEL_ID_ROOM, ERR_TYPE_OVF, 0);
    }

    count = GetTankLevelErrorCount(LEVEL_ID_ROOM, ERR_TYPE_COMPLEX);
    if( count < 3 )
    {
        SetTankLevelErrorCount(LEVEL_ID_ROOM, ERR_TYPE_COMPLEX, 0);
    }

    count = GetTankLevelErrorCount(LEVEL_ID_ROOM, ERR_TYPE_HIGH);
    if( count < 3 )
    {
        SetTankLevelErrorCount(LEVEL_ID_ROOM, ERR_TYPE_HIGH, 0);
    }
}

static void CountErrorType(U8 id, U8 level)
{
    switch( level )
    {
        case LEVEL_ERR_LOW:      // �������� ����, ������ �̰���
        case LEVEL_ERR_OVF_LOW:  // ����ħ ����, ������ ����, ������ �̰���
            LevelList[id].ErrCount[ ERR_TYPE_LOW ]++;
            break;

        case LEVEL_ERR_COMPLEX:  // �����÷ο츸 ����
            LevelList[id].ErrCount[ ERR_TYPE_COMPLEX ]++;
            break;

        case LEVEL_ERR_HIGH:  // �����÷ο� ����, ������ �̰���, ������ ����
            LevelList[id].ErrCount[ ERR_TYPE_HIGH ]++;
            break;

        case LEVEL_OVF: // �����÷ο� ����, ������ ����, ������ ����
            LevelList[id].ErrCount[ ERR_TYPE_OVF ]++;
            break;

        case LEVEL_HIGH:
            LevelList[id].ErrCount[ ERR_TYPE_HIGH ]     = 0;
            LevelList[id].ErrCount[ ERR_TYPE_COMPLEX ]  = 0;
            break;

        default:
            break;
    }

    /* ������ ���� ���� �ʱ�ȭ */
    if( (level != LEVEL_ERR_LOW)
         && (level != LEVEL_ERR_OVF_LOW)
       )
    {
        LevelList[id].ErrCount[ ERR_TYPE_LOW ] = 0;
    }
}


void ProcessTankLevel(void)
{
    U8 i = 0U;
    U8 level = 0U;

    for( i=0; i<LEVEL_ID_MAX; i++ )
    {
        /* Get new level status */
        level = LevelList[i].pGetLevel();
        if( LevelList[i].PreLevel != level )
        {
            /* if different, update time, update level status */
            LevelList[i].PreLevel = level;
            LevelList[i].DetectTime = LevelList[i].SetDetectTime; 

            continue;
        }

        if( LevelList[i].DetectTime != 0 )
        {
            LevelList[i].DetectTime--;
            continue;
        }

        if( LevelList[i].CurLevel != level )
        {
            /* Count Error */
            CountErrorType(i, level);
        }

        /* Timeout, update new level status */
        LevelList[i].CurLevel = level;
    }
}
