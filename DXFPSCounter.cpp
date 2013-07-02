
#include "DXStdafx.h"
#include "DXFPSCounter.h"
#include <deque>

namespace DXLib{
	struct DXFPSCounter::Impl{
		enum State{FPS_COUNT_QUERY = 1, FPS_COUNT_TGT	 = 2};
		
		double		GetElapsedTime();

		State			m_iCounterType;		//!<	カウンタの種類
		LARGE_INTEGER	m_Counter;			//!<	カウンタ
		double			m_fFrequency;		//!<	周波数

		long long		m_OldLongCounter;	//!<	1フレーム前のカウンタ
		unsigned long	m_dwTGTOldCount;	//!<	1フレーム前のカウンタ

		double			m_fFPS;				//!<	FPSの値
		double			m_fElapsedTime;		//!<	差分時刻
		std::deque<double> m_fElapsedTimeList;	//!<	FPSリスト
		long			m_iSampleNum;		//!<	サンプルする数
		double			m_fSumTimes;		//!<	サンプルの合計
		double			m_fTimer;			//!<	アプリケーションタイマー
		long long		m_llBaseTime;		//!<	ベース時間	
	};
	
	/**
	*@brief		現在の差分時刻を取得
	*@return	差分時刻
	*/
	double DXFPSCounter::Impl::GetElapsedTime(){
		if(m_iCounterType == Impl::FPS_COUNT_QUERY){
			QueryPerformanceCounter(&m_Counter);
			LONGLONG longDef = m_Counter.QuadPart - m_OldLongCounter;
			double def = (double)longDef;

			m_OldLongCounter = m_Counter.QuadPart;
			//アプリケーション時間
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
			//Query使用可能
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
	*@brief	FPSの更新
	*@return	
	*/
	void DXFPSCounter::UpdateFPS(){
		__impl__->m_fElapsedTime = __impl__->GetElapsedTime();

		//古いデータの削除
		__impl__->m_fElapsedTimeList.pop_front();
		//新しいデータの追加
		__impl__->m_fElapsedTimeList.push_back(__impl__->m_fElapsedTime);

		__impl__->m_fSumTimes += __impl__->m_fElapsedTime - __impl__->m_fElapsedTimeList.front();

		//FPS計算
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
	*@brief		サンプルする数の設定
	*@return	
	*@param	iSampleNum	サンプル数
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
	*@brief	フォーカスを失ったときの処理.
	*@return
	*/
	void DXFPSCounter::OnKillFocus(){
	}

	/**
	*@brief	フォーカスを得たときの処理
	*@return	
	*/
	void DXFPSCounter::OnSetFocus(){
	}
	

	double	DXFPSCounter::GetElapse()	const{return __impl__->m_fElapsedTime;}
	double	DXFPSCounter::GetFPS()	const{return __impl__->m_fFPS;}
	double	DXFPSCounter::GetTime()	const{return __impl__->m_fTimer;}
};