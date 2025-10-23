
User Interface Function
=======================
---

## 1. Main Setting ���� ���̵�

---
#### -  <1> Uart Interrupt ���� [�ʼ�]
##### #include "WIFI_Config.h" �߰��ʿ�
    void InterruptUartWifiTx ()           : Wi-Fi Module <-> MCU Uart TX ���ͷ�Ʈ ����
```C++
    __interrupt static void r_uart1_interrupt_send(void)
    {
        InterruptUartWifiTx();
    }
```
    void InterruptUartWifiRx (RXD1)       : Wi-Fi Module <-> MCU Uart RX ���ͷ�Ʈ ����
```C++
    __interrupt static void r_uart1_interrupt_receive(void)
    {
        InterruptUartWifiRx(RXD1);
    }
```
    void InterruptUartWifiError ()        : Uart Error Callback ����
```C++
    static void r_uart1_callback_error(uint8_t err_type)
    {
        InterruptUartWifiError();
    }
```
<br>

#### -  <2> �ʱ�ȭ �� �ֱ��Լ� ���� [�ʼ�] [Parameter : 1 / Return : X ]
##### #include "WIFI_Config.h" �߰��ʿ�
    WifiControlProcess(WIFI_TIME_INI)           : �ʱ�ȭ (EEPROM �ʱ�ȭ ����) 
    WifiControlProcess(WIFI_TIME_SET)           : �ʱ���� (EEPROM �ʱ�ȭ ���� While ����)
    WifiControlProcess(WIFI_TIME_WHILE)         : Main While ������ ȣ��
    WifiControlProcess(WIFI_TIME_100MS)         : Main 100ms Ÿ�̸� ���� ȣ�� (100ms �ֱ�)   
<br>

#### - <3> Wifi Key ȣ�� [Parameter : 1 / Return : X ] 
##### #include "WIFI_Config.h" �߰��ʿ�
    WifiKey(WIFI_KEY_AP)                        : Wifi Pairing ��� ���� Key 
    WifiKey(WIFI_KEY_BLE)                       : BLE Pairing ��� ���� Key
    WifiKey(WIFI_KEY_ONOFF)                     : WIFI Power On/Off ���� (Toggle)
    WifiKey(WIFI_KEY_BLE_CERT)                  : BLE �������� Ȯ��
    WifiKey(WIFI_KEY_BLE_CERT_CANCEL)           : BLE �������� ���
<br>

#### - <4> Wifi Send Data  (Server DATA ����) [Parameter : 1 / Return : X ] 
##### #include "WIFI_Config.h" �߰��ʿ�
    [�ʼ�]
    WifiSendData(WIFI_DATA_EXAMINE)             : ���� �������� �Ϸ� �� (�̺�Ʈ)
    
    [���û��� - Protocol ���� �� �̺�Ʈ ��� ����]
    WifiSendData(WIFI_DATA_FUNCTION)            : ��ɵ����� ���� �� (���º��� ��)
    WifiSendData(WIFI_DATA_ERROR)               : ���������� ���� �� (�߻�/����)
    WifiSendDataControl(WIFI_DATA_FUNCTION_CONTROL, U16 API ID) : ��ǰ KEY���� �� 
<Br>


#### - <5> ��ǰ�������� ���� [Parameter : 2 / Return : X ] 
##### #include "WIFI_Config.h" �߰��ʿ�
    (1) SetWifiSystemStatus(E_WIFI_USER_VALUE_T Status, U8 val)                

<br>

    WIFI_MODEL_TYPE                : �� Ÿ�� ����
        0 CLEAR                    : Model 0 (Model_0)
        1 SET                      : Model 1 (Model_1)

    WIFI_FACTORY_STATUS            : Wi-Fi �˻��� ���� ����
        0 CLEAR                    : Normal
        1 SET                      : Factory/PCB Test mode

    WIFI_RX_STATUS                 : �Ϲ����� �Ұ����� ����      
        0 CLEAR                    : Possible (Normal) (App Control O)
        1 SET                      : Impossible (Operating...) (App Control X)

    WIFI_SMART_POSSIBLE_STATUS     : ����Ʈ���� �Ұ����� ����       
        0 CLEAR                    : Possible (Normal)
        1 SET                      : Impossible (Operating...)

    WIFI_SMART_SENSING_STATUS      : ����Ʈ���� �� ���� ����  
        0 CLEAR                    : Normal
        1 SET                      : Operating

    WIFI_FOTA_POSSIBLE_STATUS      : FOTA �Ұ� ���� ����      
        0 CLEAR                    : Normal
        1 SET                      : Impossible (Operating...)

    WIFI_FACTORY_DIRECTFOTA_STATUS : ���̷�Ʈ ��Ÿ ���� ����
        0 CLEAR                    : Normal
        1 SET                      : Direct FOTA mode

