#include "err_csi_module.h"
#include "ster.h"
#include "csi.h"

U8 CheckErrCSI(U8 mu8Error)
{
    // 살균 중..
    if( IsStartSter() == TRUE )
    {
        return GetCSIError();
    }

    return FALSE;   // 살균 중이 아니면, 정상
}

U8 ReleaseErrCSI(U8 mu8Error)
{
    // 살균 중..
    if( IsStartSter() == TRUE )
    {
        return GetCSIError();
    }

    return FALSE;   // 살균 중이 아니면, 정상
}
