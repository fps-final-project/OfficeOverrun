#include "pch.h"
#include "Skybox.h"

void Skybox::RenderSkybox(DirectX::XMFLOAT3 cameraPos, std::shared_ptr<RenderMaster> renderMaster, std::shared_ptr<Model> skyboxModel)
{
	auto renderer = renderMaster->getModelRenderer();
	renderer->setFullyVisible();
	renderer->Render(*skyboxModel, cameraPos, { 57.f, 57.f, 57.f }, {0.f, 0.f, 0.f});
	renderer->clearFullyVisible();
}
