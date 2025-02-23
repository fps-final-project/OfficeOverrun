#include "pch.h"
#include "AssimpModelLoader.h"
#include "AssimpModel.h"
#include "AnimatedAssimpModel.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include <iostream>
#include <vector>
#include "TextureFactory.h"

std::map<std::string, std::shared_ptr<Texture>> AssimpModelLoader::m_textureCache;

#pragma region Model 

Model AssimpModelLoader::createModelFromFile(const std::string& path, std::shared_ptr<DX::DeviceResources> deviceResources)
{
	AssimpModelLoader loader(deviceResources);
	return loader.createModel(path);
}

Model AssimpModelLoader::createModel(const std::string& path)
{
	Model model;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Error::Assimp::" << importer.GetErrorString() << "\n";
		return model;
	}

	this->m_directory = path.substr(0, path.find_last_of('\\'));
	processNode(model, scene->mRootNode, scene);

	return model;
}

void AssimpModelLoader::processNode(Model& outModel, aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* m_mesh = scene->mMeshes[node->mMeshes[i]];
		outModel.meshes.push_back(processMesh(m_mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(outModel, node->mChildren[i], scene);
	}
}

Mesh AssimpModelLoader::processMesh(aiMesh* m_mesh, const aiScene* scene)
{
	std::vector<VertexData> vertexData;
	std::vector<unsigned short> indicies;
	std::vector<std::shared_ptr<Texture>> textures;

	for (unsigned int i = 0; i < m_mesh->mNumVertices; i++)
	{
		VertexData vertex;

		this->setBasicVertexData(vertex, m_mesh, i);

		vertexData.push_back(vertex);
	}

	normalizePositions(vertexData);
	// process indices
	for (unsigned int i = 0; i < m_mesh->mNumFaces; i++)
	{
		aiFace face = m_mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indicies.push_back(face.mIndices[j]);
	}

	// process material
	const std::vector<std::pair<std::string, aiTextureType>> textureTypes = {
		{"texture_diffuse", aiTextureType_DIFFUSE},
		{"texture_specular", aiTextureType_SPECULAR},
		{"texture_roughness", aiTextureType_SHININESS},
	};

	if (m_mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[m_mesh->mMaterialIndex];
		for (const auto& textureType : textureTypes)
		{
			std::vector<std::shared_ptr<Texture>> textureMaps = loadMaterialTextures(material,
				textureType.second, textureType.first);
			textures.insert(textures.end(), textureMaps.begin(), textureMaps.end());
		}
	}

	return MeshFactory<VertexData>::createMesh(vertexData, indicies, textures, m_deviceResources);
}

#pragma endregion

#pragma region AnimatedModel

AnimatedModel AssimpModelLoader::createAnimatedModelFromFile(const std::string& path, std::shared_ptr<DX::DeviceResources> deviceResources)
{
	AssimpModelLoader loader(deviceResources);
	return loader.createAnimatedModel(path);
}

void AssimpModelLoader::ExtractBoneWeightForVerticies(
	std::vector<AnimatedVertexData>& verticies,
	aiMesh* mesh,
	const aiScene* scene)
{
	for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
	{
		int boneId = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = m_boneCounter;
			newBoneInfo.offsetMatrix = this->aiToDirectXMatrix(mesh->mBones[boneIndex]->mOffsetMatrix);
			m_BoneInfoMap[boneName] = newBoneInfo;
			boneId = m_boneCounter;
			m_boneCounter++;
		}
		else
		{
			boneId = m_BoneInfoMap[boneName].id;
		}

		assert(boneId != -1);

		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexId <= verticies.size());
			for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
			{
				if (verticies[vertexId].boneIds[i] < 0 || verticies[vertexId].weights[i] == 0)
				{
					verticies[vertexId].weights[i] = weight;
					verticies[vertexId].boneIds[i] = boneId;
					break;
				}
			}
		}
	}
}

