#pragma once
#include <DirectXMath.h>
#include "GeneratedRoom.h"
#include "Room.hpp"
#include "MappingHelpers.h"


using namespace WorldGenerator;

// Class contains mapping methods for pairs of classes from the App and WorldGenerator
class MapGeneratorMapper
{
public:
	template<class T1, class T2>
	static T2 Map(T1 obj);
	template<>
	static Room Map(GeneratedRoom obj);
	template<>
	static RoomLinkData MapGeneratorMapper::Map(RoomLink obj);
	template<>
	static DirectX::XMFLOAT3 Map(Vector3 obj);
};

template<class T1, class T2>
T2 MapGeneratorMapper::Map(T1 obj)
{
	throw std::invalid_argument("Mapping not implemented");
}

template<>
Room MapGeneratorMapper::Map(GeneratedRoom obj)
{
	DirectX::XMFLOAT3 pos = Map<Vector3, DirectX::XMFLOAT3>(MappingHelpers::PositionToGameOrientation(obj.pos));
	DirectX::XMFLOAT3 size = Map<Vector3, DirectX::XMFLOAT3>(obj.size);


	std::vector<RoomLinkData> links;
	for (auto link : obj.links)
	{
		links.push_back(Map<RoomLink, RoomLinkData>(link));
	}

	return Room(pos, size, links);
}

template<>
RoomLinkData MapGeneratorMapper::Map(RoomLink obj)
{
	RoomLinkData data;
	data.pos = MapGeneratorMapper::Map<Vector3, DirectX::XMFLOAT3>(MappingHelpers::PositionToGameOrientation(obj.pos));
	data.alongX = obj.orientation == Orientation::XZ;
	data.stairs = obj.orientation == Orientation::XY;
	data.size = obj.orientation == Orientation::XZ ? DirectX::XMFLOAT3(1, 2, 0) : DirectX::XMFLOAT3(0, 2, 1);
	if (data.stairs)
		data.size = DirectX::XMFLOAT3(2, 0, 5);
	data.roomId = obj.linkedRoomIdx;
	return data;
}

template<>
DirectX::XMFLOAT3 MapGeneratorMapper::Map(Vector3 obj)
{
	return DirectX::XMFLOAT3(obj.x, obj.z, obj.y);
}

