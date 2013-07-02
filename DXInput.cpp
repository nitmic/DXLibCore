#include "DXStdafx.h"

#include "DXInput.h"
#include "DXJoypad.h"
#include "DXMouse.h"
#include "DXKeyboard.h"

#include "DXManager.h"

namespace DXLib{
	struct DXInput::Impl{
		std::shared_ptr<DXPrimitiveInput> m_pInput;         //!<	デバイスへのポインタ
		std::shared_ptr<DXKeyboard>       m_pKeyDevice;     //!<	キーボードデバイス
		std::shared_ptr<DXMouse>          m_pMouseDevice;   //!<	マウスデバイス
		std::shared_ptr<DXJoypad>         m_pJoyPadDevice;  //!<	ジョイパッドデバイス
	};

	DXInput::DXInput(){
		__impl__ = std::make_shared<Impl>();
		/*
		CNcDXCommand::m_spDInput = this;
		*/
	}

	/**
	*/
	DXInput::~DXInput(){
		//CNcDXCommand::m_spDInput = NULL;
	}

	bool DXInput::Setup(HINSTANCE hInst,HWND hWnd){
		__impl__->m_pInput = DXPrimitiveInput::Create(hInst, GetSingleton<DXManager>()->getDevice());
		//キーボードの初期化
		if(!GetSingleton<DXKeyboard>()->Setup(hWnd, __impl__->m_pInput)){
			return false;
		}
		//ジョイパッドの初期化
		if(!GetSingleton<DXJoypad>()->Setup(hWnd, __impl__->m_pInput)){
			return false;
		}
		//マウスの初期化
		if(!GetSingleton<DXMouse>()->Setup(hWnd, __impl__->m_pInput)){
			return false;
		}
		__impl__->m_pMouseDevice = GetSingleton<DXMouse>();
		__impl__->m_pKeyDevice = GetSingleton<DXKeyboard>();
		__impl__->m_pJoyPadDevice = GetSingleton<DXJoypad>();
		return true;
	}


	void DXInput::Update(){
		//マウスの更新
		__impl__->m_pMouseDevice->Update();
		//キーボードの更新
		__impl__->m_pKeyDevice->Update();
		//ジョイパッドの更新
		__impl__->m_pJoyPadDevice->Update();
	}
};