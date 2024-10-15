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

	return Room(pos, size);
}

template<>
DirectX::XMFLOAT3 MapGeneratorMapper::Map(Vector3 obj)
{
	return DirectX::XMFLOAT3(obj.x, obj.z, obj.y);
}

