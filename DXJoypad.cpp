#include "DXJoypad.h"

DXJoypad::DXJoypad(){
	m_pDevice.fill(nullptr);
	m_iStateIndex			= 0;
	m_iDetectJoypadCount	= 0;
	m_iSettingJoypad		= 0;
	
	memset(m_JoypadState, 0, sizeof(DIJOYSTATE2) * 2 * JOYPAD_MAX);
}

DXJoypad::~DXJoypad(){}

/**
*@brief	ジョイパッドの初期化
*@return
*@param	hWnd	ウィンドウハンドル
*/
bool DXJoypad::Setup(
	HWND & hWnd,
	std::shared_ptr<DXPrimitiveInput> & pInput
){
	m_pInputWrapped = pInput;
	m_pInput = **m_pInputWrapped;
	//ジョイパッドの列挙
	if(FAILED(m_pInput->EnumDevices(
		DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)GetCallback, (void *)this, DIEDFL_ATTACHEDONLY
	))){
		return false;
	}
	//検出できた数だけループを回す
	long iLoopMax = std::min(m_iDetectJoypadCount, (long)JOYPAD_MAX);

	for(long i = 0; i < iLoopMax; ++i){
		//フォーマットの設定
		if(FAILED(m_pDevice[i]->SetDataFormat(&c_dfDIJoystick2))){
			return false;
		}
		//協調レベルの設定
		if(FAILED(m_pDevice[i]->SetCooperativeLevel(
			hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND
		))){
			return false;
		}
		if(FAILED(m_pDevice[i]->EnumObjects(
			(LPDIENUMDEVICEOBJECTSCALLBACK)EnumObjectCallback, (VOID *)this, DIDFT_ALL
		))){
			return false;
		}
		m_iSettingJoypad++;
		//ジョイパッド制御開始
		m_pDevice[i]->Acquire();
	}
	return true;
}

/**
*@brief	ジョイパッドの取得
*@return	DIENUM_STOP	:	終了 DIENUM_CONTINUE : 継続
*@param	pDidIns
*@param	pCont	thisポインタ
*/
BOOL CALLBACK DXJoypad::GetCallback(DIDEVICEINSTANCE *pDidIns, void *pCont){
	DXJoypad *pThis = (DXJoypad *)pCont;
	//デバイスの作成
	pThis->m_pDeviceWrapped[pThis->m_iDetectJoypadCount] = DXPrimitiveInputDevice::Create(pDidIns->guidInstance, pThis->m_pInputWrapped);
	if(pThis->m_pDeviceWrapped[pThis->m_iDetectJoypadCount]){
		return DIENUM_STOP;
	}
	pThis->m_pDevice[pThis->m_iDetectJoypadCount] = **pThis->m_pDeviceWrapped[pThis->m_iDetectJoypadCount];


	//ジョイパッドの数を増加
	pThis->m_iDetectJoypadCount++;

	return DIENUM_CONTINUE;
}


/**
*@brief		ジョイパッドデバイスの列挙
*@return	DIENUM_STOP : 列挙の終了 DIENUM_CONTINUE : 継続
*@param	pDidIns	
*@param	pCont	thisポインタ
*/
BOOL CALLBACK DXJoypad::EnumObjectCallback(DIDEVICEOBJECTINSTANCE *pDidIns, void *pCont){
	DXJoypad *pThis = (DXJoypad *)pCont;

	if(pDidIns->dwType & DIDFT_AXIS){
		DIPROPRANGE	diprg;
		ZeroMemory(&diprg, sizeof(DIPROPRANGE));

		diprg.diph.dwSize		= sizeof(DIPROPRANGE);	//全体のサイズ
		diprg.diph.dwHeaderSize	= sizeof(DIPROPHEADER);	//ヘッダのサイズ
		diprg.diph.dwHow		= DIPH_BYID;
		diprg.diph.dwObj		= pDidIns->dwType;
		diprg.lMin				= DX_PROP_MIN;
		diprg.lMax				= DX_PROP_MAX;
		if(FAILED(pThis->m_pDevice[pThis->m_iSettingJoypad]->SetProperty(
			DIPROP_RANGE, &diprg.diph
		))){
			return DIENUM_STOP;
		}
	}
	return DIENUM_CONTINUE;
}



/**
*@brief	ジョイパッドの状態を更新
*@return
*/
void DXJoypad::Update(){
	m_iStateIndex = 1 - m_iStateIndex;
	long iLoopMax = std::min(m_iDetectJoypadCount, (long)JOYPAD_MAX);

	for(long i = 0; i < iLoopMax; ++i){
		HRESULT hr;
		if(FAILED(m_pDevice[i]->Poll())){
			do{
				hr = m_pDevice[i]->Acquire();
			}while(hr == DIERR_INPUTLOST);
			ZeroMemory(&m_JoypadState[i][m_iStateIndex], sizeof(m_JoypadState[i][m_iStateIndex]));
			return ;
		}
		hr = m_pDevice[i]->GetDeviceState(sizeof(DIJOYSTATE2), &m_JoypadState[i][m_iStateIndex]);
	}
}

/**
*@brief	ジョイパッドのボタンが押されているかを取得する
*@return	true : 押されている
*@param	iPadID	パッドのＩＤ
*@param	i	ボタンＩＤ
*/
bool DXJoypad::isPressed(long iPadID, long i){
	if(m_JoypadState[iPadID][m_iStateIndex].rgbButtons[i] & 0x80){
		return true;
	}
	return false;
}

/**
*@brief	ジョイパッドが今押されたかどうかを取得する
*@return	true :
*@param	iPadID	
*@param	i
*/
bool DXJoypad::isJustPressed(long iPadID, long i){
	if(
		(m_JoypadState[iPadID][m_iStateIndex].rgbButtons[i] & 0x80) &&
		((m_JoypadState[iPadID][1 - m_iStateIndex].rgbButtons[i] & 0x80) == 0)
	){
		return true;
	}
	return false;
}

/**
*@brief	ジョイパッドのボタンが離されているかどうかを取得する
*@return	true : 離されている
*@param	iPadID
*@param	i
*/
bool DXJoypad::isFree(long iPadID, long i){
	if(m_JoypadState[iPadID][m_iStateIndex].rgbButtons[i] & 0x80){
		return false;
	}

	return true;
}

/**
*@brief	ジョイパッドのボタンが今離されたかどうかを取得する
*@return	true : 今離された
*@param	iPadID
*@param	i
*/
bool DXJoypad::isJustPull(long iPadID, long i){
	if(
		((m_JoypadState[iPadID][m_iStateIndex].rgbButtons[i] & 0x80) == 0) &&
		m_JoypadState[iPadID][1 - m_iStateIndex].rgbButtons[i] & 0x80
	){
		return true;
	}
	return false;
}
