#pragma once
#include <Windows.h>
#include <functional>
#include <Singleton.hpp>
#include "DXManager.h"

namespace DXLib{

	class DXApp{
	public:
		DXApp();

		bool Setup(HINSTANCE hInst, long w, long h, int nCmdShow, std::shared_ptr<DXAbsRenderingPolicy> renderingPolicy);

		void	setOnFrameUpdate(std::function<bool(void)> func);
		void	setOnFrameDraw(std::function<void(void)> func);

		void AppLoop();
		bool AppIdle();

		long	getExitCode();
	private:
		struct Impl;
		std::shared_ptr<Impl> __impl__;
	};

	/*
	*DXInitialize
	*/
	
	namespace{
		template<class RenderingPolicy>
		std::shared_ptr<DXAbsRenderingPolicy> getRenderingPolicy(){
			return std::make_shared<RenderingPolicy>();
		}
		template<>
		std::shared_ptr<DXAbsRenderingPolicy> getRenderingPolicy<DXManager>(){
			return GetSingleton<DXManager>();
		}
	};

	template<class RenderingPolicy>
	std::shared_ptr<DXApp> DXInitialize(HINSTANCE hInst, long w, long h, int nCmdShow){
		auto pApp = std::make_shared<DXApp>();
		auto renderingPolicy = getRenderingPolicy<RenderingPolicy>();
		if(!pApp->Setup(hInst, w, h, nCmdShow, renderingPolicy)){
			return nullptr;
		}
		return pApp;
	}
};