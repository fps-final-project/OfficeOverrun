#pragma once
#include "AnimatedEntity.hpp"
#include "ActionTypes.hpp"
#include "Room.hpp"

class  __declspec(dllexport) Enemy : public AnimatedEntity
{
public:
	Enemy(std::shared_ptr<AnimatedModel> model);
	Action Update(const Room& room, const std::vector<Room>& rooms, XMFLOAT3 playerPosition);

	friend class EnemyBuilder;
private:
	int maxHealth;
	int health;
	int damage;
	float speed;
	float radius;
};

