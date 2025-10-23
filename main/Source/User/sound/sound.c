#include "sound.h"
#include "buzzer.h"
#include "eeprom.h"

typedef struct _sound_
{
    U8   Mute;
} Sound_T;

Sound_T sound;

typedef struct _sound_list_t
{
    SoundId_T   SoundId;
    U8   BuzzerId;
} SoundList_T;

SoundList_T  SoundList[] = 
{
    /* SOUND ID */                /* BUZZER ID */
    { SOUND_POWER_OFF             , BUZZER_OFF },
    { SOUND_POWER_ON              , BUZZER_POWER_ON },
    { SOUND_EFFLUENT              , BUZZER_EFFLUENT },
    { SOUND_EFFLUENT_END          , BUZZER_EFFLUENT_END },       
    { SOUND_CONFIG_SETUP          , BUZZER_COOL_SETUP },         
    { SOUND_CONFIG_CANCEL         , BUZZER_COOL_CANCEL },        
    { SOUND_SELECT                , BUZZER_SELECT },             
    { SOUND_ERROR                 , BUZZER_ERROR },              
    { SOUND_SETUP                 , BUZZER_SETUP },              
    { SOUND_CANCEL                , BUZZER_CANCEL },             
    { SOUND_LOCK_SETUP            , BUZZER_SETUP },              
    { SOUND_LOCK_CANCEL           , BUZZER_CANCEL },             
    { SOUND_EFFLUENT_CONTINUE     , BUZZER_EFFLUENT_CONTINUE },  
    { SOUND_STERILIZE_START       , BUZZER_STERILIZE_START },    
    { SOUND_STERILIZE_END         , BUZZER_STERILIZE_END },      
    { SOUND_MEMENTO_1             , BUZZER_MEMENTO_1 },          
    { SOUND_MEMENTO_2             , BUZZER_MEMENTO_2 },          
    { SOUND_MEMENTO_3             , BUZZER_MEMENTO_3 },          
    { SOUND_MEMENTO_4             , BUZZER_MEMENTO_4 },          
    { SOUND_MEMENTO_5             , BUZZER_MEMENTO_5 },          
    { SOUND_WIFI_SELECT           , BUZZER_WIFI_SELECT },          
    { SOUND_WIFI_SETUP            , BUZZER_WIFI_SETUP },
    { SOUND_WIFI_ERROR            , BUZZER_WIFI_ERROR },
    { SOUND_WIFI_CONNECTING       , BUZZER_WIFI_CONNECTING },          
    { SOUND_WIFI_CONNECTED        , BUZZER_WIFI_CONNECTED },          
    { SOUND_WIFI_FAIL_CONNECTING  , BUZZER_WIFI_FAIL_CONNECTING },          
    { SOUND_WIFI_DISCONNECTED     , BUZZER_WIFI_DISCONNECTED },          
    { SOUND_BLE_FAIL              , BUZZER_BLE_FAIL },          
    { SOUND_MUTE                  , BUZZER_MUTE },               
};
#define	SZ_LIST		( sizeof( SoundList ) / sizeof( SoundList_T ) )

void Sound(SoundId_T mId)
{
    U8   i;
    SoundList_T *pList = NULL;

    for( i = 0; i < SZ_LIST ; i++ )
    {
        pList = &SoundList[ i ];
        if( pList->SoundId == mId )
        {
            SetBuzzOut( pList->BuzzerId );
            break;
        }
    }
}

U8	IsSetSoundMute(void)
{
    return IsSetSilent();
}

void SetSoundMute(void)
{
    sound.Mute = ON;
    SaveEepromId( EEP_ID_CONF_MUTE );

    SetOnOffSilent( ON );
}

U8    GetSoundMute(void)
{
    return sound.Mute;
}

void	ClearSoundMute(void)
{
    sound.Mute = OFF;
    SaveEepromId( EEP_ID_CONF_MUTE );

    SetOnOffSilent( OFF );
}

void TEST_Sound(void)
{
    static U8 count = 0;


    Sound( count );
    count++;
    if( count >= MAX_SOUND_NUM )
    {
        count = 0;
    }
}
