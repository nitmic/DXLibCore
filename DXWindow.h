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
	HWND					m_hWnd;			//!<	ウィンドウハンドル
	TCHAR					m_IconName[MAXSIZE];//!<	アイコンの名前
	TCHAR					m_AppName[MAXSIZE];	//!<	アプリケーションの名前
	MSG						m_Msg;
	HACCEL					m_hAccel;		//!<	アクセラレータ

	bool (CALLBACK *m_pProcFunc)(HWND, UINT, WPARAM, LPARAM);	//!<	プロシージャ関数.falseを返すとデフォルトの処理を行う
};

