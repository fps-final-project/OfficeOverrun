#include "pch.h"
#include "AssimpModelLoader.h"
#include "AssimpModel.h"
#include "AnimatedAssimpModel.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include <iostream>
#include "TextureFactory.h"


AssimpModel AssimpModelLoader::createModelFromFile(const std::string& path, std::shared_ptr<DX::DeviceResources> deviceResources)
{
	AssimpModelLoader loader(deviceResources);
	return loader.createModel(path);
}

AnimatedAssimpModel AssimpModelLoader::createAnimatedModelFromFile(const std::string& path, std::shared_ptr<DX::DeviceResources> deviceResources)
{
	AssimpModelLoader loader(deviceResources);
	return loader.createAnimatedModel(path);
}

void AssimpModelLoader::setBasicVertexData(FirstPersonShooter::VertexData& vertex, aiMesh* m_mesh, int idx)
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

void AssimpModelLoader::ExtractBoneWeightForVerticies(
	std::vector<FirstPersonShooter::AnimatedVertexData>& verticies,
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
				if (verticies[vertexId].boneIds[i] < 0)
				{
					verticies[vertexId].weights[i] = weight;
					verticies[vertexId].boneIds[i] = boneId;
					break;
				}
			}
		}
	}
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

void AssimpModelLoader::processAnimatedNode(AnimatedAssimpModel& outModel, aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* m_mesh = scene->mMeshes[node->mMeshes[i]];
		outModel.meshes.push_back(processAnimatedMesh(m_mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processAnimatedNode(outModel, node->mChildren[i], scene);
	}
}

Mesh AssimpModelLoader::processMesh(aiMesh* m_mesh, const aiScene* scene)
{
	std::vector<FirstPersonShooter::VertexData> vertexData;
	std::vector<unsigned short> indicies;
	std::vector<std::shared_ptr<Texture>> textures;

	for (unsigned int i = 0; i < m_mesh->mNumVertices; i++)
	{
		FirstPersonShooter::VertexData vertex;

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

	return MeshFactory<FirstPersonShooter::VertexData>::createMesh(vertexData, indicies, textures, m_deviceResources);
}

Mesh AssimpModelLoader::processAnimatedMesh(aiMesh* m_mesh, const aiScene* scene)
{
	std::vector<FirstPersonShooter::AnimatedVertexData> vertexData;
	std::vector<unsigned short> indicies;
	std::vector<std::shared_ptr<Texture>> textures;

	for (unsigned int i = 0; i < m_mesh->mNumVertices; i++)
	{
		FirstPersonShooter::AnimatedVertexData vertex;

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

	return MeshFactory<FirstPersonShooter::AnimatedVertexData>::createMesh(vertexData, indicies, textures, m_deviceResources);
}

std::vector<std::shared_ptr<Texture>> AssimpModelLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<std::shared_ptr<Texture>> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string path = m_directory + "/";
		path = path.append(str.C_Str());
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

DirectX::XMFLOAT4X4 AssimpModelLoader::aiToDirectXMatrix(aiMatrix4x4 matrix)
{
	DirectX::XMFLOAT4X4 ret;
	memcpy(&ret, &matrix, 64);
	auto t = DirectX::XMLoadFloat4x4(&ret);
	t = DirectX::XMMatrixTranspose(t);
	DirectX::XMStoreFloat4x4(&ret, t);


	return ret;
}
