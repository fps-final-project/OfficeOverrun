#include "pch.h"
#include "World.h"

void World::update(float dt)
{
	for (auto& entity : m_entities)
	{
		entity.Update(dt);
	}
}
