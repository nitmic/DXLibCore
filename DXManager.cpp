#include "DXStdafx.h"

#include "DXManager.h"
#include "DXDeviceObject.h"

namespace DXLib{
	struct DXManager::Impl{
		void         InitRenderStage();				//テクスチャステージ設定

		bool       m_bWindowed;
		HWND       m_hWnd;             //!<	ウィンドウハンドル
		std::shared_ptr<D3DPRESENT_PARAMETERS> m_pd3dpp;            //!<	CreateDevice Param
		std::shared_ptr<DXPrimitiveCore>       m_pD3D;             //!<	main D3D Object
		std::shared_ptr<DXPrimitiveDevice>     m_pd3dDeviceWrapped;       //!<	レンダリングデバイス
		IDirect3DDevice9 * m_pd3dDevice;

		unsigned long       m_uCreationWidth;	//!<	ウィンドウの幅
		unsigned long       m_uCreationHeight;	//!<	ウィンドウの高さ
		bool                m_bDeviceLost;		//!<	デバイスロストしているかどうか

		//CNcDXLostManager	m_LostManager;		//!<	ロストマネージャ
		//D3DDISPLAYMODE m_d3ddm;
	};

	DXManager::DXManager(){
		__impl__ = std::make_shared<Impl>();
		__impl__->m_pd3dDevice        = nullptr;
		__impl__->m_hWnd              = nullptr;
		__impl__->m_bWindowed         = true;
		__impl__->m_bDeviceLost		= false;
	}

	DXManager::~DXManager(){}

	bool DXManager::Setup(
		HWND hwnd,
		unsigned long width,
		unsigned long height,
		int nCmdShow
	){
		__impl__->m_bWindowed = true;		//情報をメンバに格納
		__impl__->m_uCreationWidth = width;
		__impl__->m_uCreationHeight = height;
	
		__impl__->m_pD3D = DXPrimitiveCore::Create();
		if(__impl__->m_pD3D == nullptr){
			return false;
		}

		//以下P31参照
		__impl__->m_pd3dpp = std::make_shared<D3DPRESENT_PARAMETERS>();

		__impl__->m_hWnd = hwnd;

		__impl__->m_pd3dDeviceWrapped = DXPrimitiveDevice::Create(__impl__->m_pd3dpp, __impl__->m_hWnd, __impl__->m_pD3D);
		if(__impl__->m_pd3dDeviceWrapped == nullptr){
			return false;
		}
		__impl__->m_pd3dDevice = **__impl__->m_pd3dDeviceWrapped;

		__impl__->InitRenderStage();

		DXDeviceObject::m_spD3DDevice = __impl__->m_pd3dDeviceWrapped;
		DXDeviceObject::m_spD3DPP = __impl__->m_pd3dpp;

		return true;
	}


	bool DXManager::beginScene(){
		/*
		if(m_bDeviceLost == true)
		{
			HRESULT hr = m_pd3dDevice->TestCooperativeLevel();
			if(hr == D3DERR_DEVICENOTRESET)
			{
				Invalidate();	//開放

				hr = m_pd3dDevice->Reset(&m_d3dpp);

				switch(hr)
				{
				case D3DERR_DEVICELOST:
					break;	//デバイスロスト
				case D3DERR_DRIVERINTERNALERROR:
					return false;	//内部ドライバーエラー
				case D3DERR_INVALIDCALL:
					return false;
				case S_OK:
					Restore();	//初期化
					m_bDeviceLost = false;
					break;

				}
			}
		}
		*/
		__impl__->m_pd3dDevice->Clear(0,NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			0xff000066, 1.0f, 0);

		__impl__->m_pd3dDevice->BeginScene();

		return true;
	}
	bool DXManager::endScene(){
		__impl__->m_pd3dDevice->EndScene();

		HRESULT hr;
		hr = __impl__->m_pd3dDevice->Present(NULL, NULL, NULL, NULL);

		switch(hr){
		case D3DERR_DEVICELOST:
			__impl__->m_bDeviceLost = true;
			break;
		case D3DERR_DRIVERINTERNALERROR:
			return false;
		case D3DERR_INVALIDCALL:
			return false;
		}

		return true;
	}

	VOID DXManager::Impl::InitRenderStage(){
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

		m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
	std::shared_ptr<DXPrimitiveDevice> &	DXManager::getDevice(){return __impl__->m_pd3dDeviceWrapped;}
	std::shared_ptr<DXPrimitiveCore> &		DXManager::getDirect(){return __impl__->m_pD3D;}

	bool         DXManager::getWindowed(){return __impl__->m_bWindowed;}
};