//FILE COMMENT""****************************************************
//@ FILE Name : DustSensor.c
//@ Description : ���� �������� ����̹� ���� ����
//FILE COMMENT END ""************************************************

// 1. ��ũ�� ���� - �ϳ��� �����Ͽ� ���
// #define USE_PMS5003
// #define USE_PPD71
// #define USE_PPD42NS
// #define USE_PPD4260

// 2. ��� ���� ����
#include "Global_Header.h"


// 3. ���� ���� ����
// 3.1 ���� ���� ����
static const SENSOR_TYPE_T g_currentSensorType = CURRENT_SENSOR_TYPE;
static SENSITIVITY_T g_sensitivity = SENSITIVITY_NORMAL;
static U8 g_errorFlag = CLEAR;
static U8 gu8DustSensorControlTimer = 0;



// 3.2 ������ ���� ����
#if (USE_PMS5003 == 1)
    static volatile U8 s_packet_received_flag = 0;           // ���� �Ϸ� �÷���
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

    // PPD4260 �Լ� ����
    static void PPD4260_Init(void);
    static void PPD4260_Handler(void);
    static void PPD4260_ReadParticleData(void);
    static void PPD4260_CalParticleConcent(void);
#endif

// PPD4260 �Լ� ����
static void PPD4260_Init(void);
static void PPD4260_Handler(void);
static void PPD4260_ReadParticleData(void);
static void PPD4260_CalParticleConcent(void);

// 4. ��ƿ��Ƽ �Լ� ����
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

// 5. ������ ���� �Լ�
// 5.1 PMS5003 ���� ����
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
    PMS5003_RxParticleData();   // �������� ������ ����
    PMS5003_CalcParticleData(); // �������� ������ ���
    PMS5003_CheckError();        // �������� ������ ���� üũ
    PMS5003_CheckDataStream();   // �������� ������ ��Ʈ�� ���� üũ
}


static void PMS5003_RxParticleData(void) {
    MD_STATUS status = R_Config_UART2_Receive((U8*)gu8ParticleRxDataBuffer, 32);
    if (status != MD_OK) {
        // ���� ó��: ���� ����� ����
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
        
        // ������ ������Ʈ
        gu16PM1_0Finish = gu16PM1p0Data;
        gu16PM2_5Finish = gu16PM2p5Data;
        gu16PM10Finish = gu16PM10Data;
    }
}

static void PMS5003_CheckError(void) {
    // �̱���
}

static void PMS5003_CheckDataStream(void) {
    // �̱���
}
#endif

