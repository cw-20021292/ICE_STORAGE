/// @file     Hal_DustSensor_PPD71.c
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    PPD71 먼지센서 하드웨어 추상화 계층
/// @details  PPD71 먼지센서 제어를 위한 HAL 드라이버 구현

#include "Global_Header.h"

#if (DUST_SENSOR_PPD71_USE == USE)

// PPD71 센서 상태 관리 변수
static U8 gu8UARTOK_F = 0;
// static U8 gu8ParticleDataCheck = 0; // 미사용 변수
static U8 gu8ParticleRxDataBuffer[32] = {0};
static U16 gu16MassConcent10s = 0;
static U16 gu16MassConcent180s = 0;
static U16 gu16Lpo2p5 = 0;
static D64 gu64MassConcent10s = 0.0f;
static D64 gu64MassConcent180s = 0.0f;
static D64 gu64Buf10s[3] = {0.0f, 0.0f, 0.0f};
static D64 gu64Buf180s[3] = {0.0f, 0.0f, 0.0f};
static D64 gu64BufLpo2p5[3] = {0.0f, 0.0f, 0.0f};
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
static U8 gu8ErrParticle1 = CLEAR;
static U8 gu8ErrParticle2 = CLEAR;

// 민감도 설정 변수
static D64 gu64PM2p5ConcentDisplay1 = 0;

// 민감도 보정값
D64 gu64PM2p5ConcentFinalNormal = 0;
D64 gu64PM2p5ConcentFinalSharp = 0;
D64 gu64PM2p5ConcentFinalDull = 0;
PPD71_SENSITIVITY_T gu8ParticleSensitivity = PPD71_SENSITIVITY_NORMAL;

// 오류 플래그
static U8 g_errorFlag = CLEAR;

// 타이머 관련
static U8 gu8DustSensorControlTimer = 0;
static U8 gu8ParticleDataTimeout = 0;

// 내부 함수 선언 프로토타입 (기존 패턴에 맞춰 public으로 변경)
void PPD71_Init(void);
void PPD71_Handler(void);
void PPD71_RxParticleData(void);
void PPD71_CalParticleConcent(void);
void PPD71_CheckParticleDataError(void);
void PPD71_CheckParticleDatastreamError(void);

/// @brief      PPD71 센서 초기화 함수
/// @param      void
/// @return     void
/// @details    PPD71 센서의 변수들을 초기화하고 UART 수신 버퍼를 클리어
void PPD71_Init(void) {
    gu8UARTOK_F = 0;
    gu8ErrParticle1 = CLEAR;
    gu8ErrParticle2 = CLEAR;
    MEMSET( (void __FAR *)&gu8ParticleRxDataBuffer, 0x00, sizeof(gu8ParticleRxDataBuffer) );
}

/// @brief      PPD71 센서 메인 핸들러
/// @param      void
/// @return     void
/// @details    PPD71 센서의 데이터 수신, 계산, 에러 체크를 순차적으로 수행
void PPD71_Handler(void) {
    PPD71_RxParticleData();
    PPD71_CalParticleConcent();
    PPD71_CheckParticleDataError();
    PPD71_CheckParticleDatastreamError();
}

/// @brief      PPD71 센서 데이터 수신 함수
/// @param      void
/// @return     void
/// @details    UART2를 통해 PPD71 센서로부터 29바이트 데이터를 수신
void PPD71_RxParticleData(void)
{
	MD_STATUS status = R_Config_UART2_Receive((U8*)gu8ParticleRxDataBuffer, 29);
    if (status != MD_OK) {
        // 오류 처리: 수신 실패시 무시
    }
}

