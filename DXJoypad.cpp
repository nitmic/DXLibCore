#include "DXStdafx.h"
#include "DXJoypad.h"

namespace DXLib{
	struct DXJoypad::Impl{
		static BOOL CALLBACK	GetCallback(DIDEVICEINSTANCE *pDidIns, void *pThis);
		static BOOL CALLBACK	EnumObjectCallback(DIDEVICEOBJECTINSTANCE *pDidIns, void *pThis);

		enum{
		  DX_PROP_MIN = 0,
		  DX_PROP_MAX = 10000
		};
		enum{JOYPAD_MAX		= 4};

		std::array<std::shared_ptr<DXPrimitiveInputDevice>,JOYPAD_MAX>    m_pDeviceWrapped;
		std::array<IDirectInputDevice8 *,JOYPAD_MAX> m_pDevice;

		std::shared_ptr<DXPrimitiveInput> m_pInputWrapped;    //!< �f�o�C�X�m�ۂɕK�v
		IDirectInput8 * m_pInput;

		long          m_iDetectJoypadCount;            //!<	���o�����W���C�p�b�h�̐�
		long          m_iSettingJoypad;	               //!<	�ݒ肵�Ă���W���C�p�b�h
		DIJOYSTATE2   m_JoypadState[JOYPAD_MAX][2];    //!<	�W���C�p�b�h�̏��
		Binary		  m_iStateIndex;                   //!<	��Ԃ̃C���f�b�N�X
	};

	DXJoypad::DXJoypad(){
		__impl__ = std::make_shared<Impl>();
		__impl__->m_pDevice.fill(nullptr);
		__impl__->m_iDetectJoypadCount	= 0;
		__impl__->m_iSettingJoypad		= 0;
	
		memset(__impl__->m_JoypadState, 0, sizeof(DIJOYSTATE2) * 2 * Impl::JOYPAD_MAX);
	}

	DXJoypad::~DXJoypad(){}

	/**
	*@brief	�W���C�p�b�h�̏�����
	*@return
	*@param	hWnd	�E�B���h�E�n���h��
	*/
	bool DXJoypad::Setup(
		HWND & hWnd,
		std::shared_ptr<DXPrimitiveInput> & pInput
	){
		__impl__->m_pInputWrapped = pInput;
		__impl__->m_pInput = __impl__->m_pInputWrapped->getDelegateObject();
		//�W���C�p�b�h�̗�
		if(FAILED(__impl__->m_pInput->EnumDevices(
			DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)Impl::GetCallback, (void *)this, DIEDFL_ATTACHEDONLY
		))){
			return false;
		}
		//���o�ł������������[�v����
		const long iLoopMax = std::min(__impl__->m_iDetectJoypadCount, (long)Impl::JOYPAD_MAX);

