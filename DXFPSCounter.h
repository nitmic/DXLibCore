#pragma once
#include "DXStdafx.h"

class DXFPSCounter : public Singleton<DXFPSCounter>{
	friend Singleton<DXFPSCounter>;
public:
	void	UpdateFPS();
	void	SetSampleNum(long iSampleNum);

	void	OnKillFocus();
	void	OnSetFocus();
	double	GetAbsoluteTime();

	double	GetElapse()	const{return m_fElapsedTime;}
	double	GetFPS()	const{return m_fFPS;}
	double	GetTime()	const{return m_fTimer;}

	DXFPSCounter();
	~DXFPSCounter();
protected:
	double		GetElapsedTime();
protected:
	enum{FPS_COUNT_QUERY = 1};
	enum{FPS_COUNT_TGT	 = 2};

	long			m_iCounterType;		//!<	カウンタの種類
	LARGE_INTEGER	m_Counter;			//!<	カウンタ
	double			m_fFrequency;		//!<	周波数

	long long		m_OldLongCounter;	//!<	1フレーム前のカウンタ
	unsigned long			m_dwTGTOldCount;	//!<	1フレーム前のカウンタ

	double			m_fFPS;				//!<	FPSの値
	double			m_fElapsedTime;		//!<	差分時刻
	std::list<double> m_fElapsedTimeList;	//!<	FPSリスト
	long			m_iSampleNum;		//!<	サンプルする数
	double			m_fSumTimes;		//!<	サンプルの合計
	double			m_fTimer;			//!<	アプリケーションタイマー
	long long		m_llBaseTime;		//!<	ベース時間	
};