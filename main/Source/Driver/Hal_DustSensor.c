//FILE COMMENT""****************************************************
//@ FILE Name : DustSensor.c
//@ Description : 통합 먼지센서 드라이버 구현 파일
//FILE COMMENT END ""************************************************

// 1. 매크로 정의 - 하나만 선택하여 사용
// #define USE_PMS5003
// #define USE_PPD71
// #define USE_PPD42NS
// #define USE_PPD4260

// 2. 헤더 파일 포함
#include "Global_Header.h"


// 3. 전역 변수 선언
// 3.1 공통 전역 변수
static const SENSOR_TYPE_T g_currentSensorType = CURRENT_SENSOR_TYPE;
static SENSITIVITY_T g_sensitivity = SENSITIVITY_NORMAL;
static U8 g_errorFlag = CLEAR;
static U8 gu8DustSensorControlTimer = 0;



// 3.2 센서별 전역 변수
#if (USE_PMS5003 == 1)
    static volatile U8 s_packet_received_flag = 0;           // 수신 완료 플래그
    static U8 gu8UARTOK_F = 0;
    static U8 gu8ParticleRxDataBuffer[32] = {0};
    static U16 gu16PM1p0Data = 0;
    static U16 gu16PM2p5Data = 0;
    static U16 gu16PM10Data = 0;
    static U16 gu16PM1p0DataStd = 0;
    static U16 gu16PM2p5DataStd = 0;
    static U16 gu16PM10DataStd = 0;
    static U16 gu16PM2p5ConcentDisplayFinal = 0;
    static U16 gu16PM10ConcentDisplayFinal = 0;
    static U16 gu16Dust0p3CntData = 0;
    static U16 gu16Dust0p5CntData = 0;
    static U16 gu16Dust1p0CntData = 0;
    static U16 gu16Dust2p5CntData = 0;
    static U16 gu16Dust5p0CntData = 0;
    static U16 gu16Dust10CntData = 0;
    static U8 gu8RxCnt = 0;
    static U8 gu8RxProc_F = 0;
    static U8 gu8RxDone_F = 0;
    static U8 gu8ErrParticle1 = CLEAR;
    static U8 gu8ErrParticle2 = CLEAR;
    static U8 DebugNmxRx[3] = {0};
#endif

#if (USE_PPD71 == 1)
    static U8 gu8UARTOK_F = 0;
    static U8 gu8ParticleDataCheck = 0;
    static U8 gu8ParticleRxDataBuffer[32] = {0};
    static U16 gu16MassConcent10s = 0;
    static U16 gu16MassConcent30s = 0;
    static U16 gu16MassConcent60s = 0;
    static U16 gu16MassConcent180s = 0;
    static U16 gu16Lpo0p5 = 0;
    static U16 gu16Lpo0p7 = 0;
    static U16 gu16Lpo1p0 = 0;
    static U16 gu16Lpo2p5 = 0;
    static D64 gu64MassConcent10s = 0.0f;
    static D64 gu64MassConcent30s = 0.0f;
    static D64 gu64MassConcent60s = 0.0f;
    static D64 gu64MassConcent180s = 0.0f;
    static D64 gu64Buf10s[2] = {0.0f, 0.0f};
    static D64 gu64Buf180s[2] = {0.0f, 0.0f};
    static D64 gu64BufLpo2p5[2] = {0.0f, 0.0f};
    static D64 gu64PM2p5Cond1AbsVal = 0.0f;
    static D64 gu64PM2p5Cond1StdVal = 0.0f;
    static D64 gu64PM2p5Cond2AbsVal = 0.0f;
    static D64 gu64PM2p5Cond2StdVal = 0.0f;
    static D64 gu64PM2p5Cond2StdValMax = 0.0f;
    static U8 gu8PM2p5Cond1_F = 0;
    static U8 gu8PM2p5Cond2_F = 0;
    static U8 gu8PM2p5BeCond1_F = 0;
    static U8 gu8PM2p5BeCond2_F = 0;
    static D64 gu64PM2p5BeConcent = 0.0f;
    static D64 gu64PM2p5Concent1 = 0.0f;
    static D64 gu64PM2p5Concent2 = 0.0f;
    static D64 gu64PM2p5ConcentFinal = 0.0f;
    static D64 gu64PM2p5ConcentDisplayFinal = 0.0f;
    static D64 gu64PM10Concent1 = 0.0f;
    static D64 gu64PM10Concent180s[90] = {0.0f};
    static D64 gu64PM10ConcentFinal = 0.0f;
    static D64 gu64PM10ConcentDisplayFinal = 0.0f;
    static U8 gu8CntBuf180s = 0;
    static U8 gu8Buf180s_F = 0;
    static U8 gu8RxCnt = 0;
    static U8 gu8RxProc_F = 0;
    static U8 gu8RxDone_F = 0;
    static U8 gu8ErrParticle1 = CLEAR;
    static U8 gu8ErrParticle2 = CLEAR;
    static U8 gu8CountCheckSum = 0;
    static U8 gu8RxDatastreamCheckTimer = 0;
    static U8 gu8ParticleErrCount = 0;
    D64 gu64PM2p5ConcentDisplay1 = 0;
    D64	gu64PM2p5ConcentDisplayFinal = 0;
    D64 gu64PM10ConcentDisplayFinal = 0;

    D64 gu64PM2p5ConcentFinalNormal = 0;
    D64 gu64PM2p5ConcentFinalSharp = 0;
    D64 gu64PM2p5ConcentFinalDull = 0;
    SENSITIVITY_T gu8ParticleSensitivity = SENSITIVITY_NORMAL;	

#endif

#if (USE_PPD42NS == 1)
    static U16 gu16Volume = 0;
    static U16 gu16VolumeBuf[5] = {0};
    static U16 gu16SkipSensing = 0;
    static U16 gu16ParticleInitTime = PARTICLE_INITTIME;
    static U16 gu16ErrParticle = PARTICLE_ERRORTIME;
	static U8 gu8PPD42NSTimer = 0;
