#include "DXFPSCounter.h"

namespace DXLib{
	/**
	*@brief
	*/
	DXFPSCounter::DXFPSCounter(){
		SetSampleNum(10);

		m_fFPS	= 0;
		m_fTimer = 0.0;
		m_llBaseTime = 0;

		if(QueryPerformanceCounter(&m_Counter) != 0){
			//Query�g�p�\
			m_iCounterType = FPS_COUNT_QUERY;
			m_OldLongCounter = m_Counter.QuadPart;
			LARGE_INTEGER freq;
			QueryPerformanceFrequency(&freq);
			m_fFrequency = (double)freq.QuadPart;
		}else{
			m_iCounterType = FPS_COUNT_TGT;
			timeBeginPeriod(1);
			m_dwTGTOldCount = timeGetTime();
		}
	}

	/**
	*/
	DXFPSCounter::~DXFPSCounter(){
		if(m_iCounterType == FPS_COUNT_TGT){
			timeEndPeriod(1);
		}
	}

	/**
	*@brief	FPS�̍X�V
	*@return	
	*/
	void DXFPSCounter::UpdateFPS(){
		m_fElapsedTime = GetElapsedTime();

		//�Â��f�[�^�̍폜
		m_fElapsedTimeList.pop_front();
		//�V�����f�[�^�̒ǉ�
		m_fElapsedTimeList.push_back(m_fElapsedTime);

		m_fSumTimes += m_fElapsedTime - *m_fElapsedTimeList.begin();

		//FPS�v�Z
		double ave = (m_fSumTimes + m_fElapsedTime) / m_iSampleNum;

		static DWORD dwPre = timeGetTime();
		DWORD dwNow = timeGetTime();

		if(dwNow - dwPre < 500){
			return ;
		}
		dwPre = dwNow;

		if(ave != 0){
			m_fFPS = 1000.0f / ave;
		}
	}

	/**
	*@brief		���݂̍����������擾
	*@return	��������
	*/
	double DXFPSCounter::GetElapsedTime(){
		if(m_iCounterType == FPS_COUNT_QUERY){
			QueryPerformanceCounter(&m_Counter);
			LONGLONG longDef = m_Counter.QuadPart - m_OldLongCounter;
			double def = (double)longDef;

			m_OldLongCounter = m_Counter.QuadPart;
			//�A�v���P�[�V��������
			m_fTimer = (m_Counter.QuadPart - m_llBaseTime) / 1000.0 / m_fFrequency;

			return def * 1000 / m_fFrequency;
		}

		DWORD dwCur = timeGetTime();
		DWORD dwDiff = dwCur - m_dwTGTOldCount;
		m_dwTGTOldCount = dwCur;

		m_fTimer = dwCur / 1000.0f;

		return dwDiff;
	}

	/**
	*@brief		�T���v�����鐔�̐ݒ�
	*@return	
	*@param	iSampleNum	�T���v����
	*/
	void DXFPSCounter::SetSampleNum(long iSampleNum){
		m_iSampleNum = iSampleNum;
		m_fElapsedTimeList.resize(m_iSampleNum, 0.0f);
		m_fSumTimes = 0;
	}

	/**
	*@brief
	*@return
	*/
	double DXFPSCounter::GetAbsoluteTime(){
		LARGE_INTEGER qwTime = {0};
		QueryPerformanceCounter(&qwTime);

		double fTime = qwTime.QuadPart / m_fFrequency;

		return fTime;
	}

	/**
	*@brief	�t�H�[�J�X���������Ƃ��̏���.
	*@return
	*/
	void DXFPSCounter::OnKillFocus(){
	}

	/**
	*@brief	�t�H�[�J�X�𓾂��Ƃ��̏���
	*@return	
	*/
	void DXFPSCounter::OnSetFocus(){
	}

};