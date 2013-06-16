#pragma once
#include "DXPrimitive.h"

namespace DXLib{
	typedef IDirect3DTexture9 RawTexture;
	typedef std::shared_ptr<RawTexture> Texture;
	typedef std::shared_ptr<Texture> Textures;

	Texture TextureLoad(char * filename);
};