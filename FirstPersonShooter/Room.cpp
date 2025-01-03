#include "pch.h"
#include "Room.hpp"
#include "ResourceManager.h"
#include "Entity.hpp"
#include "MeshFactory.h"
#include "Stairs.h"

const float Room::wallOffset = 0.5f;

Room::Room(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, int base_enemy_count, const std::vector<RoomLinkData>& links, const std::vector<PropInstance>& props, 
	const std::vector<Gun>& guns)
	: pos(pos), size(size), m_links(links), base_enemy_count(base_enemy_count), m_props(props), m_guns(guns)
{
}

void Room::setModel(Model model)
{
	m_roomWalls = std::make_shared<Entity>(std::make_shared<Model>(model));
}

RoomCollision Room::checkCollision(DirectX::XMFLOAT3 entityPos) const
{
	// mapping:
	// 0 - x, 1 - y, 2 - z

	RoomCollision result;
	
	const float playerHeight = 1.f;
	if (entityPos.y < this->pos.y + playerHeight)
	{
		result.collision[1] = true;
		result.correction[1] = this->pos.y + playerHeight;
		result.isOnGround = true;
	}
	//else if (entityPos.y > this->pos.y + this->size.y - playerHeight)
	//{
	//	result.collision[1] = true;
	//	result.correction[1] = this->pos.y + this->size.y - playerHeight;
	//}
	
	if (entityPos.x < this->pos.x + wallOffset)
	{
		result.collision[0] = true;
		result.correction[0] = this->pos.x + wallOffset;
	}
	else if (entityPos.x > this->pos.x + this->size.x - wallOffset)
	{
		result.collision[0] = true;
		result.correction[0] = this->pos.x + this->size.x - wallOffset;
	}
	
	if (entityPos.z < this->pos.z + wallOffset)
	{
		result.collision[2] = true;
		result.correction[2] = this->pos.z + wallOffset;
	}
	else if (entityPos.z > this->pos.z + this->size.z - wallOffset)
	{
		result.collision[2] = true;
		result.correction[2] = this->pos.z + this->size.z - wallOffset;
	}

	for (auto link : m_links)
	{
		bool stairs = link.orientation == OrientationData::XZX || link.orientation == OrientationData::XZZ;
		bool alongX = !stairs && link.orientation == OrientationData::XY;

		if (stairs)
		{
			float height = link.pos.y == pos.y ? pos.y : pos.y - link.size.y;
			if (height != pos.y)
			{
				int c = 2;
			}

			if (entityPos.x >= link.pos.x && entityPos.x <= link.pos.x + link.size.x &&
				entityPos.z >= link.pos.z && entityPos.z <= link.pos.z + link.size.z)
			{
				result.collision[1] = false;
				result.correction[1] = 0.f;
				result.isOnGround = false;
			}

			Stairs::AddStairsCollision(result, { link.pos.x, height, link.pos.z }, { link.pos.x + link.size.x, height + link.size.y, link.pos.z + link.size.z }, entityPos);
			// if player on stairs - dont collide with the floor

			continue;
		}

		if (alongX)
		{
			if (result.collision[2] && std::abs(entityPos.z - link.pos.z) < wallOffset && entityPos.x > link.pos.x && entityPos.x < link.pos.x + link.size.x)
			{
				result.collision[2] = false;
				result.correction[2] = 0.f;
			}
		}
		else
		{
			if (result.collision[0] && std::abs(entityPos.x - link.pos.x) < wallOffset && entityPos.z > link.pos.z && entityPos.z < link.pos.z + link.size.z)
			{
				result.collision[0] = false;
				result.correction[0] = 0.f;
			}
		}
	}

	return result;
}


bool Room::insideRoom(DirectX::XMFLOAT3 pos) const
{
	return pos.x > this->pos.x && pos.x < this->pos.x + this->size.x &&
		pos.y >= this->pos.y && pos.y <= this->pos.y + this->size.y &&
		pos.z > this->pos.z && pos.z < this->pos.z + this->size.z;
}

void Room::Render(std::shared_ptr<RenderMaster> renderMaster)
{
	auto ceilingModel = ResourceManager::Instance().getModel("wall");
	auto floorModel = ResourceManager::Instance().getModel("floor");
	auto stairsModel = ResourceManager::Instance().getModel("stairs");
	auto renderer = renderMaster->getModelRenderer();

	// walls and floors
	m_roomWalls->Render(renderMaster);

	// stairs
	for (auto& link : m_links)
	{
		bool stairs = link.orientation == OrientationData::XZX || link.orientation == OrientationData::XZZ;
		if (stairs && pos.y == link.pos.y)
		{
			if (link.orientation == OrientationData::XZX)
			{
				renderer->Render(*stairsModel, { link.pos.x + link.size.x, pos.y, link.pos.z + link.size.z }, { 5, size.y, 2 }, { 0, DirectX::XM_PI, 0 });
			}
			else
			{
				renderer->Render(*stairsModel, { link.pos.x, pos.y, link.pos.z + link.size.z }, { 5, size.y, 2 }, { 0, DirectX::XM_PIDIV2, 0 });
			}
		}
	}

	// props
	for (const auto& prop : m_props)
	{
		renderer->Render(*ResourceManager::Instance().getModel(prop.name), prop.position, prop.size, prop.rotation);
	}
}

std::vector<int> Room::getAdjacentRooms() const
{
	std::vector<int> adjacentIds;
	for (auto link : m_links)
		adjacentIds.push_back(link.roomId);

	return adjacentIds;
}
