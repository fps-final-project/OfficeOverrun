#include "pch.h"
#include "ResourceHelper.hpp"
#include "ResourceManager.h"
#include "MeshFactory.h"

#include <filesystem>
#include "PropLoader.hpp"

void ResourceHelper::addQuad(const std::string& texturePath, std::string textureName, std::string modelName, int nSubdivs, const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	std::vector<VertexData> verticies;
	std::vector<unsigned short> indicies;


	float nSubdivsf = nSubdivs;
	for (int i = 0; i < nSubdivs; i++)
	{
		for (int j = 0; j < nSubdivs; j++)
		{
			int nVerticiesCurr = verticies.size();
			verticies.push_back(VertexData(DirectX::XMFLOAT3(i / nSubdivsf, j / nSubdivsf, 0.f),
				DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT3(0.f, 0.f, 1.f)));
			verticies.push_back(VertexData(DirectX::XMFLOAT3((i + 1) / nSubdivsf, j / nSubdivsf, 0.f),
				DirectX::XMFLOAT2(1, 0), DirectX::XMFLOAT3(0.f, 0.f, 1.f)));
			verticies.push_back(VertexData(DirectX::XMFLOAT3((i + 1) / nSubdivsf, (j + 1) / nSubdivsf, 0.f),
				DirectX::XMFLOAT2(1, 1), DirectX::XMFLOAT3(0.f, 0.f, 1.f)));
			verticies.push_back(VertexData(DirectX::XMFLOAT3(i / nSubdivsf, (j + 1) / nSubdivsf, 0.f),
				DirectX::XMFLOAT2(0, 1), DirectX::XMFLOAT3(0.f, 0.f, 1.f)));

			indicies.push_back(nVerticiesCurr);
			indicies.push_back(nVerticiesCurr + 1);
			indicies.push_back(nVerticiesCurr + 2);
			indicies.push_back(nVerticiesCurr + 2);
			indicies.push_back(nVerticiesCurr + 3);
			indicies.push_back(nVerticiesCurr);
		}
	}

	ResourceManager::Instance().loadTexture(texturePath, deviceResources, textureName);
	Model model;
	model.meshes.push_back(MeshFactory<VertexData>::createMesh(verticies, indicies,
		{ ResourceManager::Instance().getTexture(textureName) }, deviceResources));

	ResourceManager::Instance().addModel(model, modelName);

}

void ResourceHelper::createSkyboxMesh(const std::string& texturePath, const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	// order: ft, bk, lf, rt, dn, up
	int nTextures = 6;
	std::vector<VertexData> verticies = {
		// front face
		VertexData({-1.f, -1.f, -1.f}, {0.f, 1.f}, {0.f, 0.f, 1.f}),
		VertexData({1.f, -1.f, -1.f}, {1.f / nTextures, 1.f}, {0.f, 0.f, 1.f}),
		VertexData({1.f, 1.f, -1.f}, {1.f / nTextures, 0.f }, {0.f, 0.f, 1.f}),
		VertexData({-1.f, 1.f, -1.f}, {0.f, 0.f }, {0.f, 0.f, 1.f}),

		// back face
		VertexData({1.f, -1.f, 1.f}, {1.f / nTextures, 1.f}, {0.f, 0.f, -1.f}),
		VertexData({-1.f, -1.f, 1.f}, {2.f / nTextures, 1.f}, {0.f, 0.f, -1.f}),
		VertexData({-1.f, 1.f, 1.f}, {2.f / nTextures, 0.f}, {0.f, 0.f, -1.f}),
		VertexData({1.f, 1.f, 1.f}, {1.f / nTextures, 0.f}, {0.f, 0.f, -1.f}),

		// left face
		VertexData({-1.f, -1.f, 1.f}, {2.f / nTextures, 1.f}, {1.f, 0.f, 0.f}),
		VertexData({-1.f, -1.f, -1.f}, {3.f / nTextures, 1.f}, {1.f, 0.f, 0.f}),
		VertexData({-1.f, 1.f, -1.f}, {3.f / nTextures, 0.f}, {1.f, 0.f, 0.f}),
		VertexData({-1.f, 1.f, 1.f}, {2.f / nTextures, 0.f}, {1.f, 0.f, 0.f}),

		// right face
		VertexData({1.f, -1.f, -1.f}, {3.f / nTextures, 1.f}, {-1.f, 0.f, 0.f}),
		VertexData({1.f, -1.f, 1.f}, {4.f / nTextures, 1.f}, {-1.f, 0.f, 0.f}),
		VertexData({1.f, 1.f, 1.f}, {4.f / nTextures, 0.f}, {-1.f, 0.f, 0.f}),
		VertexData({1.f, 1.f, -1.f}, {3.f / nTextures, 0.f}, {-1.f, 0.f, 0.f}),

		// down face
		VertexData({-1.f, -1.f, 1.f}, {4.f / nTextures, 1.f}, {0.f, 1.f, 0.f}),
		VertexData({1.f, -1.f, 1.f}, {5.f / nTextures, 1.f}, {0.f, 1.f, 0.f}),
		VertexData({1.f, -1.f, -1.f}, {5.f / nTextures, 0.f}, {0.f, 1.f, 0.f}),
		VertexData({-1.f, -1.f, -1.f}, {4.f / nTextures, 0.f}, {0.f, 1.f, 0.f}),

		// up face
		VertexData({-1.f, 1.f, -1.f}, {5.f / nTextures, 1.f}, {0.f, -1.f, 0.f}),
		VertexData({1.f, 1.f, -1.f}, {6.f / nTextures, 1.f}, {0.f, -1.f, 0.f}),
		VertexData({1.f, 1.f, 1.f}, {6.f / nTextures, 0.f}, {0.f, -1.f, 0.f}),
		VertexData({-1.f, 1.f, 1.f}, {5.f / nTextures, 0.f}, {0.f, -1.f, 0.f})
	};

	std::vector<unsigned short> indicies;
	int currVertex = 0;
	for (int i = 0; i < 6; i++)
	{
		indicies.push_back(currVertex);
		indicies.push_back(currVertex + 1);
		indicies.push_back(currVertex + 2);
		indicies.push_back(currVertex + 2);
		indicies.push_back(currVertex + 3);
		indicies.push_back(currVertex);

		currVertex += 4;
	}

	std::string name = "skybox";
	ResourceManager::Instance().loadTexture(texturePath, deviceResources, name);
	Model model;
	model.meshes.push_back(MeshFactory<VertexData>::createMesh(verticies, indicies,
		{ ResourceManager::Instance().getTexture(name) }, deviceResources));

	ResourceManager::Instance().addModel(model, name);

}

void ResourceHelper::LoadAllPropsModels(const std::string& propFolderPath, const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	auto props = PropLoader::LoadAllProps(propFolderPath);

	for (const auto& prop : props)
	{
		ResourceManager::Instance().loadModel(prop.modelPath, deviceResources, prop.name);
	}
}

void ResourceHelper::LoadAllMuzzleFlashFrames(const std::string& folderPath, const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	for (int i = 1; i <= 21; i++)
	{
		ResourceManager::Instance().loadTexture(folderPath + "\\muzzle_" + std::to_string(i) + ".png", deviceResources);
	}
}