/// @brief      PPD71 미세먼지 농도 계산 함수
/// @param      void
/// @return     void
/// @details    수신된 데이터를 바탕으로 PM2.5와 PM10 농도를 계산하고 민감도를 적용
void PPD71_CalParticleConcent(void) {
    U8  mu8Cnt = 0;
	D64 md64PM2p5_1 = 0;
	D64 md64PM2p5_2 = 0;
	D64 md64ShiftData180s = 0;
	D64 md64SumPM10180s = 0;
	D64 md64Val = 0;
	D64 mu64ShiftDataSensitivity = 0;

    if(gu8UARTOK_F) {

        //============================================================================================================
		// #1. 수신 데이터 파싱
		//============================================================================================================

	    // 수신된 데이터 처리
		gu16MassConcent10s  = ((U16)(gu8ParticleRxDataBuffer[3]) << 8) + (U16)(gu8ParticleRxDataBuffer[4]);
		gu16MassConcent180s = ((U16)(gu8ParticleRxDataBuffer[9]) << 8) + (U16)(gu8ParticleRxDataBuffer[10]);  //PM2.5 농도 값

		gu16Lpo2p5 = ((U16)(gu8ParticleRxDataBuffer[17]) << 8) + (U16)(gu8ParticleRxDataBuffer[18]);

        // Raw Data 형변환 및 버퍼링 처리 추가 2018.06.29

        gu64MassConcent10s = (D64)gu16MassConcent10s;    // U16 -> D64 변환
		gu64MassConcent180s = (D64)gu16MassConcent180s;

        // PM2.5 10초 버퍼링
		gu64Buf10s[2] = gu64Buf10s[1];       // 4초전 Data
		gu64Buf10s[1] = gu64Buf10s[0];       // 2초전 Data
		gu64Buf10s[0] = gu64MassConcent10s;  // 현재 Data

		// PM2.5 180초 버퍼링
		gu64Buf180s[2] = gu64Buf180s[1];
		gu64Buf180s[1] = gu64Buf180s[0];
		gu64Buf180s[0] = gu64MassConcent180s;

		// PM2.5 입자수 LPO
		gu64BufLpo2p5[2] = gu64BufLpo2p5[1];
		gu64BufLpo2p5[1] = gu64BufLpo2p5[0];
		gu64BufLpo2p5[0] = (D64)gu16Lpo2p5;

		//============================================================================================================
		// *PM2.5 농도가 5ug/m^3 이하일 때 10초/180초 중에서 선택적으로 사용 여부
        //
		//  알고리즘
		// -. 평상 시 어느정도 안정된 상황에서는 180초 중에서 선택하여 사용, 복잡한 대기상태에서는 10초 사용하여 반응성 향상
		// -. 미세먼지 시 변화를 확인 때는 단시간에 10초 시 혼란스럽지 않게 처리
		//============================================================================================================

		if(gu64MassConcent180s <= 5.f) // PM2.5 180초 시 농도가 5마이크로그램 이하일 때 180초 시 대상으로 선택 (청정지역 상태)
		{
		    // 클린환경에서 무미세먼지가 1μg도 표시될 때는 상당히 민감한 먼지 고질 문제환경 상태 정청지역 프로그램 상태
            if(gu64MassConcent10s <= 5.f)
            {
                gu64PM2p5ConcentFinal = gu64MassConcent10s;  // PM2.5 180초 농도와 10초 농도가 둘다 5ug 이하일 때 10초 농도를 표시
            }
			else
			{
				gu64PM2p5ConcentFinal = gu64MassConcent180s; // PM2.5 180초 농도와 10초 농도가 둘다 5ug보다 큰 경우 180초 농도를 표시
			}

			gu64PM2p5BeConcent = gu64PM2p5ConcentFinal;  // 이전 모든 농도값 저장
			gu8PM2p5BeCond1_F = 0;			// 이전 조건1 상태 초기화
			gu8PM2p5BeCond2_F = 0;			// 이전 조건2 상태 초기화

			//============================================================================================================
	    	// 표준기 및 정밀성 보정(캘리브레이션)
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

			// PM2.5 농도값 음수 방지
			if(gu64PM2p5ConcentDisplay1 <= 0)
			{
				gu64PM2p5ConcentDisplay1 = 0;
			}

			// PM2.5 MAX값 : 1000
			if(gu64PM2p5ConcentDisplay1 >= 1000.f)
			{
				gu64PM2p5ConcentDisplay1 = 1000.f;
			}

		  //============================================================================================================
		  // 민감도 보정값 계산 및 적용 (캘리브레이션/킹스톤 보정 적용)
		  //============================================================================================================
		  mu64ShiftDataSensitivity = (SEN_COEFFICIENT_2*(gu64PM2p5ConcentDisplay1*gu64PM2p5ConcentDisplay1)) + (SEN_COEFFICIENT_1*gu64PM2p5ConcentDisplay1) + SEN_COEFFICIENT_0;
		  gu64PM2p5ConcentFinalNormal = gu64PM2p5ConcentDisplay1;
		  gu64PM2p5ConcentFinalSharp = gu64PM2p5ConcentDisplay1 + mu64ShiftDataSensitivity;
		  gu64PM2p5ConcentFinalDull = gu64PM2p5ConcentDisplay1 - mu64ShiftDataSensitivity;

		  if(gu8ParticleSensitivity == PPD71_SENSITIVITY_NORMAL)  //민감도 보통(default)
		  {
		    gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalNormal;
		  }
		  else  //민감도 샤프 또는 둔함
		  {
		    if((gu64PM2p5ConcentDisplay1 >= SEN_AREA_MIN) && (gu64PM2p5ConcentDisplay1 < SEN_AREA_MAX)) //적용 범위
		    {
		      if(gu8ParticleSensitivity == PPD71_SENSITIVITY_SHARP) //민감도 샤프
		      {
		        gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalSharp;
		      }
		      else if(gu8ParticleSensitivity == PPD71_SENSITIVITY_DULL)//민감도 둔함
		      {
		        gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalDull;
		      }
		    }
		    else  //적용 범위를 벗어남
		    {
		      gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalNormal;
		    }
		  }

		    // PM2.5 농도값 음수 방지
			if(gu64PM2p5ConcentDisplayFinal <= 0)
			{
				gu64PM2p5ConcentDisplayFinal = 0;
			}

			// PM2.5 MAX값
			if(gu64PM2p5ConcentDisplayFinal >= 1000.f)
			{
				gu64PM2p5ConcentDisplayFinal = 1000.f;
			}

			// PM10 값 계산 처리
			md64Val = (D64)gu16Lpo2p5 * 0.0001f;

	        //gu64PM10Concent1 = (73363.f*(x*(x*x))) + (25874.f*(x*x)) + (4701.f*x) + 0.9216f; // PM2.5 입자수 농도_LPP 공식
	        gu64PM10Concent1 = (100000.f*(md64Val*(md64Val*md64Val))) + (100000.f*(md64Val*md64Val)) + (10000.f*md64Val) + 1.5f; // PM2.5 입자수 농도_DR3 공식

			if(gu64PM10Concent1 <= 0.f)    // PM10 값 MIN: 0
			{
				gu64PM10Concent1 = 0.f;
			}

			if(gu64PM10Concent1 >= 1000.f) // PM10 값 MAX: 1000
			{
				gu64PM10Concent1 = 1000.f;
			}

			gu64PM10Concent180s[gu8CntBuf180s] = gu64PM10Concent1;

			++gu8CntBuf180s;

			if(gu8CntBuf180s >= 90)
		    {
		        gu8Buf180s_F = 1;
			    gu8CntBuf180s = 0;
		    }

            if(gu8Buf180s_F)
            {
        	    gu64PM10ConcentFinal = (md64SumPM10180s / 90.f); // 180sec 평균 계산
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
            else{

            }

			gu8UARTOK_F = CLEAR;
		    MEMSET( (void __FAR *)&gu8ParticleRxDataBuffer, 0x00, sizeof(gu8ParticleRxDataBuffer) );

			return;
		}

                //============================================================================================================
		// #2. 조건1, 조건2 판별 및 계산
		//============================================================================================================

		// 조건 1. 10초 값 변화량 및 표준값 계산
		gu64PM2p5Cond1AbsVal = fabs(gu64Buf10s[0] - gu64Buf10s[1]);
		gu64PM2p5Cond1StdVal = (0.00008f*gu64PM2p5BeConcent*gu64PM2p5BeConcent) + (0.0646f*gu64PM2p5BeConcent) + 2.1211f;

        // 조건 2. 10초 농도와 180초 값 비교계산
		gu64PM2p5Cond2AbsVal = fabs(gu64Buf10s[0] - gu64Buf180s[1]);
		gu64PM2p5Cond2StdVal = (0.0004f*gu64PM2p5BeConcent*gu64PM2p5BeConcent) + (0.0964f*gu64PM2p5BeConcent) + 8.5899f;

		// 이전 농도값과 조건1, 조건2의 충족여부에 따른 계산
		if(gu64PM2p5Cond1AbsVal > gu64PM2p5Cond1StdVal)
		{
			gu8PM2p5Cond1_F = 1;  // 조건 1 충족
		}
		else
		{
			gu8PM2p5Cond1_F = 0;  // 조건 1 미충족
		}

		if(gu64PM2p5Cond2AbsVal > gu64PM2p5Cond2StdVal)
		{
			gu8PM2p5Cond2_F = 1;  // 조건 2 충족
		}
		else
		{
			gu8PM2p5Cond2_F = 0;  // 조건 2 미충족
		}

		// 보정계수 농도 계산
		md64PM2p5_1 = fabs(gu64Buf10s[0]-gu64Buf10s[1]) / ((0.01*gu64Buf180s[0])+4);
        md64PM2p5_2 = fabs(gu64Buf10s[0]-gu64Buf10s[1]) / ((0.02*gu64Buf180s[0])+8);

		if(gu8PM2p5Cond1_F && gu8PM2p5Cond2_F)       // 조건1:충족, 조건2:충족
		{
			if((gu64Buf10s[0] > gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0]))       // 현재값 > 이전값, 10초 > 180초
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_1;
			}
			else if((gu64Buf10s[0] > gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0]))  // 현재값 > 이전값, 10초 < 180초
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_2;
			}
			else if((gu64Buf10s[0] < gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0]))  // 현재값 < 이전값, 10초 > 180초
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_2;
			}
			else if((gu64Buf10s[0] < gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0]))  // 현재값 < 이전값, 10초 < 180초
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_1;
			}
			else
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent;
			}
		}
		else if(gu8PM2p5Cond1_F && !gu8PM2p5Cond2_F)  // 조건1:충족, 조건2:미충족
		{
			if(gu64Buf10s[0] > gu64Buf10s[1])       // 현재값 > 이전값
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_2;
			}
			else if(gu64Buf10s[0] < gu64Buf10s[1])  // 현재값 < 이전값
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_2;
			}
			else
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent;
			}
		}
		else if(!gu8PM2p5Cond1_F && gu8PM2p5Cond2_F)  // 조건1:미충족, 조건2:충족
		{
			if((gu64Buf10s[0] > gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0]))       // 현재값 > 이전값, 10초 > 180초
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_2;
			}
			else if((gu64Buf10s[0] > gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0]))  // 현재값 > 이전값, 10초 < 180초
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_2;
			}
			else if((gu64Buf10s[0] < gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0]))  // 현재값 < 이전값, 10초 > 180초
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_2;
			}
			else if((gu64Buf10s[0] < gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0]))  // 현재값 < 이전값, 10초 < 180초
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_2;
			}
			else if((gu64Buf10s[0] == gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0])) // 현재값 = 이전값, 10초 > 180초
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_2;
			}
			else if((gu64Buf10s[0] == gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0])) // 현재값 = 이전값, 10초 < 180초
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_2;
			}
			else
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent;
			}
		}
		else                                             // 조건1:미충족, 조건2:미충족
		{
			if(gu8PM2p5BeCond1_F || gu8PM2p5BeCond2_F)         // 이전사이클 조건1 or 조건2가 충족 이었을때를 확인
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent;     // 이전값 그대로 농도 사용
			}
			else
			{
				gu64PM2p5Concent1 = gu64Buf180s[0];  // 현재 180초 농도 사용
			}
		}

		if(gu64PM2p5Concent1 <= 0) // 10초 계산값이 음수가 되는것을 방지하여 0 이하시는 0 으로 처리
		{
		    gu64PM2p5Concent1 = 0;
		}
		else
		{
		}

        //============================================================================================================
		// #3. 180초 농도 정밀성 / 최대값 제한
		//============================================================================================================

        // 180초 농도 정밀성 보정 조건 확인: 계산된 180초 농도값 사이에 급변동이 있었는지 10초 농도 사이와 비교 계산 처리
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

        // 180초 농도 최대값 제한 조건 확인
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

		gu64PM2p5BeConcent = gu64PM2p5ConcentFinal;   // 이전 모든 농도값 저장
		gu8PM2p5BeCond1_F = gu8PM2p5Cond1_F;		  // 이전 조건1 상태 저장
		gu8PM2p5BeCond2_F = gu8PM2p5Cond2_F;		  // 이전 조건2 상태 저장

		//============================================================================================================
	    // #4. 표준기 및 정밀성 보정(캘리브레이션)
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

                // PM2.5 농도값 음수 방지
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
	  // #5. 민감도 보정값 계산 및 적용 (캘리브레이션/킹스톤 보정 적용)
	  //============================================================================================================
	  mu64ShiftDataSensitivity = (SEN_COEFFICIENT_2*(gu64PM2p5ConcentDisplay1*gu64PM2p5ConcentDisplay1)) + (SEN_COEFFICIENT_1*gu64PM2p5ConcentDisplay1) + SEN_COEFFICIENT_0;
	  gu64PM2p5ConcentFinalNormal = gu64PM2p5ConcentDisplay1;
	  gu64PM2p5ConcentFinalSharp = gu64PM2p5ConcentDisplay1 + mu64ShiftDataSensitivity;
	  gu64PM2p5ConcentFinalDull = gu64PM2p5ConcentDisplay1 - mu64ShiftDataSensitivity;

	  if(gu8ParticleSensitivity == PPD71_SENSITIVITY_NORMAL)  //민감도 보통(default)
	  {
	    gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalNormal;
	  }
	  else  //민감도 샤프 또는 둔함
	  {
	    if((gu64PM2p5ConcentDisplay1 >= SEN_AREA_MIN) && (gu64PM2p5ConcentDisplay1 < SEN_AREA_MAX)) //적용 범위
	    {
	      if(gu8ParticleSensitivity == PPD71_SENSITIVITY_SHARP) //민감도 샤프
	      {
	        gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalSharp;
	      }
	      else if(gu8ParticleSensitivity == PPD71_SENSITIVITY_DULL)//민감도 둔함
	      {
	        gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalDull;
	      }
		  else
		  {
		  }
	    }
	    else  //적용 범위를 벗어남
	    {
	      gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalNormal;
	    }
	  }

	  // PM2.5 농도값 음수 방지
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
		// #6. PM10 값 계산 처리
		//============================================================================================================

		// PM10 값 계산 처리
		md64Val = (D64)gu16Lpo2p5 * 0.0001f;  // PM2.5 입자수 LPO값

	    //gu64PM10Concent1 = (73363.f*(x*(x*x))) + (25874.f*(x*x)) + (4701.f*x) + 0.9216f; // PM2.5 입자수 농도_LPP 공식
	    gu64PM10Concent1 = (100000.f*(md64Val*(md64Val*md64Val))) + (100000.f*(md64Val*md64Val)) + (10000.f*md64Val) + 1.5f;  // PM2.5 입자수 농도_DR3 공식

		if(gu64PM10Concent1 <= 0.f)    // PM10 값 MIN: 0
		{
			gu64PM10Concent1 = 0.f;
		}
		else
		{
		}

		if(gu64PM10Concent1 >= 1000.f) // PM10 값 MAX: 1000
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
        	gu64PM10ConcentFinal = (md64SumPM10180s / 90.f); // 180sec 평균 계산
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
		memset(gu8ParticleRxDataBuffer, 0, sizeof(gu8ParticleRxDataBuffer));
	}
}

