#include "DXWindow.h"

namespace DXLib{
	/**
	*@brief	
	*/
	DXWindow::DXWindow(){
		memset(m_IconName, 0, sizeof(TCHAR) * MAXSIZE);
		wcscpy_s(m_AppName, __T("DXDefaultAppName"));
		m_pProcFunc		= nullptr;
		m_hAccel = nullptr;
	}

	DXWindow::~DXWindow(){}

	LRESULT CALLBACK DXWindow::WindowProc(HWND hWnd, UINT msg, WPARAM w, LPARAM l){
		if(GetSingleton<DXWindow>() != nullptr){
			if(GetSingleton<DXWindow>()->m_pProcFunc != nullptr){
				if(GetSingleton<DXWindow>()->m_pProcFunc(hWnd, msg, w, l) == true){
					//登録されているプロシージャ関数を実行
					return 0;
				}
			}
		}else{	
			return 0;
	//		return DefWindowProc(hWnd, msg, w, l);
		}
	

		switch(msg){
		case WM_CREATE:		//ウィンドウが作成されたとき
			break;
		case WM_DESTROY:	//終了のメッセージ

			/*
			if(GetNcDXApp()->GetDirectManager()->GetWindowed() == false)
			{	//フルスクリーンの場合
				NcDXGetWindow()->ChangeWindowed(0, 0, 800, 600);
			}
			*/
			Sleep(1000);
			PostQuitMessage(0);
			break;
		case WM_CLOSE:		//閉じるのボタン
			DestroyWindow(hWnd);
			break;

	#if	defined(DEBUG)	|| defined(_DEBUG)
		case WM_KEYDOWN:		//キーのボタンが押されたとき
			if(w == VK_ESCAPE){
				SendMessage(hWnd, WM_DESTROY, 0, 0);
			}
			break;
	#endif
		default:
			return DefWindowProc(hWnd, msg, w, l);
			break;
		}
		return 0;
	}

	bool DXWindow::executeMessage(bool *pbEndApp){
		if(PeekMessage(&m_Msg, NULL, 0, 0, PM_REMOVE)){
			if(m_Msg.message == WM_QUIT){
				*pbEndApp = true;
			}

			if(m_hAccel != nullptr){
				if (!TranslateAccelerator(m_hWnd , m_hAccel , &m_Msg)) {
					TranslateMessage(&m_Msg);
					DispatchMessage(&m_Msg);
				}
			}else{
				TranslateMessage(&m_Msg);
				DispatchMessage(&m_Msg);
			}
			return true;
		}
		return false;
	}
	void DXWindow::setProc(bool (CALLBACK *pFunc)(HWND, UINT, WPARAM, LPARAM)){
		m_pProcFunc = pFunc;
	}

	bool DXWindow::Init(HINSTANCE hInstance, long w, long h, bool bWindowed){
		//ウィンドウの幅と高さを計算
		long iWndWidth  = w + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
		long iWndHeight = h + GetSystemMetrics(SM_CXFIXEDFRAME) * 2 +
			GetSystemMetrics(SM_CYCAPTION);

		//以下P25~27参照
		//ウィンドウの作成
		WNDCLASSEX wcex ={sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WindowProc, 0, 0, hInstance, NULL, NULL,
			(HBRUSH)(COLOR_WINDOW+1), NULL, m_AppName, NULL}; //P26参照


		//P26参照
		//ウィンドウクラスの登録
		if(!RegisterClassEx(&wcex)){
			return false;
		}
	
		m_hWnd = CreateWindowEx(
			0,
			m_AppName,	//アプリケーションの名前
			__T(""),	//ウィンドウのタイトル
			WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
			GetSystemMetrics(SM_CXSCREEN) / 2 - (iWndWidth) / 2,		//画面の中央に
			GetSystemMetrics(SM_CYSCREEN) / 2 - (iWndHeight) / 2,		//画面の中央に
			iWndWidth,		//ウィンドウの幅
			iWndHeight,		//ウィンドウの高さ
			0, 0,
			hInstance, 0
		);
	
		if(!m_hWnd){ //P27参照
			return false;
		}
		//WM_PAINTを無効に
		ValidateRect(m_hWnd, 0);

		return true;
	}
};