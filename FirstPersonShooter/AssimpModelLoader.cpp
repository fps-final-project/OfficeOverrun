#include "pch.h"
#include "AssimpModelLoader.h"
#include "AssimpModel.h"
#include "AnimatedAssimpModel.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include <iostream>
#include <vector>
#include "TextureFactory.h"


#pragma region AssimpModel 

AssimpModel AssimpModelLoader::createModelFromFile(const std::string& path, std::shared_ptr<DX::DeviceResources> deviceResources)
{
	AssimpModelLoader loader(deviceResources);
	return loader.createModel(path);
}

AssimpModel AssimpModelLoader::createModel(const std::string& path)
{
	AssimpModel model;

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

void AssimpModelLoader::processNode(AssimpModel& outModel, aiNode* node, const aiScene* scene)
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

#pragma region AnimatedAssimpModel

AnimatedAssimpModel AssimpModelLoader::createAnimatedModelFromFile(const std::string& path, std::shared_ptr<DX::DeviceResources> deviceResources)
{
	AssimpModelLoader loader(deviceResources);
	return loader.createAnimatedModel(path);
}

std::vector<FinalTransformData> AssimpModelLoader::ExtractBoneWeightForVerticies(
	std::vector<AnimatedVertexData>& verticies,
	aiMesh* mesh,
	const aiScene* scene)
{
	std::vector<std::pair<FinalTransformData, int>> finalTransformDataPerVertex(verticies.size(), std::make_pair(FinalTransformData(), 0));

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
			assert(vertexId <= finalTransformDataPerVertex.size());
			for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
			{
				if (finalTransformDataPerVertex[vertexId].first.boneIds[i] < 0)
				{
					finalTransformDataPerVertex[vertexId].first.weights[i] = weight;
					finalTransformDataPerVertex[vertexId].first.boneIds[i] = boneId;

					// hashing bone weights in the integer such that later we can easily generate finalTransformId out of it
					finalTransformDataPerVertex[vertexId].second |= (boneId << (8 * i));
					break;
				}
			}
		}
	}

	// now we are mapping those hashes to natural numbers - finalTransformId
	// such that each vertex will know which final premultiplied transform to use
	// and also the inverse transform

	std::vector<FinalTransformData> result;
	std::map<int, int> boneIdCodeToFinalTransformId;
	int numFinalTransforms = 0;
	for (int i = 0; i < verticies.size(); i++)
	{
		if (boneIdCodeToFinalTransformId.find(finalTransformDataPerVertex[i].second) == boneIdCodeToFinalTransformId.end())
		{
			boneIdCodeToFinalTransformId.insert(std::make_pair(finalTransformDataPerVertex[i].second, numFinalTransforms++));
			result.push_back(finalTransformDataPerVertex[i].first);
		}

		verticies[i].finalTransformId = boneIdCodeToFinalTransformId[finalTransformDataPerVertex[i].second];
	}

	return result;
}

void AssimpModelLoader::createAnimations(AnimatedAssimpModel& outModel, const aiScene* scene)
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

AnimatedAssimpModel AssimpModelLoader::createAnimatedModel(const std::string& path)
{
	AnimatedAssimpModel model;

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

void AssimpModelLoader::processAnimatedNode(AnimatedAssimpModel& outModel, aiNode* node, const aiScene* scene)
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

void AssimpModelLoader::processAnimatedMesh(AnimatedAssimpModel& outModel, aiMesh* m_mesh, const aiScene* scene)
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

	outModel.m_transformData = ExtractBoneWeightForVerticies(vertexData, m_mesh, scene);
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
		//bool skip = true;
		bool skip = false;
		for (unsigned int j = 0; j < m_texturesLoaded.size(); j++)
		{
			if (std::strcmp(m_texturesLoaded[j].get()->path.data(), path.c_str()) == 0)
			{
				textures.push_back(m_texturesLoaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			std::shared_ptr<Texture> m_texture = std::make_shared<Texture>(TextureFactory::CreateTextureFromFile(std::wstring(path.begin(), path.end()).c_str(), m_deviceResources));
			m_texture->type = typeName;
			m_texture->path = path;
			textures.push_back(m_texture);
			m_texturesLoaded.push_back(m_texture);
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
	m.textures.push_back(texture);
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

#pragma endregion