/// @brief      PPD71 센서 데이터 에러 체크 함수
/// @param      void
/// @return     void
/// @details    입자 센서의 에러 상태를 확인하여 전체 에러 플래그를 설정
void PPD71_CheckParticleDataError(void) {
    if(gu8ErrParticle1 || gu8ErrParticle2) {
        g_errorFlag = SET;
    } else {
        g_errorFlag = CLEAR;
    }
}

/// @brief      PPD71 센서 데이터스트림 에러 체크 함수
/// @param      void
/// @return     void
/// @details    데이터 수신 타임아웃 발생 시 파티클 에러2 플래그를 설정
void PPD71_CheckParticleDatastreamError(void) {
    if(gu8ParticleDataTimeout) {
        gu8ErrParticle2 = SET;
    }
}

// 외부 공개 인터페이스 함수들
/// @brief      PPD71 먼지센서 초기화
/// @param      void
/// @return     void
void Hal_DustSensor_PPD71_Init(void) {
    g_errorFlag = CLEAR;
    PPD71_Init();
}

/// @brief      PPD71 먼지센서 전원 ON
/// @param      void
/// @return     void
void Hal_DustSensor_PPD71_On(void) {
    PORT_DEFINE_PARTICLE_SENSOR_POW = VALUE_PARTICLE_POWER_ON;
}

