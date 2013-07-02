#pragma once
#include <Windows.h>
#include <Singleton.hpp>
#include <array>

struct D3DXVECTOR2;

namespace DXLib{
	class DXPrimitiveInput;

	class DXJoypad : public Singleton<DXJoypad>{
		friend Singleton<DXJoypad>;
	public:
		enum{
		  DX_PROP_MAX = 1000
		};
		enum DefaultButton{
			A = 1, B = 2, X = 0, Y = 3, L1 = 6, L2 = 4, R1 = 7, R2 = 5, Select = 8, Start = 9, Up = 12, Right = 13, Down = 14, Left = 15
		};
		DXJoypad();
		~DXJoypad();
		bool      Setup(HWND & hWnd, std::shared_ptr<DXPrimitiveInput> & pInput);
		void      Update();
		
		BYTE getButtonState(long i, DefaultButton c);
		D3DXVECTOR2 getLAnalogStickState(long i);
		D3DXVECTOR2 getRAnalogStickState(long i);

		long      getDetectJoypadCount();
	private:
		struct Impl;
		std::shared_ptr<Impl> __impl__;
	};
};