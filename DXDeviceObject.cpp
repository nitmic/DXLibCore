/**
*@file	DXDeviceObject.cpp
*@brief	デバイスの基底クラス
*@date	2010/2/28
*/

#include "DXStdafx.h"
#include "DXDeviceObject.h"

//====================================================================
//							DXDeviceObject
//====================================================================

std::shared_ptr<DXPrimitiveDevice> DXDeviceObject::m_spD3DDevice = nullptr;
std::shared_ptr<D3DPRESENT_PARAMETERS> DXDeviceObject::m_spD3DPP = nullptr;

/**
*@brief
*/
DXDeviceObject::DXDeviceObject(){}

/**
*/
DXDeviceObject::~DXDeviceObject(){}

