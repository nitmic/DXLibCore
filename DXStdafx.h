#pragma once
#define NOMINMAX

#include <windows.h>
#include <Windows.h>
#include <assert.h>
#include <tchar.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <dsound.h>

#include <string>
#include <map>
#include <vector>
#include <list>
#include <array>

#include <functional>
#include <algorithm>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "winmm.lib")

#pragma comment(lib, "user32.lib")

#pragma comment(lib, "dinput8.lib")
#include "dinput.h"

template<class T>
void SAFE_RELEASE(T * p){ p->Release(); }
template<>
void SAFE_RELEASE(IDirectInputDevice8 * p){
	p->Unacquire();
	p->Release();
}


#include "Error.h"
#include "Singleton.hpp"
#include "DXPrimitive.h"