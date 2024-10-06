#include "pch.h"
#include "ResourceManager.h"

#include "AssimpModelLoader.h"
#include "TextureFactory.h"
#include <filesystem>

ResourceManager ResourceManager::Instance;

void ResourceManager::loadModel(const std::string& path, 
	const std::shared_ptr<DX::DeviceResources>& deviceResources,
	const std::string& nameOverride)
{
	std::string name = nameOverride.empty() ? std::filesystem::path(path).stem().string() : nameOverride;

	this->m_models.insert(std::make_pair(
		name,
		std::make_shared<AssimpModel>(AssimpModelLoader::createModelFromFile(path, deviceResources))));
}

void ResourceManager::addModel(AssimpModel& model, const std::string& name)
{
	this->m_models.insert(std::make_pair(
		name,
		std::make_shared<AssimpModel>(model)));
}

void ResourceManager::loadAnimatedModel(const std::string& path, 
	const std::shared_ptr<DX::DeviceResources>& deviceResources, 
	const std::vector<std::string>& missingTextures,
	const std::string& nameOverride)
{
	std::string name = nameOverride.empty() ? std::filesystem::path(path).stem().string() : nameOverride;

	this->m_animatedModels.insert(std::make_pair(
		name,
		std::make_shared<AnimatedAssimpModel>(AssimpModelLoader::createAnimatedModelFromFile(path, deviceResources))));

	for (const auto& texturePath : missingTextures)
	{
		AssimpModelLoader::appendTextureToMesh(texturePath, this->m_animatedModels[name]->meshes[0], deviceResources);
	}
}

void ResourceManager::loadTexture(const std::string& path, 
	const std::shared_ptr<DX::DeviceResources>& deviceResources,
	const std::string& nameOverride)
{
	std::string name = nameOverride.empty() ? std::filesystem::path(path).stem().string() : nameOverride;

	this->m_textures.insert(std::make_pair(
		name,
		std::make_shared<Texture>(
			TextureFactory::CreateTextureFromFile(std::wstring(path.begin(), path.end()).c_str(),
			deviceResources
		))));
}
