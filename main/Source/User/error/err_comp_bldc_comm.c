/**
 * File : error_comp_bldc_comm.c
 * 
 * Bldc Comp Module Error Commnunication Check
*/
#include "err_comp_bldc_comm.h"

#include "comp_bldc.h"

#define CONFIG_COMP_ERROR_ONESHOT   (0)

#if ( CONFIG_COMP_ERROR_ONESHOT == 1 )
#define ERR_COUNT   (0)     // 1회 발생 시, 에러 
#else
#define ERR_COUNT   (9)     // 10회 발생 시, 에러
#endif
#define RELEASE_TIME    (12UL*60UL*60UL*10UL)  // 12hour = 12h*60m*60s*10 @100ms

typedef struct _err_comp_bldc_comm_
{
    U8  error;           // Error 상태 ( TRUE : 에러, FALSE : 정상 )
    U8  count;           // 재시도 횟수
    U32 releaseTime;     // 재소도 횟수 초기화 타이머
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
            errBldcCompComm.error = TRUE;    // 최종 에러 발생
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
