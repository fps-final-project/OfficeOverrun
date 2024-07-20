#pragma once

#include "Texture.h"
#include "Common/DeviceResources.h"
#include <string>

class TextureFactory
{
public:
	static Texture CreateTextureFromFile(const wchar_t* filename, std::shared_ptr<DX::DeviceResources> deviceResources);
};

