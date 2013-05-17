#pragma once
#include "DXStdafx.h"
#include "DXInput.h"
#include "DXWindow.h"
#include "DXFPSCounter.h"
#include "DXManager.h"

class DXApp : public Singleton<DXApp>{
	friend Singleton<DXApp>;
public:
	bool Setup(HINSTANCE hInst, long w, long h, int nCmdShow);
	void AppLoop();
	bool AppIdle();
	
	void	SetOnFrameUpdate(std::function<bool(void)> func);
	void	SetOnFrameDraw(std::function<void(void)> func);

	long	GetExitCode()	{return m_pWindow->GetMsg().wParam;}
	std::shared_ptr<DXManager> &	GetDXManager()	{return m_pDirectManager;}

	DXApp();
	~DXApp();
private:
	std::shared_ptr<DXWindow>       m_pWindow;          //!<	�E�B���h�E�N���X�ւ̃|�C���^
	std::function<bool(void)>       m_onFrameUpdate;    //!<	�X�V�p�֐��|�C���^
	std::function<void(void)>       m_onFrameDraw;      //!<	�`��p�֐��|�C���^
	std::shared_ptr<DXFPSCounter>   m_pFPSCounter;      //!<	FPS����N���X
	std::shared_ptr<DXInput>        m_pDirectInput;     //!<	DirectInput�Ǘ��N���X
	std::shared_ptr<DXManager>      m_pDirectManager;   //!<	DirectX�Ǘ��N���X
public:
	//DXDebugFont			m_DebugFont;	//!<	�f�o�b�O�p�t�H���g
};