---

## 2. WiFi ���� ���� DATA (Get Function) ���� ���̵�
#### - <1> Wi-Fi Module ���� DATA [Parameter : 1 / Return : 1 ] 
##### #include "WIFI_Config.h" �߰��ʿ�
    (1) GetWifiStatusValue(WIFI_STATUS_DISP) : WIFI ���� DISPLAY DATA (Enum ����:  E_WIFI_DISPLAY_T) 

###### Return DATA : Wi-Fi OFF [�ҵ�]
    WIFI_DISP_OFF          

###### Return DATA : Soft AP Pairing MODE [������]
    WIFI_DISP_AP_START    // Soft AP MODE START     : WiFi �� ��� ����
    WIFI_DISP_AP_STEP0    // Pairing Mode ing       : Soft AP �� ��� Ȱ��ȭ �Ϸ�
    WIFI_DISP_AP_STEP1    // APP Pairing START      : APP���κ��� �� DATA���� (25%)
    WIFI_DISP_AP_STEP2    // AP Connect             : AP����Ϸ� (50%)
    WIFI_DISP_AP_STEP3    // AUTHENTICATE           : �������� Ȯ�� �� (75&)

###### Return DATA : BLE Pairing MODE [������]
    WIFI_DISP_BLE_START   // BLE MODE START         : BLE �� ��� ����
    WIFI_DISP_BLE_STEP0   // Pairing Mode ing       : Soft AP �� ��� Ȱ��ȭ �Ϸ�
    WIFI_DISP_BLE_STEP1   // APP Pairing START      : APP���κ��� �� DATA���� (25%)
    WIFI_DISP_BLE_STEP2   // AP Connect             : AP����Ϸ� (50%)
    WIFI_DISP_BLE_STEP3   // AUTHENTICATE           : �������� Ȯ�� �� (75%)

###### Return DATA : Server ����Ϸ� [����]
    WIFI_DISP_CONNECT     // Server Connected       : ���� ����Ϸ� (100%)

###### Return DATA : Wi-Fi ON & ���� ������� [����]
    WIFI_DISP_AP1         : ������ ��ȣ ����ġ
    WIFI_DISP_AP2         : ������ �̰˻� 
    WIFI_DISP_AP3         : ������ ���� �ź�
    WIFI_DISP_AP4         : ������ ���� ����
    WIFI_DISP_AP5         : ���� �������
    WIFI_DISP_TRY         : ���� �õ� ��
<Br>
    (2) GetWifiStatusValue(WIFI_STATUS_TEST)        : TestMode ���� ����Ȯ�� 

###### Return DATA
        0: Disconnected 
        1: Connected (�˻缺��/�Ϸ�)
<Br>
    (3) GetWifiStatusValue(WIFI_STATUS_ERROR)       : Wifi Error  (���� ���� Error �̷�)

###### Return DATA
        0: Normal
        1~5: WiFi Err No.
<Br>
    (4) GetWifiStatusValue(WIFI_STATUS_AP_ERROR)    : Wifi Error  (���� Error)

###### Return DATA
        0:Normal
        1~5: �� �� Err No.
<Br>
    (5) GetWifiStatusValue(WIFI_STATUS_BLE_CERT)    : �������� ����

###### Return DATA
        0: ���������Ұ�����
        1: ������������ ����
        2: ���������Ϸ���� 
<Br> 
    (6) GetWifiStatusValue(WIFI_STATUS_SERVER_CON)  : ���� ���� �� ����

###### Return DATA
        0: ������� ��  
        1: ������
<Br>
    (7) GetWifiStatusValue(WIFI_STATUS_PAIRING)     : �� ��� ���� 

###### Return DATA
        0: �� ��� ��Ȱ��ȭ ���� 
        1: BLE Ȱ��ȭ �� ���� 
        2: Wi-Fi ������ Ȱ��ȭ �� ����
<Br>
    (8) GetWifiStatusValue(WIFI_STATUS_LOCATION)    : APP ��ġ���� �������� (Iocare2.0)

###### Return DATA
        0: APP���� ��ġ���� ���� X
        1: APP���� ��ġ���� ���� �Ϸ�
<Br>
    (9) GetWifiStatusValue(WIFI_STATUS_SAP_CODE)    : SAP �ΰ����� DATA ���ſ��� (A2021 SAP DATA) 

###### Return DATA
        0: SAP DATA �������� ����
        1: SAP DATA ���ż��� (S1000) 
        2: SAP DATA ���� (E9000)
<Br>
    (10) GetWifiStatusValue(WIFI_STATUS_FOTA)       : MCU FOTA ����  

###### Return DATA
        0: FOTA ���� �ƴ�
        1: FOTA �� ���� (FOTA ����)
