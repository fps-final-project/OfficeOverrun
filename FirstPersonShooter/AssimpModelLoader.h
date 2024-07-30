#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "Common/DeviceResources.h"
#include "assimp/scene.h"
#include "BoneInfo.h"
#include "MeshFactory.h"

class AssimpModel;
class AnimatedAssimpModel;
class Joint;
class Mesh;
class Texture;

class AssimpModelLoader
{
public:
	static AssimpModel createModelFromFile(const std::string& path, std::shared_ptr<DX::DeviceResources> deviceResources);
	static AnimatedAssimpModel createAnimatedModelFromFile(const std::string& path, std::shared_ptr<DX::DeviceResources> deviceResources);

	static DirectX::XMFLOAT4X4 aiToDirectXMatrix(aiMatrix4x4 matrix);
	static DirectX::XMFLOAT3 aiToDirectFloat3(aiVector3D v);
	static DirectX::XMFLOAT4 aiToDirectFloat4(aiQuaternion q);
private:
	AssimpModelLoader(std::shared_ptr<DX::DeviceResources> deviceResources) : m_deviceResources(deviceResources), m_boneCounter(0) {};

	std::vector<std::shared_ptr<Texture>> m_texturesLoaded;
	std::map<std::string, BoneInfo> m_BoneInfoMap;
	int m_boneCounter;
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	std::string m_directory;

	void ExtractBoneWeightForVerticies(std::vector<FirstPersonShooter::AnimatedVertexData>& verticies, aiMesh* mesh, const aiScene* scene);
	void createAnimations(AnimatedAssimpModel& outModel, const aiScene* scene);
	void loadJointHierarchy(Joint& joint, aiNode* src);

	AssimpModel createModel(const std::string& path);
	void processNode(AssimpModel& outModel, aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* m_mesh, const aiScene* scene);
	
	AnimatedAssimpModel createAnimatedModel(const std::string& path);
	void processAnimatedNode(AnimatedAssimpModel& outModel, aiNode* node, const aiScene* scene);
	Mesh processAnimatedMesh(aiMesh* m_mesh, const aiScene* scene);

	void setBasicVertexData(FirstPersonShooter::VertexData& vertex, aiMesh* m_mesh, int idx);
	std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};