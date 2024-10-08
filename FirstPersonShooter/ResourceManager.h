#pragma once

#include "Texture.h"
#include "AssimpModel.h"
#include "AnimatedAssimpModel.h"

#include <map>
#include <string>
#include <memory>
#include <vector>

class ResourceManager
{
public:
	static ResourceManager Instance;
	void loadModel(const std::string& path, 
		const std::shared_ptr<DX::DeviceResources>& deviceResources, const std::string& nameOverride = "");

	void addModel(AssimpModel& model, const std::string& name);

	void loadAnimatedModel(const std::string& path,
		const std::shared_ptr<DX::DeviceResources>& deviceResources,
		const std::vector<std::string>& missingTextures = std::vector<std::string>(), 
		const std::string& nameOverride = "");

	void loadTexture(const std::string& path, 
		const std::shared_ptr<DX::DeviceResources>& deviceResources, 
		const std::string& nameOverride = "");

	inline std::shared_ptr<AssimpModel>& getModel(std::string name) { return m_models[name]; }
	inline std::shared_ptr<AnimatedAssimpModel>& getAnimatedModel(std::string name) { return m_animatedModels[name]; }
	inline std::shared_ptr<Texture>& getTexture(std::string name) { return m_textures[name]; }
private:
	ResourceManager() {}
	std::map<std::string, std::shared_ptr<AssimpModel>> m_models;
	std::map<std::string, std::shared_ptr<AnimatedAssimpModel>> m_animatedModels;
	std::map<std::string, std::shared_ptr<Texture>> m_textures;
};