#endif

#if (USE_PPD4260 == 1)
    static U16 gu16VolumeP1 = 0;
    static U16 gu16VolumeP2 = 0;
    static U16 gu16SkipSensing = 0;
    static D64 gd64CalAvgPM2p5 = 0;
    static D64 gd64CalAvgPM10 = 0;

    // PPD4260 함수 선언
    static void PPD4260_Init(void);
    static void PPD4260_Handler(void);
    static void PPD4260_ReadParticleData(void);
    static void PPD4260_CalParticleConcent(void);
#endif

// PPD4260 함수 선언
static void PPD4260_Init(void);
static void PPD4260_Handler(void);
static void PPD4260_ReadParticleData(void);
static void PPD4260_CalParticleConcent(void);

// 4. 유틸리티 함수 정의
static void initParticleSensorVar(void) {
#if (USE_PPD42NS == 1)
    gu16Volume = 0;
    gu16VolumeSum = 0;
    gu16SkipSensing = 0;
    gu16ParticleInitTime = PARTICLE_INITTIME;
    gu16ErrParticle = PARTICLE_ERRORTIME;
    memset(gu16VolumeBuf, 0, sizeof(gu16VolumeBuf));
#endif

#if (USE_PPD4260 == 1)
    gu16VolumeP1 = 0;
    gu16VolumeP2 = 0;
    gu16SkipSensing = 0;
    gd64CalAvgPM2p5 = 0;
    gd64CalAvgPM10 = 0;
#endif
}

static void clrAu16DustDataAvr(void) {
#if (USE_PMS5003 == 1)
    gu16PM1_0Finish = 0;
    gu16PM2_5Finish = 0;
    gu16PM10Finish = 0;
    memset(gu8ParticleRxDataBuffer, 0, sizeof(gu8ParticleRxDataBuffer));
#endif
}

// 5. 센서별 구현 함수
// 5.1 PMS5003 센서 구현
#if (USE_PMS5003 == 1)
static void PMS5003_Init(void) {
    gu8UARTOK_F = 0;
    gu8RxCnt = 0;
    gu8RxProc_F = CLEAR;
    gu8RxDone_F = CLEAR;
    gu8ErrParticle1 = CLEAR;
    gu8ErrParticle2 = CLEAR;
    memset(gu8ParticleRxDataBuffer, 0, sizeof(gu8ParticleRxDataBuffer));
}

static void PMS5003_Handler(void) {
    PMS5003_RxParticleData();   // 먼지센서 데이터 수신
    PMS5003_CalcParticleData(); // 먼지센서 데이터 계산
    PMS5003_CheckError();        // 먼지센서 데이터 오류 체크
    PMS5003_CheckDataStream();   // 먼지센서 데이터 스트림 오류 체크
}


static void PMS5003_RxParticleData(void) {
    MD_STATUS status = R_Config_UART2_Receive((U8*)gu8ParticleRxDataBuffer, 32);
    if (status != MD_OK) {
        // 에러 처리: 수신 재시작 실패
    }
}

static void PMS5003_CalcParticleData(void) {
    U8 mu8Cnt = 0;
    static U8 mu8PoweroffInit_F = 0;
    D64 md64PM2p5_1 = 0;
    D64 md64PM2p5_2 = 0;
    D64 md64ShiftData180s = 0;
    D64 md64SumPM10180s = 0;    
    D64 md64Val = 0;
    D64 mu64ShiftDataSensitivity = 0;
    U16 Temp16 = 0;

    if(gu8UARTOK_F) {
        
        gu8UARTOK_F = 0;

        gu16PM1p0DataStd = gu8ParticleRxDataBuffer[4];
        gu16PM1p0DataStd <<= 8;
        gu16PM1p0DataStd |= gu8ParticleRxDataBuffer[5];

        gu16PM2p5DataStd = gu8ParticleRxDataBuffer[6];
        gu16PM2p5DataStd <<= 8;
        gu16PM2p5DataStd |= gu8ParticleRxDataBuffer[7];

        gu16PM10DataStd = gu8ParticleRxDataBuffer[8];
        gu16PM10DataStd <<= 8;
        gu16PM10DataStd |= gu8ParticleRxDataBuffer[9];

        gu16PM1p0Data = gu8ParticleRxDataBuffer[10];
        gu16PM1p0Data <<= 8;
        gu16PM1p0Data |= gu8ParticleRxDataBuffer[11];

        Temp16 = gu8ParticleRxDataBuffer[12];
        Temp16 <<= 8;
        Temp16 |= gu8ParticleRxDataBuffer[13];
        gu16PM2p5ConcentDisplayFinal = Temp16;

        Temp16 = gu8ParticleRxDataBuffer[14];
        Temp16 <<= 8;
        Temp16 |= gu8ParticleRxDataBuffer[15];
        gu16PM10ConcentDisplayFinal = Temp16;

        gu16Dust0p3CntData = gu8ParticleRxDataBuffer[16];
        gu16Dust0p3CntData <<= 8;
        gu16Dust0p3CntData |= gu8ParticleRxDataBuffer[17];

        gu16Dust0p5CntData = gu8ParticleRxDataBuffer[18];
        gu16Dust0p5CntData <<= 8;
        gu16Dust0p5CntData |= gu8ParticleRxDataBuffer[19];

        gu16Dust1p0CntData = gu8ParticleRxDataBuffer[20];
        gu16Dust1p0CntData <<= 8;
        gu16Dust1p0CntData |= gu8ParticleRxDataBuffer[21];

        gu16Dust2p5CntData = gu8ParticleRxDataBuffer[22];
        gu16Dust2p5CntData <<= 8;
        gu16Dust2p5CntData |= gu8ParticleRxDataBuffer[23];

        gu16Dust5p0CntData = gu8ParticleRxDataBuffer[24];
        gu16Dust5p0CntData <<= 8;
        gu16Dust5p0CntData |= gu8ParticleRxDataBuffer[25];

        gu16Dust10CntData = gu8ParticleRxDataBuffer[26];
        gu16Dust10CntData <<= 8;
        gu16Dust10CntData |= gu8ParticleRxDataBuffer[27];
        
        // 최종값 업데이트
        gu16PM1_0Finish = gu16PM1p0Data;
        gu16PM2_5Finish = gu16PM2p5Data;
        gu16PM10Finish = gu16PM10Data;
    }
}

