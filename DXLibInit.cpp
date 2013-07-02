#include "DXStdafx.h"

#include "DXLibInit.h"

#include "DXManager.h"
#include "DXInput.h"
#include "DXWindow.h"
#include "DXFPSCounter.h"
#include "DXDeviceObject.h"
#include "DXAbsRenderingPolicy.h"

namespace DXLib{
	struct DXApp::Impl{
		std::shared_ptr<DXWindow>       m_pWindow;          //!<	ウィンドウクラスへのポインタ
		std::function<bool(void)>       m_onFrameUpdate;    //!<	更新用関数ポインタ
		std::function<void(void)>       m_onFrameDraw;      //!<	描画用関数ポインタ
		DXFPSCounter                    m_FPSCounter;       //!<	FPS測定クラス
		std::shared_ptr<DXInput>        m_pDirectInput;     //!<	DirectInput管理クラス
		std::shared_ptr<DXManager>      m_pDirectManager;   //!<	DirectX管理クラス
		std::shared_ptr<DXAbsRenderingPolicy> m_pRenderingPolicy;

		bool setupWindow(HINSTANCE hInst, long w, long h, bool bWindowed){
			m_pWindow = GetSingleton<DXWindow>();
			if(!m_pWindow->Init(hInst, w, h, bWindowed)){
				ERROR_MSG(__T("DXWindow::Init"));
				return false;
			}
			return true;
		}
		
		bool setupInput(HINSTANCE hInst){
			m_pDirectInput = GetSingleton<DXInput>();
			if(!m_pDirectInput->Setup(hInst,m_pWindow->getHWND())){
				ERROR_MSG(__T("DXInput::Setup"));
				return false;
			}
			return true;
		}

		bool setupDirectX(long w, long h, int nCmdShow, std::shared_ptr<DXAbsRenderingPolicy> renderingPolicy){
			m_pRenderingPolicy = renderingPolicy;
			if(!m_pRenderingPolicy->Setup(m_pWindow->getHWND(), w, h, nCmdShow)){
				ERROR_MSG(__T("RenderingPolicy::Setup"));
				return false;
			}
			return true;
		}


	};

	/**
	*@brief	
	*/
	DXApp::DXApp(){
		__impl__ = std::make_shared<Impl>();
		__impl__->m_onFrameUpdate = [](){return true;};
		__impl__->m_onFrameDraw = [](){};
	}

	bool DXApp::Setup(HINSTANCE hInst, long w, long h, int nCmdShow, std::shared_ptr<DXAbsRenderingPolicy> renderingPolicy){
		
		//ウィンドウの生成
		bool bWindowed = true;
		if(!__impl__->setupWindow(hInst, w, h, bWindowed)){
			return false;
		}

		//DirectX & デフォルトのHLSLの初期化
		if(!__impl__->setupDirectX(w, h, nCmdShow, renderingPolicy)){
			return false;
		}
		DXDeviceObject::m_spRenderingEngine = renderingPolicy->getDefaultRenderingEngine();

		//DirectInputの初期化
		if(!__impl__->setupInput(hInst)){
			return false;
		}

		if(bWindowed == false) ShowCursor(false);

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
		__impl__->m_FPSCounter.UpdateFPS();

		//入力の更新
		__impl__->m_pDirectInput->Update();

		if(!__impl__->m_onFrameUpdate()){
			//更新
			return false;
		}
		
		//描画開始宣言
		if(!__impl__->m_pRenderingPolicy->beginScene()){
			ERROR_MSG(__T("RenderingPolicy::beginScene"));
			return false;
		}



		//描画
		__impl__->m_onFrameDraw();


		//描画終了宣言
		if(!__impl__->m_pRenderingPolicy->endScene()){
			ERROR_MSG(__T("RenderingPolicy::endScene"));
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
};