<br>

#### - <2> Wifi Infomation data
##### #include "WIFI_Config.h" �߰��ʿ�
    (1) GetWifiRequestValue(WIFI_RQST_WEATHER)          : ���������� 

###### Return DATA
        1: ���� / 2: �������� / 3: �������� / 4: �帲 / 5: �帮�� �� / 6: �帮�� �� / 7: ��µ� �� / 8: �ҳ��� / 9: �� �Ǵ� �� / 11: õ�չ��� / 12: �Ȱ�
<br>

    (2) GetWifiRequestValue(WIFI_RQST_AQI) : AQI (1 ~ 4)

###### Return DATA
        1: ���� / 2: ���� / 3: ���� / 4: �ſ쳪��
<br>

    (3) GetWifiRequestValue(WIFI_RQST_CURRENT_TEMP)     : ����µ�

###### Return DATA
        -50 ~ 50
<br>

    (4) GetWifiRequestValue(WIFI_RQST_HIGH_TEMP)        : �ְ�µ� 

###### Return DATA
        -50 ~ 50
<br>

    (5) GetWifiRequestValue(WIFI_RQST_LOW_TEMP)         : �����µ�

###### Return DATA
        -50 ~ 50
<br>

    (6) GetWifiRequestValue(WIFI_RQST_FILTER_D_DAY1)    : �ڰ����� / �湮���� ����

###### Return DATA
    0xFFFF : �����Ͱ� ���� ���(�ڰ�) / 9999 : �湮����
<br>

    (7) GetWifiRequestValue(WIFI_RQST_FILTER_CHANGE_CYCLE) : ���ͱ�ȯ �ֱ�(%) (Ex: 15Month(�⺻) X 100%)

###### Return DATA
    100: 100%(�⺻), 1~999 %
<br>

    (8) GetWifiRequestValue(WIFI_RQST_PART_PERIOD)      : ������� �ֱ� ���� 

###### Return DATA
    1: 1�ð�(�⺻), 1~720
<br>

#### - <3> SAP �ΰ����� DATA ��û   [Parameter : X / Return : X ] 
##### #include "WIFI_Config.h" �߰��ʿ�
    GetWifiRequestSAP(void);                            : SAP �ΰ����� DATA ��û (A2021 Json DATA ��û)
<Br>


## 3. [WIFI_Config.h] / [WIFI_Config.c] Settings : ȯ�� ���� ���̵�
---
#### - <1> [WIFI_Config.h]
##### (1) User TEST MODE ����
    - TEST_MODE_WIFI_OTA    :  OTA ���� �׽�Ʈ ���� �� Ȱ��ȭ (WiFi OFF/RESET X )
        0: Normal / 1: OTA Test Mode       
    - TEST_MODE_BLE_FCC     :  BLE FCC ������ �׽�Ʈ ���� �� Ȱ��ȭ (BLE OFF X)
        0: Normal / 1: BLE FCC Test Mode   

##### (2) Version / Model Name / POP Code ����
    - USER_WIFI_VERSION     :   MCU Version (Ex: 0.0.0.1)
    - MODEL_NAME_0          :   Main Model Name  
    - MODEL_NAME_1          :   Sub Model Name (CHP/CP �� ����ȭ ���� �� Ȱ��)
    - PRODUCT_CODE_0        :   Main Model POP Code
    - PRODUCT_CODE_1        :   Sub Model POP Code (CHP/CP �� ����ȭ ���� �� Ȱ��)
    - WIFI_REQUEST_ID       :   MCU -> SERVER DATA ���� �� Reqeust ID prefix

##### (3) H/W Port ����
    - TXD1, RXD1 ���       :   Uart Baudrate (115200bps)	
    - P_WIFI_ON             :   Wi-Fi Module Power Pin No.
    - P_WIFI_RSTN           :   Wi-Fi Module Reset Pin No.

##### (4) FOTA ����
    - USER_FOTA_ENABLE      :   MCU FOTA ��� ��/�� ����
        0: DISABLE   /   1: ENABLE
    - USER_FOTA_SIZE        :   MCU ROM SIZE ���� 

##### (5) �ΰ����� DATA ����
    - USER_USE_A2010_INFO           : A2010 / A2011 ��� ��/�� ����
        0: DISABLE   /   1: ENABLE
    - USER_USE_A2010_INFO_WEATHER   : �������� ��� ��/�� ����
        0: DISABLE   /   1: ENABLE
    - USER_USE_A2021_SAPINFO        : SAP����(JSON) ��� ��/�� ����
        0: DISABLE   /   1: ENABLE

