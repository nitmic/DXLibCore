#pragma once
#include "DXStdafx.h"

namespace DXLib{
	class DXKeyboard : public Singleton<DXKeyboard>{
		friend Singleton<DXKeyboard>;
	private:
		std::shared_ptr<DXPrimitiveInputDevice> m_pDeviceWrapped;
		IDirectInputDevice8 * m_pDevice;
		unsigned char			m_KeyboardState[2][256];		//!<	�L�[�{�[�h�̏��
		long					m_iStateIndex;		//!<	��Ԃ̃C���f�b�N�X
	public:
		DXKeyboard();
		~DXKeyboard();
		bool        Setup(HWND & hWnd, std::shared_ptr<DXPrimitiveInput> & pInput);
		void        Update();
		bool		isJustPressed(unsigned char uDikCode);
		bool		isJustPulled(unsigned char uDikCode);
		bool		isPressed(unsigned char uDikCode);
		bool		isFree(unsigned char uDikCode);
	};
};
