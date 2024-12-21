#pragma once

#include "Prop.hpp"
#include <vector>

class __declspec(dllexport) PropLoader 
{
public:
	static std::vector<Prop> LoadAllProps(std::string propFolderPath);
	static Prop LoadPropData(std::string path);
};