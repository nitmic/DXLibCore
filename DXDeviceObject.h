#pragma once
#include "DXStdafx.h"

class DXManager;

/**
*@class	DXDeviceObject
*@brief	デバイス基底クラス
*/
class DXDeviceObject{
public:
	friend	DXManager;

	static DXPrimitiveDevice *      getD3DDevice(){return m_spD3DDevice.get();}
	static D3DPRESENT_PARAMETERS *  getD3DPP(){return m_spD3DPP.get();}

	DXDeviceObject();
	~DXDeviceObject();
protected:
private:
	static std::shared_ptr<DXPrimitiveDevice>        m_spD3DDevice;  //!<	レンダリングデバイス
	static std::shared_ptr<D3DPRESENT_PARAMETERS>   m_spD3DPP;      //!<	パラメータ  
};
