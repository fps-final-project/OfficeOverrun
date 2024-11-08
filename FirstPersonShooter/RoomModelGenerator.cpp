#include "pch.h"
#include "RoomModelGenerator.h"
#include "Content\ShaderStructures.h"
#include <set>
#include "MeshFactory.h"
#include "ResourceManager.h"

const float RoomModelGenerator::frameOffset = 0.1f;

Model RoomModelGenerator::generateRoomModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, std::vector<RoomLinkData> links, const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	Model model;
	std::vector<VertexData> verticies;
	std::vector<unsigned short> indicies;

	generateWall(verticies, indicies, { pos.x, pos.y, pos.z }, { pos.x + size.x, pos.y + size.y, pos.z }, links, 0);
	generateWall(verticies, indicies, { pos.x, pos.y, pos.z + size.z }, { pos.x, pos.y + size.y, pos.z }, links, 3 * DirectX::XM_PIDIV2);
	generateWall(verticies, indicies, { pos.x + size.x, pos.y, pos.z + size.z }, { pos.x, pos.y + size.y, pos.z + size.z }, links, DirectX::XM_PI);
	generateWall(verticies, indicies, { pos.x + size.x, pos.y, pos.z }, { pos.x + size.x, pos.y + size.y, pos.z + size.z }, links, DirectX::XM_PIDIV2);
	generateFloor(verticies, indicies, { pos.x, pos.y + size.y, pos.z }, { pos.x + size.x, pos.y + size.y, pos.z + size.z }, links, true);

	model.meshes.push_back(MeshFactory<VertexData>::createMesh(verticies, indicies, { ResourceManager::Instance.getTexture("wall")}, deviceResources));

	verticies.clear();
	indicies.clear();
	
	generateFloor(verticies, indicies, { pos.x, pos.y, pos.z }, { pos.x + size.x, pos.y, pos.z + size.z }, links, false);
	model.meshes.push_back(MeshFactory<VertexData>::createMesh(verticies, indicies, { ResourceManager::Instance.getTexture("floor")}, deviceResources));



	return model;
}

void RoomModelGenerator::generateWall(std::vector<VertexData>& allVerticies, std::vector<unsigned short>& allIndicies,
	DirectX::XMFLOAT3 c1, DirectX::XMFLOAT3 c2, std::vector<RoomLinkData> links, float rotationAngle)
{
	std::vector<VertexData> verticies;
	std::vector<unsigned short> indicies;

	bool alongX = c1.x - c2.x;
	float direction = alongX ? (c1.x - c2.x) / std::abs(c1.x - c2.x) : (c1.z - c2.z) / std::abs(c1.z - c2.z);

	int L = max(std::abs(c1.x - c2.x), std::abs(c1.z - c2.z));
	int H = c2.y;

	std::set<int> doorStart;

	for (auto& link : links)
	{
		if (link.alongX == alongX)
		{
			if ((link.alongX && link.pos.z == c1.z) || (!link.alongX && link.pos.x == c2.x))
			{
				doorStart.insert(MapDoorPositionTo1D(link.pos, c1, c2, alongX, direction));
			}
		}
	}

	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < H; j++)
		{
			if (doorStart.find(i) != doorStart.end() && j < 2)
			{
				if (j == 0)
				{
					generateDoorFrame(verticies, indicies, allVerticies.size(), i, L + 1, H + 1);
				}
			}
			else
			{
				float startX = i == 0 ? frameOffset : i;
				float sizeX = (i == 0 || i == L - 1) ? 1 - frameOffset : 1;
				generateTile(verticies, indicies, startX, j, sizeX, L + 1, H + 1, allVerticies.size());
			}
		}
	}

	translateAndRotateVerticies(verticies, { c1.x + !alongX * direction * frameOffset, c1.y, c1.z - alongX * direction * frameOffset },
		rotationAngle);

	allVerticies.insert(allVerticies.end(), verticies.begin(), verticies.end());
	allIndicies.insert(allIndicies.end(), indicies.begin(), indicies.end());
}

