#pragma once

#include <DirectXMath.h>
#include "RenderMaster.hpp"

class Skybox
{

public:
	static void RenderSkybox(DirectX::XMFLOAT3 cameraPos, std::shared_ptr<RenderMaster> renderMaster);
};