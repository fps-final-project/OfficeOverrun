#include "pch.h"
#include "ResourceHelper.hpp"
#include "ResourceManager.h"
#include "MeshFactory.h"

const float ResourceHelper::wallOffset = 0.1f;

void ResourceHelper::generateWall(std::vector<VertexData>& vertexData, std::vector<unsigned short>& indicies,
	std::vector<RoomLinkData> links,
	DirectX::XMFLOAT3 bottomLeft, DirectX::XMFLOAT3 topRight, DirectX::XMFLOAT3 normal, DirectX::XMFLOAT3 wallOffset3f, bool alongX)
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
			{
				if (stepY == 0)
					ResourceHelper::generateDoorFrame(vertexData, indicies, tilePos, data->size,
						{ -normal.x * wallOffset, 0, -normal.z * wallOffset }, stepH, stepY, nStepsH + 1, nStepsY + 1);
			}
			else
			{
				tilePos = { tilePos.x + wallOffset3f.x, tilePos.y, tilePos.z + wallOffset3f.z };

				// add wall 
				ResourceHelper::generateWallTile(vertexData, indicies, tilePos, tileStep,
					normal, stepH, stepY, nStepsH + 1, nStepsY + 1,
					(stepH == 0) * wallOffset, (stepH == nStepsH) * wallOffset, alongX);
			}
		}
	}
}

void ResourceHelper::generateWallTile(std::vector<VertexData>& vertexData, std::vector<unsigned short>& indicies,
	DirectX::XMFLOAT3 bottomLeft, DirectX::XMFLOAT3 tileStep, DirectX::XMFLOAT3 normal,
	int stepH, int stepY, int nStepH, int nStepY, float leftOffset, float rightOffset, bool alongX)
{
	int nVerticies = vertexData.size();
	/*VertexData(DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(0.f, 0.f), DirectX::XMFLOAT3(0.f, 0.f, 1.f)),
		VertexData(DirectX::XMFLOAT3(1.f, 0.f, 0.f), DirectX::XMFLOAT2(1.f, 0.f), DirectX::XMFLOAT3(0.f, 0.f, 1.f)),
		VertexData(DirectX::XMFLOAT3(1.f, 1.f, 0.f), DirectX::XMFLOAT2(1.f, 1.f), DirectX::XMFLOAT3(0.f, 0.f, 1.f)),
		VertexData(DirectX::XMFLOAT3(0.f, 1.f, 0.f), DirectX::XMFLOAT2(0.f, 1.f), DirectX::XMFLOAT3(0.f, 0.f, 1.f))*/

	DirectX::XMFLOAT3 leftOffset3f = { alongX * leftOffset, 0, !alongX * leftOffset };
	DirectX::XMFLOAT3 rightOffset3f = { alongX * rightOffset, 0, !alongX * rightOffset };



	vertexData.push_back(VertexData({ bottomLeft.x + leftOffset3f.x, bottomLeft.y, bottomLeft.z + leftOffset3f.z },
		DirectX::XMFLOAT2((float)(stepH + leftOffset) / nStepH, (float)stepY / nStepY), normal));
	vertexData.push_back(VertexData({ bottomLeft.x + tileStep.x - rightOffset3f.x, bottomLeft.y, bottomLeft.z + tileStep.z - rightOffset3f.z },
		DirectX::XMFLOAT2((float)(stepH + 1 - rightOffset) / nStepH, (float)stepY / nStepY), normal));
	vertexData.push_back(VertexData({ bottomLeft.x + tileStep.x - rightOffset3f.x, bottomLeft.y + tileStep.y, bottomLeft.z + tileStep.z - rightOffset3f.z },
		DirectX::XMFLOAT2((float)(stepH + 1 - rightOffset) / nStepH, (float)(stepY + 1) / nStepY), normal));
	vertexData.push_back(VertexData({ bottomLeft.x + leftOffset3f.x, bottomLeft.y + tileStep.y, bottomLeft.z + leftOffset3f.z },
		DirectX::XMFLOAT2((float)(stepH + leftOffset) / nStepH, (float)(stepY + 1) / nStepY), normal));

	indicies.push_back(nVerticies);
	indicies.push_back(nVerticies + 1);
	indicies.push_back(nVerticies + 2);
	indicies.push_back(nVerticies + 2);
	indicies.push_back(nVerticies + 3);
	indicies.push_back(nVerticies);
}

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
	AssimpModel model;
	model.meshes.push_back(MeshFactory<VertexData>::createMesh(verticies, indicies,
		{ ResourceManager::Instance.getTexture(textureName) }, deviceResources));

	ResourceManager::Instance.addModel(model, modelName);

}

