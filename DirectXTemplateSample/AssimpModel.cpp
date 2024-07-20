#include "pch.h"
#include "AssimpModel.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include <iostream>
#include "MeshFactory.h"
#include "TextureFactory.h"


AssimpModel::AssimpModel(std::string path, std::shared_ptr<DX::DeviceResources> deviceResources) : m_deviceResources(deviceResources)
{
    this->createFromFile(path);
}

void AssimpModel::createFromFile(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "Error::Assimp::" << importer.GetErrorString() << "\n";
        return;
    }

    this->directory = path.substr(0, path.find_last_of('\\'));
    processNode(scene->mRootNode, scene);

}

void AssimpModel::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh AssimpModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<DirectXTemplateSample::VertexData> vertexData;
    std::vector<unsigned short> indicies;
    std::vector<std::shared_ptr<Texture>> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        DirectXTemplateSample::VertexData vertex;


        // process vertex positions, normals and texture coordinates
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
        vertex.pos.z = mesh->mVertices[i].z;
        if (mesh->mTextureCoords[0])
        {
            vertex.texture_pos.x = mesh->mTextureCoords[0][i].x;
            vertex.texture_pos.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.texture_pos.x = 0.f;
            vertex.texture_pos.y = 0.f;
        }

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        vertexData.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indicies.push_back(face.mIndices[j]);
    }
    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(material,
            aiTextureType_SPECULAR, "texture_specular");

        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }


    return MeshFactory::createMesh(vertexData, indicies, textures, m_deviceResources);
}

std::vector<std::shared_ptr<Texture>> AssimpModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<std::shared_ptr<Texture>> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string path = directory + "/";
        path = path.append(str.C_Str());
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].get()->path.data(), path.c_str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            std::shared_ptr<Texture> texture = std::make_shared<Texture>(TextureFactory::CreateTextureFromFile(std::wstring(path.begin(), path.end()).c_str(), m_deviceResources));
            texture->type = typeName;
            texture->path = path;
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return textures;
}
