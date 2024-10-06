#include "pch.h"
#include "ResourceHelper.hpp"
#include "ResourceManager.h"
#include "MeshFactory.h"
#include "Content/ShaderStructures.h"


void ResourceHelper::addWallModel(const std::string& texturePath, const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	std::vector<VertexData> verticies = {
		VertexData(DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(0.f, 0.f), DirectX::XMFLOAT3(0.f, 0.f, -1.f)),
		VertexData(DirectX::XMFLOAT3(0.f, 1.f, 0.f), DirectX::XMFLOAT2(0.f, 1.f), DirectX::XMFLOAT3(0.f, 0.f, -1.f)),
		VertexData(DirectX::XMFLOAT3(1.f, 1.f, 0.f), DirectX::XMFLOAT2(1.f, 1.f), DirectX::XMFLOAT3(0.f, 0.f, -1.f)),
		VertexData(DirectX::XMFLOAT3(1.f, 0.f, 0.f), DirectX::XMFLOAT2(1.f, 0.f), DirectX::XMFLOAT3(0.f, 0.f, -1.f))
	};

	std::vector<unsigned short> indicies = {
		0, 1, 2,
		2, 3, 0
	};

	ResourceManager::Instance.loadTexture(texturePath, deviceResources, "wall");
	AssimpModel model;
	model.meshes.push_back(MeshFactory<VertexData>::createMesh(verticies, indicies,
		{ ResourceManager::Instance.getTexture("wall") }, deviceResources));

	ResourceManager::Instance.addModel(model, "wall");

}
