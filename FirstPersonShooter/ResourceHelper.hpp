#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Common/DeviceResources.h"
#include "RoomLinkData.hpp"
#include "Content/ShaderStructures.h"

struct Mesh;


class ResourceHelper
{
public:
	static void addQuad(const std::string& texturePath, std::string textureName, std::string modelName, int nSubdivs, const std::shared_ptr<DX::DeviceResources>& deviceResources);
};