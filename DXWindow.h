#pragma once
#include "DXStdafx.h"

class DXWindow : public Singleton<DXWindow>{
	friend Singleton<DXWindow>;
public:
	static LRESULT CALLBACK	WindowProc(HWND, UINT, WPARAM, LPARAM);
	void   setProc(bool (CALLBACK *pFunc)(HWND, UINT, WPARAM, LPARAM));
	bool   Init(HINSTANCE hInstance, long w, long h, bool bWindowed);

	bool	executeMessage(bool *pbEndApp);
	HWND	getHWND()	{return m_hWnd;}
	MSG		getMsg()	{return m_Msg;}
	DXWindow();
	~DXWindow();
private:
	enum{
		MAXSIZE = 256
	};
	HWND					m_hWnd;			//!<	�E�B���h�E�n���h��
	TCHAR					m_IconName[MAXSIZE];//!<	�A�C�R���̖��O
	TCHAR					m_AppName[MAXSIZE];	//!<	�A�v���P�[�V�����̖��O
	MSG						m_Msg;
	HACCEL					m_hAccel;		//!<	�A�N�Z�����[�^

	bool (CALLBACK *m_pProcFunc)(HWND, UINT, WPARAM, LPARAM);	//!<	�v���V�[�W���֐�.false��Ԃ��ƃf�t�H���g�̏������s��
};

