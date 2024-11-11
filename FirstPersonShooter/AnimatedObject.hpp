#include "AnimatedEntity.hpp"

#pragma once
class AnimatedObject : public AnimatedEntity
{
public:
	AnimatedObject(std::shared_ptr<AnimatedModel> model);

	friend class AnimatedObjectBuilder;
private:
	bool pickable;
};
