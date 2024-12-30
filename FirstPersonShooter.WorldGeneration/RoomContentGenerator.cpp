#include "pch.h"
#include "RoomContentGenerator.h"
#include "PropLoader.hpp"
#include "SpaceTransformationHelper.h"

using namespace WorldGenerator;

void RoomContentGenerator::GenerateRoomContent(Graph<GeneratedRoom>& adGraph)
{
	LoadPropsData();
}

void RoomContentGenerator::LoadPropsData()
{
	all_props = PropLoader::LoadAllProps(config.propsDir());

	// Transform to internal space notation
	for (auto& prop : all_props)
		prop.size = SpaceTransformationHelper::TransformToInternalSpace(prop.size);
}
