#pragma once
#include <DirectXMath.h>

namespace WorldGenerator
{
	class SpaceTransformationHelper
	{
	public:
		static DirectX::XMFLOAT3 TransformToInternalSpace(DirectX::XMFLOAT3 vec);
	};
}

