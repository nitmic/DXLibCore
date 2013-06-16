#pragma once
#include <Windows.h>
#include <Singleton.hpp>

namespace DXLib{
	class DXManager : public Singleton<DXManager>{
		friend Singleton<DXManager>;
	public:
		bool    Setup(HWND hwnd,unsigned long width, unsigned long height, int nCmdShow);

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
};