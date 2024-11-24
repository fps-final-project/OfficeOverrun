#pragma once
#include "AssimpModel.h"
#include "RoomLinkData.hpp"
#include <memory>
#include "Common\DeviceResources.h"

class RoomModelGenerator
{
public:

	static Model generateRoomModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size,
		std::vector<RoomLinkData> links,
		const std::shared_ptr<DX::DeviceResources>& deviceResources);

	static Model generateRoof(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size,
		std::vector<RoomLinkData> links,
		const std::shared_ptr<DX::DeviceResources>& deviceResources);

private:
	static const float frameOffset;

	static void generateWall(std::vector<VertexData>& allVerticies, std::vector<unsigned short>& allIndicies, 
		DirectX::XMFLOAT3 c1, DirectX::XMFLOAT3 c2, std::vector<RoomLinkData> links, float rotationAngle);

	static void generateTile(std::vector<VertexData>& verticies, std::vector<unsigned short>& indicies,
		float x, float y, float sizeX, int L, int H, int indexOffset);

	static void generateDoorFrame(std::vector<VertexData>& verticies, std::vector<unsigned short>& indicies, int indexOffset,
		float x, int L, int H);

	static void translateAndRotateVerticies(std::vector<VertexData>& verticies, DirectX::XMFLOAT3 translation, float angle);

	static DirectX::XMFLOAT3 translate(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
	static int MapDoorPositionTo1D(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 c1, DirectX::XMFLOAT3 c2, bool alongX, float direction);

	static void generateFloor(std::vector<VertexData>& allVerticies, std::vector<unsigned short>& allIndicies,
		DirectX::XMFLOAT3 c1, DirectX::XMFLOAT3 c2, std::vector<RoomLinkData> links, bool isRoof);

};
