#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Common/DeviceResources.h"
#include "RoomLinkData.hpp"
#include "Content/ShaderStructures.h"

struct Mesh;


class ResourceHelper
{
	static void generateWall(std::vector<VertexData>& vertexData, std::vector<unsigned short>& indicies,
		std::vector<RoomLinkData> links,
		DirectX::XMFLOAT3 bottomLeft, DirectX::XMFLOAT3 topRight, DirectX::XMFLOAT3 normal, bool alongX);
	static void generateWallTile(std::vector<VertexData>& vertexData, std::vector<unsigned short>& indicies,
		DirectX::XMFLOAT3 bottomLeft, DirectX::XMFLOAT3 tileStep, DirectX::XMFLOAT3 normal);
public:
	static void addWallModel(const std::string& texturePath, const std::shared_ptr<DX::DeviceResources>& deviceResources);
	static Mesh generateRoomModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size,
	std::vector<RoomLinkData> links, const std::string& texturePath,
		const std::shared_ptr<DX::DeviceResources>& deviceResources);
};