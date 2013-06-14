#pragma once
#include "DXStdafx.h"

namespace DXLib{
	class DXMouse : public Singleton<DXMouse>{
		friend Singleton<DXMouse>;
	private:
		std::shared_ptr<DXPrimitiveInputDevice> m_pDeviceWrapped;
		IDirectInputDevice8 * m_pDevice;
		std::array<DIMOUSESTATE2,2>    m_MouseState;    //!<	マウスの状態
		long      m_iStateIndex;     //!<	状態のインデックス
	public:
		DXMouse();
		~DXMouse();
		bool   Setup(HWND & hWnd, std::shared_ptr<DXPrimitiveInput> & pInput);
		void      Update();
		bool		isJustPressed(long i);
		bool		isJustPulled(long i);
		bool		isPressed(long i);
		bool		isFree(long i);
	};
};