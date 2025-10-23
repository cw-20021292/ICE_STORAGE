/*******************************************************************************
&Description :
    Json String Parser (Key : Value)

&History : ssg@coway.com
    2024-06-11 : 


*******************************************************************************/

/***** Header *****************************************************************/
#include "WIFI_cJSON.h"


#if (USER_USE_A2021_SAPINFO == 1)
/***** Function ********************************************************/

I16 cJsonParseJson(const I8 *json_str, JSONObject *json_obj, JSONArray *json_Arrayobj) {
    I16 i=0;
    if (*json_str == JSON_OPEN) 
    {
        json_str++;
        cJsonSkipWhiteSpace(&json_str);

        while (*json_str != JSON_NULL && json_obj->count < sizeof(json_obj->items)) {  //MAX_ITEMS
            if (*json_str == JSON_CLOSE) 
            {
                json_str++;
                return 1;
            }

            if (!cJsonParseKey(&json_str, json_obj->items[json_obj->count].key, sizeof(json_obj->items[json_obj->count].key))) 
            {
                return 0;
            }

            cJsonSkipWhiteSpace(&json_str);
            if (*json_str != JSON_COLON) 
            {
                return 0;
            }
            json_str++;
            cJsonSkipWhiteSpace(&json_str);
            if ((*json_str == JSON_OPEN_B) && (json_Arrayobj != JSON_NULL)) { // [ ] Array 일 경우 별도 구조체로 파싱
                if (!cJsonParseValue(&json_str, json_Arrayobj->items[json_Arrayobj->count].value, sizeof(json_Arrayobj->items[json_Arrayobj->count].value))) {
                    return 0;
                }
                i=0;
                while ((json_obj->items[json_obj->count].key[i] != JSON_NULL) && (i < sizeof(json_Arrayobj->items[json_obj->count].key)))
                {
                    json_Arrayobj->items[json_Arrayobj->count].key[i] = (json_obj->items[json_obj->count].key[i]);
                    i++;
                }
                json_Arrayobj->count++;
            }
            else if (!cJsonParseValue(&json_str, json_obj->items[json_obj->count].value, sizeof(json_obj->items[json_obj->count].value))) 
            {
                return 0;
            }

            json_obj->count++;
            cJsonSkipWhiteSpace(&json_str);
            if (*json_str == JSON_COMMA) 
            {
                json_str++;
                continue;
            } else if (*json_str == JSON_CLOSE) 
            {
                json_str++;
                return 1;
            } else {
                return 0;
            }
        }
        return 0;
    }
    return 0;
}

I16 cJsonParseArrayJson(const I8 *json_str, JSONOArrayValue *json_Array) 
{
    json_Array->Arraycount = 0;
    cJsonSkipWhiteSpace(&json_str);

    if (*json_str == JSON_OPEN_B) 
    {
        json_str++;
        cJsonSkipWhiteSpace(&json_str);

        while (*json_str != JSON_NULL && json_Array->Arraycount < sizeof(json_Array->value))  // MAX ARRAY
        {
            if (*json_str == JSON_CLOSE_B) 
            {
                json_str++;
                return 1;
            }
            if (!cJsonParseArrayValue(&json_str, json_Array->value[json_Array->Arraycount], sizeof(json_Array->value[json_Array->Arraycount]))) 
            {
                return 0;
            }
            json_Array->Arraycount++;
            cJsonSkipWhiteSpace(&json_str);
            if (*json_str == JSON_COMMA) 
            {
                json_str++;
                continue;
            } 
            else if (*json_str == JSON_CLOSE_B) 
            {
                json_str++;
                return 1;
            }
            else
            {
                return 0;
            }
        }
        return 0;
    }
}


/* JSON Key / DATA */
/* SAP CODE */
const signed char JSON_KEY_CODE[] 						= "c"; 		// Code
const signed char JSON_CODE_SUCCESS[] 					= "S1000"; 
const signed char JSON_CODE_NO_DATA[] 					= "E9000"; 


U8 GetJsonKeyIndexSearch (I8* mKey, JSONObject *json_obj )
{
	U8 i = 0;
	for (i=0; i < USER_MAX_ITEMS; i++ ) 
	{
		if ( _STRNCMP_( (const char __far*) json_obj->items[i].key, (const char __far*) mKey, sizeof(mKey) ) == N_TRUE ) // Key Index 찾기
		{  
			return (U8)i;
		}
		
	}
	return (U8)0xFF;
}