##### (6) User �������� DATA ������ ����
    - USER_DATA_MAX_SIZE                 : A1010 ���µ����� ���������� �ִ� ���� ����
    - USER_DATA_MULTI_CONTROL_MAX_SIZE   : A1011 ��Ƽ���� ���������� �ִ� ����
    - USER_INFO_MAX_SZIE                 : A2010 �ΰ����� ���������� �ִ� ����
    - USER_DIAGNOSIS_MAX_SIZE            : A1091 ����Ʈ����(��ǰ��ü����) �������� �ִ� ����

##### (7) A2021 SAP �ΰ����� �������� ����ü ����
    - USER_JSON_MAX_ITEMS           :   [Key : Value] �ְ��� �ִ� ����  -> { Key1 : Val1, Key2 : Val2, ... Key5 : Val5 }
    - USER_JSON_MAX_ARRAY_VALUE     :   Array �� �ִ� ����    ->  [ 1 ], [ 2 ]
    - USER_JSON_MAX_ARRAY_OBJECT    :   Array �� Object �ִ� ����   -> [ {1}, {2} ]

#### - <2> [WIFI_Config.c]
##### (1) EEPROM Write / Read ����
    - EEPROM_ADDR_WIFI_POWER    : Wi-Fi ON/OFF ���� ���
    - EEPROM_ADDR_FIRST_PARING  : Wi-Fi ���� �� ���� ���
    - EEPROM_ADDR_FOTA_MODULE   : Wi-Fi Module FOTA ���� ���
    - EEPROM_ADDR_FOTA_MCU      : MCU FOTA ���� ���
##### Write �Լ� �ۼ�   
    Wi-Fi ���� EEPROM ����� ���� EEPROM Write �Լ� �ۼ�
```C++
    void UserWriteEep ( U16 mu16Addr, U8 mu8Val )
    {
        /* Start user code.*/

        /* End user code. */
    }
```
##### Read �Լ� �ۼ�   
    Wi-Fi ���� EEPROM ����� ���� EEPROM Read �Լ� �ۼ�
```C++
    U8 UserReadEep ( U16 mu16Addr )
    {
        /* Start user code.*/

        /* End user code. */
    }
```
##### (2) Wi-Fi Buzz / Sound ����
    - WIFI_BUZZER_SELECT            : �� ���� �� (������ "��")
    - WIFI_BUZZER_AP_CONNECT        : AP������ (������ "�츮��" / Wi-Fi �����尡 Ȱ��ȭ �Ǿ����ϴ�.)
    - WIFI_BUZZER_BLE_CONNECT       : AP���� (������ "�츮��" / BLE�����尡 Ȱ��ȭ �Ǿ����ϴ�.)
    - WIFI_BUZZER_SETUP             : WIFI ON (������ "�򵿢�" / ��ǰ�� Wi-Fi�� �������ϴ�.)
    - WIFI_BUZZER_SERVER_CONNECT    : �������� (������ "�츮����" / ��ǰ�� ���� ���������� ����Ǿ����ϴ�.)
    - WIFI_BUZZER_CANCEL            : WIFI OFF (������ "����" / ��ǰ�� Wi-Fi�� �������ϴ�.)
    - WIFI_BUZZER_AP_START          : AP������ (������ "�� ��")
    - WIFI_BUZZER_ERROR             : �Ұ��� (������ "������")
    - WIFI_BUZZER_WiFi_OFF          : Wi-Fi �� ������(����) / Wi-Fi ���� ��尡 ��Ȱ��ȭ �Ǿ����ϴ�.
    - WIFI_BUZZER_BLE_OFF           : ������� �� ������(����) / ������� ���� ��尡 ��Ȱ��ȭ �Ǿ����ϴ�.
    - WIFI_BUZZER_BLE_CERT          : ���������Ϸ� ��(������ "��") / ��ǰ������ ���� ���Դϴ�. �ۿ��� ���̵��ϴ� ��ư�� �����ּ���.
    - WIFI_BUZZER_SERVER_FIRST      : �� �Ϸ� �� �������� (������ "�츮����" / ��ǰ�� ���� ���������� ����Ǿ����ϴ�.)
    - WIFI_BUZZER_SMART_CHECK_OK    : ������ ("�� ��") / ����Ʈ ������ �Ϸ� �Ǿ� ��ǰ�� ����Ͻ� �� �ֽ��ϴ�.
    - WIFI_BUZZER_SMART_CHECK_ERROR : �Ұ��� (������ "������") / ����Ʈ ������ �Ϸ� �Ǿ����ϴ�.
##### Buzz / Sound ��� �Լ� �ۼ�   
    Wi-Fi���� ����/���� ���� ����� ���� �ڵ� �ۼ�  
