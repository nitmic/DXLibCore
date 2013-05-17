#pragma once
#include "DXStdafx.h"

class DXManager;

/**
*@class	DXDeviceObject
*@brief	�f�o�C�X���N���X
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
	static std::shared_ptr<DXPrimitiveDevice>        m_spD3DDevice;  //!<	�����_�����O�f�o�C�X
	static std::shared_ptr<D3DPRESENT_PARAMETERS>   m_spD3DPP;      //!<	�p�����[�^  
};
