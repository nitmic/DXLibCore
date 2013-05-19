#pragma once
#include "DXStdafx.h"

class DXJoypad : public Singleton<DXJoypad>{
	friend Singleton<DXJoypad>;
private:
	enum{
	  DX_PROP_MIN = 0,
      DX_PROP_MAX = 10000
	};
	enum{JOYPAD_MAX		= 4};

	std::array<std::shared_ptr<DXPrimitiveInputDevice>,JOYPAD_MAX>    m_pDeviceWrapped;
	std::array<IDirectInputDevice8 *,JOYPAD_MAX> m_pDevice;

	std::shared_ptr<DXPrimitiveInput> m_pInputWrapped;    //!< デバイス確保に必要
	IDirectInput8 * m_pInput;

	long          m_iDetectJoypadCount;            //!<	検出したジョイパッドの数
	long          m_iSettingJoypad;	               //!<	設定しているジョイパッド
	DIJOYSTATE2   m_JoypadState[JOYPAD_MAX][2];    //!<	ジョイパッドの状態
	long          m_iStateIndex;                   //!<	状態のインデックス
public:
	DXJoypad();
	~DXJoypad();
	bool   Setup(HWND & hWnd, std::shared_ptr<DXPrimitiveInput> & pInput);
	void      Update();

	bool      isJustPressed(long iPadID, long i);
	bool      isJustPull(long iPadID, long i);
	bool      isPressed(long iPadID, long i);
	bool      isFree(long iPadID, long i);
	DIJOYSTATE2 *	getState(long i)	{return &m_JoypadState[i][m_iStateIndex];}
	DIJOYSTATE2 *	getPreState(long i){return &m_JoypadState[i][1 - m_iStateIndex];}
	
	long      getStateIndex() const  {return m_iStateIndex;}
	long      getDetectJoypadCount() {return m_iDetectJoypadCount;}
private:	
	static BOOL CALLBACK	GetCallback(DIDEVICEINSTANCE *pDidIns, void *pThis);
	static BOOL CALLBACK	EnumObjectCallback(DIDEVICEOBJECTINSTANCE *pDidIns, void *pThis);

};