static void PMS5003_CheckError(void) {
    // 미구현
}

static void PMS5003_CheckDataStream(void) {
    // 미구현
}
#endif

// 5.2 PPD71 센서 구현
#if (USE_PPD71 == 1)
static void PPD71_Init(void) {
    gu8UARTOK_F = 0;
    gu8RxCnt = 0;
    gu8RxProc_F = CLEAR;
    gu8RxDone_F = CLEAR;
    gu8ErrParticle1 = CLEAR;
    gu8ErrParticle2 = CLEAR;
    gu8CountCheckSum = 0;
    gu8RxDatastreamCheckTimer = PPD71_DATASTREAM_CHECKTIME;
    gu8ParticleErrCount = 0;
    memset(gu8ParticleRxDataBuffer, 0, sizeof(gu8ParticleRxDataBuffer));
}

static void PPD71_Handler(void) {
    PPD71_RxParticleData();
    PPD71_CalParticleConcent();
    PPD71_CheckParticleDataError();
    PPD71_CheckParticleDatastreamError();
}


static void PPD71_RxParticleData(void) 
{
	MD_STATUS status = R_Config_UART2_Receive((U8*)gu8ParticleRxDataBuffer, 29);
    if (status != MD_OK) {
        // 에러 처리: 수신 재시작 실패
    }
}

