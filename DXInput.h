#pragma once
#include "DXStdafx.h"
#include "DXJoypad.h"
#include "DXMouse.h"
#include "DXKeyboard.h"

class DXInput : public Singleton<DXInput>{
	friend Singleton<DXInput>;
private:
	std::shared_ptr<DXPrimitiveInput> m_pInput;         //!<	�f�o�C�X�ւ̃|�C���^
	std::shared_ptr<DXKeyboard>       m_pKeyDevice;     //!<	�L�[�{�[�h�f�o�C�X
	std::shared_ptr<DXMouse>          m_pMouseDevice;   //!<	�}�E�X�f�o�C�X
	std::shared_ptr<DXJoypad>         m_pJoyPadDevice;  //!<	�W���C�p�b�h�f�o�C�X
public:
	bool     Setup(HINSTANCE hInst,HWND hWnd);
	void        Update();
	
	DXInput();
	~DXInput();
};