/// @file     Hal_DustSensor_PPD71.c
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    PPD71 �������� �ϵ���� �߻�ȭ ����
/// @details  PPD71 �������� ��� ���� HAL ����̹� ����

#include "Global_Header.h"

#if (DUST_SENSOR_PPD71_USE == USE)

// PPD71 ���� ���� ���� ����
static U8 gu8UARTOK_F = 0;
// static U8 gu8ParticleDataCheck = 0; // �̻�� ����
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

// �ΰ��� ���� ����
static D64 gu64PM2p5ConcentDisplay1 = 0;

// �ΰ��� ������
D64 gu64PM2p5ConcentFinalNormal = 0;
D64 gu64PM2p5ConcentFinalSharp = 0;
D64 gu64PM2p5ConcentFinalDull = 0;
PPD71_SENSITIVITY_T gu8ParticleSensitivity = PPD71_SENSITIVITY_NORMAL;

// ���� �÷���
static U8 g_errorFlag = CLEAR;

// Ÿ�̸� ����
static U8 gu8DustSensorControlTimer = 0;
static U8 gu8ParticleDataTimeout = 0;

// ���� �Լ� ���� ������Ÿ�� (���� ���Ͽ� ���� public���� ����)
void PPD71_Init(void);
void PPD71_Handler(void);
void PPD71_RxParticleData(void);
void PPD71_CalParticleConcent(void);
void PPD71_CheckParticleDataError(void);
void PPD71_CheckParticleDatastreamError(void);

/// @brief      PPD71 ���� �ʱ�ȭ �Լ�
/// @param      void
/// @return     void
/// @details    PPD71 ������ �������� �ʱ�ȭ�ϰ� UART ���� ���۸� Ŭ����
void PPD71_Init(void) {
    gu8UARTOK_F = 0;
    gu8ErrParticle1 = CLEAR;
    gu8ErrParticle2 = CLEAR;
    MEMSET( (void __FAR *)&gu8ParticleRxDataBuffer, 0x00, sizeof(gu8ParticleRxDataBuffer) );
}

/// @brief      PPD71 ���� ���� �ڵ鷯
/// @param      void
/// @return     void
/// @details    PPD71 ������ ������ ����, ���, ���� üũ�� ���������� ����
void PPD71_Handler(void) {
    PPD71_RxParticleData();
    PPD71_CalParticleConcent();
    PPD71_CheckParticleDataError();
    PPD71_CheckParticleDatastreamError();
}

/// @brief      PPD71 ���� ������ ���� �Լ�
/// @param      void
/// @return     void
/// @details    UART2�� ���� PPD71 �����κ��� 29����Ʈ �����͸� ����
void PPD71_RxParticleData(void)
{
	MD_STATUS status = R_Config_UART2_Receive((U8*)gu8ParticleRxDataBuffer, 29);
    if (status != MD_OK) {
        // ���� ó��: ���� ���н� ����
    }
}

