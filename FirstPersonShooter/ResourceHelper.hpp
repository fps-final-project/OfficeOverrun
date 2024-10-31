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
	static const float wallOffset;
	static void generateWall(std::vector<VertexData>& vertexData, std::vector<unsigned short>& indicies,
		std::vector<RoomLinkData> links,
		DirectX::XMFLOAT3 bottomLeft, DirectX::XMFLOAT3 topRight, DirectX::XMFLOAT3 normal, DirectX::XMFLOAT3 wallOffset3f, bool alongX);
	static void generateWallTile(std::vector<VertexData>& vertexData, std::vector<unsigned short>& indicies,
		DirectX::XMFLOAT3 bottomLeft, DirectX::XMFLOAT3 tileStep, DirectX::XMFLOAT3 normal, 
		int stepH, int stepY, int nStepH, int nStepY, float leftOffset, float rightOffset, bool alongX);
	static void generateDoorFrame(std::vector<VertexData>& vertexData, std::vector<unsigned short>& indicies,
		DirectX::XMFLOAT3 bottomLeft, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 wallNormal, int stepH, int stepY, int nStepH, int nStepY);
public:
	static void addQuad(const std::string& texturePath, std::string textureName, std::string modelName, int nSubdivs, const std::shared_ptr<DX::DeviceResources>& deviceResources);
	static Mesh generateRoomModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size,
	std::vector<RoomLinkData> links, const std::string& texturePath,
		const std::shared_ptr<DX::DeviceResources>& deviceResources);
};