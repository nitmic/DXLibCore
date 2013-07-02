#include "DXRenderingEngineStorage.h"

#include "DXAbsRenderingEngine.h"
#include <algorithm>

namespace DXLib{
	
	void DXRenderingEngineStorage::SetViewProjection(D3DXMATRIX &view, D3DXMATRIX &proj){
		std::for_each(m_pEngines.begin(), m_pEngines.end(), [&](std::shared_ptr<DXAbsRenderingEngine> & engine){
			engine->SetViewProjection(view, proj);
		});
	}

	void DXRenderingEngineStorage::SetLightDirection(D3DXVECTOR4 & lightDir){
		std::for_each(m_pEngines.begin(), m_pEngines.end(), [&](std::shared_ptr<DXAbsRenderingEngine> & engine){
			engine->SetLightDirection(lightDir);
		});
	}
};