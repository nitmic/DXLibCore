#pragma once
#include <Windows.h>
#include <Singleton.hpp>

namespace DXLib{
	class DXInput : public Singleton<DXInput>{
		friend Singleton<DXInput>;
	public:
		bool     Setup(HINSTANCE hInst,HWND hWnd);
		void        Update();
	
		DXInput();
		~DXInput();
	private:
		struct Impl;
		std::shared_ptr<Impl> __impl__;
	};
};

