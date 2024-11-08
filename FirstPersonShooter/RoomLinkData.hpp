#pragma once
#include <DirectXMath.h>

enum OrientationData {
	ZY,
	XY,
	XZX, // X is length
	XZZ // Z is length
};

struct RoomLinkData
{
	int roomId;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 size;
	bool alongX;
	OrientationData orientation;
	bool stairs;
};
