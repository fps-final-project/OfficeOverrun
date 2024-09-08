#include "pch.h"
#include "World.h"

void World::Update(float dt)
{
	for (auto& entity : m_entities)
	{
		entity.Update(dt);
	}

	std::for_each(
		m_timedEntities.begin(),
		m_timedEntities.end(),
		[dt](std::pair<Entity, float>& pair) { pair.second -= dt; }
	);

	m_timedEntities.erase(
		std::remove_if(
			m_timedEntities.begin(),
			m_timedEntities.end(),
			[](std::pair<Entity, float>& pair) { return pair.second <= 0; }),
		m_timedEntities.end()
	);

	std::for_each(
		m_timedEntities.begin(),
		m_timedEntities.end(),
		[dt](std::pair<Entity, float>& pair) { pair.first.Update(dt); }
	);

	for (auto& entity : m_animatedEntities)
	{
		entity.Update(dt);
	}
}

std::vector<Hittable> World::GetEntities()
{
	std::vector<Hittable> entities{};
	int size = m_entities.size() + m_animatedEntities.size() + m_timedEntities.size();
	entities.reserve(size);
	entities.insert(entities.end(), m_entities.begin(), m_entities.end());
	entities.insert(entities.end(), m_animatedEntities.begin(), m_animatedEntities.end());
	std::for_each(
		m_timedEntities.begin(),
		m_timedEntities.end(),
		[&entities](std::pair<Entity, float>& pair) { entities.push_back(pair.first); }
	);

	return entities;
}

void World::DeleteEntity(Hittable& entity)
{
	m_entities.erase(
		std::remove_if(
			m_entities.begin(),
			m_entities.end(),
			[entity](Hittable e) { return e.id == entity.id; }),
		m_entities.end()
	);

	m_animatedEntities.erase(
		std::remove_if(
			m_animatedEntities.begin(),
			m_animatedEntities.end(),
			[entity](Hittable e) { return e.id == entity.id; }
		),
		m_animatedEntities.end()
	);

	m_timedEntities.erase(
		std::remove_if(
			m_timedEntities.begin(),
			m_timedEntities.end(),
			[entity](std::pair<Hittable, float> pair) { return pair.first.id == entity.id; }),
		m_timedEntities.end()
	);
}
