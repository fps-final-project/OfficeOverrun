#include "Entity.hpp"

#pragma once
class Object : public Entity
{
public:
	Object(std::shared_ptr<AssimpModel> model);

	friend class ObjectBuilder;
private:
	bool pickable;
};

