/**
 * File : error_comp_bldc_comm.c
 * 
 * Bldc Comp Module Error Commnunication Check
*/
#include "err_comp_bldc_comm.h"

#include "comp_bldc.h"

#define CONFIG_COMP_ERROR_ONESHOT   (0)

#if ( CONFIG_COMP_ERROR_ONESHOT == 1 )
#define ERR_COUNT   (0)     // 1ȸ �߻� ��, ���� 
#else
#define ERR_COUNT   (9)     // 10ȸ �߻� ��, ����
#endif
#define RELEASE_TIME    (12UL*60UL*60UL*10UL)  // 12hour = 12h*60m*60s*10 @100ms

typedef struct _err_comp_bldc_comm_
{
    U8  error;           // Error ���� ( TRUE : ����, FALSE : ���� )
    U8  count;           // ��õ� Ƚ��
    U32 releaseTime;     // ��ҵ� Ƚ�� �ʱ�ȭ Ÿ�̸�
} SErrCompBldcComm_T;

SErrCompBldcComm_T  errBldcCompComm;

void InitErrCompBldcComm(void)
{
    errBldcCompComm.error = FALSE;
    errBldcCompComm.count = ERR_COUNT;
    errBldcCompComm.releaseTime = RELEASE_TIME;
}

void ResetCompBldcCommErrorCount(void)
{
    errBldcCompComm.count = ERR_COUNT;
    errBldcCompComm.releaseTime = RELEASE_TIME;
}

/******************************************************************************************************************
 *PROTECT
 */
U8 CheckProtectCompBldcComm(U8 error)
{
    if( GetCompBldcErrorComm() == TRUE )
    {
        if( errBldcCompComm.count != 0 )
        {
            errBldcCompComm.count--;
            SetCompBldcErrorComm(FALSE);
            SetCompBldcProtect(TRUE);
            return TRUE;    // error
        }
        else
        {
            errBldcCompComm.error = TRUE;    // ���� ���� �߻�
        }
    }
    else
    {
        if( errBldcCompComm.releaseTime != 0 )
        {
            errBldcCompComm.releaseTime--;
        }
        else
        {
            ResetCompBldcCommErrorCount();
        }
    }

    return FALSE;   // normal
}

U8 ReleaseProtectCompBldcComm(U8 error)
{
    if( GetCompBldcProtect() == FALSE )
    {
        return FALSE;   // normal
    }

    return TRUE;    // error
}

U8 CheckErrCompBldcComm(U8 error)
{
    if( errBldcCompComm.error == TRUE )
    {
        return TRUE;      // normal
    }

    return FALSE;         // error
}