// 5.2 PPD71 ���� ����
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
        // ���� ó��: ���� ����� ����
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
		// #1. ������ ���� ��� 
		//============================================================================================================

	    // �������� ������ ó����
		gu16MassConcent10s  = ((U16)(gu8ParticleRxDataBuffer[3]) << 8) + (U16)(gu8ParticleRxDataBuffer[4]);
    	gu16MassConcent30s  = ((U16)(gu8ParticleRxDataBuffer[5]) << 8) + (U16)(gu8ParticleRxDataBuffer[6]);
		gu16MassConcent60s  = ((U16)(gu8ParticleRxDataBuffer[7]) << 8) + (U16)(gu8ParticleRxDataBuffer[8]);
		gu16MassConcent180s = ((U16)(gu8ParticleRxDataBuffer[9]) << 8) + (U16)(gu8ParticleRxDataBuffer[10]);  //PM2.5 ��ġ ����

		gu16Lpo0p5 = ((U16)(gu8ParticleRxDataBuffer[11]) << 8) + (U16)(gu8ParticleRxDataBuffer[12]);
		gu16Lpo0p7 = ((U16)(gu8ParticleRxDataBuffer[13]) << 8) + (U16)(gu8ParticleRxDataBuffer[14]);
		gu16Lpo1p0 = ((U16)(gu8ParticleRxDataBuffer[15]) << 8) + (U16)(gu8ParticleRxDataBuffer[16]);
		gu16Lpo2p5 = ((U16)(gu8ParticleRxDataBuffer[17]) << 8) + (U16)(gu8ParticleRxDataBuffer[18]);


        // Raw Data ���� �κ� �߰��� ���� ���α׷� ���� 2018.06.29
        
        gu64MassConcent10s = (D64)gu16MassConcent10s;    // U16 -> D64 ��ȯ
		gu64MassConcent30s = (D64)gu16MassConcent30s;
		gu64MassConcent60s = (D64)gu16MassConcent60s;
		gu64MassConcent180s = (D64)gu16MassConcent180s;
		
        // PM2.5 10�� �󵵰� 
		gu64Buf10s[2] = gu64Buf10s[1];       // 4���� Data
		gu64Buf10s[1] = gu64Buf10s[0];       // 2���� Data
		gu64Buf10s[0] = gu64MassConcent10s;  // ���� Data
		
		// PM2.5 180�� �󵵰�
		gu64Buf180s[2] = gu64Buf180s[1];
		gu64Buf180s[1] = gu64Buf180s[0];
		gu64Buf180s[0] = gu64MassConcent180s;

		// PM2.5 �̻� LPO
		gu64BufLpo2p5[2] = gu64BufLpo2p5[1];
		gu64BufLpo2p5[1] = gu64BufLpo2p5[0];
		gu64BufLpo2p5[0] = (D64)gu16Lpo2p5;

		//============================================================================================================
		// *PM2.5 �󵵰� 5ug/m^3 ������ ��� 10��/180�� �󵵸� ���ǿ� ���� ���
        //
		//  �ο�����
		// -.���� �� ��Ȳ���� ���������� 180�� �󵵸� ������ ���, ���� ��鸲�� ������ �������� ���� �߻�
		// -.������ �� ��ȭ�� Ȯ�� �Ҽ� �ֵ��� 10�� �� ȥ���Ͽ� ������ ��� 
		//============================================================================================================

		if(gu64MassConcent180s <= 5.f) // PM2.5 180�� �� �����Ͱ� 5���� �� ���� 180�� �� �״�� ��� (�˰��� ����)
		{
		    // Ŭ�����¿��� ���������� 1�θ� ǥ�õ� ��� �� ���� �Ҹ� ����� ǰ�� ����ȯ ���� ��û���� ���α׷� ����
            if(gu64MassConcent10s <= 5.f)
            {
                gu64PM2p5ConcentFinal = gu64MassConcent10s;  // PM2.5 180�� �����Ϳ� 10�� ������ ��� 5���� �ΰ�� 10�� ������ ǥ�� 
            }
			else
			{
				gu64PM2p5ConcentFinal = gu64MassConcent180s; // PM2.5 180�� �����Ϳ� 10�� ������ ��� 5���ϰ� �ƴ� ��� 180�� ������ ǥ�� 
			}

			gu64PM2p5BeConcent = gu64PM2p5ConcentFinal;  // ���� ��� �󵵰� ����
			gu8PM2p5BeCond1_F = 0;			// ���� ����1 ���� �Ҹ���
			gu8PM2p5BeCond2_F = 0;			// ���� ����2 ���� �Ҹ���

			//============================================================================================================
	    	// ������ �� ���⼺ ����(���÷���)
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
    
			// PM2.5 ������ ��� ��
			if(gu64PM2p5ConcentDisplay1 <= 0)
			{
				gu64PM2p5ConcentDisplay1 = 0;
			}
			else
			{
			}

			// PM2.5 MAX�� : 1000
			if(gu64PM2p5ConcentDisplay1 >= 1000.f)
			{
				gu64PM2p5ConcentDisplay1 = 1000.f;
			}
			else
			{
			}

			
		  //============================================================================================================
		  // �ΰ��� ������ ���� �� ���� (���÷��̿�/ŷ�� �ű� ����)
		  //============================================================================================================
		  mu64ShiftDataSensitivity = (SEN_COEFFICIENT_2*(gu64PM2p5ConcentDisplay1*gu64PM2p5ConcentDisplay1)) + (SEN_COEFFICIENT_1*gu64PM2p5ConcentDisplay1) + SEN_COEFFICIENT_0;
		  gu64PM2p5ConcentFinalNormal = gu64PM2p5ConcentDisplay1;
		  gu64PM2p5ConcentFinalSharp = gu64PM2p5ConcentDisplay1 + mu64ShiftDataSensitivity;
		  gu64PM2p5ConcentFinalDull = gu64PM2p5ConcentDisplay1 - mu64ShiftDataSensitivity;
		  
		  if(gu8ParticleSensitivity == SENSITIVITY_NORMAL)  //�ΰ��� ����(default)
		  {
		    gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalNormal;
		  }
		  else  //�ΰ��� �ΰ� �Ǵ� �а�
		  {
		    if((gu64PM2p5ConcentDisplay1 >= SEN_AREA_MIN) && (gu64PM2p5ConcentDisplay1 < SEN_AREA_MAX)) //���� ����
		    {
		      if(gu8ParticleSensitivity == SENSITIVITY_SHARP) //�ΰ��� �ΰ�
		      {
		        gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalSharp;
		      }
		      else if(gu8ParticleSensitivity == SENSITIVITY_DULL)//�ΰ��� �а�
		      {
		        gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalDull;
		      }
			  else
			  {
			  }
		    }
		    else  //���� ������ �ƴҶ�
		    {
		      gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalNormal;
		    }
		  }
		  
		    // PM2.5 ������ ��� ��
			if(gu64PM2p5ConcentDisplayFinal <= 0)
			{
				gu64PM2p5ConcentDisplayFinal = 0;
			}
	    
			// PM2.5 MAX�� 
			if(gu64PM2p5ConcentDisplayFinal >= 1000.f)
			{
				gu64PM2p5ConcentDisplayFinal = 1000.f;
			}

			// PM10 �� ��� ����
			md64Val = (D64)gu16Lpo2p5 * 0.0001f;

	        //gu64PM10Concent1 = (73363.f*(x*(x*x))) + (25874.f*(x*x)) + (4701.f*x) + 0.9216f; // PM2.5 �̻��� �󵵰�_LPP ����
	        gu64PM10Concent1 = (100000.f*(md64Val*(md64Val*md64Val))) + (100000.f*(md64Val*md64Val)) + (10000.f*md64Val) + 1.5f; // PM2.5 �̻��� �󵵰�_DR3 ����
			
			if(gu64PM10Concent1 <= 0.f)    // PM10 �� MIN: 0
			{
				gu64PM10Concent1 = 0.f;
			}
			else
			{
			}

			if(gu64PM10Concent1 >= 1000.f) // PM10 �� MAX: 1000
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
        	    gu64PM10ConcentFinal = (md64SumPM10180s / 90.f); // 180sec ���� ��հ�
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
		// #2. ����1, ����2 �Ǻ� �� ���� 
		//============================================================================================================


		// ���� 1. 10�� ���� ��� �� �ϰ� �Ǻ���
		gu64PM2p5Cond1AbsVal = fabs(gu64Buf10s[0] - gu64Buf10s[1]);
		gu64PM2p5Cond1StdVal = (0.00008f*gu64PM2p5BeConcent*gu64PM2p5BeConcent) + (0.0646f*gu64PM2p5BeConcent) + 2.1211f;

        // ���� 2. 10�� �󵵿� 180�� ���� ���̰�
		gu64PM2p5Cond2AbsVal = fabs(gu64Buf10s[0] - gu64Buf180s[1]);
		gu64PM2p5Cond2StdVal = (0.0004f*gu64PM2p5BeConcent*gu64PM2p5BeConcent) + (0.0964f*gu64PM2p5BeConcent) + 8.5899f;

		// ���� �󵵰��� ����1, ����2�� �����ϴ��� �Ǻ� ���� 
		if(gu64PM2p5Cond1AbsVal > gu64PM2p5Cond1StdVal)
		{
			gu8PM2p5Cond1_F = 1;  // ���� 1 ����
		}
		else
		{
			gu8PM2p5Cond1_F = 0;  // ���� 1 �Ҹ���
		}

		if(gu64PM2p5Cond2AbsVal > gu64PM2p5Cond2StdVal)
		{
			gu8PM2p5Cond2_F = 1;  // ���� 2 ����
		}
		else
		{
			gu8PM2p5Cond2_F = 0;  // ���� 2 �Ҹ���
		}
		
		// ���ǿ� ���� �󵵰� ����
		md64PM2p5_1 = fabs(gu64Buf10s[0]-gu64Buf10s[1]) / ((0.01*gu64Buf180s[0])+4);
        md64PM2p5_2 = fabs(gu64Buf10s[0]-gu64Buf10s[1]) / ((0.02*gu64Buf180s[0])+8);
		
		if(gu8PM2p5Cond1_F && gu8PM2p5Cond2_F)       // ����1:����, ����2:����
		{
			if((gu64Buf10s[0] > gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0]))       // ���簪 > ������, 10�ʳ� > 180�ʳ�
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_1;
			}
			else if((gu64Buf10s[0] > gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0]))  // ���簪 > ������, 10�ʳ� < 180�ʳ�
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_2;
			}
			else if((gu64Buf10s[0] < gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0]))  // ���簪 < ������, 10�ʳ� > 180�ʳ�
			{	   
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_2;
			}
			else if((gu64Buf10s[0] < gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0]))  // ���簪 < ������, 10�ʳ� < 180�ʳ�
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_1;
			}
			else
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent;
			}
		}
		else if(gu8PM2p5Cond1_F && !gu8PM2p5Cond2_F)  // ����1:����, ����2:�Ҹ���
		{
			if(gu64Buf10s[0] > gu64Buf10s[1])       // ���簪 > ������
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_2;
			}
			else if(gu64Buf10s[0] < gu64Buf10s[1])  // ���簪 < ������
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_2;
			}
			else
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent;
			}
		}
		else if(!gu8PM2p5Cond1_F && gu8PM2p5Cond2_F)  // ����1:�Ҹ���, ����2:����
		{
			if((gu64Buf10s[0] > gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0]))       // ���簪 > ������, 10�ʳ� > 180�ʳ�
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_2;
			}
			else if((gu64Buf10s[0] > gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0]))  // ���簪 > ������, 10�ʳ� < 180�ʳ�
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_2;
			}
			else if((gu64Buf10s[0] < gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0]))  // ���簪 < ������, 10�ʳ� > 180�ʳ�
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_2;
			}
			else if((gu64Buf10s[0] < gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0]))  // ���簪 < ������, 10�ʳ� < 180�ʳ�
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_2;
			}
			else if((gu64Buf10s[0] == gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0])) // ���簪 = ������, 10�ʳ� > 180�ʳ�
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_2;
			}
			else if((gu64Buf10s[0] == gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0])) // ���簪 = ������, 10�ʳ� < 180�ʳ�
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_2;
			}
			else
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent;
			}
		}
		else                                             // ����1:�Ҹ���, ����2:�Ҹ���
		{
			if(gu8PM2p5BeCond1_F || gu8PM2p5BeCond2_F)         // �������� ����1 or ����2�� ���� �ߴ� ���  
			{ 
				gu64PM2p5Concent1 = gu64PM2p5BeConcent;     // ������ ���� ��� �󵵰� ���
			}
			else
			{
				gu64PM2p5Concent1 = gu64Buf180s[0];  // ���� 180�� �󵵰� ���
			}
		} 

		if(gu64PM2p5Concent1 <= 0) // 10�� ��� ���� ���������� ���i���� ���� ��³��� ���� ���� ���� 
		{
		    gu64PM2p5Concent1 = 0;
		}
		else
		{
		}


        //============================================================================================================
		// #3. 180�� �� ���⼺ / �ִ밪 ����
		//============================================================================================================

        // 180�� �� ���⼺ ���� ���� ����: ������ 180�� �󵵰� ����� ��³󵵿� ������ 10�� ���� ���̿� ���� ��� ����
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
        
        // 180�� �� �ִ밪 ���� ���� ����
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
        
		gu64PM2p5BeConcent = gu64PM2p5ConcentFinal;   // ���� ��� �󵵰� ����
		gu8PM2p5BeCond1_F = gu8PM2p5Cond1_F;		  // ���� ����1 ���� ����
		gu8PM2p5BeCond2_F = gu8PM2p5Cond2_F;		  // ���� ����2 ���� ����
		
		//============================================================================================================
	    // #4. ������ �� ���⼺ ����(���÷��̿�)
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

        // PM2.5 ������ ��� ��
		if(gu64PM2p5ConcentDisplay1 <= 0)
		{
			gu64PM2p5ConcentDisplay1 = 0;
		}
		else
		{
		}

		// PM2.5 MAX�� 
		if(gu64PM2p5ConcentDisplay1 >= 1000.f)
		{
			gu64PM2p5ConcentDisplay1 = 1000.f;
		}
		else
		{
		}
		
		
	  //============================================================================================================
	  // #5. �ΰ��� ������ ���� �� ���� (���÷��̿�/ŷ�� �ű� ����)
	  //============================================================================================================
	  mu64ShiftDataSensitivity = (SEN_COEFFICIENT_2*(gu64PM2p5ConcentDisplay1*gu64PM2p5ConcentDisplay1)) + (SEN_COEFFICIENT_1*gu64PM2p5ConcentDisplay1) + SEN_COEFFICIENT_0;
	  gu64PM2p5ConcentFinalNormal = gu64PM2p5ConcentDisplay1;
	  gu64PM2p5ConcentFinalSharp = gu64PM2p5ConcentDisplay1 + mu64ShiftDataSensitivity;
	  gu64PM2p5ConcentFinalDull = gu64PM2p5ConcentDisplay1 - mu64ShiftDataSensitivity;
	  
	  if(gu8ParticleSensitivity == SENSITIVITY_NORMAL)  //�ΰ��� ����(default)
	  {
	    gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalNormal;
	  }
	  else  //�ΰ��� �ΰ� �Ǵ� �а�
	  {
	    if((gu64PM2p5ConcentDisplay1 >= SEN_AREA_MIN) && (gu64PM2p5ConcentDisplay1 < SEN_AREA_MAX)) //���� ����
	    {
	      if(gu8ParticleSensitivity == SENSITIVITY_SHARP) //�ΰ��� �ΰ�
	      {
	        gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalSharp;
	      }
	      else if(gu8ParticleSensitivity == SENSITIVITY_DULL)//�ΰ��� �а�
	      {
	        gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalDull;
	      }
		  else
		  {
		  }
	    }
	    else  //���� ������ �ƴҶ�
	    {
	      gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalNormal;
	    }
	  }
	  
	  // PM2.5 ������ ��� ��
		if(gu64PM2p5ConcentDisplayFinal <= 0)
		{
			gu64PM2p5ConcentDisplayFinal = 0;
		}
    
		// PM2.5 MAX�� 
		if(gu64PM2p5ConcentDisplayFinal >= 1000.f)
		{
			gu64PM2p5ConcentDisplayFinal = 1000.f;
		}
	  
        //============================================================================================================
		// #6. PM10 �� ��� ���� 
		//============================================================================================================

		// PM10 �� ��� ����
		md64Val = (D64)gu16Lpo2p5 * 0.0001f;  // PM2.5 �̻��� LPO��
		 
	    //gu64PM10Concent1 = (73363.f*(x*(x*x))) + (25874.f*(x*x)) + (4701.f*x) + 0.9216f; // PM2.5 �̻��� �󵵰�_LPP ����
	    gu64PM10Concent1 = (100000.f*(md64Val*(md64Val*md64Val))) + (100000.f*(md64Val*md64Val)) + (10000.f*md64Val) + 1.5f;  // PM2.5 �̻��� �󵵰�_DR3 ����

		if(gu64PM10Concent1 <= 0.f)    // PM10 �� MIN: 0
		{
			gu64PM10Concent1 = 0.f;
		}
		else
		{
		}

		if(gu64PM10Concent1 >= 1000.f) // PM10 �� MAX: 999
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
        	gu64PM10ConcentFinal = (md64SumPM10180s / 90.f); // 180sec ���� ��հ�
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

