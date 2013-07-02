#pragma once
#include <Windows.h>
#include <memory>

namespace DXLib{
	class DXAbsRenderingEngine;

	class DXAbsRenderingPolicy{
	public:
		virtual bool    Setup(HWND hwnd,unsigned long width, unsigned long height, int nCmdShow) = 0;

		virtual std::shared_ptr<DXAbsRenderingEngine> getDefaultRenderingEngine() = 0;

		virtual bool	beginScene() = 0;
		virtual bool	endScene() = 0;
	};
};