#include "pch.h"
#include "Animation.h"
#include <assimp/Importer.hpp>
#include "assimp/postprocess.h"
#include "AssimpModelLoader.h"

Animation::Animation(const std::string& animationPath, std::shared_ptr<AnimatedAssimpModel> model)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
    assert(scene && scene->mRootNode);
    auto animation = scene->mAnimations[0];
    m_Duration = animation->mDuration;
    m_TicksPerSecond = animation->mTicksPerSecond;
    readHierarchyData(m_RootNode, scene->mRootNode);
    readMissingBones(animation, *model);
}

Bone* Animation::getBone(const std::string& name)
{
    auto iter = std::find_if(m_Bones.begin(), m_Bones.end(), [&](const Bone& b)
        {
            return b.m_name == name;
        });
    if (iter == m_Bones.end()) return nullptr;
    return &(*iter);
}

void Animation::readMissingBones(const aiAnimation* animation, AnimatedAssimpModel& model)
{
    int size = animation->mNumChannels;

    auto& boneInfoMap = model.m_BoneInfoMap;
    int& boneCount = model.m_boneCounter;

    for (int i = 0; i < size; i++)
    {
        auto channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }

        m_Bones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel));
    }

    m_BoneInfoMap = boneInfoMap;
}

void Animation::readHierarchyData(AssimpNodeData& dest, const aiNode* src)
{
    assert(src);

    dest.name = src->mName.data;
    dest.transformation = AssimpModelLoader::aiToDirectXMatrix(src->mTransformation);
    
    for (int i = 0; i < src->mNumChildren; i++)
    {
        AssimpNodeData data;
        readHierarchyData(data, src->mChildren[i]);
        dest.children.push_back(data);
    }
}
