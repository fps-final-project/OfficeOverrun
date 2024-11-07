#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Mesh.h"
#include "Common/DeviceResources.h"

struct Model
{
	std::vector<Mesh> meshes;
};

