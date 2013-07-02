#pragma once
#include "DXPrimitive.h"
#include <Singleton.hpp>

namespace DXLib{
	class DXAbsRenderingEngine{
	public:
		virtual HRESULT Begin(UINT *puPasses, DWORD dwFlags){return (*getEffect())->Begin(puPasses, dwFlags);}
		virtual HRESULT Begin(){return (*getEffect())->Begin(NULL, 0);}
		virtual HRESULT BeginPass(long uPass){return (*getEffect())->BeginPass(uPass);}

		virtual void EndPass(){(*getEffect())->EndPass();}
		virtual void End(){(*getEffect())->End();}
		virtual void CommitChanges(){(*getEffect())->CommitChanges();}

		virtual void SetMatrix(D3DXMATRIX &matWorld) = 0;
		virtual void SetViewProjection(D3DXMATRIX &view, D3DXMATRIX &proj) = 0;
		virtual void SetLightDirection(D3DXVECTOR4 &vLightDir) = 0;
	private:
		virtual std::shared_ptr<DXPrimitiveHLSL> getEffect() = 0;
	};


};