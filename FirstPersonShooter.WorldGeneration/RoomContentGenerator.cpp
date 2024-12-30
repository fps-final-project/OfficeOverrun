#include "pch.h"
#include "RoomContentGenerator.h"
#include "PropLoader.hpp"

using namespace WorldGenerator;

void RoomContentGenerator::GenerateRoomContent(Graph<GeneratedRoom>& adGraph)
{
	LoadPropsData();
}

void RoomContentGenerator::LoadPropsData()
{
	all_props = PropLoader::LoadAllProps(config.propsDir());
}