static void PPD71_CalParticleConcent(void) {
    U8  mu8Cnt = 0;
	static U8 mu8PoweroffInit_F = 0;
	D64 md64PM2p5_1 = 0;
	D64 md64PM2p5_2 = 0;
	D64 md64ShiftData180s = 0;
	D64 md64SumPM10180s = 0;	
	D64 md64Val = 0;
	D64 mu64ShiftDataSensitivity = 0;

    if(gu8UARTOK_F) {

        //============================================================================================================
		// #1. 데이터 수신 대기 
		//============================================================================================================

	    // 먼지센서 데이터 처리부
		gu16MassConcent10s  = ((U16)(gu8ParticleRxDataBuffer[3]) << 8) + (U16)(gu8ParticleRxDataBuffer[4]);
    	gu16MassConcent30s  = ((U16)(gu8ParticleRxDataBuffer[5]) << 8) + (U16)(gu8ParticleRxDataBuffer[6]);
		gu16MassConcent60s  = ((U16)(gu8ParticleRxDataBuffer[7]) << 8) + (U16)(gu8ParticleRxDataBuffer[8]);
		gu16MassConcent180s = ((U16)(gu8ParticleRxDataBuffer[9]) << 8) + (U16)(gu8ParticleRxDataBuffer[10]);  //PM2.5 수치 적용

		gu16Lpo0p5 = ((U16)(gu8ParticleRxDataBuffer[11]) << 8) + (U16)(gu8ParticleRxDataBuffer[12]);
		gu16Lpo0p7 = ((U16)(gu8ParticleRxDataBuffer[13]) << 8) + (U16)(gu8ParticleRxDataBuffer[14]);
		gu16Lpo1p0 = ((U16)(gu8ParticleRxDataBuffer[15]) << 8) + (U16)(gu8ParticleRxDataBuffer[16]);
		gu16Lpo2p5 = ((U16)(gu8ParticleRxDataBuffer[17]) << 8) + (U16)(gu8ParticleRxDataBuffer[18]);


        // Raw Data 보정 부분 추가로 인한 프로그램 수정 2018.06.29
        
        gu64MassConcent10s = (D64)gu16MassConcent10s;    // U16 -> D64 변환
		gu64MassConcent30s = (D64)gu16MassConcent30s;
		gu64MassConcent60s = (D64)gu16MassConcent60s;
		gu64MassConcent180s = (D64)gu16MassConcent180s;
		
        // PM2.5 10초 농도값 
		gu64Buf10s[2] = gu64Buf10s[1];       // 4초전 Data
		gu64Buf10s[1] = gu64Buf10s[0];       // 2초전 Data
		gu64Buf10s[0] = gu64MassConcent10s;  // 현재 Data
		
		// PM2.5 180초 농도값
		gu64Buf180s[2] = gu64Buf180s[1];
		gu64Buf180s[1] = gu64Buf180s[0];
		gu64Buf180s[0] = gu64MassConcent180s;

		// PM2.5 이상 LPO
		gu64BufLpo2p5[2] = gu64BufLpo2p5[1];
		gu64BufLpo2p5[1] = gu64BufLpo2p5[0];
		gu64BufLpo2p5[0] = (D64)gu16Lpo2p5;

		//============================================================================================================
		// *PM2.5 농도가 5ug/m^3 이하인 경우 10초/180초 농도를 조건에 따라 사용
        //
		//  부연설명
		// -.극저 농도 상황에서 지속적으로 180초 농도를 참조할 경우, 값의 흔들림이 현저히 적어지는 문제 발생
		// -.가시적 농도 변화를 확인 할수 있도록 10초 농도 혼용하여 반응성 고려 
		//============================================================================================================

		if(gu64MassConcent180s <= 5.f) // PM2.5 180초 농도 데이터가 5이하 인 경우는 180초 농도 그대로 사용 (알고리즘 없음)
		{
		    // 클린상태에서 지속적으로 1로만 표시될 경우 고객 감성 불만 우려로 품질 임재환 부장 요청으로 프로그램 수정
            if(gu64MassConcent10s <= 5.f)
            {
                gu64PM2p5ConcentFinal = gu64MassConcent10s;  // PM2.5 180초 데이터와 10초 데이터 모두 5이하 인경우 10초 데이터 표시 
            }
			else
			{
				gu64PM2p5ConcentFinal = gu64MassConcent180s; // PM2.5 180초 데이터와 10초 데이터 모두 5이하가 아닌 경우 180초 데이터 표시 
			}

			gu64PM2p5BeConcent = gu64PM2p5ConcentFinal;  // 최종 출력 농도값 저장
			gu8PM2p5BeCond1_F = 0;			// 기존 조건1 상태 불만족
			gu8PM2p5BeCond2_F = 0;			// 기존 조건2 상태 불만족

			//============================================================================================================
	    	// 계측기 농도 지향성 보정(디스플레이)
	    	//============================================================================================================

        	md64ShiftData180s = (REF_COEFFICIENT_2*(gu64PM2p5ConcentFinal*gu64PM2p5ConcentFinal)) + (REF_COEFFICIENT_1*gu64PM2p5ConcentFinal) + REF_COEFFICIENT_0; 

		    if((gu64PM2p5ConcentFinal < 140.f) && (gu64PM2p5ConcentFinal >= 15.f))
        	{
        		gu64PM2p5ConcentDisplay1 = gu64PM2p5ConcentFinal + md64ShiftData180s;
        	}
			else
			{
			    gu64PM2p5ConcentDisplay1 = gu64PM2p5ConcentFinal;
		    }
    
			// PM2.5 음수값 출력 시
			if(gu64PM2p5ConcentDisplay1 <= 0)
			{
				gu64PM2p5ConcentDisplay1 = 0;
			}
			else
			{
			}

			// PM2.5 MAX값 : 1000
			if(gu64PM2p5ConcentDisplay1 >= 1000.f)
			{
				gu64PM2p5ConcentDisplay1 = 1000.f;
			}
			else
			{
			}

			
		  //============================================================================================================
		  // 민감도 설정에 따른 농도 보정 (디스플레이용/킹덤 신규 적용)
		  //============================================================================================================
		  mu64ShiftDataSensitivity = (SEN_COEFFICIENT_2*(gu64PM2p5ConcentDisplay1*gu64PM2p5ConcentDisplay1)) + (SEN_COEFFICIENT_1*gu64PM2p5ConcentDisplay1) + SEN_COEFFICIENT_0;
		  gu64PM2p5ConcentFinalNormal = gu64PM2p5ConcentDisplay1;
		  gu64PM2p5ConcentFinalSharp = gu64PM2p5ConcentDisplay1 + mu64ShiftDataSensitivity;
		  gu64PM2p5ConcentFinalDull = gu64PM2p5ConcentDisplay1 - mu64ShiftDataSensitivity;
		  
		  if(gu8ParticleSensitivity == SENSITIVITY_NORMAL)  //민감도 보통(default)
		  {
		    gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalNormal;
		  }
		  else  //민감도 민감 또는 둔감
		  {
		    if((gu64PM2p5ConcentDisplay1 >= SEN_AREA_MIN) && (gu64PM2p5ConcentDisplay1 < SEN_AREA_MAX)) //보정 범위
		    {
		      if(gu8ParticleSensitivity == SENSITIVITY_SHARP) //민감도 민감
		      {
		        gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalSharp;
		      }
		      else if(gu8ParticleSensitivity == SENSITIVITY_DULL)//민감도 둔감
		      {
		        gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalDull;
		      }
			  else
			  {
			  }
		    }
		    else  //보정 범위가 아닐때
		    {
		      gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalNormal;
		    }
		  }
		  
		    // PM2.5 음수값 출력 시
			if(gu64PM2p5ConcentDisplayFinal <= 0)
			{
				gu64PM2p5ConcentDisplayFinal = 0;
			}
	    
			// PM2.5 MAX값 
			if(gu64PM2p5ConcentDisplayFinal >= 1000.f)
			{
				gu64PM2p5ConcentDisplayFinal = 1000.f;
			}

			// PM10 농도 계산 수행
			md64Val = (D64)gu16Lpo2p5 * 0.0001f;

	        //gu64PM10Concent1 = (73363.f*(x*(x*x))) + (25874.f*(x*x)) + (4701.f*x) + 0.9216f; // PM2.5 이상의 농도값_LPP 버젼
	        gu64PM10Concent1 = (100000.f*(md64Val*(md64Val*md64Val))) + (100000.f*(md64Val*md64Val)) + (10000.f*md64Val) + 1.5f; // PM2.5 이상의 농도값_DR3 버젼
			
			if(gu64PM10Concent1 <= 0.f)    // PM10 농도 MIN: 0
			{
				gu64PM10Concent1 = 0.f;
			}
			else
			{
			}

			if(gu64PM10Concent1 >= 1000.f) // PM10 농도 MAX: 1000
			{
				gu64PM10Concent1 = 1000.f;
			}
			else
			{
			}

			gu64PM10Concent180s[gu8CntBuf180s] = gu64PM10Concent1;

			++gu8CntBuf180s;

			if(gu8CntBuf180s >= 90)
		    {
		        gu8Buf180s_F = 1;
			    gu8CntBuf180s = 0;
		    }
			else
			{
			}

            if(gu8Buf180s_F)  
            {
        	    gu64PM10ConcentFinal = (md64SumPM10180s / 90.f); // 180sec 적산 평균값
        	}
			else
			{
				gu64PM10ConcentFinal = (md64SumPM10180s / (D64)gu8CntBuf180s);
			}
			
			gu64PM10ConcentDisplayFinal = gu64PM10ConcentFinal + gu64PM2p5ConcentDisplayFinal;

			if(gu64PM10ConcentDisplayFinal >= 1000.f)
			{
				gu64PM10ConcentDisplayFinal = 1000.f;
			}
			else
			{
			}

			gu8UARTOK_F = CLEAR;
		    memset_f(gu8ParticleRxDataBuffer, 0, sizeof(gu8ParticleRxDataBuffer));
			
			return;
		}
		else
		{
		}

        //============================================================================================================
		// #2. 조건1, 조건2 판별 및 보정 
		//============================================================================================================


		// 조건 1. 10초 농도의 상승 및 하강 판별값
		gu64PM2p5Cond1AbsVal = fabs(gu64Buf10s[0] - gu64Buf10s[1]);
		gu64PM2p5Cond1StdVal = (0.00008f*gu64PM2p5BeConcent*gu64PM2p5BeConcent) + (0.0646f*gu64PM2p5BeConcent) + 2.1211f;

        // 조건 2. 10초 농도와 180초 농도의 차이값
		gu64PM2p5Cond2AbsVal = fabs(gu64Buf10s[0] - gu64Buf180s[1]);
		gu64PM2p5Cond2StdVal = (0.0004f*gu64PM2p5BeConcent*gu64PM2p5BeConcent) + (0.0964f*gu64PM2p5BeConcent) + 8.5899f;

		// 현재 농도값이 조건1, 조건2를 만족하는지 판별 수행 
		if(gu64PM2p5Cond1AbsVal > gu64PM2p5Cond1StdVal)
		{
			gu8PM2p5Cond1_F = 1;  // 조건 1 만족
		}
		else
		{
			gu8PM2p5Cond1_F = 0;  // 조건 1 불만족
		}

		if(gu64PM2p5Cond2AbsVal > gu64PM2p5Cond2StdVal)
		{
			gu8PM2p5Cond2_F = 1;  // 조건 2 만족
		}
		else
		{
			gu8PM2p5Cond2_F = 0;  // 조건 2 불만족
		}
		
		// 조건에 따른 농도값 보정
		md64PM2p5_1 = fabs(gu64Buf10s[0]-gu64Buf10s[1]) / ((0.01*gu64Buf180s[0])+4);
        md64PM2p5_2 = fabs(gu64Buf10s[0]-gu64Buf10s[1]) / ((0.02*gu64Buf180s[0])+8);
		
		if(gu8PM2p5Cond1_F && gu8PM2p5Cond2_F)       // 조건1:만족, 조건2:만족
		{
			if((gu64Buf10s[0] > gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0]))       // 현재값 > 직전값, 10초농도 > 180초농도
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_1;
			}
			else if((gu64Buf10s[0] > gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0]))  // 현재값 > 직전값, 10초농도 < 180초농도
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_2;
			}
			else if((gu64Buf10s[0] < gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0]))  // 현재값 < 직전값, 10초농도 > 180초농도
			{	   
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_2;
			}
			else if((gu64Buf10s[0] < gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0]))  // 현재값 < 직전값, 10초농도 < 180초농도
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_1;
			}
			else
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent;
			}
		}
		else if(gu8PM2p5Cond1_F && !gu8PM2p5Cond2_F)  // 조건1:만족, 조건2:불만족
		{
			if(gu64Buf10s[0] > gu64Buf10s[1])       // 현재값 > 직전값
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_2;
			}
			else if(gu64Buf10s[0] < gu64Buf10s[1])  // 현재값 < 직전값
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_2;
			}
			else
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent;
			}
		}
		else if(!gu8PM2p5Cond1_F && gu8PM2p5Cond2_F)  // 조건1:불만족, 조건2:만족
		{
			if((gu64Buf10s[0] > gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0]))       // 현재값 > 직전값, 10초농도 > 180초농도
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_2;
			}
			else if((gu64Buf10s[0] > gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0]))  // 현재값 > 직전값, 10초농도 < 180초농도
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_2;
			}
			else if((gu64Buf10s[0] < gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0]))  // 현재값 < 직전값, 10초농도 > 180초농도
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_2;
			}
			else if((gu64Buf10s[0] < gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0]))  // 현재값 < 직전값, 10초농도 < 180초농도
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_2;
			}
			else if((gu64Buf10s[0] == gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0])) // 현재값 = 직전값, 10초농도 > 180초농도
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_2;
			}
			else if((gu64Buf10s[0] == gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0])) // 현재값 = 직전값, 10초농도 < 180초농도
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_2;
			}
			else
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent;
			}
		}
		else                                             // 조건1:불만족, 조건2:불만족
		{
			if(gu8PM2p5BeCond1_F || gu8PM2p5BeCond2_F)         // 직전값이 조건1 or 조건2를 만족 했던 경우  
			{ 
				gu64PM2p5Concent1 = gu64PM2p5BeConcent;     // 직전의 최종 출력 농도값 출력
			}
			else
			{
				gu64PM2p5Concent1 = gu64Buf180s[0];  // 현재 180초 농도값 출력
			}
		} 

		if(gu64PM2p5Concent1 <= 0) // 10초 평균 농도의 비정상적인 낙푝으로 인한 출력농도의 음수 저장 방지 
		{
		    gu64PM2p5Concent1 = 0;
		}
		else
		{
		}


        //============================================================================================================
		// #3. 180초 농도 지향성 / 최대값 보정
		//============================================================================================================

        // 180초 농도 지향성 보정 수행 조건: 현재의 180초 농도가 저장된 출력농도와 현재의 10초 농도의 사이에 있을 경우 만족
        if(((gu64PM2p5Concent1 < gu64Buf180s[0]) && (gu64Buf180s[0] < gu64Buf10s[0])) || ((gu64Buf10s[0] < gu64Buf180s[0]) && (gu64Buf180s[0] < gu64PM2p5Concent1)))  
        {
            if(gu64Buf10s[0] >= gu64Buf180s[0])
            {
            	gu64PM2p5Concent2 = gu64PM2p5Concent1 + ((gu64Buf10s[0] - gu64Buf180s[0])/(0.02*gu64Buf180s[0]+8)); 
            }
			else
			{
				gu64PM2p5Concent2 = gu64PM2p5Concent1 - ((gu64Buf180s[0] - gu64Buf10s[0])/(0.02*gu64Buf180s[0]+8));
			}	
        }
        else
        {
        	gu64PM2p5Concent2 = gu64PM2p5Concent1;
        }
        
        // 180초 농도 최대값 보정 수행 조건
        gu64PM2p5Cond2StdValMax = (0.0004f*gu64Buf180s[0]*gu64Buf180s[0]) + (0.0964f*gu64Buf180s[0]) + 8.5899f;

		if(gu64PM2p5Concent2 > (gu64Buf180s[0] + gu64PM2p5Cond2StdValMax))
		{
			gu64PM2p5ConcentFinal = gu64Buf180s[0] + gu64PM2p5Cond2StdValMax;
		}
		else if(gu64PM2p5Concent2 < (gu64Buf180s[0] - gu64PM2p5Cond2StdValMax))
		{
				gu64PM2p5ConcentFinal = gu64Buf180s[0] - gu64PM2p5Cond2StdValMax;
		}
		else
		{
				gu64PM2p5ConcentFinal = gu64PM2p5Concent2;
		}
        
		gu64PM2p5BeConcent = gu64PM2p5ConcentFinal;   // 최종 출력 농도값 저장
		gu8PM2p5BeCond1_F = gu8PM2p5Cond1_F;		  // 기존 조건1 상태 저장
		gu8PM2p5BeCond2_F = gu8PM2p5Cond2_F;		  // 기존 조건2 상태 저장
		
		//============================================================================================================
	    // #4. 계측기 농도 지향성 보정(디스플레이용)
	    //============================================================================================================

        md64ShiftData180s = (REF_COEFFICIENT_2*(gu64PM2p5ConcentFinal*gu64PM2p5ConcentFinal)) + (REF_COEFFICIENT_1*gu64PM2p5ConcentFinal) + REF_COEFFICIENT_0; 

		if((gu64PM2p5ConcentFinal < REF_AREA_MAX) && (gu64PM2p5ConcentFinal >= REF_AREA_MIN))
        {
        	gu64PM2p5ConcentDisplay1 = gu64PM2p5ConcentFinal + md64ShiftData180s;
        }
		else
		{
			gu64PM2p5ConcentDisplay1 = gu64PM2p5ConcentFinal;
		}

        // PM2.5 음수값 출력 시
		if(gu64PM2p5ConcentDisplay1 <= 0)
		{
			gu64PM2p5ConcentDisplay1 = 0;
		}
		else
		{
		}

		// PM2.5 MAX값 
		if(gu64PM2p5ConcentDisplay1 >= 1000.f)
		{
			gu64PM2p5ConcentDisplay1 = 1000.f;
		}
		else
		{
		}
		
		
	  //============================================================================================================
	  // #5. 민감도 설정에 따른 농도 보정 (디스플레이용/킹덤 신규 적용)
	  //============================================================================================================
	  mu64ShiftDataSensitivity = (SEN_COEFFICIENT_2*(gu64PM2p5ConcentDisplay1*gu64PM2p5ConcentDisplay1)) + (SEN_COEFFICIENT_1*gu64PM2p5ConcentDisplay1) + SEN_COEFFICIENT_0;
	  gu64PM2p5ConcentFinalNormal = gu64PM2p5ConcentDisplay1;
	  gu64PM2p5ConcentFinalSharp = gu64PM2p5ConcentDisplay1 + mu64ShiftDataSensitivity;
	  gu64PM2p5ConcentFinalDull = gu64PM2p5ConcentDisplay1 - mu64ShiftDataSensitivity;
	  
	  if(gu8ParticleSensitivity == SENSITIVITY_NORMAL)  //민감도 보통(default)
	  {
	    gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalNormal;
	  }
	  else  //민감도 민감 또는 둔감
	  {
	    if((gu64PM2p5ConcentDisplay1 >= SEN_AREA_MIN) && (gu64PM2p5ConcentDisplay1 < SEN_AREA_MAX)) //보정 범위
	    {
	      if(gu8ParticleSensitivity == SENSITIVITY_SHARP) //민감도 민감
	      {
	        gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalSharp;
	      }
	      else if(gu8ParticleSensitivity == SENSITIVITY_DULL)//민감도 둔감
	      {
	        gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalDull;
	      }
		  else
		  {
		  }
	    }
	    else  //보정 범위가 아닐때
	    {
	      gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalNormal;
	    }
	  }
	  
	  // PM2.5 음수값 출력 시
		if(gu64PM2p5ConcentDisplayFinal <= 0)
		{
			gu64PM2p5ConcentDisplayFinal = 0;
		}
    
		// PM2.5 MAX값 
		if(gu64PM2p5ConcentDisplayFinal >= 1000.f)
		{
			gu64PM2p5ConcentDisplayFinal = 1000.f;
		}
	  
        //============================================================================================================
		// #6. PM10 농도 계산 수행 
		//============================================================================================================

		// PM10 농도 계산 수행
		md64Val = (D64)gu16Lpo2p5 * 0.0001f;  // PM2.5 이상의 LPO값
		 
	    //gu64PM10Concent1 = (73363.f*(x*(x*x))) + (25874.f*(x*x)) + (4701.f*x) + 0.9216f; // PM2.5 이상의 농도값_LPP 버젼
	    gu64PM10Concent1 = (100000.f*(md64Val*(md64Val*md64Val))) + (100000.f*(md64Val*md64Val)) + (10000.f*md64Val) + 1.5f;  // PM2.5 이상의 농도값_DR3 버젼

		if(gu64PM10Concent1 <= 0.f)    // PM10 농도 MIN: 0
		{
			gu64PM10Concent1 = 0.f;
		}
		else
		{
		}

		if(gu64PM10Concent1 >= 1000.f) // PM10 농도 MAX: 999
		{
			gu64PM10Concent1 = 1000.f;
		}
		else
		{
		}

		gu64PM10Concent180s[gu8CntBuf180s] = gu64PM10Concent1;

		++gu8CntBuf180s;

		if(gu8CntBuf180s >= 90)
		{
		    gu8Buf180s_F = 1;
			gu8CntBuf180s = 0;
		}
		else
		{
		}

		for(mu8Cnt = 0; mu8Cnt < 90; mu8Cnt++)
		{
			md64SumPM10180s += gu64PM10Concent180s[mu8Cnt];	
		}

        if(gu8Buf180s_F)  
        {
        	gu64PM10ConcentFinal = (md64SumPM10180s / 90.f); // 180sec 적산 평균값
        }
		else
		{
			gu64PM10ConcentFinal = (md64SumPM10180s / (D64)gu8CntBuf180s);
		}
		
		gu64PM10ConcentDisplayFinal = gu64PM10ConcentFinal + gu64PM2p5ConcentDisplayFinal;

		if(gu64PM10ConcentDisplayFinal <= 0.f)
		{
			gu64PM10ConcentDisplayFinal = 0.f;
		}
		else
		{
		}

		if(gu64PM10ConcentDisplayFinal >= 1000.f)
		{
			gu64PM10ConcentDisplayFinal = 1000.f;
		}
		else
		{
		}

		gu8UARTOK_F = CLEAR;
		
		memset_f(gu8ParticleRxDataBuffer, 0, sizeof(gu8ParticleRxDataBuffer));
	}
}

