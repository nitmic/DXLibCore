#include "DXStdafx.h"
#include "DXKeyboard.h"
#include <Binary.h>

namespace DXLib{
	struct DXKeyboard::Impl{
		std::shared_ptr<DXPrimitiveInputDevice> m_pDeviceWrapped;
		IDirectInputDevice8 * m_pDevice;
		unsigned char			m_KeyboardState[2][256];		//!<	キーボードの状態
		Binary					m_iStateIndex;		//!<	状態のインデックス
	};

	DXKeyboard::DXKeyboard(){
		__impl__ = std::make_shared<Impl>();
		__impl__->m_pDevice = nullptr;
		memset(__impl__->m_KeyboardState, 0, sizeof(unsigned char) * 2 * 256);
	}
	DXKeyboard::~DXKeyboard(){}

	bool DXKeyboard::Setup(
		HWND & hWnd,
		std::shared_ptr<DXPrimitiveInput> & pInput
	){
		//デバイスの生成
		__impl__->m_pDeviceWrapped = DXPrimitiveInputDevice::Create(GUID_SysKeyboard, pInput);
		if(!__impl__->m_pDeviceWrapped){
			return false;
		}
		__impl__->m_pDevice = __impl__->m_pDeviceWrapped->getDelegateObject();

		//デバイスをキーボードに設定
		if(FAILED(__impl__->m_pDevice->SetDataFormat(&c_dfDIKeyboard))){
			return false;
		}
		//協調レベルの設定
		if(FAILED(__impl__->m_pDevice->SetCooperativeLevel(
			  hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND
		))){
			return false;
		}
		//デバイスの取得
		__impl__->m_pDevice->Acquire();

		return true;
	}

	void DXKeyboard::Update(){
		__impl__->m_iStateIndex.reverse();
		__impl__->m_pDevice->GetDeviceState(sizeof(__impl__->m_KeyboardState[__impl__->m_iStateIndex]), &__impl__->m_KeyboardState[__impl__->m_iStateIndex]);

		HRESULT hr;
		do{
			hr = __impl__->m_pDevice->Acquire();
		}while(hr == DIERR_INPUTLOST);
	}

	bool DXKeyboard::isJustPressed(unsigned char uDikCode){
		if(
			__impl__->m_KeyboardState[__impl__->m_iStateIndex][uDikCode] & 0x80 &&
			((__impl__->m_KeyboardState[1 - __impl__->m_iStateIndex][uDikCode] & 0x80) == 0)
		){
			return true;
		}
		return false;
	}

	bool DXKeyboard::isJustPulled(unsigned char uDikCode){
		if(
			((__impl__->m_KeyboardState[__impl__->m_iStateIndex][uDikCode] & 0x80) == 0) &&
			__impl__->m_KeyboardState[1 - __impl__->m_iStateIndex][uDikCode] & 0x80
		){
			return true;
		}
		return false;
	}

	bool DXKeyboard::isPressed(unsigned char uDikCode){
		if(__impl__->m_KeyboardState[__impl__->m_iStateIndex][uDikCode] & 0x80){
			return true;
		}
		return false;
	}

	bool DXKeyboard::isFree(unsigned char uDikCode){
		if((__impl__->m_KeyboardState[__impl__->m_iStateIndex][uDikCode] & 0x80) == 0){
			return true;
		}
		return false;
	}
};