U8 JsonSAPCodeSuccess ( JSONObject *json_obj )
{
	U8 i = 0;
	for (i=0; i < USER_MAX_ITEMS; i++ ) 
	{
		if ( _STRNCMP_( (const char __far*) json_obj->items[i].key, (const char __far*) JSON_KEY_CODE, sizeof(JSON_KEY_CODE) ) == N_TRUE ) // Key == "Code" 일 경우
		{  
			if ( _STRNCMP_( (const char __far*) json_obj->items[i].value, (const char __far*) JSON_CODE_SUCCESS, sizeof(JSON_CODE_SUCCESS)  ) == N_TRUE ) // S1000 일경우
			{
				return S1000;	// S1000 성공.
			}
			else if ( _STRNCMP_( (const char __far*) json_obj->items[i].value, (const char __far*) JSON_CODE_NO_DATA, sizeof(JSON_CODE_NO_DATA)  ) == N_TRUE ) // E9000 일경우
			{
				return E9000;	// E9000 DATA 없음
			}
		}
		
	}
	return F1000;
}


void GetJsonValue (I16* mData, I8* mKey, JSONObject *json_obj )
{
	U8 i = 0;
	U8 j = 0;
	U8 mSize = 0;
	U8 mu8len = 0;
	I8 mi8TempBuf[6]; //signed int 자릿수
	_MEMSET_( (void __far*) &mi8TempBuf, 0, sizeof(mi8TempBuf) );
	for (i=0; i < USER_MAX_ITEMS; i++ ) 
	{
		if ( _STRNCMP_( (const char __far*) json_obj->items[i].key, (const char __far*) mKey, sizeof(mKey) ) == N_TRUE ) // Key Index 찾기
		{  
			if ((const char __far*)json_obj->items[i].value[0] != NULL)  // 값이 있는 경우
			{
				mSize = (U8)_STRLEN_( (const char __far*) json_obj->items[i].value );
				for (mu8len=0; mu8len < mSize; mu8len++ )
				{	
					mi8TempBuf[mu8len] = json_obj->items[i].value[j++];
					if (json_obj->items[i].value[j] == '.')  // 소수점 check
					{
						j++;
					}
					else if (json_obj->items[i].value[j] == 0x00) // NULL check
					{
						break;
					}
				}
				*mData = _ATOI_((const char __far*) mi8TempBuf);
				break;
			}
		}
	}
}

void GetJsonDateValue ( U8* mDateData, I8* mKey, JSONObject *json_obj  )
{
	U8 i = 0;
	U8 j = 0;
	U8 mSize = 0;
	U8 mu8len = 0;
	U8 mu8mod = 0;
	for (i=0; i < USER_MAX_ITEMS; i++ ) 
	{
		if ( _STRNCMP_( (const char __far*) json_obj->items[i].key, (const char __far*) mKey, sizeof(mKey) ) == N_TRUE ) // Key (ld or nd) 일 경우
		{  
			// YYYY MM DD HH SS  or  YYYY MM DD -> YY MM DD HH SS or YY MM DD
			if ((const char __far*)json_obj->items[i].value[0] != NULL)  // 값이 있는 경우
			{
				mSize = (U8)_STRLEN_( (const char __far*) json_obj->items[i].value );
				// mSize = (U8)sizeof(mDateData);
				j=2;
				for (mu8len=0; mu8len < ((mSize-2)/2); mu8len++ )
				{	
					mDateData[mu8len] = (WIFI_ASCII2HEX(json_obj->items[i].value[j++]) << 4) & 0xF0;  // 2024 --> 24 ~
					mDateData[mu8len] += WIFI_ASCII2HEX(json_obj->items[i].value[j++]) & 0x0F;

					mu8mod = (U8)(mDateData[mu8len] / 16);
					if (mu8mod > 0)
					{
						mDateData[mu8len] -= (U8)(6 * mu8mod);
					}
				}
			}
			break;
		}
		
	}
}


/***** Function ********************************************************/
static void cJsonSkipWhiteSpace(const char **str) {
    while (**str == JSON_SPACE || **str == JSON_HT || **str == JSON_LF || **str == JSON_CR) 
    {
        (*str)++;
    }
}

