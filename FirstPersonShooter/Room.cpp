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
	//renderer->Render(Entity(model, { pos.x, pos.y, pos.z }, { size.x, size.y, 1.f}));
	this->renderWall(model, renderer, { pos.x, pos.y, pos.z }, { pos.x + size.x, pos.y + size.y, pos.z }, { 0.f, 0.f, 0.f }, true);

	// left wall
	//renderer->Render(Entity(model, { pos.x, pos.y, pos.z + size.z }, { size.z, size.y, 1.f }, {0.f, DirectX::XM_PIDIV2, 0.f}));
	this->renderWall(model, renderer, { pos.x, pos.y, pos.z + size.z }, { pos.x, pos.y + size.y, pos.z }, { 0.f, DirectX::XM_PIDIV2, 0.f }, false);

	// back wall
	//renderer->Render(Entity(model, { pos.x + size.x, pos.y, pos.z + size.z }, { size.x, size.y, 1.f }, {0.f, DirectX::XM_PI, 0.f}));
	this->renderWall(model, renderer, { pos.x + size.x, pos.y, pos.z + size.z }, { pos.x, pos.y + size.y, pos.z + size.z }, { 0.f, DirectX::XM_PI, 0.f }, true);

	// right wall
	//renderer->Render(Entity(model, { pos.x + size.x, pos.y, pos.z }, { size.z, size.y, 1.f }, {0.f, -DirectX::XM_PIDIV2, 0.f}));
	this->renderWall(model, renderer, { pos.x + size.x, pos.y, pos.z }, { pos.x + size.x, pos.y + size.y, pos.z + size.z }, { 0.f, -DirectX::XM_PIDIV2, 0.f }, false);

	// bottom wall
	renderer->Render(Entity(model, { pos.x, pos.y, pos.z + size.z }, { size.x, size.z, 1.f }, { -DirectX::XM_PIDIV2, 0.f, 0.f }));
	//renderer->Render(Entity(model, { pos.x + size.x, pos.y, pos.z }, { size.z, size.y, 1.f }, {0.f, -DirectX::XM_PIDIV2, 0.f}));
}

void Room::renderWall(std::shared_ptr<AssimpModel> model, std::shared_ptr<ModelRenderer> modelRenderer,
	DirectX::XMFLOAT3 bottomLeft, DirectX::XMFLOAT3 topRight, DirectX::XMFLOAT3 rotation, bool alongX)
{
	// check if there is a door on that particular wall
	RoomLinkData* data = nullptr;
	for (auto& link : m_links)
	{
		if (link.alongX == alongX)
		{
			if ((link.alongX && link.pos.z == bottomLeft.z) || (!link.alongX && link.pos.x == topRight.x))
			{
				data = &link;
				break;
			}
		}
	}

	DirectX::XMFLOAT3 tileStep = {
		alongX ? (topRight.x - bottomLeft.x < 0 ? -1.f : 1.f) : 0.f,
		1.f,
		alongX ? 0.f : (topRight.z - bottomLeft.z < 0 ? -1.f : 1.f),
	};

	int nStepsY = topRight.y - bottomLeft.y;
	int nStepsH = alongX ? std::abs(topRight.x - bottomLeft.x) - 0.5f : std::abs(topRight.z - bottomLeft.z) - 0.5f;

	auto overlapsDoor = [&](DirectX::XMFLOAT3 tilePos, DirectX::XMFLOAT3 tileTopRight) {
		// overlaps in y coordinate
		if (data && data->size.y - tilePos.y > 0.5f)
		{
			if (alongX)
			{
				if (tilePos.x >= data->pos.x && tilePos.x <= data->pos.x + data->size.x &&
					tileTopRight.x >= data->pos.x && tileTopRight.x <= data->pos.x + data->size.x)
				{
					return true;
				}

			}
			else
			{
				if (tilePos.z >= data->pos.z && tilePos.z <= data->pos.z + data->size.z &&
					tileTopRight.z >= data->pos.z && tileTopRight.z <= data->pos.z + data->size.z)
				{
					return true;
				}
			}
		}

		return false;
		};

	for (int stepY = 0; stepY < nStepsY; stepY++)
	{
		for (int stepH = 0; stepH <= nStepsH; stepH++)
		{
			// if there is a door and it overlaps with current tile - dont draw the tile
			DirectX::XMFLOAT3 tilePos = { bottomLeft.x + tileStep.x * stepH, bottomLeft.y + tileStep.y * stepY, bottomLeft.z + tileStep.z * stepH };
			if (overlapsDoor(tilePos, { tilePos.x + tileStep.x, tilePos.y + tileStep.y, tilePos.z + tileStep.z }))
				continue;
			modelRenderer->Render(Entity(model, tilePos, { 1.f, 1.f, 1.f }, rotation));
		}
	}

}

std::vector<int> Room::getAdjacentRooms()
{
	std::vector<int> adjacentIds;
	for (auto link : m_links)
		adjacentIds.push_back(link.roomId);

	return adjacentIds;
}
