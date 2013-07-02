#pragma once
#include <Singleton.hpp>
#include <vector>
#include "DXPrimitive.h"

namespace DXLib{
	class DXAbsRenderingEngine;

	class DXRenderingEngineStorage : Singleton<DXRenderingEngineStorage>{
	public:
		friend Singleton<DXRenderingEngineStorage>;

		template<class T>
		std::shared_ptr<T> getEngine(){
			static std::shared_ptr<T> engine;
			if(!engine){
				engine = std::make_shared<T>();
				this->m_pEngines.push_back(engine);
			}
			return engine;
		}
		void SetViewProjection(D3DXMATRIX &view, D3DXMATRIX &proj);
		void SetLightDirection(D3DXVECTOR4 & lightDir);
	private:
		std::vector<std::shared_ptr<DXAbsRenderingEngine>> m_pEngines;
	};
};