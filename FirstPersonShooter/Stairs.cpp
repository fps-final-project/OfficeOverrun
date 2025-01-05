#include "pch.h"
#include "Stairs.h"
#include "RoomCollision.hpp"

void Stairs::AddStairsCollision(RoomCollision& collisionData, DirectX::XMFLOAT3 c1, DirectX::XMFLOAT3 c2, DirectX::XMFLOAT3 pos)
{
	RoomCollision stairsCollision;

	float sideOffset = 0.1f;
	const float playerHeight = 1.f;

	bool alongX = std::abs(c1.x - c2.x) > std::abs(c1.z - c2.z);
	float length = alongX * std::abs(c1.x - c2.x) + !alongX * std::abs(c1.z - c2.z);
	float slope = (c2.y - c1.y) / length;

	float t = alongX ? pos.x - c1.x : pos.z - c1.z;
	float thresholdHeight = c1.y + t * slope + playerHeight;
	const float error = 0.2f;

	float interruptX = 100.f, interruptZ = 100.f;

	c1.x -= sideOffset;
	c2.x += sideOffset;

	c1.z -= sideOffset;
	c2.z += sideOffset;

	bool inBounds = pos.x >= c1.x && pos.x <= c2.x && pos.z >= c1.z && pos.z <= c2.z;

	if (inBounds && pos.y > c1.y)
		collisionData.collision[1] = false;

	if (pos.y < thresholdHeight - error)
	{
		if (pos.x > c1.x && pos.x < c2.x)
		{
			interruptZ = min(std::abs(pos.z - c1.z), std::abs(pos.z - c2.z));
			if (pos.z > c1.z && pos.z < (c1.z + c2.z) / 2)
			{
				stairsCollision.collision[2] = true;
				stairsCollision.correction[2] = c1.z;
			}
			else if (pos.z < c2.z && pos.z > (c1.z + c2.z) / 2)
			{
				stairsCollision.collision[2] = true;
				stairsCollision.correction[2] = c2.z;
			}
		}

		if (pos.z > c1.z && pos.z < c2.z)
		{
			interruptX = min(std::abs(pos.x - c1.x), std::abs(pos.x - c2.x));
			if (pos.x > c1.x && pos.x < (c1.x + c2.x) / 2)
			{
				stairsCollision.collision[0] = true;
				stairsCollision.correction[0] = c1.x;
			}
			else if (pos.x < c2.x && pos.x >(c1.x + c2.x) / 2)
			{
				stairsCollision.collision[0] = true;
				stairsCollision.correction[0] = c2.x;
			}
		}
	}
	else
	{
		if (inBounds && pos.y < thresholdHeight)
		{
			stairsCollision.collision[1] = true;
			stairsCollision.correction[1] = thresholdHeight;
			stairsCollision.isOnGround = true;
		}
	}

	stairsCollision.collision[0] &= interruptX < interruptZ;
	stairsCollision.collision[2] &= !(interruptX < interruptZ);

	for (int i = 0; i < 3; i++)
	{
		if (!collisionData.collision[i] || (i == 1 && stairsCollision.collision[i]))
		{
			collisionData.collision[i] = stairsCollision.collision[i];
			collisionData.correction[i] = stairsCollision.correction[i];
			if (i == 1)
				collisionData.isOnGround = stairsCollision.isOnGround;
		}
	}
}
