#include "pch.h"
#include "Room.hpp"
#include "ResourceManager.h"
#include "Entity.hpp"

const float Room::wallOffset = 0.5f;

Room::Room(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, const std::vector<RoomLinkData>& links)
	: pos(pos), size(size), m_links(links)
{
}

RoomCollision Room::isInBounds(DirectX::XMFLOAT3 entityPos) const
{
	// mapping:
	// 0 - x, 1 - y, 2 - z

	RoomCollision result;
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

	const float playerHeight = 1.f;
	if (entityPos.y < this->pos.y + playerHeight)
	{
		result.collision[1] = true;
		result.correction[1] = this->pos.y + playerHeight;
		result.isOnGround = true;
	}
	else if (entityPos.y > this->pos.y + this->size.y - playerHeight)
	{
		result.collision[1] = true;
		result.correction[1] = this->pos.y + this->size.y - playerHeight;
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

	return result;
}

void Room::Render(std::shared_ptr<RenderMaster> renderMaster)
{
	auto model = ResourceManager::Instance.getModel("wall");
	auto renderer = renderMaster->getModelRenderer();
	// front wall
	renderer->Render(Entity(model, { pos.x, pos.y, pos.z }, { size.x, size.y, 1.f}));

	// left wall
	renderer->Render(Entity(model, { pos.x, pos.y, pos.z + size.z }, { size.z, size.y, 1.f }, {0.f, DirectX::XM_PIDIV2, 0.f}));

	// back wall
	renderer->Render(Entity(model, { pos.x + size.x, pos.y, pos.z + size.z }, { size.x, size.y, 1.f }, {0.f, DirectX::XM_PI, 0.f}));

	// right wall
	renderer->Render(Entity(model, { pos.x + size.x, pos.y, pos.z }, { size.z, size.y, 1.f }, {0.f, -DirectX::XM_PIDIV2, 0.f}));

	// bottom wall
	renderer->Render(Entity(model, { pos.x, pos.y, pos.z + size.z }, { size.x, size.z, 1.f }, {-DirectX::XM_PIDIV2, 0.f, 0.f}));
	//renderer->Render(Entity(model, { pos.x + size.x, pos.y, pos.z }, { size.z, size.y, 1.f }, {0.f, -DirectX::XM_PIDIV2, 0.f}));
}

std::vector<int> Room::getAdjacentRooms()
{
	std::vector<int> adjacentIds;
	for (auto link : m_links)
		adjacentIds.push_back(link.roomId);

	return adjacentIds;
}