/// @brief      PPD71 먼지센서 전원 OFF
/// @param      void
/// @return     void
void Hal_DustSensor_PPD71_Off(void) {
    PORT_DEFINE_PARTICLE_SENSOR_POW = VALUE_PARTICLE_POWER_OFF;
}

/// @brief      PPD71 먼지센서 데이터 읽기
/// @param      pm1_0: PM1.0 농도 데이터 포인터
/// @param      pm2_5: PM2.5 농도 데이터 포인터
/// @param      pm10: PM10 농도 데이터 포인터
/// @param      error: 오류 상태 플래그 포인터
/// @return     void
void Hal_DustSensor_PPD71_ReadData(U16* pm1_0, U16* pm2_5, U16* pm10, U8* error) {
    // NULL 포인터 체크는 이미 있음 - 유지
    if (!pm1_0 || !pm2_5 || !pm10 || !error) return;

    // 기존 로직 유지하되 에러 상태 체크 강화
    *error = g_errorFlag;

    // 센서 데이터 유효성 추가 체크
    if (g_errorFlag != CLEAR) {
        *pm1_0 = 0;
        *pm2_5 = 0;
        *pm10 = 0;
        return;
    }

    // 기존 데이터 할당 로직
    *pm1_0 = 0; // PPD71은 PM1.0 데이터를 제공하지 않음
    *pm2_5 = (U16)gu64PM2p5ConcentFinal;
    *pm10 = (U16)gu64PM10ConcentFinal;
}

