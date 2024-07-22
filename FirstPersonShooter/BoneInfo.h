#pragma once
#include <DirectXMath.h>

struct BoneInfo
{
	int id;
	DirectX::XMFLOAT4X4 offsetMatrix;
};