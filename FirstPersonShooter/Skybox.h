#pragma once

#include <DirectXMath.h>
#include "RenderMaster.hpp"

class __declspec(dllexport) Skybox
{

public:
	static void RenderSkybox(DirectX::XMFLOAT3 cameraPos, std::shared_ptr<RenderMaster> renderMaster, std::shared_ptr<Model> skyboxModel);
};