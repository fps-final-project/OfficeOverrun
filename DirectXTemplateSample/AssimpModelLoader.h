#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Common/DeviceResources.h"
#include "assimp/scene.h"
#include "MeshFactory.h"

class AssimpModel;
class Mesh;
class Texture;

class AssimpModelLoader
{
public:
	static AssimpModel createModelFromFile(const std::string& path, std::shared_ptr<DX::DeviceResources> deviceResources);
private:
	AssimpModelLoader(std::shared_ptr<DX::DeviceResources> deviceResources) : m_deviceResources(deviceResources) {};
	std::vector<std::shared_ptr<Texture>> m_texturesLoaded;
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	std::string m_directory;


	AssimpModel createModel(const std::string& path);
	void processNode(AssimpModel& outModel, aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* m_mesh, const aiScene* scene);
	std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};