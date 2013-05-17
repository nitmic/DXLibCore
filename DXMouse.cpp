#include "DXMouse.h"

DXMouse::DXMouse(){
	m_pDevice = nullptr;
	m_iStateIndex = 0;
	std::for_each(m_MouseState.begin(),m_MouseState.end(),[](DIMOUSESTATE2 & state){
		memset(&state, 0, sizeof(DIMOUSESTATE2));
	});
}
DXMouse::~DXMouse(){}

bool DXMouse::Setup(
	HWND & hWnd,
	std::shared_ptr<DXPrimitiveInput> & pInput
){
	//デバイスの作成
	m_pDevice = DXPrimitiveInputDevice::Create(GUID_SysMouse, pInput);
	if(!m_pDevice){
		return false;
	}
	//デバイスをマウスに設定
	if(FAILED((*m_pDevice)->SetDataFormat(&c_dfDIMouse2))){
		return false;
	}
	//協調レベルの設定
	if(FAILED((*m_pDevice)->SetCooperativeLevel(
		  hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND
	))){
		return false;
	}
	//制御開始
	(*m_pDevice)->Acquire();
	return true;
}

void DXMouse::Update(){
	m_iStateIndex = 1 - m_iStateIndex;
	HRESULT hr;
	(*m_pDevice)->GetDeviceState(sizeof(DIMOUSESTATE2), &m_MouseState[m_iStateIndex]);
	do{
		hr = (*m_pDevice)->Acquire();
	}while(hr == DIERR_INPUTLOST);
}

bool DXMouse::isPressed(long i){
	if(m_MouseState[m_iStateIndex].rgbButtons[i] & 0x80){
		return true;
	}
	return false;
}

bool DXMouse::isFree(long i){
	if((m_MouseState[m_iStateIndex].rgbButtons[i] & 0x80) == 0){
		return true;
	}
	return false;
}

bool DXMouse::isJustPressed(long i){
	if(
		m_MouseState[m_iStateIndex].rgbButtons[i] & 0x80 &&
		(m_MouseState[1 - m_iStateIndex].rgbButtons[i] & 0x80) == 0
	){
		return true;
	}
	return false;
}

bool DXMouse::isJustPulled(long i)
{
	if(
		(m_MouseState[m_iStateIndex].rgbButtons[i] & 0x80) == 0 &&
		(m_MouseState[m_iStateIndex].rgbButtons[i] & 0x80)
	){
		return true;
	}
	return false;
}

