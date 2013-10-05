#include "DXStdafx.h"
#include "DXKeyboard.h"
#include <array>
#include <RingBuffer.hpp>
//#include <Binary.h>

namespace DXLib{
	struct DXKeyboard::Impl{
		std::shared_ptr<DXPrimitiveInputDevice> m_pDeviceWrapped;
		IDirectInputDevice8 * m_pDevice;
		TUL::RingBuffer<std::array<unsigned char,256>,2> m_KeyboardState;		//!<	�L�[�{�[�h�̏��
		//Binary					m_iStateIndex;		//!<	��Ԃ̃C���f�b�N�X
	};

	DXKeyboard::DXKeyboard(){
		__impl__ = std::make_shared<Impl>();
		__impl__->m_pDevice = nullptr;
	}
	DXKeyboard::~DXKeyboard(){}

	bool DXKeyboard::Setup(
		HWND & hWnd,
		std::shared_ptr<DXPrimitiveInput> & pInput
	){
		//�f�o�C�X�̐���
		__impl__->m_pDeviceWrapped = DXPrimitiveInputDevice::Create(GUID_SysKeyboard, pInput);
		if(!__impl__->m_pDeviceWrapped){
			return false;
		}
		__impl__->m_pDevice = __impl__->m_pDeviceWrapped->getDelegateObject();

		//�f�o�C�X���L�[�{�[�h�ɐݒ�
		if(FAILED(__impl__->m_pDevice->SetDataFormat(&c_dfDIKeyboard))){
			return false;
		}
		//�������x���̐ݒ�
		if(FAILED(__impl__->m_pDevice->SetCooperativeLevel(
			  hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND
		))){
			return false;
		}
		//�f�o�C�X�̎擾
		__impl__->m_pDevice->Acquire();

		return true;
	}

	void DXKeyboard::Update(){
		__impl__->m_KeyboardState.update();
		__impl__->m_pDevice->GetDeviceState(256, __impl__->m_KeyboardState.current().data());

		HRESULT hr;
		do{
			hr = __impl__->m_pDevice->Acquire();
		}while(hr == DIERR_INPUTLOST);
	}

	bool DXKeyboard::isJustPressed(unsigned char uDikCode){
		if(
			__impl__->m_KeyboardState.current()[uDikCode] & 0x80 &&
			((__impl__->m_KeyboardState.prev()[uDikCode] & 0x80) == 0)
		){
			return true;
		}
		return false;
	}

	bool DXKeyboard::isJustPulled(unsigned char uDikCode){
		if(
			((__impl__->m_KeyboardState.current()[uDikCode] & 0x80) == 0) &&
			__impl__->m_KeyboardState.prev()[uDikCode] & 0x80
		){
			return true;
		}
		return false;
	}

	bool DXKeyboard::isPressed(unsigned char uDikCode){
		if(__impl__->m_KeyboardState.current()[uDikCode] & 0x80){
			return true;
		}
		return false;
	}

	bool DXKeyboard::isFree(unsigned char uDikCode){
		if((__impl__->m_KeyboardState.current()[uDikCode] & 0x80) == 0){
			return true;
		}
		return false;
	}
};