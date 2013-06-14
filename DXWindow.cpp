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
					//�o�^����Ă���v���V�[�W���֐������s
					return 0;
				}
			}
		}else{	
			return 0;
	//		return DefWindowProc(hWnd, msg, w, l);
		}
	

		switch(msg){
		case WM_CREATE:		//�E�B���h�E���쐬���ꂽ�Ƃ�
			break;
		case WM_DESTROY:	//�I���̃��b�Z�[�W

			/*
			if(GetNcDXApp()->GetDirectManager()->GetWindowed() == false)
			{	//�t���X�N���[���̏ꍇ
				NcDXGetWindow()->ChangeWindowed(0, 0, 800, 600);
			}
			*/
			Sleep(1000);
			PostQuitMessage(0);
			break;
		case WM_CLOSE:		//����̃{�^��
			DestroyWindow(hWnd);
			break;

	#if	defined(DEBUG)	|| defined(_DEBUG)
		case WM_KEYDOWN:		//�L�[�̃{�^���������ꂽ�Ƃ�
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
		//�E�B���h�E�̕��ƍ������v�Z
		long iWndWidth  = w + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
		long iWndHeight = h + GetSystemMetrics(SM_CXFIXEDFRAME) * 2 +
			GetSystemMetrics(SM_CYCAPTION);

		//�ȉ�P25~27�Q��
		//�E�B���h�E�̍쐬
		WNDCLASSEX wcex ={sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WindowProc, 0, 0, hInstance, NULL, NULL,
			(HBRUSH)(COLOR_WINDOW+1), NULL, m_AppName, NULL}; //P26�Q��


		//P26�Q��
		//�E�B���h�E�N���X�̓o�^
		if(!RegisterClassEx(&wcex)){
			return false;
		}
	
		m_hWnd = CreateWindowEx(
			0,
			m_AppName,	//�A�v���P�[�V�����̖��O
			__T(""),	//�E�B���h�E�̃^�C�g��
			WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
			GetSystemMetrics(SM_CXSCREEN) / 2 - (iWndWidth) / 2,		//��ʂ̒�����
			GetSystemMetrics(SM_CYSCREEN) / 2 - (iWndHeight) / 2,		//��ʂ̒�����
			iWndWidth,		//�E�B���h�E�̕�
			iWndHeight,		//�E�B���h�E�̍���
			0, 0,
			hInstance, 0
		);
	
		if(!m_hWnd){ //P27�Q��
			return false;
		}
		//WM_PAINT�𖳌���
		ValidateRect(m_hWnd, 0);

		return true;
	}
};