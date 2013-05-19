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
	//�L�[�{�[�h�̏�����
	if(!GetSingleton<DXKeyboard>()->Setup(hWnd, m_pInput)){
		return false;
	}
	//�W���C�p�b�h�̏�����
	if(!GetSingleton<DXJoypad>()->Setup(hWnd, m_pInput)){
		return false;
	}
	//�}�E�X�̏�����
	if(!GetSingleton<DXMouse>()->Setup(hWnd, m_pInput)){
		return false;
	}
	m_pMouseDevice = GetSingleton<DXMouse>();
	m_pKeyDevice = GetSingleton<DXKeyboard>();
	m_pJoyPadDevice = GetSingleton<DXJoypad>();
	return true;
}


void DXInput::Update(){
	//�}�E�X�̍X�V
	m_pMouseDevice->Update();
	//�L�[�{�[�h�̍X�V
	m_pKeyDevice->Update();
	//�W���C�p�b�h�̍X�V
	m_pJoyPadDevice->Update();
}