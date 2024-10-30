#include "pch.h"
#include "ResourceHelper.hpp"
#include "ResourceManager.h"
#include "MeshFactory.h"


void ResourceHelper::generateWall(std::vector<VertexData>& vertexData, std::vector<unsigned short>& indicies,
	std::vector<RoomLinkData> links,
	DirectX::XMFLOAT3 bottomLeft, DirectX::XMFLOAT3 topRight, DirectX::XMFLOAT3 normal, bool alongX)
{
	// check if there is a door on that particular wall
	RoomLinkData* data = nullptr;
	for (auto& link : links)
	{
		if (link.alongX == alongX)
		{
			if ((link.alongX && link.pos.z == bottomLeft.z) || (!link.alongX && link.pos.x == topRight.x))
			{
				data = &link;
				break;
			}
		}
	}

	DirectX::XMFLOAT3 tileStep = {
		alongX ? (topRight.x - bottomLeft.x < 0 ? -1.f : 1.f) : 0.f,
		1.f,
		alongX ? 0.f : (topRight.z - bottomLeft.z < 0 ? -1.f : 1.f),
	};

	int nStepsY = topRight.y - bottomLeft.y;
	int nStepsH = alongX ? std::abs(topRight.x - bottomLeft.x) - 0.5f : std::abs(topRight.z - bottomLeft.z) - 0.5f;

	auto overlapsDoor = [&](DirectX::XMFLOAT3 tilePos, DirectX::XMFLOAT3 tileTopRight) {
		// overlaps in y coordinate
		if (data && data->size.y - tilePos.y > 0.5f)
		{
			if (alongX)
			{
				if (tilePos.x >= data->pos.x && tilePos.x <= data->pos.x + data->size.x &&
					tileTopRight.x >= data->pos.x && tileTopRight.x <= data->pos.x + data->size.x)
				{
					return true;
				}

			}
			else
			{
				if (tilePos.z >= data->pos.z && tilePos.z <= data->pos.z + data->size.z &&
					tileTopRight.z >= data->pos.z && tileTopRight.z <= data->pos.z + data->size.z)
				{
					return true;
				}
			}
		}

		return false;
		};

	for (int stepY = 0; stepY < nStepsY; stepY++)
	{
		for (int stepH = 0; stepH <= nStepsH; stepH++)
		{
			// if there is a door and it overlaps with current tile - dont draw the tile
			DirectX::XMFLOAT3 tilePos = { bottomLeft.x + tileStep.x * stepH, bottomLeft.y + tileStep.y * stepY, bottomLeft.z + tileStep.z * stepH };
			if (overlapsDoor(tilePos, { tilePos.x + tileStep.x, tilePos.y + tileStep.y, tilePos.z + tileStep.z }))
				continue;

			// add wall 
			ResourceHelper::generateWallTile(vertexData, indicies, tilePos, tileStep, normal, stepH, stepY, nStepsH + 1, nStepsY + 1);
		}
	}
}

void ResourceHelper::generateWallTile(std::vector<VertexData>& vertexData, std::vector<unsigned short>& indicies, 
	DirectX::XMFLOAT3 bottomLeft, DirectX::XMFLOAT3 tileStep, DirectX::XMFLOAT3 normal,
	int stepH, int stepY, int nStepH, int nStepY)
{
	int nVerticies = vertexData.size();
	/*VertexData(DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(0.f, 0.f), DirectX::XMFLOAT3(0.f, 0.f, 1.f)),
		VertexData(DirectX::XMFLOAT3(1.f, 0.f, 0.f), DirectX::XMFLOAT2(1.f, 0.f), DirectX::XMFLOAT3(0.f, 0.f, 1.f)),
		VertexData(DirectX::XMFLOAT3(1.f, 1.f, 0.f), DirectX::XMFLOAT2(1.f, 1.f), DirectX::XMFLOAT3(0.f, 0.f, 1.f)),
		VertexData(DirectX::XMFLOAT3(0.f, 1.f, 0.f), DirectX::XMFLOAT2(0.f, 1.f), DirectX::XMFLOAT3(0.f, 0.f, 1.f))*/




	vertexData.push_back(VertexData(bottomLeft, DirectX::XMFLOAT2((float)stepH / nStepH, (float)stepY / nStepY), normal));
	vertexData.push_back(VertexData({ bottomLeft.x + tileStep.x, bottomLeft.y, bottomLeft.z + tileStep.z },
		DirectX::XMFLOAT2((float)(stepH + 1)/ nStepH, (float)stepY / nStepY), normal));
	vertexData.push_back(VertexData({ bottomLeft.x + tileStep.x, bottomLeft.y + tileStep.y, bottomLeft.z + tileStep.z },
		DirectX::XMFLOAT2((float)(stepH + 1)/ nStepH, (float)(stepY + 1) / nStepY), normal));
	vertexData.push_back(VertexData({ bottomLeft.x, bottomLeft.y + tileStep.y, bottomLeft.z },
		DirectX::XMFLOAT2((float)stepH / nStepH, (float)(stepY + 1) / nStepY), normal));

	indicies.push_back(nVerticies);
	indicies.push_back(nVerticies + 1);
	indicies.push_back(nVerticies + 2);
	indicies.push_back(nVerticies + 2);
	indicies.push_back(nVerticies + 3);
	indicies.push_back(nVerticies);
}

void ResourceHelper::addWallModel(const std::string& texturePath, const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	std::vector<VertexData> verticies = {
		VertexData(DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(0.f, 0.f), DirectX::XMFLOAT3(0.f, 0.f, 1.f)),
		VertexData(DirectX::XMFLOAT3(1.f, 0.f, 0.f), DirectX::XMFLOAT2(1.f, 0.f), DirectX::XMFLOAT3(0.f, 0.f, 1.f)),
		VertexData(DirectX::XMFLOAT3(1.f, 1.f, 0.f), DirectX::XMFLOAT2(1.f, 1.f), DirectX::XMFLOAT3(0.f, 0.f, 1.f)),
		VertexData(DirectX::XMFLOAT3(0.f, 1.f, 0.f), DirectX::XMFLOAT2(0.f, 1.f), DirectX::XMFLOAT3(0.f, 0.f, 1.f))
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

Mesh ResourceHelper::generateRoomModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, 
	std::vector<RoomLinkData> links,
	const std::string& texturePath, const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	std::vector<VertexData> vertexData;
	std::vector<unsigned short> indicies;

	ResourceHelper::generateWall(vertexData, indicies, links,
		{ pos.x, pos.y, pos.z }, { pos.x + size.x, pos.y + size.y, pos.z }, { 0.f, 0.f, 1.f }, true);

	// left wall
	ResourceHelper::generateWall(vertexData, indicies, links,
		{ pos.x, pos.y, pos.z + size.z }, { pos.x, pos.y + size.y, pos.z }, { 1.f, 0.f, 0.f }, false);

	// back wall
	ResourceHelper::generateWall(vertexData, indicies, links,
		{ pos.x + size.x, pos.y, pos.z + size.z }, { pos.x, pos.y + size.y, pos.z + size.z }, { 0.f, 0.f, -1.f }, true);

	// right wall
	ResourceHelper::generateWall(vertexData, indicies, links,
		{ pos.x + size.x, pos.y, pos.z }, { pos.x + size.x, pos.y + size.y, pos.z + size.z }, { -1.f, 0.f, 0.f }, false);

	return MeshFactory<VertexData>::createMesh(vertexData, indicies, { ResourceManager::Instance.getTexture(texturePath) }, deviceResources);
	
}
