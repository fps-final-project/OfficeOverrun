#include "pch.h"
#include "PropLoader.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

std::vector<Prop> PropLoader::LoadAllProps(std::string propFolderPath)
{
    std::vector<Prop> props;
	for (const auto& dir : std::filesystem::directory_iterator(propFolderPath))
	{
		if (std::filesystem::is_directory(dir))
			for (const auto& entry : std::filesystem::directory_iterator(dir))
			{
				auto path = std::filesystem::path(entry);
				if (path.extension() == ".txt")
				{
					Prop prop = LoadPropData(path.string());
					if (!prop.name.empty())
						props.push_back(prop);
				}
			}
	}

	return props;
}

Prop PropLoader::LoadPropData(std::string path)
{
	std::ifstream ifs(path, std::ios::in);
	if (!ifs.is_open())
	{
		std::cout << "Error, could not open " + path + "\n";
		ifs.close();
		return Prop();
	}


	Prop prop;
	ifs >> prop.name;
	ifs >> prop.modelPath;
	ifs >> prop.size.x >> prop.size.y >> prop.size.z;
	ifs >> prop.isSolid;

	ifs.close();
	return prop;
}
