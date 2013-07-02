#pragma once
#include <Windows.h>
#include <Singleton.hpp>
#include "DXAbsRenderingPolicy.h"
#include "DXAbsRenderingEngine.h"


namespace DXLib{
	class DXPrimitiveDevice;
	class DXPrimitiveCore;

	class DXManager : public Singleton<DXManager>, public DXAbsRenderingPolicy{
		friend Singleton<DXManager>;
	public:
		bool    Setup(HWND hwnd,unsigned long width, unsigned long height, int nCmdShow);
		
		std::shared_ptr<DXAbsRenderingEngine> getDefaultRenderingEngine();

		std::shared_ptr<DXPrimitiveDevice> &	getDevice();
		std::shared_ptr<DXPrimitiveCore> &		getDirect();
		
		bool				beginScene();
		bool				endScene();
	
		DXManager();
		~DXManager();
	
		bool         getWindowed();
	private:
		struct Impl;
		std::shared_ptr<Impl> __impl__;

		//BOOL    FindBestWindowedMode(BOOL bRequireHAL, BOOL bRequireREF );
		//BOOL    FindBestFullscreenMode(BOOL bRequireHAL, BOOL bRequireREF );
	};

	class DXDefaultRenderingEngine : public DXAbsRenderingEngine{
	public:
		HRESULT Begin(UINT *puPasses, DWORD dwFlags){return S_OK;}
		HRESULT Begin(){return S_OK;}
		HRESULT BeginPass(long uPass){return S_OK;}

		void EndPass(){}
		void End(){}
		void CommitChanges(){}

		void SetMatrix(D3DXMATRIX &matWorld){
			(*GetSingleton<DXManager>()->getDevice())->SetTransform(D3DTS_WORLD, &matWorld);
		}
		void SetViewProjection(D3DXMATRIX &view, D3DXMATRIX &proj){
			(*GetSingleton<DXManager>()->getDevice())->SetTransform(D3DTS_VIEW, &view);
			(*GetSingleton<DXManager>()->getDevice())->SetTransform(D3DTS_PROJECTION, &proj);
		}
		void SetLightDirection(D3DXVECTOR4 &vLightDir){}
	private:
		std::shared_ptr<DXPrimitiveHLSL> getEffect(){return nullptr;};
	};
};