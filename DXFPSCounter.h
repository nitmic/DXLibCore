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

	long			m_iCounterType;		//!<	�J�E���^�̎��
	LARGE_INTEGER	m_Counter;			//!<	�J�E���^
	double			m_fFrequency;		//!<	���g��

	long long		m_OldLongCounter;	//!<	1�t���[���O�̃J�E���^
	unsigned long			m_dwTGTOldCount;	//!<	1�t���[���O�̃J�E���^

	double			m_fFPS;				//!<	FPS�̒l
	double			m_fElapsedTime;		//!<	��������
	std::list<double> m_fElapsedTimeList;	//!<	FPS���X�g
	long			m_iSampleNum;		//!<	�T���v�����鐔
	double			m_fSumTimes;		//!<	�T���v���̍��v
	double			m_fTimer;			//!<	�A�v���P�[�V�����^�C�}�[
	long long		m_llBaseTime;		//!<	�x�[�X����	
};