void RoomModelGenerator::generateTile(std::vector<VertexData>& verticies, std::vector<unsigned short>& indicies,
	float x, float y, float sizeX, int L, int H, int indexOffset)
{
	int nVerticies = verticies.size();

	verticies.push_back(VertexData({ x, y, 0 }, { x / L, y / H }, { 0, 0, 1 }));
	verticies.push_back(VertexData({ x + sizeX, y, 0 }, { (x + sizeX) / L, y / H }, { 0, 0, 1 }));
	verticies.push_back(VertexData({ x + sizeX, y + 1, 0 }, { (x + sizeX) / L, (y + 1) / H }, { 0, 0, 1 }));
	verticies.push_back(VertexData({ x, y + 1, 0 }, { x / L, (y + 1) / H }, { 0, 0, 1 }));

	indicies.push_back(indexOffset + nVerticies);
	indicies.push_back(indexOffset + nVerticies + 1);
	indicies.push_back(indexOffset + nVerticies + 2);
	indicies.push_back(indexOffset + nVerticies + 2);
	indicies.push_back(indexOffset + nVerticies + 3);
	indicies.push_back(indexOffset + nVerticies);

}

void RoomModelGenerator::generateDoorFrame(std::vector<VertexData>& verticies, std::vector<unsigned short>& indicies,
	int indexOffset, float x, int L, int H)
{
	float nVerticies = verticies.size();

	// left side
	verticies.push_back(VertexData({ x, 0, 0 }, { x / L, 0 }, { 1, 0, 0 }));
	verticies.push_back(VertexData({ x, 0, -frameOffset }, { (x - frameOffset) / L, 0 }, { 1, 0, 0 }));
	verticies.push_back(VertexData({ x, 2, -frameOffset }, { (x - frameOffset) / L, (float)2 / H }, { 1, 0, 0 }));
	verticies.push_back(VertexData({ x, 2, 0 }, { x / L, (float)2 / H }, { 1, 0, 0 }));

	// right side
	verticies.push_back(VertexData({ x + 1, 0, -frameOffset }, { (x + 1 - frameOffset) / L, 0 }, { -1, 0, 0 }));
	verticies.push_back(VertexData({ x + 1, 0, 0 }, { (x + 1) / L, 0 }, { -1, 0, 0 }));
	verticies.push_back(VertexData({ x + 1, 2, 0 }, { (x + 1) / L, (float)2 / H }, { -1, 0, 0 }));
	verticies.push_back(VertexData({ x + 1, 2, -frameOffset }, { (x + 1 - frameOffset) / L, (float)2 / H }, { -1, 0, 0 }));

	// top side
	verticies.push_back(VertexData({ x, 2, -frameOffset }, { (x - frameOffset) / L, (float)2 / H  }, { 0, -1, 0 }));
	verticies.push_back(VertexData({ x, 2, 0 }, { x / L, (float)2 / H }, { 0, -1, 0 }));
	verticies.push_back(VertexData({ x + 1, 2, 0 }, { (x + 1) / L, (float)2 / H }, { 0, -1, 0 }));
	verticies.push_back(VertexData({ x + 1, 2, -frameOffset }, { (x + 1 - frameOffset) / L, (float)2 / H }, { 0, -1, 0 }));

	for (int i = 0; i < 2; i++)
	{
		indicies.push_back(indexOffset + nVerticies);
		indicies.push_back(indexOffset + nVerticies + 1);
		indicies.push_back(indexOffset + nVerticies + 2);
		indicies.push_back(indexOffset + nVerticies + 2);
		indicies.push_back(indexOffset + nVerticies + 3);
		indicies.push_back(indexOffset + nVerticies);

		nVerticies += 4;
	}

}