static void PPD71_CheckParticleDataError(void) {
    if(gu8ErrParticle1 || gu8ErrParticle2) {
        g_errorFlag = SET;
    } else {
        g_errorFlag = CLEAR;
    }
}

static void PPD71_CheckParticleDatastreamError(void) {
    if(gu8ParticleDataTimeout) {
        gu8ErrParticle2 = SET;
    }
}
#endif

// 5.3 PPD42NS 센서 구현
#if (USE_PPD42NS == 1)
static void PPD42NS_Init(void) {
    gu16Volume = 0;
    gu16VolumeSum = 0;
    gu16SkipSensing = 0;
    gu16ParticleInitTime = PARTICLE_INITTIME;
    gu16ErrParticle = PARTICLE_ERRORTIME;
    memset(gu16VolumeBuf, 0, sizeof(gu16VolumeBuf));
}

static void PPD42NS_Handler(void) {
    PPD42NS_ReadParticleData();
    PPD42NS_CalParticleConcent();
    PPD42NS_CheckParticleDataError();
}

static void PPD42NS_ReadParticleData(void) {
	static U8 mu8PoweroffInit_F = 0;

    // 1mSec 주기로 호출
	// if(!checkTimeOver(TimerParticleCount))
	// {
	// 	return;
	// }
	// else
	// {
	// 	registTimer(TimerParticleCount, Time1mSec*5);
	// }
	
	if(readParticleSensorPower() == POW_OFF)
	{
		if(mu8PoweroffInit_F == 0)
		{
			initParticleSensorVar();
			mu8PoweroffInit_F = 1;
			return;
		}
		else
		{
			return;
		}
	}
	else
	{
		mu8PoweroffInit_F = 0; 
	}

	if(gu16SkipSensing)
	{
		gu16SkipSensing--;
		return;
	}
	else
	{
	}

	if(gu16ParticleInitTime)
	{
		gu16ParticleInitTime--;
		return; 
	}
	
	if(PORT_DEFINE_PARTICLE_IN() == 0)
	{
		gu16Volume++;
		if(gu16ErrParticle)
		{
			gu16ErrParticle--;
		}
		else
		{
		}
	}
	else
	{
		gu16ErrParticle = PARTICLE_ERRORTIME;	
	}
	
}

