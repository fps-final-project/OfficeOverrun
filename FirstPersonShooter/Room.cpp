#include "pch.h"
#include "Room.hpp"
#include "ResourceManager.h"
#include "Entity.hpp"

Room::Room(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size)
	: pos(pos), size(size)
{
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
