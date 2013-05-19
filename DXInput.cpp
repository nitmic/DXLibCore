#include "DXInput.h"
#include "DXManager.h"
DXInput::DXInput(){
	m_pInput		= nullptr;
	m_pKeyDevice	= nullptr;
	m_pMouseDevice	= nullptr;
	m_pJoyPadDevice = nullptr;
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
	m_pInput = DXPrimitiveInput::Create(hInst, GetSingleton<DXManager>()->getDevice());
	//キーボードの初期化
	if(!GetSingleton<DXKeyboard>()->Setup(hWnd, m_pInput)){
		return false;
	}
	//ジョイパッドの初期化
	if(!GetSingleton<DXJoypad>()->Setup(hWnd, m_pInput)){
		return false;
	}
	//マウスの初期化
	if(!GetSingleton<DXMouse>()->Setup(hWnd, m_pInput)){
		return false;
	}
	m_pMouseDevice = GetSingleton<DXMouse>();
	m_pKeyDevice = GetSingleton<DXKeyboard>();
	m_pJoyPadDevice = GetSingleton<DXJoypad>();
	return true;
}


void DXInput::Update(){
	//マウスの更新
	m_pMouseDevice->Update();
	//キーボードの更新
	m_pKeyDevice->Update();
	//ジョイパッドの更新
	m_pJoyPadDevice->Update();
}