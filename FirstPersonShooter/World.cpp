#include "pch.h"
#include "World.h"
#include "ctime"

void World::Update(float dt)
{
	for (auto& entity : m_entities)
	{
		entity.Update(dt);
	}

	for (auto& entity : m_timedEntities)
	{
		entity.first.Update(dt);
	}

	if (m_animatedEntities.size() && m_animatedEntities[0].isIdle())
	{
		int idx = (time(NULL) % 2) + 1;
		m_animatedEntities[0].setAnimation("attack" + std::to_string(idx), 1.5f);
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

LightingData World::getLightingData()
{
	LightingData data;
	data.lightPositions.push_back({ 1.5f, 2.f, 2.f });
	return data;
}

RenderQueue World::createRenderQueue()
{
	RenderQueue queue;
	for (const auto& entity : m_entities)
	{
		queue.push(RenderData(RendererType::MODEL, (Drawable*)&entity));
	}

	for (const auto& entity : m_timedEntities)
	{
		queue.push(RenderData(RendererType::MODEL, (Drawable*)&entity.first));
	}

	for (const auto& entity : m_animatedEntities)
	{
		queue.push(RenderData(RendererType::ANIMATED, (Drawable*)&entity));
	}

	for (const auto& entity : m_rooms)
	{
		queue.push(RenderData(RendererType::MODEL, (Drawable*)&entity));
	}

	return queue;
}
