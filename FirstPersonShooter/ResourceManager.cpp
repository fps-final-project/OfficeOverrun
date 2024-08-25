#include "ResourceManager.h"
#include "pch.h"

#include "AssimpModelLoader.h"
#include <filesystem>

ResourceManager ResourceManager::Instance;

void ResourceManager::loadModel(const std::string& path, const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	this->m_models.insert(std::make_pair(
		std::filesystem::path(path).stem().string(),
		std::make_shared<AssimpModel>(AssimpModelLoader::createModelFromFile(path, deviceResources))));
}

void ResourceManager::loadAnimatedModel(const std::string& path, const std::shared_ptr<DX::DeviceResources>& deviceResources, const std::vector<std::string>& missingTextures)
{
	this->m_animatedModels.insert(std::make_pair(
		std::filesystem::path(path).stem().string(),
		std::make_shared<AnimatedAssimpModel>(AssimpModelLoader::createAnimatedModelFromFile(path, deviceResources))));

	for (const auto& texturePath : missingTextures)
	{
		AssimpModelLoader::appendTextureToMesh(texturePath, this->m_animatedModels[path]->meshes[0], deviceResources);
	}
}
