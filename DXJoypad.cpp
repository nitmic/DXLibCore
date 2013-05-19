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
*@brief	�W���C�p�b�h�̏�����
*@return
*@param	hWnd	�E�B���h�E�n���h��
*/
bool DXJoypad::Setup(
	HWND & hWnd,
	std::shared_ptr<DXPrimitiveInput> & pInput
){
	m_pInputWrapped = pInput;
	m_pInput = **m_pInputWrapped;
	//�W���C�p�b�h�̗�
	if(FAILED(m_pInput->EnumDevices(
		DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)GetCallback, (void *)this, DIEDFL_ATTACHEDONLY
	))){
		return false;
	}
	//���o�ł������������[�v����
	long iLoopMax = std::min(m_iDetectJoypadCount, (long)JOYPAD_MAX);

	for(long i = 0; i < iLoopMax; ++i){
		//�t�H�[�}�b�g�̐ݒ�
		if(FAILED(m_pDevice[i]->SetDataFormat(&c_dfDIJoystick2))){
			return false;
		}
		//�������x���̐ݒ�
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
		//�W���C�p�b�h����J�n
		m_pDevice[i]->Acquire();
	}
	return true;
}

/**
*@brief	�W���C�p�b�h�̎擾
*@return	DIENUM_STOP	:	�I�� DIENUM_CONTINUE : �p��
*@param	pDidIns
*@param	pCont	this�|�C���^
*/
BOOL CALLBACK DXJoypad::GetCallback(DIDEVICEINSTANCE *pDidIns, void *pCont){
	DXJoypad *pThis = (DXJoypad *)pCont;
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
BOOL CALLBACK DXJoypad::EnumObjectCallback(DIDEVICEOBJECTINSTANCE *pDidIns, void *pCont){
	DXJoypad *pThis = (DXJoypad *)pCont;

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
*@brief	�W���C�p�b�h�̃{�^����������Ă��邩���擾����
*@return	true : ������Ă���
*@param	iPadID	�p�b�h�̂h�c
*@param	i	�{�^���h�c
*/
bool DXJoypad::isPressed(long iPadID, long i){
	if(m_JoypadState[iPadID][m_iStateIndex].rgbButtons[i] & 0x80){
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
		(m_JoypadState[iPadID][m_iStateIndex].rgbButtons[i] & 0x80) &&
		((m_JoypadState[iPadID][1 - m_iStateIndex].rgbButtons[i] & 0x80) == 0)
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
	if(m_JoypadState[iPadID][m_iStateIndex].rgbButtons[i] & 0x80){
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
		((m_JoypadState[iPadID][m_iStateIndex].rgbButtons[i] & 0x80) == 0) &&
		m_JoypadState[iPadID][1 - m_iStateIndex].rgbButtons[i] & 0x80
	){
		return true;
	}
	return false;
}
