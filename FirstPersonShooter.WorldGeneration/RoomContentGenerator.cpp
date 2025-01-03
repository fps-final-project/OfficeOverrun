#include "pch.h"
#include "RoomContentGenerator.h"
#include "PropLoader.hpp"
#include "SpaceTransformationHelper.h"
#include "RNG.h"
#include "PropMeshGenerator.h"
#include "GunPropSelector.h"

using namespace WorldGenerator;

void RoomContentGenerator::GenerateRoomContent(Graph<GeneratedRoom>& adGraph)
{
	LoadPropsData();

	for (auto& node : adGraph.nodes)
	{
		GenerateGunsInRoom(node);
		GeneratePropsInRoom(node);
	}
}

void RoomContentGenerator::LoadPropsData()
{
	all_props = PropLoader::LoadAllProps(config.propsDir());

	// Transform to internal space notation
	for (auto& prop : all_props)
		prop.size = SpaceTransformationHelper::TransformToInternalSpace(prop.size);
}

void WorldGenerator::RoomContentGenerator::GenerateGunsInRoom(Node<GeneratedRoom>& node)
{
	GeneratedRoom& room = *node.value;

	std::vector<GunProp> gun_props = GunPropSelector::SelectGunsForRoom(node);

	for (auto gun_prop : gun_props)
	{
		PropInstance* prop_instance = GeneratePropForRoom(room, gun_prop);
		if (prop_instance != nullptr)
			room.gun_props.push_back(GunPropInstance(gun_prop, *prop_instance));
	}
}

void WorldGenerator::RoomContentGenerator::GeneratePropsInRoom(Node<GeneratedRoom>& node)
{
	GeneratedRoom& room = *node.value;

	DistributionParameters params;
	params.binomial_t = room.size.x * room.size.y * RoomContentConfig::PROPS_IN_ROOM_SIZE_TO_COUNT_COEFF;
	params.binomial_p = RoomContentConfig::PROPS_IN_ROOM_BINOMIAL_P;

	int props_count = RNG::RandIntInRange(RoomContentConfig::PROPS_IN_ROOM_MIN, RoomContentConfig::PROPS_IN_ROOM_MAX, Binomial, params);

	for (int i = 0; i < props_count; i++)
	{
		Prop prop = RNG::SelectRandomElement<Prop>(all_props);
		
		PropInstance* prop_instance = GeneratePropForRoom(room, prop);
		if (prop_instance != nullptr)
			room.props.push_back(*prop_instance);
	}
}

// Returns false if prop can not be generated
PropInstance* WorldGenerator::RoomContentGenerator::GeneratePropForRoom(GeneratedRoom& room, Prop prop)
{
	// Create mesh for room and prop
	std::vector<MeshBox> mesh = PropMeshGenerator::GenerateMeshForProp(room, prop);

	// Delete meshboxes overlapping with existing props or door area
	PropMeshGenerator::DeleteUnavailableBoxes(mesh, room);

	if (mesh.empty())
		return nullptr;

	// Select random available box
	MeshBox box = RNG::SelectRandomElement<MeshBox>(mesh);

	// Select prop position in box
	DirectX::XMFLOAT2 prop_pos = PropMeshGenerator::PlacePropInBox(box, prop);
	DirectX::XMFLOAT3 prop_pos3f(prop_pos.x, prop_pos.y, room.pos.z);
	
	DirectX::XMFLOAT3 prop_orient(0, 0, 0);

	return new PropInstance(prop, prop_pos3f, prop_orient);
}