void RoomModelGenerator::translateAndRotateVerticies(std::vector<VertexData>& verticies, DirectX::XMFLOAT3 translation, float angle)
{
	float cosAngle = cos(angle);
	float sinAngle = sin(angle);

	std::transform(verticies.begin(), verticies.end(), verticies.begin(), [&](VertexData& data) {
		data.pos = { data.pos.x * cosAngle - data.pos.z * sinAngle, data.pos.y, data.pos.z * cosAngle + data.pos.x * sinAngle };
		data.pos = translate(data.pos, translation);
		data.normal = { data.normal.x * cosAngle - data.normal.z * sinAngle, data.normal.y, data.normal.z * cosAngle + data.normal.x * sinAngle };
		return data;
		});
}

DirectX::XMFLOAT3 RoomModelGenerator::translate(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2)
{
	return DirectX::XMFLOAT3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

int RoomModelGenerator::MapDoorPositionTo1D(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 c1, DirectX::XMFLOAT3 c2, bool alongX, float direction)
{
	if (direction > 0)
		pos = { pos.x + alongX, pos.y, pos.z + !alongX };
	DirectX::XMFLOAT3 posTranslated = translate(pos, { -c1.x, -c1.y, -c1.z });

	return max(std::abs(posTranslated.x), std::abs(posTranslated.z));

}

void RoomModelGenerator::generateFloor(std::vector<VertexData>& allVerticies, std::vector<unsigned short>& allIndicies, DirectX::XMFLOAT3 c1, DirectX::XMFLOAT3 c2, std::vector<RoomLinkData> links, bool isRoof)
{
	std::vector<VertexData> verticies;
	std::vector<unsigned short> indicies;

	int L = c2.x - c1.x, W = c2.z - c1.z;
	int indexOffset = allVerticies.size();

	int vertexWinding[6] = { 0, 1, 2, 2, 3, 0 };
	if (!isRoof)
	{
		vertexWinding[1] = 3;
		vertexWinding[4] = 1;
	}

	RoomLinkData* stairs = nullptr;

	for (auto& link : links)
	{
		if (link.orientation == OrientationData::XZX || link.orientation == OrientationData::XZZ)
		{
			stairs = &link;
		}
	}


	for (int x = 0; x < L; x++)
	{
		for (int z = 0; z < W; z++)
		{
			if (stairs && stairs->pos.y + stairs->size.y == c1.y)
			{
				int x_global = x + c1.x;
				int z_global = z + c1.z;

				if (x_global >= stairs->pos.x && x_global < stairs->pos.x + stairs->size.x &&
					z_global >= stairs->pos.z && z_global < stairs->pos.z + stairs->size.z)
				{
					continue;
				}
			}



			int nVerticies = verticies.size();
			DirectX::XMFLOAT3 normal = { 0, (isRoof ? -1.f : 1.f), 0 };

			int L_tex = isRoof ? L : 3;
			int W_tex = isRoof ? W : 3;

			float x_tex = x % L_tex;
			float z_tex = z % W_tex;


			verticies.push_back(VertexData({ (float)x, 0, (float)z }, { x_tex / L_tex, z_tex / W_tex }, normal));
			verticies.push_back(VertexData({ (float)x + 1, 0, (float)z }, { (x_tex + 1) / L_tex, z_tex / W_tex }, normal));
			verticies.push_back(VertexData({ (float)x + 1, 0, (float)z + 1 }, { (x_tex + 1) / L_tex, (z_tex + 1) / W_tex }, normal));
			verticies.push_back(VertexData({ (float)x, 0, (float)z + 1 }, { x_tex / L_tex, (z_tex + 1) / W_tex }, normal));

			for(int i = 0; i < 6; i++)
				indicies.push_back(indexOffset + nVerticies + vertexWinding[i]);
		}
	}

	std::transform(verticies.begin(), verticies.end(), verticies.begin(), [&](VertexData& data) {
		data.pos = translate(data.pos, c1);
		return data;
		});


	allVerticies.insert(allVerticies.end(), verticies.begin(), verticies.end());
	allIndicies.insert(allIndicies.end(), indicies.begin(), indicies.end());
}
