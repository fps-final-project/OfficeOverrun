#include "pch.h"
#include "Enemy.hpp"

Enemy::Enemy(std::shared_ptr<AnimatedModel> model) : AnimatedEntity{model}
{
}

Action Enemy::Update(const Room& room, const std::vector<Room>& rooms, XMFLOAT3 playerPosition)
{
	Action currentAction;
	std::vector<int> indexes = room.getAdjacentRooms();

	int currentRoomIndex = -1;
	bool inAdjacent = std::any_of(indexes.begin(), indexes.end(), [&](int index) {
		if (rooms[index].insideRoom(position))
		{
			currentRoomIndex = index;
			return true;
		}
		return false;
		});

	bool inCurrent = room.insideRoom(position);
	if(!inCurrent && !inAdjacent) return currentAction;
	XMVECTOR direction = {
		playerPosition.x - position.x,
		0,
		playerPosition.z - position.z
	};
	float l = XMVector3Length(direction).m128_f32[0];
	direction = XMVector3Normalize(direction);

	float dx = XMVectorGetX(direction);
	float dz = XMVectorGetZ(direction);

	float yaw = atan2(dx, dz);

	setRotation({0.0f, yaw, 0.0f});
	
	if (!isIdle()) return currentAction;
	
	if (l <= radius)
	{
		int id = (rand() % 2) + 1;
		setAnimation( "attack" + std::to_string(id), 1.5);
		currentAction = Action{ ActionType::ATTACK, {damage} };
	}


	XMFLOAT3 changedPos = XMFLOAT3{
		position.x + direction.m128_f32[0] * speed,
		position.y,
		position.z + direction.m128_f32[2] * speed,
	};

	setPosition(changedPos);
	return currentAction;
}
