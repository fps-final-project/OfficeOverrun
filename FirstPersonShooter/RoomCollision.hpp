#pragma once 

struct RoomCollision
{
	bool collision[3];
	bool isOnGround;
	float correction[3];

	RoomCollision()
	{
		memset(collision, 0, 3 * sizeof(bool));
		memset(correction, 0, 3 * sizeof(float));
		isOnGround = false;
	}
};
