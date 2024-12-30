#include "pch.h"
#include "SpaceTransformationHelper.h"

using namespace WorldGenerator;

DirectX::XMFLOAT3 WorldGenerator::SpaceTransformationHelper::TransformToInternalSpace(DirectX::XMFLOAT3 vec)
{
	return DirectX::XMFLOAT3(vec.x, vec.z, vec.y);
}