void AssimpModelLoader::createAnimations(AnimatedModel& outModel, const aiScene* scene)
{
	for (int i = 0; i < scene->mNumAnimations; i++)
	{
		Animation animation;

		auto aiAnimation = scene->mAnimations[i];

		animation.name = std::string(aiAnimation->mName.C_Str());

		int nJoints = aiAnimation->mNumChannels;
		int nKeyframes = aiAnimation->mChannels[0]->mNumPositionKeys;

		assert(nJoints > 0 && nKeyframes > 0);

		animation.m_TicksPerSecond = aiAnimation->mTicksPerSecond;
		animation.m_Duration = aiAnimation->mDuration;

		animation.frames.resize(nKeyframes);

		for (int indexJoint = 0; indexJoint < nJoints; indexJoint++)
		{
			auto channel = aiAnimation->mChannels[indexJoint];

			for (int indexFrame = 0; indexFrame < nKeyframes; indexFrame++)
			{
				//if (animation.frames[indexFrame].pose.find(channel->mNodeName.data) != animation.frames[indexFrame].pose.end())
				//	animation.frames[indexFrame].pose[channel->mNodeName.data] = JointTransform();

				std::string nodeName = std::string(channel->mNodeName.data);

				if (outModel.m_BoneInfoMap.find(nodeName) == outModel.m_BoneInfoMap.end())
				{
					outModel.m_BoneInfoMap[nodeName].id = m_boneCounter;
					m_boneCounter++;
				}

				animation.frames[indexFrame].timestamp = channel->mPositionKeys[indexFrame].mTime;

				animation.frames[indexFrame].pose[nodeName].position = AssimpModelLoader::aiToDirectFloat3(
					channel->mPositionKeys[indexFrame].mValue);

				animation.frames[indexFrame].pose[nodeName].orientation = AssimpModelLoader::aiToDirectFloat4(
					channel->mRotationKeys[indexFrame].mValue);

				animation.frames[indexFrame].pose[nodeName].scale = AssimpModelLoader::aiToDirectFloat3(
					channel->mScalingKeys[indexFrame].mValue);
			}
		}

		outModel.m_animations.insert(std::make_pair(aiAnimation->mName.C_Str(), std::make_shared<Animation>(animation)));
	}
}

void AssimpModelLoader::loadJointHierarchy(Joint& joint, aiNode* src)
{
	assert(src);

	joint.name = src->mName.data;
	joint.transformation = AssimpModelLoader::aiToDirectXMatrix(src->mTransformation);

	for (int i = 0; i < src->mNumChildren; i++)
	{
		Joint data;
		this->loadJointHierarchy(data, src->mChildren[i]);
		joint.children.push_back(data);
	}
}

AnimatedModel AssimpModelLoader::createAnimatedModel(const std::string& path)
{
	AnimatedModel model;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Error::Assimp::" << importer.GetErrorString() << "\n";
		return model;
	}

	this->m_directory = path.substr(0, path.find_last_of('\\'));
	processAnimatedNode(model, scene->mRootNode, scene);

	model.m_BoneInfoMap = m_BoneInfoMap;
	model.m_boneCounter = m_BoneInfoMap.size();

	this->createAnimations(model, scene);
	this->loadJointHierarchy(model.m_rootJoint, scene->mRootNode);

	return model;
}

void AssimpModelLoader::processAnimatedNode(AnimatedModel& outModel, aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* m_mesh = scene->mMeshes[node->mMeshes[i]];
		processAnimatedMesh(outModel, m_mesh, scene);
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processAnimatedNode(outModel, node->mChildren[i], scene);
	}
}

void AssimpModelLoader::processAnimatedMesh(AnimatedModel& outModel, aiMesh* m_mesh, const aiScene* scene)
{
	std::vector<AnimatedVertexData> vertexData;
	std::vector<unsigned short> indicies;
	std::vector<std::shared_ptr<Texture>> textures;

	for (unsigned int i = 0; i < m_mesh->mNumVertices; i++)
	{
		AnimatedVertexData vertex;

		this->setBasicVertexData(vertex, m_mesh, i);

		vertexData.push_back(vertex);
	}
	// process indices
	for (unsigned int i = 0; i < m_mesh->mNumFaces; i++)
	{
		aiFace face = m_mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indicies.push_back(face.mIndices[j]);
	}

	// process material
	const std::vector<std::pair<std::string, aiTextureType>> textureTypes = {
		{"texture_diffuse", aiTextureType_DIFFUSE},
		{"texture_specular", aiTextureType_SPECULAR},
		{"texture_roughness", aiTextureType_SHININESS},
	};

	if (m_mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[m_mesh->mMaterialIndex];
		for (const auto& textureType : textureTypes)
		{
			std::vector<std::shared_ptr<Texture>> textureMaps = loadMaterialTextures(material,
				textureType.second, textureType.first);
			textures.insert(textures.end(), textureMaps.begin(), textureMaps.end());
		}
	}

	ExtractBoneWeightForVerticies(vertexData, m_mesh, scene);
	outModel.meshes.push_back(MeshFactory<AnimatedVertexData>::createMesh(vertexData, indicies, textures, m_deviceResources));
}

