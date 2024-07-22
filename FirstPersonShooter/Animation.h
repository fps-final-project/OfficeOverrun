#pragma once
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <memory>
#include "AnimatedAssimpModel.h" 
#include "Bone.h" 
#include "BoneInfo.h" 

#include <assimp/scene.h>



struct AssimpNodeData
{
	DirectX::XMFLOAT4X4 transformation;
	std::string name;
	std::vector<AssimpNodeData> children;
};


class Animation
{
public:
	Animation() = default;
	Animation(const std::string& animationPath, std::shared_ptr<AnimatedAssimpModel> model);
	Bone* getBone(const std::string& name);
	float m_Duration;
	int m_TicksPerSecond;
	std::vector<Bone> m_Bones;
	AssimpNodeData m_RootNode;
	std::map<std::string, BoneInfo> m_BoneInfoMap;
private:
	void readMissingBones(const aiAnimation* animation, AnimatedAssimpModel& model);
	void readHierarchyData(AssimpNodeData& dest, const aiNode* src);

};