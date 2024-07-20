#pragma once

#include <string>

struct Texture
{
	std::string type;
	std::string path;
	Microsoft::WRL::ComPtr<ID3D11Resource> textureHandle;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
};