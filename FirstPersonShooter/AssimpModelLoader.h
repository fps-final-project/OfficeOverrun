#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "Common/DeviceResources.h"
#include "assimp/scene.h"
#include "BoneInfo.h"
#include "MeshFactory.h"

struct Model;
struct AnimatedModel;
struct FinalTransformData;
struct Joint;
struct Mesh;
struct Texture;

class __declspec(dllexport) AssimpModelLoader
{
public:
	static Model createModelFromFile(const std::string& path, std::shared_ptr<DX::DeviceResources> deviceResources);
	static AnimatedModel createAnimatedModelFromFile(const std::string& path, std::shared_ptr<DX::DeviceResources> deviceResources);

	static void appendTextureToMesh(const std::string& path, Mesh& m, std::shared_ptr<DX::DeviceResources> deviceResources);

	static DirectX::XMFLOAT4X4 aiToDirectXMatrix(aiMatrix4x4 matrix);
	static DirectX::XMFLOAT3 aiToDirectFloat3(aiVector3D v);
	static DirectX::XMFLOAT4 aiToDirectFloat4(aiQuaternion q);
private:
	static std::map<std::string, std::shared_ptr<Texture>> m_textureCache;
	AssimpModelLoader(std::shared_ptr<DX::DeviceResources> deviceResources) : m_deviceResources(deviceResources), m_boneCounter(0) {};
	void normalizePositions(std::vector<VertexData>& verticies);

	std::map<std::string, BoneInfo> m_BoneInfoMap;
	int m_boneCounter;
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	std::string m_directory;

	void ExtractBoneWeightForVerticies(std::vector<AnimatedVertexData>& verticies, aiMesh* mesh, const aiScene* scene);
	void createAnimations(AnimatedModel& outModel, const aiScene* scene);
	void loadJointHierarchy(Joint& joint, aiNode* src);

	Model createModel(const std::string& path);
	void processNode(Model& outModel, aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* m_mesh, const aiScene* scene);
	
	AnimatedModel createAnimatedModel(const std::string& path);
	void processAnimatedNode(AnimatedModel& outModel, aiNode* node, const aiScene* scene);
	void processAnimatedMesh(AnimatedModel& outModel, aiMesh* m_mesh, const aiScene* scene);

	void setBasicVertexData(VertexData& vertex, aiMesh* m_mesh, int idx);
	std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	bool validateTextureName(std::string name);


};