		for(long i = 0; i < iLoopMax; ++i){
			//�t�H�[�}�b�g�̐ݒ�
			if(FAILED(__impl__->m_pDevice[i]->SetDataFormat(&c_dfDIJoystick2))){
				return false;
			}
			//�������x���̐ݒ�
			if(FAILED(__impl__->m_pDevice[i]->SetCooperativeLevel(
				hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND
			))){
				return false;
			}
			if(FAILED(__impl__->m_pDevice[i]->EnumObjects(
				(LPDIENUMDEVICEOBJECTSCALLBACK)Impl::EnumObjectCallback, (VOID *)this, DIDFT_ALL
			))){
				return false;
			}
			__impl__->m_iSettingJoypad++;
			//�W���C�p�b�h����J�n
			__impl__->m_pDevice[i]->Acquire();
		}
		return true;
	}

	/**
	*@brief	�W���C�p�b�h�̎擾
	*@return	DIENUM_STOP	:	�I�� DIENUM_CONTINUE : �p��
	*@param	pDidIns
	*@param	pCont	this�|�C���^
	*/
	BOOL CALLBACK DXJoypad::Impl::GetCallback(DIDEVICEINSTANCE *pDidIns, void *pCont){
		DXJoypad::Impl *pThis = (DXJoypad::Impl *)pCont;
		//�f�o�C�X�̍쐬
		pThis->m_pDeviceWrapped[pThis->m_iDetectJoypadCount] = DXPrimitiveInputDevice::Create(pDidIns->guidInstance, pThis->m_pInputWrapped);
		if(pThis->m_pDeviceWrapped[pThis->m_iDetectJoypadCount]){
			return DIENUM_STOP;
		}
		pThis->m_pDevice[pThis->m_iDetectJoypadCount] = **pThis->m_pDeviceWrapped[pThis->m_iDetectJoypadCount];


		//�W���C�p�b�h�̐��𑝉�
		pThis->m_iDetectJoypadCount++;

		return DIENUM_CONTINUE;
	}


	/**
	*@brief		�W���C�p�b�h�f�o�C�X�̗�
	*@return	DIENUM_STOP : �񋓂̏I�� DIENUM_CONTINUE : �p��
	*@param	pDidIns	
	*@param	pCont	this�|�C���^
	*/
	BOOL CALLBACK DXJoypad::Impl::EnumObjectCallback(DIDEVICEOBJECTINSTANCE *pDidIns, void *pCont){
		DXJoypad::Impl *pThis = (DXJoypad::Impl *)pCont;

		if(pDidIns->dwType & DIDFT_AXIS){
			DIPROPRANGE	diprg;
			ZeroMemory(&diprg, sizeof(DIPROPRANGE));

			diprg.diph.dwSize		= sizeof(DIPROPRANGE);	//�S�̂̃T�C�Y
			diprg.diph.dwHeaderSize	= sizeof(DIPROPHEADER);	//�w�b�_�̃T�C�Y
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
	*@brief	�W���C�p�b�h�̏�Ԃ��X�V
	*@return
	*/
	void DXJoypad::Update(){
		__impl__->m_iStateIndex.reverse();
		const long iLoopMax = std::min(__impl__->m_iDetectJoypadCount, (long)Impl::JOYPAD_MAX);

		for(long i = 0; i < iLoopMax; ++i){
			HRESULT hr;
			if(FAILED(__impl__->m_pDevice[i]->Poll())){
				do{
					hr = __impl__->m_pDevice[i]->Acquire();
				}while(hr == DIERR_INPUTLOST);
				ZeroMemory(&__impl__->m_JoypadState[i][__impl__->m_iStateIndex], sizeof(__impl__->m_JoypadState[i][__impl__->m_iStateIndex]));
				return ;
			}
			hr = __impl__->m_pDevice[i]->GetDeviceState(sizeof(DIJOYSTATE2), &__impl__->m_JoypadState[i][__impl__->m_iStateIndex]);
		}
	}

	/**
	*@brief	�W���C�p�b�h�̃{�^����������Ă��邩���擾����
	*@return	true : ������Ă���
	*@param	iPadID	�p�b�h�̂h�c
	*@param	i	�{�^���h�c
	*/
	bool DXJoypad::isPressed(long iPadID, long i){
		if(__impl__->m_JoypadState[iPadID][__impl__->m_iStateIndex].rgbButtons[i] & 0x80){
			return true;
		}
		return false;
	}

	/**
	*@brief	�W���C�p�b�h���������ꂽ���ǂ������擾����
	*@return	true :
	*@param	iPadID	
	*@param	i
	*/
	bool DXJoypad::isJustPressed(long iPadID, long i){
		if(
			(__impl__->m_JoypadState[iPadID][__impl__->m_iStateIndex].rgbButtons[i] & 0x80) &&
			((__impl__->m_JoypadState[iPadID][1 - __impl__->m_iStateIndex].rgbButtons[i] & 0x80) == 0)
		){
			return true;
		}
		return false;
	}

	/**
	*@brief	�W���C�p�b�h�̃{�^����������Ă��邩�ǂ������擾����
	*@return	true : ������Ă���
	*@param	iPadID
	*@param	i
	*/
	bool DXJoypad::isFree(long iPadID, long i){
		if(__impl__->m_JoypadState[iPadID][__impl__->m_iStateIndex].rgbButtons[i] & 0x80){
			return false;
		}

		return true;
	}

	/**
	*@brief	�W���C�p�b�h�̃{�^�����������ꂽ���ǂ������擾����
	*@return	true : �������ꂽ
	*@param	iPadID
	*@param	i
	*/
	bool DXJoypad::isJustPull(long iPadID, long i){
		if(
			((__impl__->m_JoypadState[iPadID][__impl__->m_iStateIndex].rgbButtons[i] & 0x80) == 0) &&
			__impl__->m_JoypadState[iPadID][1 - __impl__->m_iStateIndex].rgbButtons[i] & 0x80
		){
			return true;
		}
		return false;
	}

	
	DIJOYSTATE2 *	DXJoypad::getState(long i)	{return &__impl__->m_JoypadState[i][__impl__->m_iStateIndex];}
	DIJOYSTATE2 *	DXJoypad::getPreState(long i){return &__impl__->m_JoypadState[i][1 - __impl__->m_iStateIndex];}
	
	Binary      DXJoypad::getStateIndex() const  {return __impl__->m_iStateIndex;}
	long      DXJoypad::getDetectJoypadCount() {return __impl__->m_iDetectJoypadCount;}
};