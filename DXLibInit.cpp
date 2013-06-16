#include "DXStdafx.h"

#include "DXLibInit.h"

#include "DXManager.h"
#include "DXInput.h"
#include "DXWindow.h"
#include "DXFPSCounter.h"
namespace DXLib{
	struct DXApp::Impl{
		std::shared_ptr<DXWindow>       m_pWindow;          //!<	ウィンドウクラスへのポインタ
		std::function<bool(void)>       m_onFrameUpdate;    //!<	更新用関数ポインタ
		std::function<void(void)>       m_onFrameDraw;      //!<	描画用関数ポインタ
		std::shared_ptr<DXFPSCounter>   m_pFPSCounter;      //!<	FPS測定クラス
		std::shared_ptr<DXInput>        m_pDirectInput;     //!<	DirectInput管理クラス
		std::shared_ptr<DXManager>      m_pDirectManager;   //!<	DirectX管理クラス
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
		//コモンコントロール
		/*
		INITCOMMONCONTROLSEX ic;
		ic.dwSize = sizeof(INITCOMMONCONTROLSEX);
		ic.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_UPDOWN_CLASS;
		InitCommonControlsEx(&ic);
		*/
		if(!__impl__->m_pWindow->Init(hInst, w, h, bWindowed)){
			//ウィンドウの生成
			ERROR_MSG(__T("DXWindow::Init"));
			return false;
		}
		//DirectXの初期化
		__impl__->m_pDirectManager = GetSingleton<DXManager>();
		if(!__impl__->m_pDirectManager->Setup(__impl__->m_pWindow->getHWND(), w, h, nCmdShow)){
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
		__impl__->m_pDirectInput = GetSingleton<DXInput>();
		if(!__impl__->m_pDirectInput->Setup(hInst,__impl__->m_pWindow->getHWND())){
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
			if(__impl__->m_pWindow->executeMessage(&bEndApp)){
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
		__impl__->m_pFPSCounter->UpdateFPS();

		//入力の更新
		__impl__->m_pDirectInput->Update();

		if(!__impl__->m_onFrameUpdate()){
			//更新
			return false;
		}
		//描画開始宣言
		if(!__impl__->m_pDirectManager->beginScene()){
			ERROR_MSG(__T("DXManager::beginScene"));
			return false;
		}

		//描画
		__impl__->m_onFrameDraw();

		//描画終了宣言
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