```C++
    void UserBuzzSound ( U16 mu16Buzz )
    {
        switch( mu16Buzz )
        {
        case WIFI_BUZZER_SELECT :
        /* Start user code.*/

        /* End user code. */
        break;
        ...
        }
    }
```
##### (3) ���� �ð� ����
    Ex)
    - mYear     : 2024 (2024��)
    - mMonth    : 12 (12��)
    - mDate     : 25 (25��)
    - mHour     : 23 (���� 11��)
    - mMin      : 59 (59��)
    - mSec      : 59 (59��)
    - mDay      : 3 (������) (1:�� ~ 7:��)
##### ���� �ð� ���� �Լ� �ۼ�
    �����κ��� �ð赥���� ���� �� ���� �Ʒ� �Լ� ȣ�� (�̺�Ʈ)
```C++
    void SetUserSystemTime ( U16 mYear, U16 mMonth, U16 mDate, U16 mHour, U16 mMin, U16 mSec, U16 mDay )
    {
        /* Start user code.*/

        /* End user code. */
    }
```
<br>

## 4. Protocol List Settings  : �������� ���� ���̵�
---
### ����: WifiServerProtocolList/
#### - <1> WIFI_A101x_FunctionList.c (���� ������)
##### (1) �������� LIST �ۼ�
    - API ID            : ���������� API ID 
<br>

    - DATA TYPE         : A1014 ��뿩�� / ����Ʈ���� �������� ��뿩�� ���� (�ܴ̿� �̻��)
        -> TYPE_UNUSED                  : DATA TYPE �̻�� (Normal)
        -> TYPE_A1014                   : A1014 ��뼱�� (��ǰ���� ���� ���� �� DATA����)  
        -> TYPE_SMART_DIAGNOSIS_CONTROL : ����Ʈ���� ����/���� �������� ID, ���ܽ��� �� ����ó���� ���� DATA TYPE ����
<br>

    - Initial VALUE     : Event DATA ó���� ���� �ʱⰪ (���� �ʿ� ����. [0]����)
<br>

    - Event Form        : �ش� �������� ID�� Event ó���� ���� Evnet ���� ���� (1�ʸ��� Event Check ����)
        -> EVENT_UNUSED         : Event ó�� ����
        -> EVENT_STATUS_CHANGE  : �ش� ID�� DATA ���� �� Event DATA ����
        -> EVENT_ZERO_TO_ONE    : �ش� ID�� DATA�� "0"���� "1"�� ���� �ø� Event DATA ����
        -> EVENT_ZERO_TO_OTHER  : �ش� ID�� DATA�� "0"���� "?"(0�� �ƴѰ�)���� ���� �� Event DATA ����
    [Eventó���� ��� ����(Event Form)�� �ش���� �ʴ� �̺�Ʈ ó���� ��� [EVENT_UNUSED]�� ���� �� Main ���α׷����� "WifiSendData(WIFI_DATA_FUNCTION)" ȣ�� �Ͽ� Event ó��]
<br>

```C++
// Ex)
WifiTxFuncEventList_T WifiFuncEventList[] = 
{   /*  API ID        DATA_TYPE                       (Initial val)         EventForm     */
    {   0x0001,       TYPE_A1014,                           0,         EVENT_STATUS_CHANGE },
    {   0x0002,       TYPE_A1014,                           0,         EVENT_STATUS_CHANGE },   
    {   0x0003,       TYPE_A1014,                           0,         EVENT_STATUS_CHANGE },   
    {   0x0004,       TYPE_UNUSED,                          0,         EVENT_STATUS_CHANGE },
    {   0x0005,       TYPE_UNUSED,                          0,         EVENT_STATUS_CHANGE },
    {   0x0006,       TYPE_UNUSED,                          0,         EVENT_STATUS_CHANGE },
    {   0x0007,       TYPE_UNUSED,                          0,         EVENT_STATUS_CHANGE },
    {   0x0008,       TYPE_SMART_DIAGNOSIS_CONTROL,         0,         EVENT_STATUS_CHANGE },   
    {   0x0009,       TYPE_UNUSED,                          0,         EVENT_STATUS_CHANGE },
    {   0x000A,       TYPE_UNUSED,                          0,         EVENT_STATUS_CHANGE },
};
```
##### (2) �������� DATA �ۼ�
    - �������� ID�� �ش��ϴ� DATA �ۼ�
<br>

    - [��ǰ] -> [����]

```C++
// Ex)
U16 GetUserSystemFunction ( U16 mu16Func )
{
    U16 mu16Data = 0U;
    switch ( mu16Func )
    {
        case 0x0002 :               /* ID */
            mu16Data = F_ColdOff;   /* ID�� �ش��ϴ� DATA ���� �ۼ� */
        break;
    }
}
```
<br>

    - [����] -> [��ǰ]
    
