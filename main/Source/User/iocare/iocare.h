#ifndef __IOCARE_H__
#define __IOCARE_H__

#include "prj_type.h"

/* IOCARE MODE LIST */

enum
{
    IOC_ID_INIT,             
    IOC_ID_ERASE_ALL,        
    IOC_ID_SERVICE_CHECK_DAY,
    IOC_ID_ERROR,            
    IOC_ID_UPDATE_SETTING,   
    IOC_ID_DAILY_DATA,       
    IOC_ID_USE_WATER_DATA,   
    IOC_ID_STER_PASS_DAY,    
    IOC_ID_STER_HISTORY,
};

void InitIoCare(void);

void SetIoCareMode(U16 mu16Mode);

void ProcessIoCare(void);

#endif /* __IOCARE_H__ */