/// @brief      PPD71 먼지센서 민감도 설정
/// @param      sensitivity: 민감도 설정값 (NORMAL/SHARP/DULL)
/// @return     void
void Hal_DustSensor_PPD71_SetSensitivity(PPD71_SENSITIVITY_T sensitivity) {
    gu8ParticleSensitivity = sensitivity;
}

/// @brief      PPD71 먼지센서 민감도 반환
/// @param      void
/// @return     PPD71_SENSITIVITY_T 현재 민감도 설정값
/// @details    현재 설정된 센서 민감도 값을 반환
PPD71_SENSITIVITY_T Hal_DustSensor_PPD71_GetSensitivity(void) {
    return gu8ParticleSensitivity;
}

/// @brief      PPD71 먼지센서 핸들러 (외부 인터페이스)
/// @param      void
/// @return     void
/// @details    외부에서 호출 가능한 PPD71 센서 핸들러 인터페이스
void Hal_DustSensor_PPD71_Handler(void) {
    PPD71_Handler();
}

/// @brief      Hal Dust Sensor PPD71 Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 PPD71 센서 관련 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_DustSensor_PPD71_ControlTimer(void)
{
    // 1s마다 타이머 카운트 증가
    if (gu8DustSensorControlTimer < PPD71_CONTROL_TIME_PERIOD)
    {
        gu8DustSensorControlTimer++;
    }
}

