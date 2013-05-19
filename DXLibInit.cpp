#include "DXLibInit.h"
/**
*@brief	
*/
DXApp::DXApp(){
	m_pWindow = nullptr;
	m_onFrameUpdate = [](){return true;};
	m_onFrameDraw = [](){};
}

/**
*/
DXApp::~DXApp(){}

bool DXApp::Setup(HINSTANCE hInst, long w, long h, int nCmdShow){
	m_pWindow = GetSingleton<DXWindow>();
	m_pFPSCounter = GetSingleton<DXFPSCounter>();
	bool bWindowed = true;
	//�R�����R���g���[��
	/*
	INITCOMMONCONTROLSEX ic;
	ic.dwSize = sizeof(INITCOMMONCONTROLSEX);
	ic.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_UPDOWN_CLASS;
	InitCommonControlsEx(&ic);
	*/
	if(!m_pWindow->Init(hInst, w, h, bWindowed)){
		//�E�B���h�E�̐���
		ERROR_MSG(__T("DXWindow::Init"));
		return false;
	}
	//DirectX�̏�����
	m_pDirectManager = GetSingleton<DXManager>();
	if(!m_pDirectManager->Setup(m_pWindow->getHWND(), w, h, nCmdShow)){
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
	m_pDirectInput = GetSingleton<DXInput>();
	if(!m_pDirectInput->Setup(hInst,m_pWindow->getHWND())){
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
		if(m_pWindow->executeMessage(&bEndApp)){
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
	m_pFPSCounter->UpdateFPS();

	//���͂̍X�V
	m_pDirectInput->Update();

	if(!m_onFrameUpdate()){
		//�X�V
		return false;
	}
	//�`��J�n�錾
	if(!m_pDirectManager->beginScene()){
		ERROR_MSG(__T("DXManager::beginScene"));
		return false;
	}

	//�`��
	m_onFrameDraw();

	//�`��I���錾
	if(!m_pDirectManager->endScene()){
		ERROR_MSG(__T("DXManager::endScene"));
		return false;
	}

	return true;
}

void DXApp::setOnFrameUpdate(std::function<bool(void)> func){
	m_onFrameUpdate = func;
}

void DXApp::setOnFrameDraw(std::function<void(void)> func){
	m_onFrameDraw = func;
}
