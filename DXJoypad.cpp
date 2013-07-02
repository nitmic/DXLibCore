#include "DXStdafx.h"
#include "DXJoypad.h"

namespace DXLib{
	struct DXJoypad::Impl{
		static BOOL CALLBACK	GetCallback(DIDEVICEINSTANCE *pDidIns, void *pThis);
		static BOOL CALLBACK	EnumObjectCallback(DIDEVICEOBJECTINSTANCE *pDidIns, void *pThis);

		enum{JOYPAD_MAX		= 4};

		std::array<std::shared_ptr<DXPrimitiveInputDevice>,JOYPAD_MAX>    m_pDeviceWrapped;
		std::array<IDirectInputDevice8 *,JOYPAD_MAX> m_pDevice;

		std::shared_ptr<DXPrimitiveInput> m_pInputWrapped;    //!< デバイス確保に必要
		IDirectInput8 * m_pInput;

		long          m_iDetectJoypadCount;            //!<	検出したジョイパッドの数
		long          m_iSettingJoypad;	               //!<	設定しているジョイパッド
		std::array<DIJOYSTATE2,JOYPAD_MAX>   m_JoypadState;
		int convert(DXJoypad::DefaultButton c){return c;}
	};

	DXJoypad::DXJoypad(){
		__impl__ = std::make_shared<Impl>();
		__impl__->m_pDevice.fill(nullptr);
		__impl__->m_iDetectJoypadCount	= 0;
		__impl__->m_iSettingJoypad		= 0;
	
		memset(&__impl__->m_JoypadState[0], 0, sizeof(DIJOYSTATE2) * Impl::JOYPAD_MAX);
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
		__impl__->m_pInputWrapped = pInput;
		__impl__->m_pInput = __impl__->m_pInputWrapped->getDelegateObject();
		//ジョイパッドの列挙
		if(FAILED(__impl__->m_pInput->EnumDevices(
			DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)Impl::GetCallback, (void *)(__impl__.get()), DIEDFL_ATTACHEDONLY
		))){
			return false;
		}
		//検出できた数だけループを回す
		const long iLoopMax = std::min(__impl__->m_iDetectJoypadCount, (long)Impl::JOYPAD_MAX);

		for(long i = 0; i < iLoopMax; ++i){
			//フォーマットの設定
			if(FAILED(__impl__->m_pDevice[i]->SetDataFormat(&c_dfDIJoystick2))){
				return false;
			}
			//協調レベルの設定
			if(FAILED(__impl__->m_pDevice[i]->SetCooperativeLevel(
				hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND
			))){
				return false;
			}
			if(FAILED(__impl__->m_pDevice[i]->EnumObjects(
				(LPDIENUMDEVICEOBJECTSCALLBACK)Impl::EnumObjectCallback, (VOID *)(__impl__.get()), DIDFT_ALL
			))){
				return false;
			}
			__impl__->m_iSettingJoypad++;
			//ジョイパッド制御開始
			__impl__->m_pDevice[i]->Acquire();
		}
		return true;
	}

	/**
	*@brief	ジョイパッドの取得
	*@return	DIENUM_STOP	:	終了 DIENUM_CONTINUE : 継続
	*@param	pDidIns
	*@param	pCont	thisポインタ
	*/
	BOOL CALLBACK DXJoypad::Impl::GetCallback(DIDEVICEINSTANCE *pDidIns, void *pCont){
		DXJoypad::Impl *pThis = (DXJoypad::Impl *)pCont;
		//デバイスの作成
		pThis->m_pDeviceWrapped[pThis->m_iDetectJoypadCount] = DXPrimitiveInputDevice::Create(pDidIns->guidInstance, pThis->m_pInputWrapped);
		if(!pThis->m_pDeviceWrapped[pThis->m_iDetectJoypadCount]){
			return DIENUM_STOP;
		}
		pThis->m_pDevice[pThis->m_iDetectJoypadCount] = pThis->m_pDeviceWrapped[pThis->m_iDetectJoypadCount]->getDelegateObject();


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
	BOOL CALLBACK DXJoypad::Impl::EnumObjectCallback(DIDEVICEOBJECTINSTANCE *pDidIns, void *pCont){
		DXJoypad::Impl *pThis = (DXJoypad::Impl *)pCont;

		if(pDidIns->dwType & DIDFT_AXIS){
			DIPROPRANGE	diprg;
			ZeroMemory(&diprg, sizeof(DIPROPRANGE));

			diprg.diph.dwSize		= sizeof(DIPROPRANGE);	//全体のサイズ
			diprg.diph.dwHeaderSize	= sizeof(DIPROPHEADER);	//ヘッダのサイズ
			diprg.diph.dwHow		= DIPH_BYID;
			diprg.diph.dwObj		= pDidIns->dwType;
			diprg.lMin				= - DXJoypad::DX_PROP_MAX; // スティックの傾きの範囲
			diprg.lMax				= DXJoypad::DX_PROP_MAX; // スティックの傾きの範囲
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
		//__impl__->m_iStateIndex.reverse();
		const long iLoopMax = std::min(__impl__->m_iDetectJoypadCount, (long)Impl::JOYPAD_MAX);

		for(long i = 0; i < iLoopMax; ++i){
			HRESULT hr;
			if(FAILED(__impl__->m_pDevice[i]->Poll())){
				do{
					hr = __impl__->m_pDevice[i]->Acquire();
				}while(hr == DIERR_INPUTLOST);
				ZeroMemory(&__impl__->m_JoypadState[i], sizeof(__impl__->m_JoypadState[i]));
				return ;
			}
			hr = __impl__->m_pDevice[i]->GetDeviceState(sizeof(DIJOYSTATE2), &__impl__->m_JoypadState[i]);
		}
	}

	BYTE DXJoypad::getButtonState(long i, DefaultButton c){
		return __impl__->m_JoypadState[i].rgbButtons[__impl__->convert(c)];
	}

	D3DXVECTOR2 DXJoypad::getLAnalogStickState(long i){
		D3DXVECTOR2 v;
		v.x = __impl__->m_JoypadState[i].lX;
		v.y = __impl__->m_JoypadState[i].lY;
		return v;
	}

	D3DXVECTOR2 DXJoypad::getRAnalogStickState(long i){
		D3DXVECTOR2 v;
		v.x = __impl__->m_JoypadState[i].lZ;
		v.y = __impl__->m_JoypadState[i].lRz;
		return v;
	}

	long      DXJoypad::getDetectJoypadCount() {return __impl__->m_iDetectJoypadCount;}
};