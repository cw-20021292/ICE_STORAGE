
/*******************************************************************************
&Description :
  제품에서 사용하는 WIFI 함수 File
  
&History : 

  ssg@coway.com 2024.05.30 update
  - DATA 이벤트 처리 추가
*******************************************************************************/

#include "WIFI_Common.h"
#include "WIFI_Status.h"
#include "WIFI_cJSON.h"


/* RAM Monitor */
U16 WifiRamWriteCheck_address = 0;

void SetUserSystemRamWriteStatus ( U16 mu16Address)
{
	WifiRamWriteCheck_address = (U16)mu16Address;
}
U16 GetUserSystemRamWriteStatus ( U16 mu16Address )
{
	if (WifiRamWriteCheck_address == mu16Address)
	{
		return WifiRamWriteCheck_address;
	}
	return CLEAR;
}

/* Event 처리 A1010, A1030, A1014*/
void SystemFunctionEventCheck(void)
{
	U8 u8EventCheck;
	U16 u16Index;
    WifiTxFuncEventList_T *pList;
	U16 u16SizeEvent;
	static U8 u8Functionini = 0;
	U8 u8IsChangedStatus = 0;
	U8 u8IsChangedStatusErr = 0;
	U8 u8A1014Pass = 0;
	U16 mu16CurVal = 0;
	U16 mu8FuncAck = 0;

	mu8FuncAck = GetWifiSendStatusAck() ;
	
	for (u8EventCheck=0; u8EventCheck < WIFI_EVENT_CHECK_MAX; u8EventCheck++)
	{
		u8IsChangedStatus = 0;
		u8A1014Pass = 0;
		if (u8EventCheck ==  WIFI_EVENT_CHECK_FUNC)
		{
			pList = (WifiTxFuncEventList_T *)GetWifiFuncDataEventList();
			u16SizeEvent = GetWifiFuncEventListSize();
		}
		else if (u8EventCheck ==  WIFI_EVENT_CHECK_ERR)
		{
			pList = (WifiTxFuncEventList_T *)GetWifiErrDataEventList();
			u16SizeEvent = GetWifiErrEventListSize();
		}
		for( u16Index = 0; u16Index < u16SizeEvent; u16Index++ )
		{
			if (u8EventCheck ==  WIFI_EVENT_CHECK_FUNC)
			{
				mu16CurVal = GetUserSystemFunction((pList+u16Index)->Id);  /* @2024-07-19 14:10:04 : mu16CurVal 삭제하고 Local로 처리...   */ 
			}
			else if (u8EventCheck ==  WIFI_EVENT_CHECK_ERR)
			{
				mu16CurVal = GetUserSystemError((pList+u16Index)->Id);	
			}
			if (mu8FuncAck == TRUE)
			{
				u8IsChangedStatus = TRUE;
				mu8FuncAck = FALSE;
			}

			if (((pList+u16Index)->mu16PreVal[u16Index] != mu16CurVal)) 	// 현재값 != 이전값
			{
				if ((u8Functionini == TRUE)  )  // initial check 
				{
					if (((pList+u16Index)->mu8Type == TYPE_A1014)  && (!u8A1014Pass))
					{
						if (GetWifiSendStatusAck() == TRUE)
						{
							// SetWifiSendStatusAck(CLEAR);
						}
						else
						{
							if (((pList+u16Index)->mu8EventForm == EVENT_ZERO_TO_OTHER) // A1014 DATA & [ZERO_TO_OTHER] DATA가 인 경우 변경된 DATA가 [0]이 아닐 경우만 A1014 전송
								&& ((pList+u16Index)->mu16PreVal[u16Index] != 0) 
								&& (mu16CurVal == 0 ))
							{
							}
							else
							{
								WifiSendDataControl( WIFI_DATA_FUNCTION_CONTROL, (pList+u16Index)->Id);
							}
							u8A1014Pass = TRUE;
						}
					}
					if ((pList+u16Index)->mu8EventForm == EVENT_STATUS_CHANGE)
					{
						u8IsChangedStatus = TRUE;
					}
					else if ((pList+u16Index)->mu8EventForm == EVENT_ZERO_TO_ONE)		//// OFF -> ON 
					{
						if (	(((pList+u16Index)->mu16PreVal[u16Index] == OFF) && (mu16CurVal == ON))			// OFF -> ON
								|| ( (((pList+u16Index)->mu16PreVal[u16Index] == ON) || ((pList+u16Index)->mu16PreVal[u16Index] == IoT_GOING)) && (mu16CurVal == OFF))	// 1 or 2  -> OFF , DONE -> OFF (X)
								|| (((pList+u16Index)->mu16PreVal[u16Index] != OFF) && (mu16CurVal == IoT_DONE))	)	// ?? -> 3 (DONE)
						{
							u8IsChangedStatus = TRUE;
						}
					}
					else if ((pList+u16Index)->mu8EventForm == EVENT_ZERO_TO_OTHER)
					{
						if (	(((pList+u16Index)->mu16PreVal[u16Index] == OFF) && (mu16CurVal != OFF))	)
						{
							u8IsChangedStatus = TRUE;
						}
					}
					else if ((pList+u16Index)->mu8EventForm == EVENT_SPECIFIC_CONDITION)
					{
						// if (((pList+u16Index)->Id >= 0xFFFF) || ((pList+u16Index)->Id <= 0xFFFF))
						if ((pList+u16Index)->Id == 0xFFFF) // e.g.
						{
							if (mu16CurVal >= 30) // e.g. 필터교체 도래일 30일 이하
							{
								u8IsChangedStatus = TRUE;
							}
						}
						else // DATA가 홀수 일 경우 Event 전송  EX) ((pList+u16Index)->Id == WIFI_FUNC_0093_ICEMAKING_STAUTS) 제빙스텝 등
						{
							if ((((mu16CurVal % 2) == TRUE) && ((pList+u16Index)->mu16PreVal[u16Index] < mu16CurVal)) // 1,3,5,7 .. [시작] DATA 인 경우 & 이전값보다 현재값이 더 클 경우
								|| (((pList+u16Index)->mu16PreVal[u16Index] != OFF) && (mu16CurVal == OFF)))		  // or 이전값이 0이 아닌 값에서 --> 현재데이터가 0으로 변경 시 
							{
								u8IsChangedStatus = TRUE;
							}
						}
					}
				}
				if (((pList+u16Index)->mu8EventForm == EVENT_ZERO_TO_ONE) 	// Going 처리 (현재값으로 덮어쓰지 않음)
				&& ((pList+u16Index)->mu16PreVal[u16Index] >= mu16CurVal)
				&& (mu16CurVal != 0))
				{
				}
				else if (((pList+u16Index)->mu8EventForm == EVENT_SPECIFIC_CONDITION)	// Going 처리 (현재값으로 덮어쓰지 않음)
				&& ((pList+u16Index)->mu16PreVal[u16Index] >= mu16CurVal)
				&& (mu16CurVal != 0))	
				{
				}
				else
				{
					(pList+u16Index)->mu16PreVal[u16Index] = mu16CurVal;
				}
			}
			else	// 현재값 == 이전값
			{
				if (u8Functionini == TRUE)  // initial check
				{
					if ((pList+u16Index)->mu8EventForm == EVENT_ZERO_TO_ONE)	// ON -> Going
					{
						if((pList+u16Index)->mu16PreVal[u16Index] == 1) 
						{
							(pList+u16Index)->mu16PreVal[u16Index] = 2;
						}
					}
					else if ((pList+u16Index)->mu8EventForm == EVENT_SPECIFIC_CONDITION)	// ON -> Going
					{
						// if (((pList+u16Index)->Id >= 0xFFFF) || ((pList+u16Index)->Id <= 0xFFFF))
						if ((pList+u16Index)->Id == 0xFFFF) // e.g.
						{

						}
						else // DATA가 홀수 일 경우 Event Ex) 스텝 등
						{
							if(((pList+u16Index)->mu16PreVal[u16Index] % 2) == 1) // 홀수 일 경우 : + 1 = (Going)
							{
								(pList+u16Index)->mu16PreVal[u16Index]++;
							}
						}
					}

				}
			}
		}
		u8Functionini = TRUE;

		if ((u8IsChangedStatus == TRUE) )
		{
			if (u8EventCheck ==  WIFI_EVENT_CHECK_FUNC)
			{
				WifiSendData( WIFI_DATA_FUNCTION_EVENT );
			}
			else if (u8EventCheck ==  WIFI_EVENT_CHECK_ERR)
			{
				WifiSendData( WIFI_DATA_ERROR_EVENT );
			}
			SetWifiSendStatusAck(CLEAR);
		}
	}

}

