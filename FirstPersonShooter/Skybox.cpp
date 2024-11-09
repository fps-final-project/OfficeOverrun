#include "pch.h"
#include "Skybox.h"
#include "ResourceManager.h"
#include "Entity.hpp"

void Skybox::RenderSkybox(DirectX::XMFLOAT3 cameraPos, std::shared_ptr<RenderMaster> renderMaster)
{
	auto model = ResourceManager::Instance.getModel("skybox");
	auto renderer = renderMaster->getModelRenderer();
	renderer->setFullyVisible();
	renderer->Render(Entity(model, cameraPos, { 57.f, 57.f, 57.f }));
	renderer->clearFullyVisible();
}
