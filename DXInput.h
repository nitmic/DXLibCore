#pragma once
#include "DXStdafx.h"
#include "DXJoypad.h"
#include "DXMouse.h"
#include "DXKeyboard.h"

class DXInput : public Singleton<DXInput>{
	friend Singleton<DXInput>;
private:
	std::shared_ptr<DXPrimitiveInput> m_pInput;         //!<	デバイスへのポインタ
	std::shared_ptr<DXKeyboard>       m_pKeyDevice;     //!<	キーボードデバイス
	std::shared_ptr<DXMouse>          m_pMouseDevice;   //!<	マウスデバイス
	std::shared_ptr<DXJoypad>         m_pJoyPadDevice;  //!<	ジョイパッドデバイス
public:
	bool     Setup(HINSTANCE hInst,HWND hWnd);
	void        Update();
	
	DXInput();
	~DXInput();
};