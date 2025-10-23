/// @file   User_Config.h
/// @date   2025/03/21
/// @author Jaejin Ham
/// @brief  User�� �����ؼ� ����ϴ� File

#ifndef _USER_CONFIG_H_
#define _USER_CONFIG_H_


#define  CONFIG_FOTA_LIB                0   /* FOTA ��� Ȱ��ȭ ����(ǥ�� ���̺귯�� xxx_f ���) */

#if (CONFIG_FOTA_LIB == 0)
#define __FAR
#define SPRINTF     sprintf
#define MEMSET      memset
#define MEMCPY      memcpy
#define ATOI        atoi
#define QSORT       qsort
#define FABS        fabs
#define POW         pow
#else
#define __FAR       __far
#define SPRINTF     sprintf_f
#define MEMSET      memset_f
#define MEMCPY      memcpy_f
#define ATOI        atoi_f
#define QSORT       qsort_f
#define FABS        fabs
#define POW         pow
#endif


#endif

