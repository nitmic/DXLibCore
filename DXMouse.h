#pragma once
#include <Windows.h>
#include <Singleton.hpp>

namespace DXLib{
	class DXPrimitiveInput;

	class DXMouse : public Singleton<DXMouse>{
		friend Singleton<DXMouse>;
	public:
		DXMouse();
		~DXMouse();
		bool   Setup(HWND & hWnd, std::shared_ptr<DXPrimitiveInput> & pInput);
		void      Update();
		bool		isJustPressed(long i);
		bool		isJustPulled(long i);
		bool		isPressed(long i);
		bool		isFree(long i);
	private:
		struct Impl;
		std::shared_ptr<Impl> __impl__;

	};
};