#include "DXKeyboard.h"

namespace DXLib{
	DXKeyboard::DXKeyboard(){
		m_pDevice = nullptr;
		m_pDeviceWrapped = nullptr;
		m_iStateIndex			= 0;
		memset(m_KeyboardState, 0, sizeof(unsigned char) * 2 * 256);
	}
	DXKeyboard::~DXKeyboard(){}

	bool DXKeyboard::Setup(
		HWND & hWnd,
		std::shared_ptr<DXPrimitiveInput> & pInput
	){
		//デバイスの生成
		m_pDeviceWrapped = DXPrimitiveInputDevice::Create(GUID_SysKeyboard, pInput);
		if(!m_pDeviceWrapped){
			return false;
		}
		m_pDevice = **m_pDeviceWrapped;

		//デバイスをキーボードに設定
		if(FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard))){
			return false;
		}
		//協調レベルの設定
		if(FAILED(m_pDevice->SetCooperativeLevel(
			  hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND
		))){
			return false;
		}
		//デバイスの取得
		m_pDevice->Acquire();

		return true;
	}

	void DXKeyboard::Update(){
		m_iStateIndex = 1 - m_iStateIndex;
		m_pDevice->GetDeviceState(sizeof(m_KeyboardState[m_iStateIndex]), &m_KeyboardState[m_iStateIndex]);

		HRESULT hr;
		do{
			hr = m_pDevice->Acquire();
		}while(hr == DIERR_INPUTLOST);
	}

	bool DXKeyboard::isJustPressed(unsigned char uDikCode){
		if(
			m_KeyboardState[m_iStateIndex][uDikCode] & 0x80 &&
			((m_KeyboardState[1 - m_iStateIndex][uDikCode] & 0x80) == 0)
		){
			return true;
		}
		return false;
	}

	bool DXKeyboard::isJustPulled(unsigned char uDikCode){
		if(
			((m_KeyboardState[m_iStateIndex][uDikCode] & 0x80) == 0) &&
			m_KeyboardState[1 - m_iStateIndex][uDikCode] & 0x80
		){
			return true;
		}
		return false;
	}

	bool DXKeyboard::isPressed(unsigned char uDikCode){
		if(m_KeyboardState[m_iStateIndex][uDikCode] & 0x80){
			return true;
		}
		return false;
	}

	bool DXKeyboard::isFree(unsigned char uDikCode){
		if((m_KeyboardState[m_iStateIndex][uDikCode] & 0x80) == 0){
			return true;
		}
		return false;
	}
};