static void PPD42NS_CalParticleConcent(void) {
    static U8 mu8index = 0;
	static U8 mu8BufFull_F = 0;
	U8 mu8tmp = 0;


    // 2초 주기로 호출
	// if(!checkTimeOver(TimerParticleVolumeSum))
	// {
	// 	return;
	// }
	// else
	// {
	// 	registTimer(TimerParticleVolumeSum, Time1Sec*2);
	// }

	if(PORT_DEFINE_PARTICLE_IN == 0)
	{
		mu8index = 0;
		mu8BufFull_F = CLEAR;
		return;
	}
	else
	{
	}

	if(!gu16ParticleInitTime)
	{
		return;
	}
	else
	{
	}

	// 최대값 제한
	gu16Volume = MIN(gu16Volume, 250);
	
	gu16VolumeBuf[mu8index] = gu16Volume;
	gu16Volume = 0;
	gu16VolumeSum = 0;
	mu8index++;
	
	if(mu8BufFull_F)	// Initial time 종료 후, 버퍼에 값이 완전히 찼는지 확인
	{
		for(mu8tmp = 0; mu8tmp < 5; mu8tmp++)
		{

			gu16VolumeSum += gu16VolumeBuf[mu8tmp];
		}
	}
	else	// 버퍼에 값이 완전히 차지 않았다면, 버퍼에 존재하는 값의 평균을 구한 후, 그 평균값을 사용하여 gu16VolumeSum을 확정
	{
		for(mu8tmp = 0; mu8tmp < mu8index; mu8tmp++)
		{

			gu16VolumeSum += gu16VolumeBuf[mu8tmp];
		}
		gu16VolumeSum = (gu16VolumeSum/mu8index)*5;

		if(mu8index == 5)
		{
			mu8BufFull_F = SET;
		}
		else
		{
		}
	}
	mu8index = mu8index % 5;
	
}