/// @brief      Hal Dust Sensor PPD71 Module in While Loop
/// @details    Basic Loop 의 While문 안에서 PPD71 센서 제어 한다
/// @param      void
/// @return     void
void Hal_DustSensor_PPD71_Module_Control(void)
{
    if (gu8DustSensorControlTimer >= PPD71_CONTROL_TIME_PERIOD)
    {
        Hal_DustSensor_PPD71_Handler();
        gu8DustSensorControlTimer = 0;
    }
}

// Getter 함수들
/// @brief      PPD71 PM2.5 농도 값 반환 (U16)
/// @param      void
/// @return     U16 PM2.5 농도 값 (μg/m³)
/// @details    PPD71 센서에서 측정된 PM2.5 농도 값을 정수형으로 반환
U16 Hal_DustSensor_PPD71_GetPM2_5(void) {
    return (U16)gu64PM2p5ConcentDisplayFinal;
}

/// @brief      PPD71 PM10 농도 값 반환 (U16)
/// @param      void
/// @return     U16 PM10 농도 값 (μg/m³)
/// @details    PPD71 센서에서 측정된 PM10 농도 값을 정수형으로 반환
U16 Hal_DustSensor_PPD71_GetPM10(void) {
    return (U16)gu64PM10ConcentDisplayFinal;
}

