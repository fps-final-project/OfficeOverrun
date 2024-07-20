#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Mesh.h"

#include "Common/DeviceResources.h"

#include "assimp/scene.h"

class Mesh;
class Texture;

class AssimpModel
{
	std::vector<std::shared_ptr<Texture>> textures_loaded;
	std::string directory;
	std::shared_ptr<DX::DeviceResources> m_deviceResources;


	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
	AssimpModel() {}
	AssimpModel(std::string path, std::shared_ptr<DX::DeviceResources> deviceResources);
	void createFromFile(std::string path);
	std::vector<Mesh> meshes;
};

