#pragma once
#include <DirectXMath.h>

struct PathNodeData
{
	int index;
	DirectX::XMFLOAT3 position;
	PathNodeData(int _index, DirectX::XMFLOAT3 _pos) : index(_index), position(_pos) {}
};