/// @brief      PPD71 �̼����� �� ��� �Լ�
/// @param      void
/// @return     void
/// @details    ���ŵ� �����͸� �������� PM2.5�� PM10 �󵵸� ����ϰ� �ΰ����� ����
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
		// #1. ���� ������ �Ľ�
		//============================================================================================================

	    // ���ŵ� ������ ó��
		gu16MassConcent10s  = ((U16)(gu8ParticleRxDataBuffer[3]) << 8) + (U16)(gu8ParticleRxDataBuffer[4]);
		gu16MassConcent180s = ((U16)(gu8ParticleRxDataBuffer[9]) << 8) + (U16)(gu8ParticleRxDataBuffer[10]);  //PM2.5 �� ��

		gu16Lpo2p5 = ((U16)(gu8ParticleRxDataBuffer[17]) << 8) + (U16)(gu8ParticleRxDataBuffer[18]);

        // Raw Data ����ȯ �� ���۸� ó�� �߰� 2018.06.29

        gu64MassConcent10s = (D64)gu16MassConcent10s;    // U16 -> D64 ��ȯ
		gu64MassConcent180s = (D64)gu16MassConcent180s;

        // PM2.5 10�� ���۸�
		gu64Buf10s[2] = gu64Buf10s[1];       // 4���� Data
		gu64Buf10s[1] = gu64Buf10s[0];       // 2���� Data
		gu64Buf10s[0] = gu64MassConcent10s;  // ���� Data

		// PM2.5 180�� ���۸�
		gu64Buf180s[2] = gu64Buf180s[1];
		gu64Buf180s[1] = gu64Buf180s[0];
		gu64Buf180s[0] = gu64MassConcent180s;

		// PM2.5 ���ڼ� LPO
		gu64BufLpo2p5[2] = gu64BufLpo2p5[1];
		gu64BufLpo2p5[1] = gu64BufLpo2p5[0];
		gu64BufLpo2p5[0] = (D64)gu16Lpo2p5;

		//============================================================================================================
		// *PM2.5 �󵵰� 5ug/m^3 ������ �� 10��/180�� �߿��� ���������� ��� ����
        //
		//  �˰���
		// -. ��� �� ������� ������ ��Ȳ������ 180�� �߿��� �����Ͽ� ���, ������ �����¿����� 10�� ����Ͽ� ������ ���
		// -. �̼����� �� ��ȭ�� Ȯ�� ���� �ܽð��� 10�� �� ȥ�������� �ʰ� ó��
		//============================================================================================================

		if(gu64MassConcent180s <= 5.f) // PM2.5 180�� �� �󵵰� 5����ũ�α׷� ������ �� 180�� �� ������� ���� (û������ ����)
		{
		    // Ŭ��ȯ�濡�� ���̼������� 1��g�� ǥ�õ� ���� ����� �ΰ��� ���� ���� ����ȯ�� ���� ��û���� ���α׷� ����
            if(gu64MassConcent10s <= 5.f)
            {
                gu64PM2p5ConcentFinal = gu64MassConcent10s;  // PM2.5 180�� �󵵿� 10�� �󵵰� �Ѵ� 5ug ������ �� 10�� �󵵸� ǥ��
            }
			else
			{
				gu64PM2p5ConcentFinal = gu64MassConcent180s; // PM2.5 180�� �󵵿� 10�� �󵵰� �Ѵ� 5ug���� ū ��� 180�� �󵵸� ǥ��
			}

			gu64PM2p5BeConcent = gu64PM2p5ConcentFinal;  // ���� ��� �󵵰� ����
			gu8PM2p5BeCond1_F = 0;			// ���� ����1 ���� �ʱ�ȭ
			gu8PM2p5BeCond2_F = 0;			// ���� ����2 ���� �ʱ�ȭ

			//============================================================================================================
	    	// ǥ�ر� �� ���м� ����(Ķ���극�̼�)
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

			// PM2.5 �󵵰� ���� ����
			if(gu64PM2p5ConcentDisplay1 <= 0)
			{
				gu64PM2p5ConcentDisplay1 = 0;
			}

			// PM2.5 MAX�� : 1000
			if(gu64PM2p5ConcentDisplay1 >= 1000.f)
			{
				gu64PM2p5ConcentDisplay1 = 1000.f;
			}

		  //============================================================================================================
		  // �ΰ��� ������ ��� �� ���� (Ķ���극�̼�/ŷ���� ���� ����)
		  //============================================================================================================
		  mu64ShiftDataSensitivity = (SEN_COEFFICIENT_2*(gu64PM2p5ConcentDisplay1*gu64PM2p5ConcentDisplay1)) + (SEN_COEFFICIENT_1*gu64PM2p5ConcentDisplay1) + SEN_COEFFICIENT_0;
		  gu64PM2p5ConcentFinalNormal = gu64PM2p5ConcentDisplay1;
		  gu64PM2p5ConcentFinalSharp = gu64PM2p5ConcentDisplay1 + mu64ShiftDataSensitivity;
		  gu64PM2p5ConcentFinalDull = gu64PM2p5ConcentDisplay1 - mu64ShiftDataSensitivity;

		  if(gu8ParticleSensitivity == PPD71_SENSITIVITY_NORMAL)  //�ΰ��� ����(default)
		  {
		    gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalNormal;
		  }
		  else  //�ΰ��� ���� �Ǵ� ����
		  {
		    if((gu64PM2p5ConcentDisplay1 >= SEN_AREA_MIN) && (gu64PM2p5ConcentDisplay1 < SEN_AREA_MAX)) //���� ����
		    {
		      if(gu8ParticleSensitivity == PPD71_SENSITIVITY_SHARP) //�ΰ��� ����
		      {
		        gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalSharp;
		      }
		      else if(gu8ParticleSensitivity == PPD71_SENSITIVITY_DULL)//�ΰ��� ����
		      {
		        gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalDull;
		      }
		    }
		    else  //���� ������ ���
		    {
		      gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalNormal;
		    }
		  }

		    // PM2.5 �󵵰� ���� ����
			if(gu64PM2p5ConcentDisplayFinal <= 0)
			{
				gu64PM2p5ConcentDisplayFinal = 0;
			}

			// PM2.5 MAX��
			if(gu64PM2p5ConcentDisplayFinal >= 1000.f)
			{
				gu64PM2p5ConcentDisplayFinal = 1000.f;
			}

			// PM10 �� ��� ó��
			md64Val = (D64)gu16Lpo2p5 * 0.0001f;

	        //gu64PM10Concent1 = (73363.f*(x*(x*x))) + (25874.f*(x*x)) + (4701.f*x) + 0.9216f; // PM2.5 ���ڼ� ��_LPP ����
	        gu64PM10Concent1 = (100000.f*(md64Val*(md64Val*md64Val))) + (100000.f*(md64Val*md64Val)) + (10000.f*md64Val) + 1.5f; // PM2.5 ���ڼ� ��_DR3 ����

			if(gu64PM10Concent1 <= 0.f)    // PM10 �� MIN: 0
			{
				gu64PM10Concent1 = 0.f;
			}

			if(gu64PM10Concent1 >= 1000.f) // PM10 �� MAX: 1000
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
        	    gu64PM10ConcentFinal = (md64SumPM10180s / 90.f); // 180sec ��� ���
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
		// #2. ����1, ����2 �Ǻ� �� ���
		//============================================================================================================

		// ���� 1. 10�� �� ��ȭ�� �� ǥ�ذ� ���
		gu64PM2p5Cond1AbsVal = fabs(gu64Buf10s[0] - gu64Buf10s[1]);
		gu64PM2p5Cond1StdVal = (0.00008f*gu64PM2p5BeConcent*gu64PM2p5BeConcent) + (0.0646f*gu64PM2p5BeConcent) + 2.1211f;

        // ���� 2. 10�� �󵵿� 180�� �� �񱳰��
		gu64PM2p5Cond2AbsVal = fabs(gu64Buf10s[0] - gu64Buf180s[1]);
		gu64PM2p5Cond2StdVal = (0.0004f*gu64PM2p5BeConcent*gu64PM2p5BeConcent) + (0.0964f*gu64PM2p5BeConcent) + 8.5899f;

		// ���� �󵵰��� ����1, ����2�� �������ο� ���� ���
		if(gu64PM2p5Cond1AbsVal > gu64PM2p5Cond1StdVal)
		{
			gu8PM2p5Cond1_F = 1;  // ���� 1 ����
		}
		else
		{
			gu8PM2p5Cond1_F = 0;  // ���� 1 ������
		}

		if(gu64PM2p5Cond2AbsVal > gu64PM2p5Cond2StdVal)
		{
			gu8PM2p5Cond2_F = 1;  // ���� 2 ����
		}
		else
		{
			gu8PM2p5Cond2_F = 0;  // ���� 2 ������
		}

		// ������� �� ���
		md64PM2p5_1 = fabs(gu64Buf10s[0]-gu64Buf10s[1]) / ((0.01*gu64Buf180s[0])+4);
        md64PM2p5_2 = fabs(gu64Buf10s[0]-gu64Buf10s[1]) / ((0.02*gu64Buf180s[0])+8);

		if(gu8PM2p5Cond1_F && gu8PM2p5Cond2_F)       // ����1:����, ����2:����
		{
			if((gu64Buf10s[0] > gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0]))       // ���簪 > ������, 10�� > 180��
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_1;
			}
			else if((gu64Buf10s[0] > gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0]))  // ���簪 > ������, 10�� < 180��
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_2;
			}
			else if((gu64Buf10s[0] < gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0]))  // ���簪 < ������, 10�� > 180��
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_2;
			}
			else if((gu64Buf10s[0] < gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0]))  // ���簪 < ������, 10�� < 180��
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_1;
			}
			else
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent;
			}
		}
		else if(gu8PM2p5Cond1_F && !gu8PM2p5Cond2_F)  // ����1:����, ����2:������
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
		else if(!gu8PM2p5Cond1_F && gu8PM2p5Cond2_F)  // ����1:������, ����2:����
		{
			if((gu64Buf10s[0] > gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0]))       // ���簪 > ������, 10�� > 180��
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_2;
			}
			else if((gu64Buf10s[0] > gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0]))  // ���簪 > ������, 10�� < 180��
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_2;
			}
			else if((gu64Buf10s[0] < gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0]))  // ���簪 < ������, 10�� > 180��
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_2;
			}
			else if((gu64Buf10s[0] < gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0]))  // ���簪 < ������, 10�� < 180��
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_2;
			}
			else if((gu64Buf10s[0] == gu64Buf10s[1]) && (gu64Buf10s[0] > gu64Buf180s[0])) // ���簪 = ������, 10�� > 180��
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent + md64PM2p5_2;
			}
			else if((gu64Buf10s[0] == gu64Buf10s[1]) && (gu64Buf10s[0] < gu64Buf180s[0])) // ���簪 = ������, 10�� < 180��
			{
			    gu64PM2p5Concent1 = gu64PM2p5BeConcent - md64PM2p5_2;
			}
			else
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent;
			}
		}
		else                                             // ����1:������, ����2:������
		{
			if(gu8PM2p5BeCond1_F || gu8PM2p5BeCond2_F)         // ��������Ŭ ����1 or ����2�� ���� �̾������� Ȯ��
			{
				gu64PM2p5Concent1 = gu64PM2p5BeConcent;     // ������ �״�� �� ���
			}
			else
			{
				gu64PM2p5Concent1 = gu64Buf180s[0];  // ���� 180�� �� ���
			}
		}

		if(gu64PM2p5Concent1 <= 0) // 10�� ��갪�� ������ �Ǵ°��� �����Ͽ� 0 ���Ͻô� 0 ���� ó��
		{
		    gu64PM2p5Concent1 = 0;
		}
		else
		{
		}

        //============================================================================================================
		// #3. 180�� �� ���м� / �ִ밪 ����
		//============================================================================================================

        // 180�� �� ���м� ���� ���� Ȯ��: ���� 180�� �󵵰� ���̿� �޺����� �־����� 10�� �� ���̿� �� ��� ó��
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

        // 180�� �� �ִ밪 ���� ���� Ȯ��
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
	    // #4. ǥ�ر� �� ���м� ����(Ķ���극�̼�)
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

                // PM2.5 �󵵰� ���� ����
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
	  // #5. �ΰ��� ������ ��� �� ���� (Ķ���극�̼�/ŷ���� ���� ����)
	  //============================================================================================================
	  mu64ShiftDataSensitivity = (SEN_COEFFICIENT_2*(gu64PM2p5ConcentDisplay1*gu64PM2p5ConcentDisplay1)) + (SEN_COEFFICIENT_1*gu64PM2p5ConcentDisplay1) + SEN_COEFFICIENT_0;
	  gu64PM2p5ConcentFinalNormal = gu64PM2p5ConcentDisplay1;
	  gu64PM2p5ConcentFinalSharp = gu64PM2p5ConcentDisplay1 + mu64ShiftDataSensitivity;
	  gu64PM2p5ConcentFinalDull = gu64PM2p5ConcentDisplay1 - mu64ShiftDataSensitivity;

	  if(gu8ParticleSensitivity == PPD71_SENSITIVITY_NORMAL)  //�ΰ��� ����(default)
	  {
	    gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalNormal;
	  }
	  else  //�ΰ��� ���� �Ǵ� ����
	  {
	    if((gu64PM2p5ConcentDisplay1 >= SEN_AREA_MIN) && (gu64PM2p5ConcentDisplay1 < SEN_AREA_MAX)) //���� ����
	    {
	      if(gu8ParticleSensitivity == PPD71_SENSITIVITY_SHARP) //�ΰ��� ����
	      {
	        gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalSharp;
	      }
	      else if(gu8ParticleSensitivity == PPD71_SENSITIVITY_DULL)//�ΰ��� ����
	      {
	        gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalDull;
	      }
		  else
		  {
		  }
	    }
	    else  //���� ������ ���
	    {
	      gu64PM2p5ConcentDisplayFinal = gu64PM2p5ConcentFinalNormal;
	    }
	  }

	  // PM2.5 �󵵰� ���� ����
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
		// #6. PM10 �� ��� ó��
		//============================================================================================================

		// PM10 �� ��� ó��
		md64Val = (D64)gu16Lpo2p5 * 0.0001f;  // PM2.5 ���ڼ� LPO��

	    //gu64PM10Concent1 = (73363.f*(x*(x*x))) + (25874.f*(x*x)) + (4701.f*x) + 0.9216f; // PM2.5 ���ڼ� ��_LPP ����
	    gu64PM10Concent1 = (100000.f*(md64Val*(md64Val*md64Val))) + (100000.f*(md64Val*md64Val)) + (10000.f*md64Val) + 1.5f;  // PM2.5 ���ڼ� ��_DR3 ����

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

		for(mu8Cnt = 0; mu8Cnt < 90; mu8Cnt++)
		{
			md64SumPM10180s += gu64PM10Concent180s[mu8Cnt];
		}

        if(gu8Buf180s_F)
        {
        	gu64PM10ConcentFinal = (md64SumPM10180s / 90.f); // 180sec ��� ���
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

/// @brief      PPD71 ���� ������ ���� üũ �Լ�
/// @param      void
/// @return     void
/// @details    ���� ������ ���� ���¸� Ȯ���Ͽ� ��ü ���� �÷��׸� ����
void PPD71_CheckParticleDataError(void) {
    if(gu8ErrParticle1 || gu8ErrParticle2) {
        g_errorFlag = SET;
    } else {
        g_errorFlag = CLEAR;
    }
}

/// @brief      PPD71 ���� �����ͽ�Ʈ�� ���� üũ �Լ�
/// @param      void
/// @return     void
/// @details    ������ ���� Ÿ�Ӿƿ� �߻� �� ��ƼŬ ����2 �÷��׸� ����
void PPD71_CheckParticleDatastreamError(void) {
    if(gu8ParticleDataTimeout) {
        gu8ErrParticle2 = SET;
    }
}

// �ܺ� ���� �������̽� �Լ���
/// @brief      PPD71 �������� �ʱ�ȭ
/// @param      void
/// @return     void
void Hal_DustSensor_PPD71_Init(void) {
    g_errorFlag = CLEAR;
    PPD71_Init();
}

/// @brief      PPD71 �������� ���� ON
/// @param      void
/// @return     void
void Hal_DustSensor_PPD71_On(void) {
    PORT_DEFINE_PARTICLE_SENSOR_POW = VALUE_PARTICLE_POWER_ON;
}

/// @brief      PPD71 �������� ���� OFF
/// @param      void
/// @return     void
void Hal_DustSensor_PPD71_Off(void) {
    PORT_DEFINE_PARTICLE_SENSOR_POW = VALUE_PARTICLE_POWER_OFF;
}

/// @brief      PPD71 �������� ������ �б�
/// @param      pm1_0: PM1.0 �� ������ ������
/// @param      pm2_5: PM2.5 �� ������ ������
/// @param      pm10: PM10 �� ������ ������
/// @param      error: ���� ���� �÷��� ������
/// @return     void
void Hal_DustSensor_PPD71_ReadData(U16* pm1_0, U16* pm2_5, U16* pm10, U8* error) {
    // NULL ������ üũ�� �̹� ���� - ����
    if (!pm1_0 || !pm2_5 || !pm10 || !error) return;

    // ���� ���� �����ϵ� ���� ���� üũ ��ȭ
    *error = g_errorFlag;

    // ���� ������ ��ȿ�� �߰� üũ
    if (g_errorFlag != CLEAR) {
        *pm1_0 = 0;
        *pm2_5 = 0;
        *pm10 = 0;
        return;
    }

    // ���� ������ �Ҵ� ����
    *pm1_0 = 0; // PPD71�� PM1.0 �����͸� �������� ����
    *pm2_5 = (U16)gu64PM2p5ConcentFinal;
    *pm10 = (U16)gu64PM10ConcentFinal;
}

/// @brief      PPD71 �������� �ΰ��� ����
/// @param      sensitivity: �ΰ��� ������ (NORMAL/SHARP/DULL)
/// @return     void
void Hal_DustSensor_PPD71_SetSensitivity(PPD71_SENSITIVITY_T sensitivity) {
    gu8ParticleSensitivity = sensitivity;
}

/// @brief      PPD71 �������� �ΰ��� ��ȯ
/// @param      void
/// @return     PPD71_SENSITIVITY_T ���� �ΰ��� ������
/// @details    ���� ������ ���� �ΰ��� ���� ��ȯ
PPD71_SENSITIVITY_T Hal_DustSensor_PPD71_GetSensitivity(void) {
    return gu8ParticleSensitivity;
}

/// @brief      PPD71 �������� �ڵ鷯 (�ܺ� �������̽�)
/// @param      void
/// @return     void
/// @details    �ܺο��� ȣ�� ������ PPD71 ���� �ڵ鷯 �������̽�
void Hal_DustSensor_PPD71_Handler(void) {
    PPD71_Handler();
}

/// @brief      Hal Dust Sensor PPD71 Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� PPD71 ���� ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_DustSensor_PPD71_ControlTimer(void)
{
    // 1s���� Ÿ�̸� ī��Ʈ ����
    if (gu8DustSensorControlTimer < PPD71_CONTROL_TIME_PERIOD)
    {
        gu8DustSensorControlTimer++;
    }
}

/// @brief      Hal Dust Sensor PPD71 Module in While Loop
/// @details    Basic Loop �� While�� �ȿ��� PPD71 ���� ���� �Ѵ�
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

// Getter �Լ���
/// @brief      PPD71 PM2.5 �� �� ��ȯ (U16)
/// @param      void
/// @return     U16 PM2.5 �� �� (��g/m��)
/// @details    PPD71 �������� ������ PM2.5 �� ���� ���������� ��ȯ
U16 Hal_DustSensor_PPD71_GetPM2_5(void) {
    return (U16)gu64PM2p5ConcentDisplayFinal;
}

/// @brief      PPD71 PM10 �� �� ��ȯ (U16)
/// @param      void
/// @return     U16 PM10 �� �� (��g/m��)
/// @details    PPD71 �������� ������ PM10 �� ���� ���������� ��ȯ
U16 Hal_DustSensor_PPD71_GetPM10(void) {
    return (U16)gu64PM10ConcentDisplayFinal;
}

/// @brief      PPD71 PM2.5 �� �� ��ȯ (D64)
/// @param      void
/// @return     D64 PM2.5 �� �� (��g/m��)
/// @details    PPD71 �������� ������ PM2.5 �� ���� �����е� �Ǽ������� ��ȯ
D64 Hal_DustSensor_PPD71_GetPM2_5Final(void) {
    return gu64PM2p5ConcentDisplayFinal;
}

/// @brief      PPD71 PM10 �� �� ��ȯ (D64)
/// @param      void
/// @return     D64 PM10 �� �� (��g/m��)
/// @details    PPD71 �������� ������ PM10 �� ���� �����е� �Ǽ������� ��ȯ
D64 Hal_DustSensor_PPD71_GetPM10Final(void) {
    return gu64PM10ConcentDisplayFinal;
}

/// @brief      PPD71 ���� ���� �÷��� ��ȯ
/// @param      void
/// @return     U8 ���� �÷��� (SET/CLEAR)
/// @details    PPD71 ������ ���� ���� ���¸� ��ȯ
U8 Hal_DustSensor_PPD71_GetErrorFlag(void) {
    return g_errorFlag;
}

// Hal Dust Sensor Module
//******************************************************************************************

/// @brief      Hal Dust Sensor PPD71 Initialize Module
/// @details    PPD71 �������� ���� ��� ���� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_DustSensor_PPD71_Module_Initialize(void)
{
#if (DUST_SENSOR_PPD71_USE == USE)
    Hal_DustSensor_PPD71_Init();                // PPD71 ���� �ʱ�ȭ
#endif
}

/// @brief      Hal Dust Sensor PPD71 Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� PPD71 ���� ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_DustSensor_PPD71_Module_1ms_Control(void)
{
#if (DUST_SENSOR_PPD71_USE == USE)
    Hal_DustSensor_PPD71_ControlTimer();       // PPD71 ���� Ÿ�̸� ī����
#endif
}

#endif  // DUST_SENSOR_PPD71_USE == USE
