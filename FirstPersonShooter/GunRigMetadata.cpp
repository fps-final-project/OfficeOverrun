#include "pch.h"
#include "GunRigMetadata.hpp"
#include <fstream>

GunRigMetadata GunRigMetadata::loadFromFile(const std::string& path)
{
    GunRigMetadata metadata;
    std::ifstream file(path, std::ios::in);

    file >> metadata.name;
    file >> metadata.shootingAnimationSpeedup;
    

    file >> metadata.gunOffset.x;
    file >> metadata.gunOffset.y;
    file >> metadata.gunOffset.z;

    file >> metadata.barrelOffsetX;
    file >> metadata.barrelOffsetY;

    file >> metadata.rigOffset.x;
    file >> metadata.rigOffset.y;
    file >> metadata.rigOffset.z;

    file >> metadata.damage;
    file >> metadata.clipSize;

    file.close();
    return metadata;
}
