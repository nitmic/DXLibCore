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
		std::shared_ptr<DXWindow>       m_pWindow;          //!<	�E�B���h�E�N���X�ւ̃|�C���^
		std::function<bool(void)>       m_onFrameUpdate;    //!<	�X�V�p�֐��|�C���^
		std::function<void(void)>       m_onFrameDraw;      //!<	�`��p�֐��|�C���^
		DXFPSCounter                    m_FPSCounter;       //!<	FPS����N���X
		std::shared_ptr<DXInput>        m_pDirectInput;     //!<	DirectInput�Ǘ��N���X
		std::shared_ptr<DXManager>      m_pDirectManager;   //!<	DirectX�Ǘ��N���X
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
		
		//�E�B���h�E�̐���
		bool bWindowed = true;
		if(!__impl__->setupWindow(hInst, w, h, bWindowed)){
			return false;
		}

		//DirectX & �f�t�H���g��HLSL�̏�����
		if(!__impl__->setupDirectX(w, h, nCmdShow, renderingPolicy)){
			return false;
		}
		DXDeviceObject::m_spRenderingEngine = renderingPolicy->getDefaultRenderingEngine();

		//DirectInput�̏�����
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
			}else{	//���b�Z�[�W�������Ƃ��̏���
				if(AppIdle() == false){
					break;
				}
			}
		}
	}
	bool DXApp::AppIdle(){
		//FPS�̍X�V
		__impl__->m_FPSCounter.UpdateFPS();

		//���͂̍X�V
		__impl__->m_pDirectInput->Update();

		if(!__impl__->m_onFrameUpdate()){
			//�X�V
			return false;
		}
		
		//�`��J�n�錾
		if(!__impl__->m_pRenderingPolicy->beginScene()){
			ERROR_MSG(__T("RenderingPolicy::beginScene"));
			return false;
		}



		//�`��
		__impl__->m_onFrameDraw();


		//�`��I���錾
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