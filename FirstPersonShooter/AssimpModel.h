#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Mesh.h"
#include "Common/DeviceResources.h"

struct AssimpModel
{
	std::vector<Mesh> meshes;
};

