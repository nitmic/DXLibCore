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
	
	void	setOnFrameUpdate(std::function<bool(void)> func);
	void	setOnFrameDraw(std::function<void(void)> func);

	long	getExitCode()	{return m_pWindow->getMsg().wParam;}
	std::shared_ptr<DXManager> &	getDXManager()	{return m_pDirectManager;}

	DXApp();
	~DXApp();
private:
	std::shared_ptr<DXWindow>       m_pWindow;          //!<	ウィンドウクラスへのポインタ
	std::function<bool(void)>       m_onFrameUpdate;    //!<	更新用関数ポインタ
	std::function<void(void)>       m_onFrameDraw;      //!<	描画用関数ポインタ
	std::shared_ptr<DXFPSCounter>   m_pFPSCounter;      //!<	FPS測定クラス
	std::shared_ptr<DXInput>        m_pDirectInput;     //!<	DirectInput管理クラス
	std::shared_ptr<DXManager>      m_pDirectManager;   //!<	DirectX管理クラス
public:
	//DXDebugFont			m_DebugFont;	//!<	デバッグ用フォント
};

