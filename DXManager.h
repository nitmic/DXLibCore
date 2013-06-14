#pragma once
#include "DXStdafx.h"

namespace DXLib{
	class DXManager : public Singleton<DXManager>{
		friend Singleton<DXManager>;
	private:
		bool       m_bWindowed;
		HWND       m_hWnd;             //!<	�E�B���h�E�n���h��
		std::shared_ptr<D3DPRESENT_PARAMETERS> m_pd3dpp;            //!<	CreateDevice Param
		std::shared_ptr<DXPrimitiveCore>       m_pD3D;             //!<	main D3D Object
		std::shared_ptr<DXPrimitiveDevice>     m_pd3dDeviceWrapped;       //!<	�����_�����O�f�o�C�X
		IDirect3DDevice9 * m_pd3dDevice;

		unsigned long       m_uCreationWidth;	//!<	�E�B���h�E�̕�
		unsigned long       m_uCreationHeight;	//!<	�E�B���h�E�̍���
		bool                m_bDeviceLost;		//!<	�f�o�C�X���X�g���Ă��邩�ǂ���

		//CNcDXLostManager	m_LostManager;		//!<	���X�g�}�l�[�W��
		//D3DDISPLAYMODE m_d3ddm;
	public:
		bool    Setup(HWND hwnd,unsigned long width, unsigned long height, int nCmdShow);

		std::shared_ptr<DXPrimitiveDevice> &	getDevice(){return m_pd3dDeviceWrapped;}
		std::shared_ptr<DXPrimitiveCore> &			getDirect(){return m_pD3D;}
	
		bool				beginScene();
		bool				endScene();
	
		DXManager();
		~DXManager();
	
		bool         getWindowed(){return m_bWindowed;}
	private:
		void         InitRenderStage();				//�e�N�X�`���X�e�[�W�ݒ�

		//BOOL    FindBestWindowedMode(BOOL bRequireHAL, BOOL bRequireREF );
		//BOOL    FindBestFullscreenMode(BOOL bRequireHAL, BOOL bRequireREF );
	};
};