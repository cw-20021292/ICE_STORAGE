#ifndef _WIFI_CJSON_H_
#define _WIFI_CJSON_H_

#include "WIFI_Common.h"

#if (USER_USE_A2021_SAPINFO == 1)
/***** Define ********************************************************/

#define JSON_NULL      '\0'    // 0x00 // null
#define JSON_QUOTE     '\"'    // 0x22
#define JSON_SPACE     ' '     // 0x20
#define JSON_HT        '\t'    // 0x09
#define JSON_LF        '\n'    // 0x0A
#define JSON_CR        '\r'    // 0x0D
#define JSON_COLON     ':'     // 0x3A // :
#define JSON_COMMA     ','     // 0x2C // ,
#define JSON_OPEN      '{'     // 0x7B	// {
#define JSON_CLOSE     '}'     // 0x7D	// }
#define JSON_OPEN_B    '['     // 0x5B	// }
#define JSON_CLOSE_B   ']'     // 0x5D	// }
#define JSON_DOT       '.'     // 0x2E	// .
#define JSON_ValueNullS      'n'     
#define JSON_ValueNullE      'l'     


/***** Define ***************************************************************/

/*  KEY : Value 구조 정의
    "ABC" : "123"  
*/
typedef struct 
{
    char key[USER_MAX_KEY_LEN];
    char value[USER_MAX_VALUE_LEN];
} JSONItem;


/*  KEY : Value(Array)구조 정의
    "ABC" : [{}, {}] 
*/
typedef struct 
{
    char key[USER_MAX_KEY_LEN];
    char value[USER_MAX_VALUE_ARRAY_LEN];
} JSONArrayItem;

/*  Key - Value 쌍의 갯 수
    "AAA" : "111" 
    "BBB" : {"BBB1":"111", "BBB2":"222"}
    "CCC" : "111"
*/
typedef struct
{
    JSONItem items[USER_MAX_ITEMS];
    int count;
} JSONObject;

/*  Array 전체 Value 값
    "KEY" : [{ },{ }]
 */
typedef struct
{
    JSONArrayItem items[USER_MAX_ARRAY_VALUE];
    int count;
} JSONArray;


/*  Array에서 object 추출
    [ {}, {} ]  -> { }, { }
*/
typedef struct
{
    char value[USER_MAX_ARRAY_OBJECT][USER_MAX_VALUE_LEN];
    int Arraycount;
} JSONOArrayValue;


/***** Function ********************************************************/
I16 cJsonParseJson(const I8 *json_str, JSONObject *json_obj, JSONArray *json_Arrayobj);
I16 cJsonParseArrayJson(const I8 *json_str, JSONOArrayValue *json_Array) ;

U8 GetJsonKeyIndexSearch (I8* mKey, JSONObject *json_obj );
U8 JsonSAPCodeSuccess ( JSONObject *json_obj );
void GetJsonValue (I16* mData, I8* mKey, JSONObject *json_obj );
void GetJsonDateValue ( U8* mDateData, I8* mKey, JSONObject *json_obj );

/***** Static Function ********************************************************/
static void cJsonSkipWhiteSpace(const char **str);
static int cJsonParseKey(const char **str, char *key, int MaxKeyLen);               // Key Parser "Key":
static int cJsonParseValue(const char **str, char *value, int MaxValueLen);         // Value Parser "Value"
static int cJsonParseObject(const char **str, char *value, int MaxValueLen);        // Value object Parser "{ }"
static int cJsonParseArray(const char **str, char *value, int MaxValueLen);         // Value Array Parser "[ ]"
static int cJsonParseArrayValue(const char **str, char *value, int MaxValueLen);    // Array Value object Parser "[{ }]"



#endif

#endif