void ResourceHelper::generateDoorFrame(std::vector<VertexData>& vertexData, std::vector<unsigned short>& indicies,
	DirectX::XMFLOAT3 bottomLeft, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 offset, int stepH, int stepY, int nStepH, int nStepY)
{
	int nVerticies = vertexData.size();
	// left frame

	vertexData.push_back(VertexData({ bottomLeft.x, bottomLeft.y, bottomLeft.z },
		DirectX::XMFLOAT2((float)stepH / nStepH, (float)stepY / nStepY), { 1, 0, 0 }));
	vertexData.push_back(VertexData({ bottomLeft.x + offset.x, bottomLeft.y, bottomLeft.z + offset.z },
		DirectX::XMFLOAT2((float)(stepH + max(offset.x, offset.z)) / nStepH, (float)stepY / nStepY), { 1, 0, 0 }));
	vertexData.push_back(VertexData({ bottomLeft.x + offset.x, bottomLeft.y + size.y, bottomLeft.z + offset.z },
		DirectX::XMFLOAT2((float)(stepH + max(offset.x, offset.z)) / nStepH, (float)(stepY + size.y) / nStepY), { 1, 0, 0 }));
	vertexData.push_back(VertexData({ bottomLeft.x, bottomLeft.y + size.y, bottomLeft.z },
		DirectX::XMFLOAT2((float)stepH / nStepH, (float)(stepY + size.y) / nStepY), { 1, 0, 0 }));

	// right frame

	vertexData.push_back(VertexData({ bottomLeft.x + size.x + offset.x, bottomLeft.y, bottomLeft.z + size.z + offset.z },
		DirectX::XMFLOAT2((float)(stepH + max(size.x, size.z) - max(offset.x, offset.z)) / nStepH, (float)stepY / nStepY), { -1, 0, 0 }));
	vertexData.push_back(VertexData({ bottomLeft.x + size.x, bottomLeft.y, bottomLeft.z + size.z },
		DirectX::XMFLOAT2((float)(stepH + max(size.x, size.z)) / nStepH, (float)stepY / nStepY), { -1, 0, 0 }));
	vertexData.push_back(VertexData({ bottomLeft.x + size.x, bottomLeft.y + size.y, bottomLeft.z + size.z },
		DirectX::XMFLOAT2((float)(stepH + max(offset.x, offset.z)) / nStepH, (float)(stepY + size.y) / nStepY), { -1, 0, 0 }));
	vertexData.push_back(VertexData({ bottomLeft.x + size.x + offset.x, bottomLeft.y + size.y, bottomLeft.z + size.z + offset.z },
		DirectX::XMFLOAT2((float)(stepH + max(size.x, size.z) - max(offset.x, offset.z)) / nStepH, (float)(stepY + size.y) / nStepY), { -1, 0, 0 }));

	for (int i = 0; i < 2; i++)
	{
		indicies.push_back(nVerticies);
		indicies.push_back(nVerticies + 1);
		indicies.push_back(nVerticies + 2);
		indicies.push_back(nVerticies + 2);
		indicies.push_back(nVerticies + 3);
		indicies.push_back(nVerticies);

		nVerticies += 4;
	}

}

Mesh ResourceHelper::generateRoomModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size,
	std::vector<RoomLinkData> links,
	const std::string& texturePath, const std::shared_ptr<DX::DeviceResources>& deviceResources)
{


	std::vector<VertexData> vertexData;
	std::vector<unsigned short> indicies;

	ResourceHelper::generateWall(vertexData, indicies, links,
		{ pos.x, pos.y, pos.z }, { pos.x + size.x, pos.y + size.y, pos.z }, { 0.f, 0.f, 1.f }, { 0, 0, wallOffset }, true);

	// left wall
	ResourceHelper::generateWall(vertexData, indicies, links,
		{ pos.x, pos.y, pos.z + size.z }, { pos.x, pos.y + size.y, pos.z }, { 1.f, 0.f, 0.f }, { wallOffset, 0, 0 }, false);

	// back wall
	ResourceHelper::generateWall(vertexData, indicies, links,
		{ pos.x + size.x, pos.y, pos.z + size.z }, { pos.x, pos.y + size.y, pos.z + size.z }, { 0.f, 0.f, -1.f }, { 0, 0, -wallOffset }, true);

	// right wall
	ResourceHelper::generateWall(vertexData, indicies, links,
		{ pos.x + size.x, pos.y, pos.z }, { pos.x + size.x, pos.y + size.y, pos.z + size.z }, { -1.f, 0.f, 0.f }, { -wallOffset, 0, 0 }, false);

	return MeshFactory<VertexData>::createMesh(vertexData, indicies, { ResourceManager::Instance.getTexture(texturePath) }, deviceResources);

}
