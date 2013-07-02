/**
*@file	DXDeviceObject.cpp
*@brief	デバイスの基底クラス
*@date	2010/2/28
*/

#include "DXDeviceObject.h"

//====================================================================
//							DXDeviceObject
//====================================================================

namespace DXLib{
	std::shared_ptr<DXPrimitiveDevice>     DXDeviceObject::m_spD3DDevice = nullptr;
	std::shared_ptr<D3DPRESENT_PARAMETERS> DXDeviceObject::m_spD3DPP = nullptr;
	std::shared_ptr<DXAbsRenderingEngine>    DXDeviceObject::m_spRenderingEngine = nullptr;
	MediaPath DXDeviceObject::m_pathStorage;
	
	/**
	*@brief
	*/
	DXDeviceObject::DXDeviceObject(){}

	/**
	*/
	DXDeviceObject::~DXDeviceObject(){}

};