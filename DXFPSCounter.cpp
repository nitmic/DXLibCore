
#include "DXStdafx.h"
#include "DXFPSCounter.h"
#include <deque>

namespace DXLib{
	struct DXFPSCounter::Impl{
		enum State{FPS_COUNT_QUERY = 1, FPS_COUNT_TGT	 = 2};
		
		double		GetElapsedTime();

		State			m_iCounterType;		//!<	�J�E���^�̎��
		LARGE_INTEGER	m_Counter;			//!<	�J�E���^
		double			m_fFrequency;		//!<	���g��

		long long		m_OldLongCounter;	//!<	1�t���[���O�̃J�E���^
		unsigned long	m_dwTGTOldCount;	//!<	1�t���[���O�̃J�E���^

		double			m_fFPS;				//!<	FPS�̒l
		double			m_fElapsedTime;		//!<	��������
		std::deque<double> m_fElapsedTimeList;	//!<	FPS���X�g
		long			m_iSampleNum;		//!<	�T���v�����鐔
		double			m_fSumTimes;		//!<	�T���v���̍��v
		double			m_fTimer;			//!<	�A�v���P�[�V�����^�C�}�[
		long long		m_llBaseTime;		//!<	�x�[�X����	
	};
	
	/**
	*@brief		���݂̍����������擾
	*@return	��������
	*/
	double DXFPSCounter::Impl::GetElapsedTime(){
		if(m_iCounterType == Impl::FPS_COUNT_QUERY){
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
	*@brief
	*/
	DXFPSCounter::DXFPSCounter(){
		__impl__ = std::make_shared<Impl>();
		__impl__->m_fFPS	= 0;
		__impl__->m_fTimer = 0.0;
		__impl__->m_llBaseTime = 0;
		SetSampleNum(10);

		if(QueryPerformanceCounter(&__impl__->m_Counter) != 0){
			//Query�g�p�\
			__impl__->m_iCounterType = Impl::FPS_COUNT_QUERY;
			__impl__->m_OldLongCounter = __impl__->m_Counter.QuadPart;
			LARGE_INTEGER freq;
			QueryPerformanceFrequency(&freq);
			__impl__->m_fFrequency = (double)freq.QuadPart;
		}else{
			__impl__->m_iCounterType = Impl::FPS_COUNT_TGT;
			timeBeginPeriod(1);
			__impl__->m_dwTGTOldCount = timeGetTime();
		}
	}

	/**
	*/
	DXFPSCounter::~DXFPSCounter(){
		if(__impl__->m_iCounterType == Impl::FPS_COUNT_TGT){
			timeEndPeriod(1);
		}
	}

	/**
	*@brief	FPS�̍X�V
	*@return	
	*/
	void DXFPSCounter::UpdateFPS(){
		__impl__->m_fElapsedTime = __impl__->GetElapsedTime();

		//�Â��f�[�^�̍폜
		__impl__->m_fElapsedTimeList.pop_front();
		//�V�����f�[�^�̒ǉ�
		__impl__->m_fElapsedTimeList.push_back(__impl__->m_fElapsedTime);

		__impl__->m_fSumTimes += __impl__->m_fElapsedTime - __impl__->m_fElapsedTimeList.front();

		//FPS�v�Z
		const double ave = (__impl__->m_fSumTimes + __impl__->m_fElapsedTime) / __impl__->m_iSampleNum;

		static DWORD dwPre = timeGetTime();
		const DWORD dwNow = timeGetTime();

		if(dwNow - dwPre < 500){
			return ;
		}
		dwPre = dwNow;

		if(ave != 0){
			__impl__->m_fFPS = 1000.0f / ave;
		}
	}
	/**
	*@brief		�T���v�����鐔�̐ݒ�
	*@return	
	*@param	iSampleNum	�T���v����
	*/
	void DXFPSCounter::SetSampleNum(long iSampleNum){
		__impl__->m_iSampleNum = iSampleNum;
		__impl__->m_fElapsedTimeList.resize(__impl__->m_iSampleNum, 0.0f);
		__impl__->m_fSumTimes = 0;
	}

	/**
	*@brief
	*@return
	*/
	double DXFPSCounter::GetAbsoluteTime(){
		LARGE_INTEGER qwTime = {0};
		QueryPerformanceCounter(&qwTime);

		return qwTime.QuadPart / __impl__->m_fFrequency;
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
	

	double	DXFPSCounter::GetElapse()	const{return __impl__->m_fElapsedTime;}
	double	DXFPSCounter::GetFPS()	const{return __impl__->m_fFPS;}
	double	DXFPSCounter::GetTime()	const{return __impl__->m_fTimer;}
};