// 5.3 PPD42NS ���� ����
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

    // 1mSec �ֱ�� ȣ��
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


    // 2�� �ֱ�� ȣ��
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

	// �ִ밪 ����
	gu16Volume = MIN(gu16Volume, 250);
	
	gu16VolumeBuf[mu8index] = gu16Volume;
	gu16Volume = 0;
	gu16VolumeSum = 0;
	mu8index++;
	
	if(mu8BufFull_F)	// Initial time ���� ��, ���ۿ� ���� ������ á���� Ȯ��
	{
		for(mu8tmp = 0; mu8tmp < 5; mu8tmp++)
		{

			gu16VolumeSum += gu16VolumeBuf[mu8tmp];
		}
	}
	else	// ���ۿ� ���� ������ ���� �ʾҴٸ�, ���ۿ� �����ϴ� ���� ����� ���� ��, �� ��հ��� ����Ͽ� gu16VolumeSum�� Ȯ��
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
// Description   : ���� �ջ� ī��Ʈ�� ��������
// Return Value  : ���� �ջ� ī��Ʈ��
// FUNCTION COMMENT END ""************************************************
U16 getParticleVolumeSum(void)
{
	return gu16VolumeSum;
}
#endif

// 5.4 PPD4260 ���� ����
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
    // PPD4260�� �� ���
    D64 d64Temp = 0;
    
    // PM2.5 ���
    d64Temp = (D64)gu16VolumeP1 * 1000.0 / 30000.0;
    gd64CalAvgPM2p5 = d64Temp;
    
    // PM10 ���
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

// 6. ���� �������̽� �Լ�
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
    *pm1_0 = 0; // PPD71�� PM1.0 ���� �Ұ�
    *pm2_5 = (U16)gu64PM2p5ConcentFinal;
    *pm10 = (U16)gu64PM10ConcentFinal;
#endif
#if (USE_PPD42NS == 1)
    *pm1_0 = 0; // PPD42NS�� PM1.0 ���� �Ұ�
    *pm2_5 = gu16VolumeSum;
    *pm10 = 0; // PPD42NS�� PM10 ���� �Ұ�
#endif
#if (USE_PPD4260 == 1)
    *pm1_0 = 0; // PPD4260�� PM1.0 ���� �Ұ�
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
    // 1s���� Ÿ�̸� ī��Ʈ ����
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
		PPD42NS_CalParticleConcent(); // 2�� �ֱ�� ȣ��
	}
	PPD42NS_ReadParticleData(); // 1msec �ֱ�� ȣ��
#endif
}