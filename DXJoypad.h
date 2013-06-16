#pragma once
#include <Windows.h>
#include <Singleton.hpp>
#include <Binary.h>

namespace DXLib{
	class DXPrimitiveInput;

	class DXJoypad : public Singleton<DXJoypad>{
		friend Singleton<DXJoypad>;
	public:
		DXJoypad();
		~DXJoypad();
		bool   Setup(HWND & hWnd, std::shared_ptr<DXPrimitiveInput> & pInput);
		void      Update();

		bool      isJustPressed(long iPadID, long i);
		bool      isJustPull(long iPadID, long i);
		bool      isPressed(long iPadID, long i);
		bool      isFree(long iPadID, long i);
		DIJOYSTATE2 *	getState(long i);
		DIJOYSTATE2 *	getPreState(long i);
	
		Binary      getStateIndex() const;
		long      getDetectJoypadCount();
	private:
		struct Impl;
		std::shared_ptr<Impl> __impl__;
	};

};