```C++
// Ex)
void SetUserSystemFunction ( U16 mu16Func, U16 mData )
{
    switch ( mu16Func )
    {
        case 0x0002 :               /* ID */
            if (mData == 1)         /* ID�� �ش��ϴ� DATA ���� �ۼ� */
            {
                F_ColdOff = CLEAR;
                Play_Voice(VOICE_COLD_ON_SETTING);
            }
            else 
            {
                F_ColdOff = SET;
                Play_Voice(VOICE_COLD_OFF_SETTING);
            }
        break;
    }
}
```

#### - <2> WIFI_A102x_SensorList.c (���� ������)
##### (1) �������� LIST �ۼ�
    - API ID            : ���������� API ID 
<br>

    - DATA TYPE         : A1014 ��뿩�� / ����Ʈ���� �������� ��뿩�� ���� (�ܴ̿� �̻��)
        -> TYPE_UNSIGNED                 : ��ȣ ���� ������ (0 ~ 65535)
        -> TYPE_SIGNED                   : ��ȣ �ִ� ������ (-32768 ~ 32767) 
        -> TYPE_STYPE_SENSOR_WATT        : ���� ������ 0.00 ~ 655.35
<br>

```C++
// Ex)
static const WifiNormalList_T WifiSensorProtocolList[] =
{   /*  API ID        Data Type     */
    {   0x0001,    TYPE_UNSIGNED    },
    {   0x0002,    TYPE_SIGNED      },
    {   0x0003,    TYPE_SENSOR_WATT },
    {   0x0004,    TYPE_SIGNED      },
    {   0x0005,    TYPE_SIGNED      },
    {   0x0006,    TYPE_SIGNED      },
    {   0x0007,    TYPE_SIGNED      },
    {   0x0008,    TYPE_SIGNED      },
    {   0x0009,    TYPE_SIGNED      },
    {   0x000A,    TYPE_SIGNED      },
};
```
##### (2) �������� DATA �ۼ�
    - �������� ID�� �ش��ϴ� DATA �ۼ�
<br>

    - [��ǰ] -> [����]

```C++
// Ex)
I16 GetUserSystemSensor ( U16 mu16Sen )
{
    I16 mi16Data = 0;

    switch ( mu16Sen )
    {
        case 0x0001:
            mi16Data = 1;
        break;
    }
    return mi16Data;
} 
```
<br>

#### - <3> WIFI_A103x_ErrorList.c (���� ������)
##### (1) �������� LIST �ۼ�
    - API ID            : ���������� API ID 
<br>

    - DATA TYPE         : (�̻��)
        -> TYPE_UNUSED  : DATA TYPE �̻�� (Normal)
<br>

    - Initial VALUE     : Event DATA ó���� ���� �ʱⰪ (���� �ʿ� ����. [0]����)
<br>

    - Event Form        : �ش� �������� ID�� Event ó���� ���� Evnet ���� ���� (1�ʸ��� Event Check ����)
        -> EVENT_STATUS_CHANGE  : �ش� ID�� DATA ���� �� Event DATA ���� (����)
<br>

```C++
// Ex)
WifiTxFuncEventList_T WifiErrorProtocolList[] =
{   /*  API ID              DATA_TYPE       PreVal(Initial)         EnventForm     */
    {   0x0001,          TYPE_UNUSED,           0,              EVENT_STATUS_CHANGE },
    {   0x0002,          TYPE_UNUSED,           0,              EVENT_STATUS_CHANGE },
    {   0x0003,          TYPE_UNUSED,           0,              EVENT_STATUS_CHANGE },
    {   0x0004,          TYPE_UNUSED,           0,              EVENT_STATUS_CHANGE },
    {   0x0005,          TYPE_UNUSED,           0,              EVENT_STATUS_CHANGE },
    {   0x0006,          TYPE_UNUSED,           0,              EVENT_STATUS_CHANGE },
    {   0x0007,          TYPE_UNUSED,           0,              EVENT_STATUS_CHANGE },
    {   0x0008,          TYPE_UNUSED,           0,              EVENT_STATUS_CHANGE },
    {   0x0009,          TYPE_UNUSED,           0,              EVENT_STATUS_CHANGE },
    {   0x000A,          TYPE_UNUSED,           0,              EVENT_STATUS_CHANGE },
};
```
##### (2) �������� DATA �ۼ�
    - �������� ID�� �ش��ϴ� DATA �ۼ�
<br>

    - [��ǰ] -> [����]
    - �ش� �������������� Error ���� [0] / Error ���� [1] 

```C++
// Ex)
I16 GetUserSystemError ( U16 mu16Err )
{
    U16 mu16Data = 0;

    switch ( mu16Err )
    {
        case 0x0001:
            mu16Data = 1;
        break;
    }
    return mu16Data;
} 
```
<br>

