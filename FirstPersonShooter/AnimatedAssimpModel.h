#pragma once

#include "AssimpModel.h"
#include "BoneInfo.h"

#include <map>

struct AnimatedAssimpModel : public AssimpModel
{
	std::map<std::string, BoneInfo> m_BoneInfoMap;
	int m_boneCounter;
};