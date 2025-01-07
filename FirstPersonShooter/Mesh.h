#pragma once

#include <d3d11.h>
#include <vector>
#include <memory>
#include "Content/ShaderStructures.h"
#include "Texture.h"

struct Mesh
{
	Microsoft::WRL::ComPtr<ID3D11Buffer>		vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		indexBuffer;
	uint32										indexCount;
	std::vector<std::shared_ptr<Texture>>		textures;
};