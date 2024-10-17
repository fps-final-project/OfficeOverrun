#include "pch.h"
#include "Room.hpp"
#include "ResourceManager.h"
#include "Entity.hpp"

const float Room::wallOffset = 0.5f;

Room::Room(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, const std::vector<RoomLinkData>& links)
	: pos(pos), size(size), m_links(links)
{
}

RoomCollision Room::checkCollision(DirectX::XMFLOAT3 entityPos) const
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

	for (auto link : m_links)
	{
		if (link.alongX)
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
		pos.y > this->pos.y && pos.y < this->pos.y + this->size.y &&
		pos.z > this->pos.z && pos.z < this->pos.z + this->size.z;
}

void Room::Render(std::shared_ptr<RenderMaster> renderMaster)
{
	auto model = ResourceManager::Instance.getModel("wall");
	auto renderer = renderMaster->getModelRenderer();

	// front wall
	renderer->Render(Entity(model, { pos.x, pos.y, pos.z }, { size.x, size.y, 1.f}));
	//this->renderWall(model, renderer, { pos.x, pos.y, pos.z }, size, { 0.f, 0.f, 0.f }, true);

	// left wall
	renderer->Render(Entity(model, { pos.x, pos.y, pos.z + size.z }, { size.z, size.y, 1.f }, {0.f, DirectX::XM_PIDIV2, 0.f}));
	//this->renderWall(model, renderer, { pos.x, pos.y, pos.z + size.z }, size, { 0.f, DirectX::XM_PIDIV2, 0.f }, false);

	// back wall
	renderer->Render(Entity(model, { pos.x + size.x, pos.y, pos.z + size.z }, { size.x, size.y, 1.f }, {0.f, DirectX::XM_PI, 0.f}));
	//this->renderWall(model, renderer, { pos.x + size.x, pos.y, pos.z + size.z }, size, { 0.f, DirectX::XM_PI, 0.f }, true);

	// right wall
	renderer->Render(Entity(model, { pos.x + size.x, pos.y, pos.z }, { size.z, size.y, 1.f }, {0.f, -DirectX::XM_PIDIV2, 0.f}));
	//this->renderWall(model, renderer, { pos.x + size.x, pos.y, pos.z }, size, { 0.f, -DirectX::XM_PIDIV2, 0.f }, false);

	// bottom wall
	renderer->Render(Entity(model, { pos.x, pos.y, pos.z + size.z }, { size.x, size.z, 1.f }, { -DirectX::XM_PIDIV2, 0.f, 0.f }));
	//renderer->Render(Entity(model, { pos.x + size.x, pos.y, pos.z }, { size.z, size.y, 1.f }, {0.f, -DirectX::XM_PIDIV2, 0.f}));
}

void Room::renderWall(std::shared_ptr<AssimpModel> model, std::shared_ptr<ModelRenderer> modelRenderer,
	DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotation, bool alongX)
{
	RoomLinkData* data = nullptr;
	for (auto& link : m_links)
	{
		if (link.alongX == alongX)
		{
			if ((link.alongX && link.pos.z == position.z) || (!link.alongX && link.pos.x == position.x))
			{
				data = &link;
				break;
			}
		}
	}

	DirectX::XMFLOAT3 scaling = { alongX ? size.x : size.z, size.y, 1.f };

	if (data)
	{
		modelRenderer->Render(Entity(model,
			{ position.x, position.y + data->size.y, position.z },
			{ scaling.x, scaling.y - data->size.y, scaling.z }, rotation));


		if (alongX)
		{
			float leftSize = data->pos.x > position.x ? data->pos.x - position.x : position.x - data->pos.x - 1;
			float rightSize = scaling.x - leftSize - 1;


			if (leftSize)
			{
				if (data->pos.x > position.x)
				{
					modelRenderer->Render(Entity(model,
						{ position.x, position.y, position.z },
						{ leftSize, data->size.y, scaling.z }, rotation));
				}
				else
				{
					modelRenderer->Render(Entity(model,
						{ data->pos.x + 1, position.y, position.z },
						{ leftSize, data->size.y, scaling.z }, rotation));
				}
			}
			/*if (rightSize)
			{
				if (data->pos.x > position.x)
				{
					modelRenderer->Render(Entity(model,
						{ position.x + leftSize + 1, position.y, position.z },
						{ rightSize, data->size.y, scaling.z }, rotation));
				}
				else
				{
					modelRenderer->Render(Entity(model,
						{ position.x - leftSize - 1, position.y, position.z },
						{ rightSize, data->size.y, scaling.z }, rotation));
				}
			}*/


		}
		else
		{
			float leftSize = data->pos.z > position.z ? data->pos.z - position.z : position.z - data->pos.z - 1;
			float rightSize = scaling.x - leftSize - 1;


			if (leftSize)
			{
				if (data->pos.z > position.z)
				{
					modelRenderer->Render(Entity(model,
						{ position.x, position.y, position.z },
						{ leftSize, data->size.y, scaling.z }, rotation));
				}
				else
				{
					modelRenderer->Render(Entity(model,
						{ position.x, position.y, position.z - rightSize - 1 },
						{ leftSize, data->size.y, scaling.z }, rotation));
				}
			}
			/*if (rightSize)
			{
				if (data->pos.z > position.z)
				{
					modelRenderer->Render(Entity(model,
						{ position.x, position.y, position.z + leftSize + 1},
						{ rightSize, data->size.y, scaling.z }, rotation));
				}
				else
				{
					modelRenderer->Render(Entity(model,
						{ position.x, position.y, position.z - leftSize - 1 },
						{ rightSize, data->size.y, scaling.z }, rotation));
				}
			}*/
		}
	}
	else
	{
		modelRenderer->Render(Entity(model, position, scaling, rotation));
	}
}

std::vector<int> Room::getAdjacentRooms()
{
	std::vector<int> adjacentIds;
	for (auto link : m_links)
		adjacentIds.push_back(link.roomId);

	return adjacentIds;
}
