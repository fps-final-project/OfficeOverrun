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
		const std::shared_ptr<DX::DeviceResources>& deviceResources);
	void loadAnimatedModel(const std::string& path,
		const std::shared_ptr<DX::DeviceResources>& deviceResources,
		const std::vector<std::string>& missingTextures = std::vector<std::string>());
private:
	ResourceManager();
	std::map<std::string, std::shared_ptr<AssimpModel>> m_models;
	std::map<std::string, std::shared_ptr<AnimatedAssimpModel>> m_animatedModels;
};