#### - <4> WIFI_A108x_A109x_PartList.c (����Ʈ���� ������)
##### (1) �������� LIST �ۼ�
    - API ID            : ���������� API ID 
<br>

    - DATA TYPE         
        -> TYPE_UNSIGNED                 : ��ȣ ���� ������ (0 ~ 65535)
        -> TYPE_SIGNED                   : ��ȣ �ִ� ������ (-32768 ~ 32767) 
<br>

```C++
// Ex)
static const WifiNormalList_T WifiParaProtocolList[] =
{   /*  API ID        */
    {   0x0001,    TYPE_UNSIGNED   },
    {   0x0002,    TYPE_UNSIGNED   },
    {   0x0003,    TYPE_UNSIGNED   },
    {   0x0004,    TYPE_UNSIGNED   },
    {   0x0005,    TYPE_UNSIGNED   },
    {   0x0006,    TYPE_UNSIGNED   },
    {   0x0007,    TYPE_UNSIGNED   },
    {   0x0008,    TYPE_UNSIGNED   },
    {   0x0009,    TYPE_UNSIGNED   },
    {   0x000A,    TYPE_UNSIGNED   },
};
```
##### (2) �������� DATA �ۼ�
    - �������� ID�� �ش��ϴ� DATA �ۼ�
<br>

    - [��ǰ] -> [����]

```C++
// Ex)
U16 GetUserSystemPara ( U16 mu16Para )
{
    U16 mu16Data = 0U;
    switch ( mu16Para )
    {
        case 0x0001 :       /* ID */
            mu16Data = 1;   /* Value */
        break;
    }
    return mi16Data;
} 
```
<br>

#### - <5> WIFI_A20xx_InformationList.c (�ΰ����� ������)
##### (1) �������� LIST �ۼ�
    - API ID            : ���������� API ID 
<br>

    - DATA TYPE         
        -> TYPE_INFO_NORMAL                 : �⺻ ������
        -> TYPE_INFO_TIME                   : �ð� ������
        -> TYPE_INFO_WEATHER                : ���� ������
        -> TYPE_INFO_AQI                    : ������ ������
        -> TYPE_INFO_TEMP                   : �µ� ������
        -> TYPE_INFO_DIAGNOSIS_RQ           : �������� ��� ��û ������
        -> TYPE_INFO_JSON                   : SAP �ΰ����� JSON ������
<br>

```C++
// Ex) ����ϴ� �����͸� Ȱ��ȭ ����
/* (1) Initial or 24H cycle                 : TYPE_INFO_TIME / TYPE_INFO_NORMAL / TYPE_INFO_WEATHER / TYPE_INFO_TEMP / TYPE_INFO_JSON */
/* (2) WifiSendData(WIFI_DATA_INFO) Reqest  : TYPE_INFO_TIME / TYPE_INFO_NORMAL / TYPE_INFO_WEATHER */
/* (3) 20Min cycle                          : TYPE_INFO_NORMAL / TYPE_INFO_WEATHER / TYPE_INFO_TEMP */
/* (4) Smart Daignosis Result receive (Server->Product) : TYPE_INFO_DIAGNOSIS_RQ (Displaying results on LCD)*/
/* (5) GetWifiRequestSAP() Reqest           : TYPE_INFO_JSON */
/* Event List */
static const WifiNormalList_T WifiInfoProtocolList[] =
{   /*  API ID                                      Data Type     */
    {   WIFI_INFO_0000_YEAR,                     TYPE_INFO_TIME         },	
    {   WIFI_INFO_0001_MONTH,                    TYPE_INFO_TIME         },  
    {   WIFI_INFO_0002_DATE,                     TYPE_INFO_TIME         },  
    {   WIFI_INFO_0003_HOUR,                     TYPE_INFO_TIME         },  
    {   WIFI_INFO_0004_MINUTE,                   TYPE_INFO_TIME         },  
    {   WIFI_INFO_0005_SECOND,                   TYPE_INFO_TIME         },  
    {   WIFI_INFO_0006_DAY,                      TYPE_INFO_TIME         },  
    {   WIFI_INFO_0007_PART_PERIOD,              TYPE_INFO_NORMAL       },
    
    {   WIFI_INFO_0010_WEATHER,                  TYPE_INFO_WEATHER      },
    {   WIFI_INFO_0011_AQI,                      TYPE_INFO_AQI          },
    // {   WIFI_INFO_0012_PM10,                     TYPE_INFO_AQI          },
    // {   WIFI_INFO_0013_PM25,                     TYPE_INFO_AQI          },
    {   WIFI_INFO_0014_CURRENT_TEMP,             TYPE_INFO_TEMP         },	
    {   WIFI_INFO_0015_HIGH_TEMP,                TYPE_INFO_TEMP         },	
    {   WIFI_INFO_0016_LOW_TEMP,                 TYPE_INFO_TEMP         },
    
    // {   WIFI_INFO_0020_LAST_HEART_YEAR,          TYPE_INFO_NORMAL       },
    // {   WIFI_INFO_0021_LAST_HEART_MONTH,         TYPE_INFO_NORMAL       },
    // {   WIFI_INFO_0022_LAST_HEART_DAY,           TYPE_INFO_NORMAL       },
    // {   WIFI_INFO_0023_NEXT_HEART_YEAR,          TYPE_INFO_NORMAL       },
    // {   WIFI_INFO_0024_NEXT_HEART_MONTH,         TYPE_INFO_NORMAL       },
    // {   WIFI_INFO_0025_NEXT_HEART_DAY,           TYPE_INFO_NORMAL       },
    {   WIFI_INFO_0026_FILTER_CHANGE_DAY1,       TYPE_INFO_NORMAL       },
    // {   WIFI_INFO_0027_FILTER_CHANGE_DAY2,       TYPE_INFO_NORMAL       },   
    // {   WIFI_INFO_0028_FILTER_CHANGE_CYCLE,      TYPE_INFO_NORMAL       },
    {   WIFI_INFO_0029_DIAGNOSIS_RESULT_REQUEST, TYPE_INFO_DIAGNOSIS_RQ },
    {   WIFI_INFO_0030_STERILIZE_DATE,           TYPE_INFO_JSON         },
    {   WIFI_INFO_0031_SUPPLAY,                  TYPE_INFO_JSON         },
    {   WIFI_INFO_0032_ENVIRONMENT,              TYPE_INFO_JSON         } 
};
```
##### (2) JSON �������� ������ �ۼ�
    - �������� ID�� �ش��ϴ� DATA �ۼ�
