#include "DXPrimitive.h"

namespace DXLib{
	std::shared_ptr<DXPrimitiveCore> DXPrimitiveCore::Create(){
		//以下P30参照
		//Direct3Dオブジェクトの生成
		auto pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		if(pD3D == nullptr){
			return nullptr;
		}
		auto shared_pD3D = std::shared_ptr<IDirect3D9>(pD3D, SAFE_RELEASE<IDirect3D9>);

		auto self = std::shared_ptr<DXPrimitiveCore>(new DXPrimitiveCore, ReleaseDelegater<DXPrimitiveCore>);
		self->setDelegateObject(shared_pD3D);
		return self;
	}

	std::shared_ptr<DXPrimitiveDevice> DXPrimitiveDevice::Create(
		std::shared_ptr<D3DPRESENT_PARAMETERS> & pd3dpp,
		HWND & hWnd,
		std::shared_ptr<DXPrimitiveCore> & core
	){
		auto pD3D = **core;
		// 現在のディスプレイモードを取得
		D3DDISPLAYMODE d3ddm;
		if(FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
			return nullptr;
		}
		ZeroMemory(pd3dpp.get(), sizeof(D3DPRESENT_PARAMETERS));
		pd3dpp->BackBufferCount	= 1;
		pd3dpp->BackBufferFormat	= d3ddm.Format;
		pd3dpp->MultiSampleType = D3DMULTISAMPLE_NONE;
		pd3dpp->MultiSampleQuality = 0;
		pd3dpp->SwapEffect = D3DSWAPEFFECT_DISCARD;
		pd3dpp->hDeviceWindow = hWnd;
		pd3dpp->Windowed = TRUE;//フルスクリーン表示の指定
	
		pd3dpp->EnableAutoDepthStencil = TRUE;//
		pd3dpp->AutoDepthStencilFormat = D3DFMT_D24S8;//
		pd3dpp->Flags = 0;
		pd3dpp->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		pd3dpp->PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		pd3dpp->BackBufferHeight=600;
		pd3dpp->BackBufferWidth=800;
	
		//以下P33参照
		IDirect3DDevice9 * pd3dDevice;
		if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, pd3dpp.get(), &pd3dDevice ) ) )
		if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, pd3dpp.get(), &pd3dDevice ) ) )
		if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, pd3dpp.get(), &pd3dDevice ) ) )
		if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, pd3dpp.get(), &pd3dDevice ) ) )
		{
			return nullptr;
		}
		pd3dDevice->Reset(pd3dpp.get());

		auto shared_pd3dDevice = std::shared_ptr<IDirect3DDevice9>(pd3dDevice, SAFE_RELEASE<IDirect3DDevice9>);
		auto self = std::shared_ptr<DXPrimitiveDevice>(new DXPrimitiveDevice, ReleaseDelegater<DXPrimitiveDevice>);
		self->setDelegateObject(shared_pd3dDevice);
		self->m_pCore = core;
		return self;
	}

	std::shared_ptr<DXPrimitiveInput> DXPrimitiveInput::Create(
		HINSTANCE & hInst,
		std::shared_ptr<DXPrimitiveDevice> & pDevice
	){
		//デバイスの作成
		IDirectInput8 * pInput;
		if(FAILED(DirectInput8Create(
			  hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&pInput, NULL
		))){
			return nullptr;
		}
		auto shared_pInput = std::shared_ptr<IDirectInput8>(pInput, SAFE_RELEASE<IDirectInput8>);
		auto self = std::shared_ptr<DXPrimitiveInput>(new DXPrimitiveInput, ReleaseDelegater<DXPrimitiveInput>);
		self->setDelegateObject(shared_pInput);
		self->m_pDevice = pDevice;
		return self;
	}

	std::shared_ptr<DXPrimitiveInputDevice> DXPrimitiveInputDevice::Create(
		const GUID & guid,
		std::shared_ptr<DXPrimitiveInput> & pInput
	){
		IDirectInputDevice8 * pInputDevice;
		if(FAILED((*pInput)->CreateDevice(guid, &pInputDevice, NULL))){
			return nullptr;
		}
		auto shared_pInputDevice = std::shared_ptr<IDirectInputDevice8>(pInputDevice, SAFE_RELEASE<IDirectInputDevice8>);
		auto self = std::shared_ptr<DXPrimitiveInputDevice>(new DXPrimitiveInputDevice, ReleaseDelegater<DXPrimitiveInputDevice>);
		self->setDelegateObject(shared_pInputDevice);
		self->m_pInput = pInput;
		return self;
	}

	std::shared_ptr<DXPrimitiveHLSL> DXPrimitiveHLSL::Create(
			TUL::tString name,
			std::shared_ptr<DXPrimitiveDevice> & pDevice
	){
		ID3DXBuffer * pErrorMsgs;
		ID3DXEffect * pEffect;
		HRESULT hr = D3DXCreateEffectFromFile(
			pDevice->getDelegateObject(),
			name.c_str(),
			NULL,
			NULL,
			0,
			NULL,
			&pEffect,
			&pErrorMsgs
		);
		if( FAILED( hr )){
			if(pErrorMsgs != nullptr && pErrorMsgs->GetBufferPointer() != 0 ){
				OutputDebugStringA( reinterpret_cast<char*>(pErrorMsgs->GetBufferPointer()) );
				//↑これを使うとfxファイルのコンパイルエラーを出力内に書いてくれる！すげぇ！今日まで知らんかった！
			}
			exit(-1);
		}
		auto shared_pEffect = std::shared_ptr<ID3DXEffect>(pEffect, SAFE_RELEASE<ID3DXEffect>);
		auto self = std::shared_ptr<DXPrimitiveHLSL>(new DXPrimitiveHLSL, ReleaseDelegater<DXPrimitiveHLSL>);
		self->setDelegateObject(shared_pEffect);
		self->m_pDevice = pDevice;
		return self;
	}
};