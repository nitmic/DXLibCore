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
	//コモンコントロール
	/*
	INITCOMMONCONTROLSEX ic;
	ic.dwSize = sizeof(INITCOMMONCONTROLSEX);
	ic.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_UPDOWN_CLASS;
	InitCommonControlsEx(&ic);
	*/
	if(!m_pWindow->Init(hInst, w, h, bWindowed)){
		//ウィンドウの生成
		ERROR_MSG(__T("DXWindow::Init"));
		return false;
	}
	//DirectXの初期化
	m_pDirectManager = GetSingleton<DXManager>();
	if(!m_pDirectManager->Setup(m_pWindow->getHWND(), w, h, nCmdShow)){
		ERROR_MSG(__T("DXManager::Setup"));
		return false;
	}

	/*
	//DirectSound初期化
	if(FAILED(m_DirectSound.Setup(m_pWindow->GetHWND(), DSSCL_PRIORITY)))
	{
		DX_ERROR_MSG(__T("CNcDXDirectSound::Seutp"));
		return false;
	}
	*/
	//DirectInputの初期化
	m_pDirectInput = GetSingleton<DXInput>();
	if(!m_pDirectInput->Setup(hInst,m_pWindow->getHWND())){
		ERROR_MSG(__T("DXInput::Setup"));
		return false;
	}

	//デバッグ用フォント
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
		}else{	//メッセージが無いときの処理
			if(AppIdle() == false){
				break;
			}
		}
	}
}
bool DXApp::AppIdle(){
	//FPSの更新
	m_pFPSCounter->UpdateFPS();

	//入力の更新
	m_pDirectInput->Update();

	if(!m_onFrameUpdate()){
		//更新
		return false;
	}
	//描画開始宣言
	if(!m_pDirectManager->beginScene()){
		ERROR_MSG(__T("DXManager::beginScene"));
		return false;
	}

	//描画
	m_onFrameDraw();

	//描画終了宣言
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
