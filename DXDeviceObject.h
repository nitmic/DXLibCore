#pragma once
#include "DXStdafx.h"
#include <MediaPath.h>

namespace DXLib{
	class DXManager;

	/**
	*@class	DXDeviceObject
	*@brief	デバイス基底クラス
	*/
	class DXDeviceObject{
	public:
		friend	DXManager;

		static IDirect3DDevice9 *      getD3DDevice(){return **m_spD3DDevice;}
		static D3DPRESENT_PARAMETERS *  getD3DPP(){return m_spD3DPP.get();}
		static MediaPath & getPathStorage(){return m_pathStorage;}
		DXDeviceObject();
		~DXDeviceObject();
	protected:
	private:
		static MediaPath m_pathStorage;
		static std::shared_ptr<DXPrimitiveDevice>        m_spD3DDevice;  //!<	レンダリングデバイス
		static std::shared_ptr<D3DPRESENT_PARAMETERS>   m_spD3DPP;      //!<	パラメータ  
	};
};