#pragma endregion

#pragma region Common

void AssimpModelLoader::setBasicVertexData(VertexData& vertex, aiMesh* m_mesh, int idx)
{
	// process vertex positions, normals and m_texture coordinates
	vertex.pos.x = m_mesh->mVertices[idx].x;
	vertex.pos.y = m_mesh->mVertices[idx].y;
	vertex.pos.z = m_mesh->mVertices[idx].z;

	if (m_mesh->mTextureCoords[0])
	{
		vertex.texture_pos.x = m_mesh->mTextureCoords[0][idx].x;
		vertex.texture_pos.y = m_mesh->mTextureCoords[0][idx].y;
	}
	else
	{
		vertex.texture_pos.x = 0.f;
		vertex.texture_pos.y = 0.f;
	}

	vertex.normal.x = m_mesh->mNormals[idx].x;
	vertex.normal.y = m_mesh->mNormals[idx].y;
	vertex.normal.z = m_mesh->mNormals[idx].z;
}

std::vector<std::shared_ptr<Texture>> AssimpModelLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<std::shared_ptr<Texture>> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		if (!this->validateTextureName(std::string(str.C_Str())))
			continue;

		std::string path = m_directory + "/";
		path = path.append(str.C_Str());
		bool skip = false;
		if (m_textureCache.find(str.C_Str()) != m_textureCache.end())
		{
			textures.push_back(m_textureCache[str.C_Str()]);
		}
		else
		{
			std::shared_ptr<Texture> texture = std::make_shared<Texture>(TextureFactory::CreateTextureFromFile(std::wstring(path.begin(), path.end()).c_str(), m_deviceResources));
			texture->type = typeName;
			texture->path = path;
			textures.push_back(texture);
			m_textureCache.insert(std::make_pair(str.C_Str(), texture));
		}
	}
	return textures;
}

bool AssimpModelLoader::validateTextureName(std::string name)
{
	auto extension_begin_idx = name.find(".");
	if (extension_begin_idx == std::string::npos)
		return false;
	auto extension = name.substr(extension_begin_idx, name.size() - extension_begin_idx);
	return extension == ".png" || extension == ".jpg";
}

void AssimpModelLoader::appendTextureToMesh(const std::string& path, Mesh& m, std::shared_ptr<DX::DeviceResources> deviceResources)
{
	auto texture = TextureFactory::CreateTextureFromFile(std::wstring(path.begin(), path.end()).c_str(), deviceResources);
	texture.path = path;
	texture.type = "diffuse";
	m.textures.push_back(std::make_shared<Texture>(texture));
}


DirectX::XMFLOAT4X4 AssimpModelLoader::aiToDirectXMatrix(aiMatrix4x4 matrix)
{
	DirectX::XMFLOAT4X4 ret;
	memcpy(&ret, &matrix, 64);
	auto t = DirectX::XMLoadFloat4x4(&ret);
	t = DirectX::XMMatrixTranspose(t);
	DirectX::XMStoreFloat4x4(&ret, t);


	return ret;
}

DirectX::XMFLOAT3 AssimpModelLoader::aiToDirectFloat3(aiVector3D v)
{
	return DirectX::XMFLOAT3(v.x, v.y, v.z);
}

DirectX::XMFLOAT4 AssimpModelLoader::aiToDirectFloat4(aiQuaternion q)
{
	return DirectX::XMFLOAT4(q.x, q.y, q.z, q.w);
}

void AssimpModelLoader::normalizePositions(std::vector<VertexData>& verticies)
{
	float max_x, max_y, max_z, min_x, min_y, min_z;

	max_x = min_x = verticies[0].pos.x;
	max_y = min_y = verticies[0].pos.y;
	max_z = min_z = verticies[0].pos.z;


	for (auto& v : verticies)
	{
		max_x = max(max_x, v.pos.x);
		max_y = max(max_y, v.pos.y);
		max_z = max(max_z, v.pos.z);

		min_x = min(min_x, v.pos.x);
		min_y = min(min_y, v.pos.y);
		min_z = min(min_z, v.pos.z);
	}

	for (int i = 0; i < verticies.size(); i++)
	{
		verticies[i].pos.x = (verticies[i].pos.x - min_x) /  (max_x - min_x);
		verticies[i].pos.y = (verticies[i].pos.y - min_y) /  (max_y - min_y);
		verticies[i].pos.z = (verticies[i].pos.z - min_z) /  (max_z - min_z);
	}
}

#pragma endregion
