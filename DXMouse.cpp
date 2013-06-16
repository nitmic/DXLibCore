#include "DXStdafx.h"
#include "DXMouse.h"
#include <Binary.h>

namespace DXLib{
	struct DXMouse::Impl{
		std::shared_ptr<DXPrimitiveInputDevice> m_pDeviceWrapped;
		IDirectInputDevice8 * m_pDevice;
		std::array<DIMOUSESTATE2,2>    m_MouseState;    //!<	マウスの状態
		Binary      m_iStateIndex;     //!<	状態のインデックス
	};


	DXMouse::DXMouse(){
		__impl__ = std::make_shared<Impl>();
		__impl__->m_pDevice = nullptr;
		std::for_each(
			__impl__->m_MouseState.begin(),
			__impl__->m_MouseState.end(),
			[](DIMOUSESTATE2 & state){ memset(&state, 0, sizeof(DIMOUSESTATE2)); }
		);
	}
	DXMouse::~DXMouse(){}

	bool DXMouse::Setup(
		HWND & hWnd,
		std::shared_ptr<DXPrimitiveInput> & pInput
	){
		//デバイスの作成
		__impl__->m_pDeviceWrapped = DXPrimitiveInputDevice::Create(GUID_SysMouse, pInput);
		if(!__impl__->m_pDeviceWrapped){
			return false;
		}
		__impl__->m_pDevice = __impl__->m_pDeviceWrapped->getDelegateObject();

		//デバイスをマウスに設定
		if(FAILED(
			__impl__->m_pDevice->SetDataFormat(&c_dfDIMouse2)
		)){
			return false;
		}
		//協調レベルの設定
		if(FAILED(
			__impl__->m_pDevice->SetCooperativeLevel(
			  hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND
			)
		)){
			return false;
		}
		//制御開始
		__impl__->m_pDevice->Acquire();
		return true;
	}

	void DXMouse::Update(){
		__impl__->m_iStateIndex.reverse();
		HRESULT hr;
		__impl__->m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &__impl__->m_MouseState[__impl__->m_iStateIndex]);
		do{
			hr = __impl__->m_pDevice->Acquire();
		}while(hr == DIERR_INPUTLOST);
	}

	bool DXMouse::isPressed(long i){
		if(__impl__->m_MouseState[__impl__->m_iStateIndex].rgbButtons[i] & 0x80){
			return true;
		}
		return false;
	}

	bool DXMouse::isFree(long i){
		if((__impl__->m_MouseState[__impl__->m_iStateIndex].rgbButtons[i] & 0x80) == 0){
			return true;
		}
		return false;
	}

	bool DXMouse::isJustPressed(long i){
		if(
			__impl__->m_MouseState[__impl__->m_iStateIndex].rgbButtons[i] & 0x80 &&
			(__impl__->m_MouseState[1 - __impl__->m_iStateIndex].rgbButtons[i] & 0x80) == 0
		){
			return true;
		}
		return false;
	}

	bool DXMouse::isJustPulled(long i)
	{
		if(
			(__impl__->m_MouseState[__impl__->m_iStateIndex].rgbButtons[i] & 0x80) == 0 &&
			(__impl__->m_MouseState[__impl__->m_iStateIndex].rgbButtons[i] & 0x80)
		){
			return true;
		}
		return false;
	}

};