static int cJsonParseKey(const char **str, char *key, int MaxKeyLen) 
{
    int i = 0;
    cJsonSkipWhiteSpace(str);
    if (**str != JSON_QUOTE) 
    {
        return 0;
    }
    (*str)++;
    i = 0;
    while (**str != JSON_QUOTE && **str != JSON_NULL && i < MaxKeyLen - 1) 
    {
        key[i++] = *(*str)++;
    }
    if (**str != JSON_QUOTE) 
    {
        return 0;
    }
    key[i] = JSON_NULL;
    (*str)++;
    return 1;
}

static int cJsonParseValue(const char **str, char *value, int MaxValueLen) {
    int i = 0;
    int f_work = 0;
    cJsonSkipWhiteSpace(str);
    if (**str == JSON_OPEN_B) 
    {  // [ ] array
        return cJsonParseArray(str, value, MaxValueLen);
    }
    else if (**str == JSON_OPEN) 
    {  // { }
        return cJsonParseObject(str, value, MaxValueLen);
    }
    else if (**str == JSON_QUOTE) 
    {  // " "
        (*str)++;
        i = 0;
        while (**str != JSON_QUOTE && **str != JSON_NULL && i < MaxValueLen - 1) 
        {
            value[i++] = *(*str)++;
        }
        if (**str != JSON_QUOTE) 
        {
            return 0;
        }
        value[i] = JSON_NULL;
        (*str)++;
        return 1;
    }
    else if (**str == JSON_ValueNullS) 
    {  // null
        (*str)++;
        i = 0;
        while (**str != JSON_ValueNullE && **str != JSON_NULL && i < MaxValueLen - 1)  // 'l'
        {
            value[i++] = JSON_NULL;
            *(*str)++;
        }
        if (**str != JSON_ValueNullE) 
        {
            return 0;
        }
        value[i++] = JSON_NULL;
        (*str)++;
        if(**str == JSON_ValueNullE) // 'l'
        {
            value[i] = JSON_NULL;
            (*str)++;
        }
        return 1;
    }
    return 0;

}


static int cJsonParseObject(const char **str, char *value, int MaxValueLen) 
{
    int i = 0;
    value[i++] = JSON_OPEN;
    (*str)++;
    while (**str != JSON_CLOSE && **str != JSON_NULL && i < MaxValueLen - 1) 
    {
        if (**str == JSON_QUOTE) {
            value[i++] = *(*str)++;
            while (**str != JSON_QUOTE && **str != JSON_NULL && i < MaxValueLen - 1) 
            {
                value[i++] = *(*str)++;
            }
            if (**str == JSON_QUOTE) 
            {
                value[i++] = *(*str)++;
            }
        } else {
            value[i++] = *(*str)++;
        }
    }
    if (**str != JSON_CLOSE) 
    {
        return 0;
    }
    value[i++] = JSON_CLOSE;
    (*str)++;
    value[i] = JSON_NULL;
    return i;
}

static int cJsonParseArray(const char **str, char *value, int MaxValueLen) 
{
    int i = 0;
    int jsonArrayCount = 0;
    value[i++] = JSON_OPEN_B;
    (*str)++;
    cJsonSkipWhiteSpace(str);
    while (**str != JSON_CLOSE_B && **str != JSON_NULL && i < MaxValueLen - 1) 
    {
        value[i++] = *(*str)++;
    }
    if (**str != JSON_CLOSE_B) 
    {
        return 0;
    }
    value[i++] = JSON_CLOSE_B;
    (*str)++;
    value[i] = JSON_NULL;
    return i;
}

static int cJsonParseArrayValue(const char **str, char *value, int MaxValueLen) 
{
    int i = 0;
    cJsonSkipWhiteSpace(str);
    if (**str == JSON_OPEN) 
    {  
        value[i++] = JSON_OPEN;
        (*str)++;
        while (**str != JSON_CLOSE && **str != JSON_NULL && i < MaxValueLen - 1) 
        {
            value[i++] = *(*str)++;
        }
        value[i++] = JSON_CLOSE;
        value[i] = JSON_NULL;
        (*str)++;
        return 1;
    }
    return 0;
}

#endif







