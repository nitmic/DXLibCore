#pragma once
#include <Windows.h>
#include <functional>
#include <Singleton.hpp>

namespace DXLib{
	class DXManager;

	class DXApp : public Singleton<DXApp>{
		friend Singleton<DXApp>;
	public:
		bool Setup(HINSTANCE hInst, long w, long h, int nCmdShow);
		void AppLoop();
		bool AppIdle();
	
		void	setOnFrameUpdate(std::function<bool(void)> func);
		void	setOnFrameDraw(std::function<void(void)> func);
		
		long	getExitCode();
		std::shared_ptr<DXManager> & getDXManager();

		DXApp();
		~DXApp();
	private:
		struct Impl;
		std::shared_ptr<Impl> __impl__;
	public:
		//DXDebugFont			m_DebugFont;	//!<	デバッグ用フォント
	};
};