static void PPD42NS_CheckParticleDataError(void) {
    if(gu16ErrParticle) {
        g_errorFlag = SET;
    } else {
        g_errorFlag = CLEAR;
    }
}
// FUNCTION COMMENT""*****************************************************
// Function Name : getParticleVolumeSum
// Function ID   : F-019-001-008
// Description   : 최종 합산 카운트값 가져오기
// Return Value  : 최종 합산 카운트값
// FUNCTION COMMENT END ""************************************************
U16 getParticleVolumeSum(void)
{
	return gu16VolumeSum;
}
#endif

// 5.4 PPD4260 센서 구현
#if (USE_PPD4260 == 1)
static void PPD4260_Init(void) {
    gu16VolumeP1 = 0;
    gu16VolumeP2 = 0;
    gu16SkipSensing = 0;
    gd64CalAvgPM2p5 = 0;
    gd64CalAvgPM10 = 0;
}

static void PPD4260_Handler(void) {
    PPD4260_ReadParticleData();
    PPD4260_CalParticleConcent();
    // PPD4260_CheckParticleDataError();
}

static void PPD4260_ReadParticleData(void) {
    if(gu16SkipSensing < PARTICLE_INITTIME) {
        gu16SkipSensing++;
        return;
    }
    
    // if(getGPIOState(GPIO_PARTICLE_SENSOR_P1)) {
    //     gu16VolumeP1++;
    // }
    
    // if(getGPIOState(GPIO_PARTICLE_SENSOR_P2)) {
    //     gu16VolumeP2++;
    // }
}

