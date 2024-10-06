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
	renderer->Render(Entity(model, pos));
}
