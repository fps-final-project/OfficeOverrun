#pragma once
#include <DirectXMath.h>

struct RoomLinkData
{
	int roomId;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 size;
	bool alongX;
};
