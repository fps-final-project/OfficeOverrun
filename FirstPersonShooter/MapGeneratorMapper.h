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
	static RoomLinkData Map(RoomLink obj);
	template<>
	static DirectX::XMFLOAT3 Map(Vector3 obj);
	template<>
	static OrientationData Map(Orientation obj);
};

template<class T1, class T2>
T2 MapGeneratorMapper::Map(T1 obj)
{
	throw std::invalid_argument("Mapping not implemented");
}

template<>
Room MapGeneratorMapper::Map(GeneratedRoom obj)
{
	DirectX::XMFLOAT3 pos = Map<Vector3, DirectX::XMFLOAT3>(MappingHelpers::MapVector(obj.pos));
	DirectX::XMFLOAT3 size = Map<Vector3, DirectX::XMFLOAT3>(MappingHelpers::MapVector(obj.size));

	std::vector<RoomLinkData> links;
	for (auto link : obj.links)
	{
		links.push_back(Map<RoomLink, RoomLinkData>(link));
	}

	std::vector<PropInstance> props;
	for (const auto& prop : obj.props)
	{
		props.push_back(MappingHelpers::MapPropInstance(prop));
	}

	return Room(pos, size, obj.enemies, links, props);
}

template<>
RoomLinkData MapGeneratorMapper::Map(RoomLink obj)
{
	RoomLinkData data;
	data.pos = MapGeneratorMapper::Map<Vector3, DirectX::XMFLOAT3>(MappingHelpers::MapVector(obj.pos));
	data.orientation = MapGeneratorMapper::Map<Orientation, OrientationData>(obj.orientation);
	data.size = MappingHelpers::OrientationToSize(data.orientation);
	data.roomId = obj.linkedRoomIdx;
	return data;
}

template<>
DirectX::XMFLOAT3 MapGeneratorMapper::Map(Vector3 obj)
{
	return DirectX::XMFLOAT3(obj.x, obj.y, obj.z);
}

template<>
OrientationData MapGeneratorMapper::Map(Orientation obj)
{
	switch (obj)
	{
	case Orientation::XZ:
		return OrientationData::XY;
	case Orientation::YZ:
		return OrientationData::ZY;
	case Orientation::XYX:
		return OrientationData::XZX;
	default:
		return OrientationData::XZZ;
	}
}