/// @brief      PPD71 PM2.5 농도 값 반환 (D64)
/// @param      void
/// @return     D64 PM2.5 농도 값 (μg/m³)
/// @details    PPD71 센서에서 측정된 PM2.5 농도 값을 고정밀도 실수형으로 반환
D64 Hal_DustSensor_PPD71_GetPM2_5Final(void) {
    return gu64PM2p5ConcentDisplayFinal;
}

/// @brief      PPD71 PM10 농도 값 반환 (D64)
/// @param      void
/// @return     D64 PM10 농도 값 (μg/m³)
/// @details    PPD71 센서에서 측정된 PM10 농도 값을 고정밀도 실수형으로 반환
D64 Hal_DustSensor_PPD71_GetPM10Final(void) {
    return gu64PM10ConcentDisplayFinal;
}

/// @brief      PPD71 센서 에러 플래그 반환
/// @param      void
/// @return     U8 에러 플래그 (SET/CLEAR)
/// @details    PPD71 센서의 현재 에러 상태를 반환
U8 Hal_DustSensor_PPD71_GetErrorFlag(void) {
    return g_errorFlag;
}

// Hal Dust Sensor Module
//******************************************************************************************

/// @brief      Hal Dust Sensor PPD71 Initialize Module
/// @details    PPD71 먼지센서 관련 모든 것을 초기화 시킨다
/// @param      void
/// @return     void
void Hal_DustSensor_PPD71_Module_Initialize(void)
{
#if (DUST_SENSOR_PPD71_USE == USE)
    Hal_DustSensor_PPD71_Init();                // PPD71 센서 초기화
#endif
}

/// @brief      Hal Dust Sensor PPD71 Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 PPD71 센서 관련 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_DustSensor_PPD71_Module_1ms_Control(void)
{
#if (DUST_SENSOR_PPD71_USE == USE)
    Hal_DustSensor_PPD71_ControlTimer();       // PPD71 센서 타이머 카운터
#endif
}

#endif  // DUST_SENSOR_PPD71_USE == USE
