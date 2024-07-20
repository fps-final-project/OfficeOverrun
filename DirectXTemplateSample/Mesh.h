#pragma once

#include "Common\DeviceResources.h"
#include "Common\StepTimer.h"
#include <d3d11.h>
#include <vector>
#include <Content/ShaderStructures.h>
#include "Texture.h"

struct Mesh
{
	Microsoft::WRL::ComPtr<ID3D11Buffer>		vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	inputLayout;
	uint32	indexCount;
	std::vector<Texture> textures;
};