#include "DXManager.h"
#include "DXDeviceObject.h"

DXManager::DXManager(){
    m_pD3D              = nullptr;
    m_pd3dDevice        = nullptr;
    m_pd3dDeviceWrapped = nullptr;
    m_hWnd              = nullptr;
    m_bWindowed         = true;
	m_bDeviceLost		= false;
}

DXManager::~DXManager(){}

bool DXManager::Setup(
	HWND hwnd,
	unsigned long width,
	unsigned long height,
	int nCmdShow
){
	m_bWindowed = true;		//情報をメンバに格納
	m_uCreationWidth = width;
	m_uCreationHeight = height;
	
    m_pD3D = DXPrimitiveCore::Create();
    if(m_pD3D == nullptr){
		return false;
	}

	//以下P31参照
	m_pd3dpp = std::make_shared<D3DPRESENT_PARAMETERS>();

	m_hWnd = hwnd;

	m_pd3dDeviceWrapped = DXPrimitiveDevice::Create(m_pd3dpp, m_hWnd, m_pD3D);
	if(m_pd3dDeviceWrapped == nullptr){
		return false;
	}
	m_pd3dDevice = **m_pd3dDeviceWrapped;

	InitRenderStage();

	DXDeviceObject::m_spD3DDevice = m_pd3dDeviceWrapped;
	DXDeviceObject::m_spD3DPP = m_pd3dpp;

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
	m_pd3dDevice->Clear(0,NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		0xff000066, 1.0f, 0);

	m_pd3dDevice->BeginScene();

	return true;
}
bool DXManager::endScene(){
	m_pd3dDevice->EndScene();

	HRESULT hr;
	hr = m_pd3dDevice->Present(NULL, NULL, NULL, NULL);

	switch(hr){
	case D3DERR_DEVICELOST:
		m_bDeviceLost = true;
		break;
	case D3DERR_DRIVERINTERNALERROR:
		return false;
	case D3DERR_INVALIDCALL:
		return false;
	}

	return true;
}

VOID DXManager::InitRenderStage(){
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
