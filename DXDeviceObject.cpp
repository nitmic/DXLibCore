/**
*@file	DXDeviceObject.cpp
*@brief	�f�o�C�X�̊��N���X
*@date	2010/2/28
*/

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

