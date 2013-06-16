#include "DXStdafx.h"
#include "DXWindow.h"

namespace DXLib{
	struct DXWindow::Impl{
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


	/**
	*@brief	
	*/
	DXWindow::DXWindow(){
		__impl__ = std::make_shared<Impl>();
		memset(__impl__->m_IconName, 0, sizeof(TCHAR) * Impl::MAXSIZE);
		wcscpy_s(__impl__->m_AppName, __T("DXDefaultAppName"));
		__impl__->m_pProcFunc		= nullptr;
		__impl__->m_hAccel = nullptr;
	}

	DXWindow::~DXWindow(){}

	LRESULT CALLBACK DXWindow::WindowProc(HWND hWnd, UINT msg, WPARAM w, LPARAM l){
		if(GetSingleton<DXWindow>() != nullptr){
			if(GetSingleton<DXWindow>()->__impl__->m_pProcFunc != nullptr){
				if(GetSingleton<DXWindow>()->__impl__->m_pProcFunc(hWnd, msg, w, l) == true){
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
		if(PeekMessage(&__impl__->m_Msg, NULL, 0, 0, PM_REMOVE)){
			if(__impl__->m_Msg.message == WM_QUIT){
				*pbEndApp = true;
			}

			if(__impl__->m_hAccel != nullptr){
				if (!TranslateAccelerator(__impl__->m_hWnd , __impl__->m_hAccel , &__impl__->m_Msg)) {
					TranslateMessage(&__impl__->m_Msg);
					DispatchMessage(&__impl__->m_Msg);
				}
			}else{
				TranslateMessage(&__impl__->m_Msg);
				DispatchMessage(&__impl__->m_Msg);
			}
			return true;
		}
		return false;
	}
	void DXWindow::setProc(bool (CALLBACK *pFunc)(HWND, UINT, WPARAM, LPARAM)){
		__impl__->m_pProcFunc = pFunc;
	}

	bool DXWindow::Init(HINSTANCE hInstance, long w, long h, bool bWindowed){
		//�E�B���h�E�̕��ƍ������v�Z
		const long iWndWidth  = w + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
		const long iWndHeight = h + GetSystemMetrics(SM_CXFIXEDFRAME) * 2 +
			GetSystemMetrics(SM_CYCAPTION);

		//�ȉ�P25~27�Q��
		//�E�B���h�E�̍쐬
		WNDCLASSEX wcex ={sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WindowProc, 0, 0, hInstance, NULL, NULL,
			(HBRUSH)(COLOR_WINDOW+1), NULL, __impl__->m_AppName, NULL}; //P26�Q��


		//P26�Q��
		//�E�B���h�E�N���X�̓o�^
		if(!RegisterClassEx(&wcex)){
			return false;
		}
	
		__impl__->m_hWnd = CreateWindowEx(
			0,
			__impl__->m_AppName,	//�A�v���P�[�V�����̖��O
			__T(""),	//�E�B���h�E�̃^�C�g��
			WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
			GetSystemMetrics(SM_CXSCREEN) / 2 - (iWndWidth) / 2,		//��ʂ̒�����
			GetSystemMetrics(SM_CYSCREEN) / 2 - (iWndHeight) / 2,		//��ʂ̒�����
			iWndWidth,		//�E�B���h�E�̕�
			iWndHeight,		//�E�B���h�E�̍���
			0, 0,
			hInstance, 0
		);
	
		if(!__impl__->m_hWnd){ //P27�Q��
			return false;
		}
		//WM_PAINT�𖳌���
		ValidateRect(__impl__->m_hWnd, 0);

		return true;
	}
	
	HWND	DXWindow::getHWND()	{return __impl__->m_hWnd;}
	MSG		DXWindow::getMsg()	{return __impl__->m_Msg;}
};