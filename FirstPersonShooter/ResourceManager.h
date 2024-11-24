#pragma once

#include "Texture.h"
#include "AudioFile.hpp"
#include "AssimpModel.h"
#include "AnimatedAssimpModel.h"
#include "GunRigMetadata.hpp"

#include <map>
#include <string>
#include <memory>
#include <vector>

class __declspec(dllexport) ResourceManager
{
public:
	static ResourceManager& Instance();

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	void loadModel(const std::string& path, 
		const std::shared_ptr<DX::DeviceResources>& deviceResources, const std::string& nameOverride = "");

	void addModel(Model& model, const std::string& name);

	void loadAnimatedModel(const std::string& path,
		const std::shared_ptr<DX::DeviceResources>& deviceResources,
		const std::vector<std::string>& missingTextures = std::vector<std::string>(), 
		const std::string& nameOverride = "");

	void loadTexture(const std::string& path, 
		const std::shared_ptr<DX::DeviceResources>& deviceResources, 
		const std::string& nameOverride = "");

	void loadGunRigMetadata(const std::string& path);
	void loadAudioFile(const std::string& path, const UINT32 loop, const std::shared_ptr<DX::DeviceResources>& deviceResources, const std::string& nameOverride = "");

	std::shared_ptr<Model> getModel(std::string name);
	std::shared_ptr<AnimatedModel> getAnimatedModel(std::string name);
	std::shared_ptr<Texture> getTexture(std::string name);
	std::shared_ptr<GunRigMetadata> getGunRigMetadata(std::string name);
	std::shared_ptr<AudioFile> getAudioFile(std::string name);

	
private:
	ResourceManager() {}
	std::map<std::string, std::shared_ptr<Model>> m_models;
	std::map<std::string, std::shared_ptr<AnimatedModel>> m_animatedModels;
	std::map<std::string, std::shared_ptr<Texture>> m_textures;
	std::map<std::string, std::shared_ptr<GunRigMetadata>> m_gunRigMetadata;
	std::map<std::string, std::shared_ptr<AudioFile>> m_audioFiles;
};