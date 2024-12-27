#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Common/DeviceResources.h"
#include "RoomLinkData.hpp"
#include "Content/ShaderStructures.h"

struct Mesh;


class __declspec(dllexport)  ResourceHelper
{
public:
	static void addQuad(const std::string& texturePath, std::string textureName, std::string modelName, int nSubdivs, const std::shared_ptr<DX::DeviceResources>& deviceResources);
	static void createSkyboxMesh(const std::string& texturePath, const std::shared_ptr<DX::DeviceResources>& deviceResources);

	static void LoadAllPropsModels(const std::string& propFolderPath, const std::shared_ptr<DX::DeviceResources>& deviceResources);
	static void LoadAllMuzzleFlashFrames(const std::string& folderPath, const std::shared_ptr<DX::DeviceResources>& deviceResources);
};