static void PPD4260_CalParticleConcent(void) {
    // PPD4260의 농도 계산
    D64 d64Temp = 0;
    
    // PM2.5 계산
    d64Temp = (D64)gu16VolumeP1 * 1000.0 / 30000.0;
    gd64CalAvgPM2p5 = d64Temp;
    
    // PM10 계산
    d64Temp = (D64)gu16VolumeP2 * 1000.0 / 30000.0;
    gd64CalAvgPM10 = d64Temp;
}

// static void PPD4260_CheckParticleDataError(void) {
//     if(gu16SkipSensing >= PARTICLE_ERRORTIME) {
//         g_errorFlag = SET;
//     } else {
//         g_errorFlag = CLEAR;
//     }
// }
#endif

// 6. 공통 인터페이스 함수
void DustSensor_Init(void) {
    g_errorFlag = CLEAR;
    
#if (USE_PMS5003 == 1)
    PMS5003_Init();
#endif
#if (USE_PPD71 == 1)
    PPD71_Init();
#endif
#if (USE_PPD42NS == 1)
    PPD42NS_Init();
#endif
#if (USE_PPD4260 == 1)
    PPD4260_Init();
#endif
}

void DustSensor_On(void) {
    PORT_DEFINE_PARTICLE_SENSOR_POW = VALUE_PARTICLE_POWER_ON;
}

void DustSensor_Off(void) {
    PORT_DEFINE_PARTICLE_SENSOR_POW = VALUE_PARTICLE_POWER_OFF;
}

void DustSensor_ReadData(U16* pm1_0, U16* pm2_5, U16* pm10, U8* error) {
    if (!pm1_0 || !pm2_5 || !pm10 || !error) return;
    
    *error = g_errorFlag;
    
#if (USE_PMS5003 == 1)
    *pm1_0 = gu16PM1_0Finish;
    *pm2_5 = gu16PM2_5Finish;
    *pm10 = gu16PM10Finish;
#endif
#if (USE_PPD71 == 1)
    *pm1_0 = 0; // PPD71은 PM1.0 측정 불가
    *pm2_5 = (U16)gu64PM2p5ConcentFinal;
    *pm10 = (U16)gu64PM10ConcentFinal;
#endif
#if (USE_PPD42NS == 1)
    *pm1_0 = 0; // PPD42NS는 PM1.0 측정 불가
    *pm2_5 = gu16VolumeSum;
    *pm10 = 0; // PPD42NS는 PM10 측정 불가
#endif
#if (USE_PPD4260 == 1)
    *pm1_0 = 0; // PPD4260은 PM1.0 측정 불가
    *pm2_5 = (U16)gd64CalAvgPM2p5;
    *pm10 = (U16)gd64CalAvgPM10;
#endif
}

void DustSensor_SetSensitivity(SENSITIVITY_T sensitivity) {
    g_sensitivity = sensitivity;
}

SENSITIVITY_T DustSensor_GetSensitivity(void) {
    return g_sensitivity;
}

void DustSensor_Handler(void) {
#if (USE_PMS5003 == 1)
    PMS5003_Handler();
#endif
#if (USE_PPD71 == 1)
    PPD71_Handler();
#endif
#if (USE_PPD42NS == 1)
    PPD42NS_Handler();
#endif
#if (USE_PPD4260 == 1)
    PPD4260_Handler();
#endif
}

SENSOR_TYPE_T DustSensor_GetType(void) {
    return g_currentSensorType;
}

/// @brief      Dust Sensor control Timer (@1ms interrupt)
/// @param      void
/// @return     void
void DustSensorControlTimer(void)
{
    // 1s마다 타이머 카운트 증가
	if (gu8DustSensorControlTimer < DUST_SENSOR_CONTROL_TIME_PERIOD)
	{
		gu8DustSensorControlTimer++;
	}
	
#if (PPD42NS_USE == 1)
	if (gu8PPD42NSTimer < PPD42NS_CAL_TIME_PERIOD)
	{
		gu8PPD42NSTimer++;
	}
	
#endif
}

void DustSensorControl(void)
{
    if (gu8DustSensorControlTimer >= DUST_SENSOR_CONTROL_TIME_PERIOD)
    {
        DustSensor_Handler();
        gu8DustSensorControlTimer = 0;
    }
#if (PPD42NS_USE == 1)
	if (gu8PPD42NSTimer >= PPD42NS_CAL_TIME_PERIOD)
	{
		gu8PPD42NSTimer = 0;
		PPD42NS_CalParticleConcent(); // 2초 주기로 호출
	}
	PPD42NS_ReadParticleData(); // 1msec 주기로 호출
#endif
}