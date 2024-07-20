#pragma once

#include <vector>
#include "Common\DeviceResources.h"
#include <Content/ShaderStructures.h>
#include "Texture.h"
#include "Mesh.h"


class MeshFactory {
public:
	static Mesh createMesh(
		const std::vector<FirstPersonShooter::VertexData>& verticies,
		const std::vector<unsigned short>& indicies, 
		const std::vector<std::shared_ptr<Texture>>& textures,
		std::shared_ptr<DX::DeviceResources> deviceResources);
};