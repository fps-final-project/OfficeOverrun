#pragma once
#include <DirectXMath.h>

struct RoomCollision;

class Stairs
{
public:
	static void AddStairsCollision(RoomCollision& collisionData, DirectX::XMFLOAT3 c1, DirectX::XMFLOAT3 c2, DirectX::XMFLOAT3 pos);
};