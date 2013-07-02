#include "DXStdafx.h"

#include "DXInput.h"
#include "DXJoypad.h"
#include "DXMouse.h"
#include "DXKeyboard.h"

#include "DXManager.h"

namespace DXLib{
	struct DXInput::Impl{
		std::shared_ptr<DXPrimitiveInput> m_pInput;         //!<	�f�o�C�X�ւ̃|�C���^
		std::shared_ptr<DXKeyboard>       m_pKeyDevice;     //!<	�L�[�{�[�h�f�o�C�X
		std::shared_ptr<DXMouse>          m_pMouseDevice;   //!<	�}�E�X�f�o�C�X
		std::shared_ptr<DXJoypad>         m_pJoyPadDevice;  //!<	�W���C�p�b�h�f�o�C�X
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
		//�L�[�{�[�h�̏�����
		if(!GetSingleton<DXKeyboard>()->Setup(hWnd, __impl__->m_pInput)){
			return false;
		}
		//�W���C�p�b�h�̏�����
		if(!GetSingleton<DXJoypad>()->Setup(hWnd, __impl__->m_pInput)){
			return false;
		}
		//�}�E�X�̏�����
		if(!GetSingleton<DXMouse>()->Setup(hWnd, __impl__->m_pInput)){
			return false;
		}
		__impl__->m_pMouseDevice = GetSingleton<DXMouse>();
		__impl__->m_pKeyDevice = GetSingleton<DXKeyboard>();
		__impl__->m_pJoyPadDevice = GetSingleton<DXJoypad>();
		return true;
	}


	void DXInput::Update(){
		//�}�E�X�̍X�V
		__impl__->m_pMouseDevice->Update();
		//�L�[�{�[�h�̍X�V
		__impl__->m_pKeyDevice->Update();
		//�W���C�p�b�h�̍X�V
		__impl__->m_pJoyPadDevice->Update();
	}
};