#pragma once
#include <string>
#include <memory>
#include "Common/DeviceResources.h"

class ResourceHelper
{
public:
	static void addWallModel(const std::string& texturePath, const std::shared_ptr<DX::DeviceResources>& deviceResources);
};