U16 GetFilterDayData (U16 mu16Data)
{
	U32 work = 0;
	U16 mu16Datareturn = 0;
	work = (U16)GetUserSystemFunction(mu16Data);
	if (work <= 720U)
	{
		work = (U32)(work)*1000U;
		work = (U32)(work/24U);
		if ((work)%1000U > 0)
		{
			mu16Datareturn = (U16)(work/1000U)+1U;
		}
		else
		{
			mu16Datareturn = (U16)(work/1000U);
		}
	}
	else
	{
		mu16Datareturn = 65535U;
	}
	return mu16Datareturn;
}

I16 GetFilterData (U16 mu16SensorDataFilter, U16 mu16FilterTotalUsage)
{
	U32 work = 0;
	U16 mu16Filter_WaterUsageLiter = 0U;
	U16 mu16Filter_WaterUsagePercent = 0U;
	I16 mi16Data = 0;

	mu16Filter_WaterUsageLiter = (U16)GetUserSystemFunction(mu16SensorDataFilter);
	if (mu16Filter_WaterUsageLiter >= mu16FilterTotalUsage)
	{
		mu16Filter_WaterUsageLiter = mu16FilterTotalUsage;
	}
	work = (U32)((mu16FilterTotalUsage)-(mu16FilterTotalUsage - mu16Filter_WaterUsageLiter)) * 10000U;
	work = (U32)(work / mu16FilterTotalUsage);
	mu16Filter_WaterUsagePercent = (U16)(work/100U);
	// if ((work % 100U) > 0U )
	// {
	// 	mu16Filter_WaterUsagePercent = (U16)(work/100U)+1U;
	// }
	// else
	// {
	// 	mu16Filter_WaterUsagePercent = (U16)(work/100U);
	// }
	mi16Data = (I16)mu16Filter_WaterUsagePercent;

	return mi16Data;
}

