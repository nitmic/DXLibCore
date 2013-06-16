#pragma once
#include <Windows.h>
#include <Singleton.hpp>



namespace DXLib{
	class DXPrimitiveInput;
	
	class DXKeyboard : public Singleton<DXKeyboard>{
		friend Singleton<DXKeyboard>;
	public:
		DXKeyboard();
		~DXKeyboard();
		bool        Setup(HWND & hWnd, std::shared_ptr<DXPrimitiveInput> & pInput);
		void        Update();
		bool		isJustPressed(unsigned char uDikCode);
		bool		isJustPulled(unsigned char uDikCode);
		bool		isPressed(unsigned char uDikCode);
		bool		isFree(unsigned char uDikCode);
	private:
		struct Impl;
		std::shared_ptr<Impl> __impl__;

	};
};
