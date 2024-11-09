#include "pch.h"
#include "ResourceHelper.hpp"
#include "ResourceManager.h"
#include "MeshFactory.h"

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

	ResourceManager::Instance.loadTexture(texturePath, deviceResources, textureName);
	Model model;
	model.meshes.push_back(MeshFactory<VertexData>::createMesh(verticies, indicies,
		{ ResourceManager::Instance.getTexture(textureName) }, deviceResources));

	ResourceManager::Instance.addModel(model, modelName);

}