U32 GetUserSystemRamMonitor ( U16 mu16Func, U16 mLen )
{
	U32 mu32Data = 0U;
	__near U8 *mPRamAddress_read; // 1byte
	U16 mu16temp_2byte_value = 0U;
	U32 mu32temp_4byte_value = 0U;

	if ((mu16Func > Z_WIFI_rammax) || (mu16Func < Z_WIFI_rammin))
	{ 
		return mu32Data;
	}

	if ((mLen == 0) && (mLen > 4))
	{
		return mu32Data;
	}
	mPRamAddress_read = (__near U8*)mu16Func;  
	mu32Data = *mPRamAddress_read; 

	if (mLen > 1)
	{
		mPRamAddress_read++;
		mu16temp_2byte_value = *mPRamAddress_read;
		mu32Data += (U16)((mu16temp_2byte_value & 0x00FF) << 8);

	}
	if (mLen > 2)
	{
		mPRamAddress_read++;
		mu32temp_4byte_value = *mPRamAddress_read;
		mPRamAddress_read++;
		mu16temp_2byte_value = *mPRamAddress_read;
		mu32temp_4byte_value += (U16)((mu16temp_2byte_value & 0x00FF) << 8);
		mu32Data += (U32)((mu32temp_4byte_value & 0x0000FFFF) << 16);
	}
	return mu32Data;
}

void  SetUserSystemRamMonitor (__near U16 mu16address, I8 *mData )
{
	__near U8 *mPRamAddress;
	U16 mLen = 0U;
	U16 mRWMode = 0U;
	U32 mRamWriteData_4byte = 0U;
	U32 mRamWriteData_4byte_buf = 0U;
	U8 mi = 0U;
	U8 mDec = 0U;
	U32 mDEC_Positional_Number = 1U;

	mLen = WIFI_ASCII2HEX(mData[0]) & 0x000F;	
	mRWMode = WIFI_ASCII2HEX(mData[1]) & 0x000F;  

	if ((mu16address > Z_WIFI_rammax) || (mu16address < Z_WIFI_rammin)){ 
		return;
	}
	if (mRWMode != Write) 
	{
		return;
	}
	for (mi=0; mi<(mData[LENGTH_WIFI_RAM_DATA-1]-1); mi++) 
	{
		mDEC_Positional_Number *= 10; 
	}
	for (mi=0; mi<mData[LENGTH_WIFI_RAM_DATA-1]; mi++)
	{
		mDec = (U8)((mData[2+mi])-0x30);		
		mRamWriteData_4byte += mDec * (mDEC_Positional_Number); 
		mDEC_Positional_Number /= 10;	
	}

	mPRamAddress = (__near U8*)mu16address;
	mRamWriteData_4byte_buf = mRamWriteData_4byte;
	for (mi = 0; mi < mLen; mi++)
	{
		*mPRamAddress = (U8)(mRamWriteData_4byte_buf & 0x000000FF);	
		mRamWriteData_4byte_buf = mRamWriteData_4byte_buf  >> 8;
		*(mPRamAddress++);	
	}

	SetUserSystemRamWriteStatus((U16)mu16address);
}
