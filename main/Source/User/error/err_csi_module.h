#ifndef __ERR_CSI_MODULE_H__
#define __ERR_CSI_MODULE_H__

#include "prj_type.h"

// Return : TRUE - 에러, FALSE - 정상
U8 CheckErrCSI(U8 mu8Error);

U8 ReleaseErrCSI(U8 mu8Error);

#endif /* __ERR_CSI_MODULE_H__ */