<br>

    - [����] -> [��ǰ]
```C++
// Ex) JSON ������ �Ľ�
void SetUserSystemJsonObject ( U16 mu16Info, I8 *pBuf )
{
	U8 mu8KeyIndex = 0;
	U8 mu8SAPCode = 0;
	switch ( mu16Info )
    {
		case WIFI_INFO_JSON_0030_UV_DATE:   // UV �ֱٻ�սð� / UV��տ����ð� ��������

            _MEMSET_( (void __far*) &JsonObjectDepth_1, 0, sizeof(JSONObject) );
            _MEMSET_( (void __far*) &JsonObjectDepth_2, 0, sizeof(JSONObject) );

            cJsonParseJson(pBuf, &JsonObjectDepth_1, NULL);			
            // Object O, Array X - Json parsing Depth1 c:"S1000"

            mu8SAPCode = JsonSAPCodeSuccess(&JsonObjectDepth_1);
            if (mu8SAPCode == F1000)      // SAP ���� ���� (CODE : F1000, SAP001)
            {
                SetWifiSapStatus(STATUS_SVR_SAP_0030_UV_DATE, F1000);
                return;
            }
            else if (mu8SAPCode == E9000) // E9000 (������ ����)
            {
                SetWifiSapStatus(STATUS_SVR_SAP_0030_UV_DATE, E9000);
            }
            else	                      // S1000 (������ �������� ����)
            {
                SetWifiSapStatus(STATUS_SVR_SAP_0030_UV_DATE, S1000);
            }

            mu8KeyIndex = (U8)GetJsonKeyIndexSearch(JSON_KEY_UV_FAUCETINFO, &JsonObjectDepth_1);   // Key "fc"
            if(mu8KeyIndex != (U8)0xFF){ // Index ã�� �Ϸ� ��
                cJsonParseJson(JsonObjectDepth_1.items[mu8KeyIndex].value, &JsonObjectDepth_2, NULL); 
                // Object O, Array X  - Depth2 Parsing : "fc": { Key:Value, Key:Value }

                /* �Ŀ�� UV �ֱ� ���۽ð� */
                GetJsonDateValue(gJsonValue.UV_Faucet.u8JsonDateLD, JSON_KEY_LAST_DATE, &JsonObjectDepth_2);
                // [gJsonValue.UV_Faucet.u8JsonDateLD]�� �Ľ� DATA ����

                /* �Ŀ�� UV ���� �����ð� */
                GetJsonDateValue(gJsonValue.UV_Faucet.u8JsonDateND, JSON_KEY_NEXT_DATE, &JsonObjectDepth_2);
                //[gJsonValue.UV_Faucet.u8JsonDateND]�� �Ľ� DATA ����
            }
		break;
		default:
			
			break;

	}
}


```

<br>

#### - <6> WIFI_A105x_ParaList.c (�Ķ���� ������)
#### - <7> WIFI_A10Ax_AccumulateList.c (�������� ������)
