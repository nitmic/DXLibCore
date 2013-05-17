#pragma once
#include "DXStdafx.h"

template<class T>
class Delegater{
public:
	void setDelegateObject(std::shared_ptr<T> & object){
		delegate_object = object;
	}
	void releaseDelegateObject(){
		delegate_object.reset();
	}
	T * getDelegateObject(){
		return delegate_object.get();
	}
	T * operator->(){
		return getDelegateObject();
	}
	T * operator*(){
		return getDelegateObject();
	}
private:
	std::shared_ptr<T> delegate_object;
};

template<class T>
void ReleaseDelegater(T * p){
	p->releaseDelegateObject();
	delete p;
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
