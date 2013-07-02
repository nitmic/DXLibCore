#include "DXStdafx.h"
#include "DXMouse.h"
//#include <Binary.h>
#include <RingBuffer.hpp>

namespace DXLib{
	struct DXMouse::Impl{
		Impl(DIMOUSESTATE2 & inited) : m_MouseState(inited){};
		std::shared_ptr<DXPrimitiveInputDevice> m_pDeviceWrapped;
		IDirectInputDevice8 * m_pDevice;
		TUL::RingBuffer<DIMOUSESTATE2,2>    m_MouseState;    //!<	マウスの状態
	};


	DXMouse::DXMouse(){
		DIMOUSESTATE2 state;
		memset(&state, 0, sizeof(DIMOUSESTATE2));
		__impl__ = std::make_shared<Impl>(state);
		__impl__->m_pDevice = nullptr;
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
		HRESULT hr;
		__impl__->m_MouseState.update();
		__impl__->m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &(__impl__->m_MouseState.current()));
		do{
			hr = __impl__->m_pDevice->Acquire();
		}while(hr == DIERR_INPUTLOST);
	}

	bool DXMouse::isPressed(long i){
		if(__impl__->m_MouseState.current().rgbButtons[i] & 0x80){
			return true;
		}
		return false;
	}

	bool DXMouse::isFree(long i){
		if((__impl__->m_MouseState.current().rgbButtons[i] & 0x80) == 0){
			return true;
		}
		return false;
	}

	bool DXMouse::isJustPressed(long i){
		if(
			__impl__->m_MouseState.current().rgbButtons[i] & 0x80 &&
			(__impl__->m_MouseState.prev().rgbButtons[i] & 0x80) == 0
		){
			return true;
		}
		return false;
	}

	bool DXMouse::isJustPulled(long i)
	{
		if(
			(__impl__->m_MouseState.current().rgbButtons[i] & 0x80) == 0 &&
			(__impl__->m_MouseState.current().rgbButtons[i] & 0x80)
		){
			return true;
		}
		return false;
	}

};