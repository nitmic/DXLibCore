#pragma once
#include "DXStdafx.h"

class DXWindow : public Singleton<DXWindow>{
	friend Singleton<DXWindow>;
public:
	static LRESULT CALLBACK	WindowProc(HWND, UINT, WPARAM, LPARAM);
	void   setProc(bool (CALLBACK *pFunc)(HWND, UINT, WPARAM, LPARAM));
	bool   init(HINSTANCE hInstance, long w, long h, bool bWindowed);

	bool	ExecuteMessage(bool *pbEndApp);
	HWND	getHWND()	{return m_hWnd;}
	MSG		getMsg()	{return m_Msg;}
	DXWindow();
	~DXWindow();
private:
	HWND					m_hWnd;			//!<	�E�B���h�E�n���h��
	TCHAR					m_IconName[256];//!<	�A�C�R���̖��O
	TCHAR					m_AppName[256];	//!<	�A�v���P�[�V�����̖��O
	MSG						m_Msg;
	HACCEL					m_hAccel;		//!<	�A�N�Z�����[�^

	bool (CALLBACK *m_pProcFunc)(HWND, UINT, WPARAM, LPARAM);	//!<	�v���V�[�W���֐�.false��Ԃ��ƃf�t�H���g�̏������s��
};