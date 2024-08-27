#include "pch.h"
#include "World.h"

void World::update(float dt, std::queue<AnimatedModelDrawRequest>& m_drawQueue)
{
	for (auto& entity : m_entities)
	{
		m_drawQueue.push(entity.Update(dt));
	}
}
