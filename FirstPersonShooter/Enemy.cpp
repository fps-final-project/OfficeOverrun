#include "pch.h"
#include "Enemy.hpp"

Enemy::Enemy(std::shared_ptr<AnimatedAssimpModel> model) : AnimatedEntity{model}
{
}
