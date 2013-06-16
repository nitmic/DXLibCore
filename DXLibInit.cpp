#include "DXStdafx.h"

#include "DXLibInit.h"

#include "DXManager.h"
#include "DXInput.h"
#include "DXWindow.h"
#include "DXFPSCounter.h"
namespace DXLib{
	struct DXApp::Impl{
		std::shared_ptr<DXWindow>       m_pWindow;          //!<	�E�B���h�E�N���X�ւ̃|�C���^
		std::function<bool(void)>       m_onFrameUpdate;    //!<	�X�V�p�֐��|�C���^
		std::function<void(void)>       m_onFrameDraw;      //!<	�`��p�֐��|�C���^
		std::shared_ptr<DXFPSCounter>   m_pFPSCounter;      //!<	FPS����N���X
		std::shared_ptr<DXInput>        m_pDirectInput;     //!<	DirectInput�Ǘ��N���X
		std::shared_ptr<DXManager>      m_pDirectManager;   //!<	DirectX�Ǘ��N���X
	};

	/**
	*@brief	
	*/
	DXApp::DXApp(){
		__impl__ = std::make_shared<Impl>();
		__impl__->m_onFrameUpdate = [](){return true;};
		__impl__->m_onFrameDraw = [](){};
	}

	/**
	*/
	DXApp::~DXApp(){}

	bool DXApp::Setup(HINSTANCE hInst, long w, long h, int nCmdShow){
		__impl__->m_pWindow = GetSingleton<DXWindow>();
		__impl__->m_pFPSCounter = GetSingleton<DXFPSCounter>();
		bool bWindowed = true;
		//�R�����R���g���[��
		/*
		INITCOMMONCONTROLSEX ic;
		ic.dwSize = sizeof(INITCOMMONCONTROLSEX);
		ic.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_UPDOWN_CLASS;
		InitCommonControlsEx(&ic);
		*/
		if(!__impl__->m_pWindow->Init(hInst, w, h, bWindowed)){
			//�E�B���h�E�̐���
			ERROR_MSG(__T("DXWindow::Init"));
			return false;
		}
		//DirectX�̏�����
		__impl__->m_pDirectManager = GetSingleton<DXManager>();
		if(!__impl__->m_pDirectManager->Setup(__impl__->m_pWindow->getHWND(), w, h, nCmdShow)){
			ERROR_MSG(__T("DXManager::Setup"));
			return false;
		}

		/*
		//DirectSound������
		if(FAILED(m_DirectSound.Setup(m_pWindow->GetHWND(), DSSCL_PRIORITY)))
		{
			DX_ERROR_MSG(__T("CNcDXDirectSound::Seutp"));
			return false;
		}
		*/
		//DirectInput�̏�����
		__impl__->m_pDirectInput = GetSingleton<DXInput>();
		if(!__impl__->m_pDirectInput->Setup(hInst,__impl__->m_pWindow->getHWND())){
			ERROR_MSG(__T("DXInput::Setup"));
			return false;
		}

		//�f�o�b�O�p�t�H���g
	#if	defined(_DEBUG) || defined(DEBUG)
	//	m_DebugFont.Restore();
	#endif

		if(bWindowed == false){
			ShowCursor(false);
		}

		return true;
	}

	void DXApp::AppLoop(){
		bool bEndApp;
		while(1){
			bEndApp = false;
			if(__impl__->m_pWindow->executeMessage(&bEndApp)){
				if(bEndApp){
					break;
				}
			}else{	//���b�Z�[�W�������Ƃ��̏���
				if(AppIdle() == false){
					break;
				}
			}
		}
	}
	bool DXApp::AppIdle(){
		//FPS�̍X�V
		__impl__->m_pFPSCounter->UpdateFPS();

		//���͂̍X�V
		__impl__->m_pDirectInput->Update();

		if(!__impl__->m_onFrameUpdate()){
			//�X�V
			return false;
		}
		//�`��J�n�錾
		if(!__impl__->m_pDirectManager->beginScene()){
			ERROR_MSG(__T("DXManager::beginScene"));
			return false;
		}

		//�`��
		__impl__->m_onFrameDraw();

		//�`��I���錾
		if(!__impl__->m_pDirectManager->endScene()){
			ERROR_MSG(__T("DXManager::endScene"));
			return false;
		}

		return true;
	}

	void DXApp::setOnFrameUpdate(std::function<bool(void)> func){
		__impl__->m_onFrameUpdate = func;
	}

	void DXApp::setOnFrameDraw(std::function<void(void)> func){
		__impl__->m_onFrameDraw = func;
	}
	
	long DXApp::getExitCode()	{
		return __impl__->m_pWindow->getMsg().wParam;
	}
	std::shared_ptr<DXManager> & DXApp::getDXManager()	{
		return __impl__->m_pDirectManager;
	}
};