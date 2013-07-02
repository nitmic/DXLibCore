#pragma once
#include <Windows.h>
#include <Singleton.hpp>

namespace DXLib{
	class DXWindow : public Singleton<DXWindow>{
		friend Singleton<DXWindow>;
	public:
		static LRESULT CALLBACK	WindowProc(HWND, UINT, WPARAM, LPARAM);
		void   setProc(bool (CALLBACK *pFunc)(HWND, UINT, WPARAM, LPARAM));
		bool   Init(HINSTANCE hInstance, long w, long h, bool bWindowed);

		bool	executeMessage(bool *pbEndApp);
		HWND	getHWND();
		MSG		getMsg();
		DXWindow();
		~DXWindow();
	private:
		struct Impl;
		std::shared_ptr<Impl> __impl__;
	};
};
