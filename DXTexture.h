#pragma once
#include "DXPrimitive.h"
#include "DXDeviceObject.h"
#include <tString.h>

namespace DXLib{
	typedef IDirect3DTexture9 RawTexture;
	typedef std::shared_ptr<RawTexture> Texture;
	typedef std::shared_ptr<Texture> Textures;

	Texture TextureLoad(char * filename){

		TCHAR texName[MAX_PATH] = {0};
		if(filename != nullptr){
			MultiByteToWideChar(CP_ACP, 0, filename, -1, texName, MAX_PATH);
		}
		
		RawTexture * pTexture;
		if(FAILED(D3DXCreateTextureFromFile(
			DXDeviceObject::getD3DDevice(),
			texName,
			&pTexture
		))){
			auto path = DXDeviceObject::getPathStorage().find(texName);
			if(FAILED(D3DXCreateTextureFromFile(
				DXDeviceObject::getD3DDevice(),
				path.c_str(),
				&pTexture
			))){
				ERROR_MSG(_T("DXMesh::LoadModelFromFile テクスチャ確保失敗"));
				exit(1);
			}
		}
		return Texture(pTexture, SAFE_RELEASE<RawTexture>);
	}
};