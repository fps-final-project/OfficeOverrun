#pragma once
#include <DirectXMath.h>

namespace WorldGenerator
{
	struct MeshBox
	{
		DirectX::XMFLOAT2 pos;
		DirectX::XMFLOAT2 size;

		MeshBox(const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& size)
			: pos(pos), size(size)
		{
		}
	};
}