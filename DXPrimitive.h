#pragma once
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "winmm.lib")

#pragma comment(lib, "user32.lib")

#pragma comment(lib, "dinput8.lib")
#include <windows.h>
#include <Windows.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <dsound.h>
#include <memory>
#include <TTL\Delegater.hpp>
#include "dinput.h"

template<class T>
void SAFE_RELEASE(T * p){ p->Release(); }

template<>
void SAFE_RELEASE(IDirectInputDevice8 * p){
	p->Unacquire();
	p->Release();
}


class DXPrimitiveCore : public Delegater<IDirect3D9>{
public:
	static std::shared_ptr<DXPrimitiveCore> Create();
};

class DXPrimitiveDevice : public Delegater<IDirect3DDevice9>{
public:
	static std::shared_ptr<DXPrimitiveDevice> Create(
		std::shared_ptr<D3DPRESENT_PARAMETERS> & pd3dpp,
		HWND & hWnd,
		std::shared_ptr<DXPrimitiveCore> & core
	);
private:
	std::shared_ptr<DXPrimitiveCore> m_pCore;
};

class DXPrimitiveInput : public Delegater<IDirectInput8>{
public:
	static std::shared_ptr<DXPrimitiveInput> Create(
		HINSTANCE & hInst,
		std::shared_ptr<DXPrimitiveDevice> & pDevice
	);
private:
	std::shared_ptr<DXPrimitiveDevice> m_pDevice;
};

class DXPrimitiveInputDevice : public Delegater<IDirectInputDevice8>{
public:
	static std::shared_ptr<DXPrimitiveInputDevice> Create(
		const GUID & guid,
		std::shared_ptr<DXPrimitiveInput> & pInput
	);
private:
	std::shared_ptr<DXPrimitiveInput> m_pInput;
};
