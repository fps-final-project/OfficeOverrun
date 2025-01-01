#include "pch.h"
#include "PropMeshGenerator.h"
#include <algorithm>
#include "RoomContentConfig.h"
#include "GeometryUtils.h"
#include "RoomLayoutConfig.h"

using namespace WorldGenerator;

std::vector<MeshBox> PropMeshGenerator::GenerateMeshForProp(GeneratedRoom room, Prop prop)
{
	std::vector<MeshBox> mesh;

	float min_box_size = std::max<float>(prop.size.x, prop.size.y) + 2.0 * RoomContentConfig::MIN_PROP_OFFSET;

	int x_boxes = room.size.x / min_box_size;
	int y_boxes = room.size.y / min_box_size;

	for (int x_idx = 0; x_idx < x_boxes; x_idx++)
	{
		for (int y_idx = 0; y_idx < y_boxes; y_idx++)
		{
			DirectX::XMFLOAT2 box_size(min_box_size, min_box_size);

			// Extend the box to room borders
			if (x_idx == x_boxes - 1)
				box_size.x = room.size.x - x_idx * min_box_size;

			if (y_idx == y_boxes - 1)
				box_size.y = room.size.y - y_idx * min_box_size;

			DirectX::XMFLOAT2 box_pos(room.pos.x + x_idx * min_box_size, room.pos.y + y_idx * min_box_size);
			mesh.push_back(MeshBox(box_pos, box_size));
		}
	}

	return mesh;
}

void PropMeshGenerator::DeleteUnavailableBoxes(std::vector<MeshBox>& mesh, GeneratedRoom room)
{
	mesh.erase(std::remove_if(mesh.begin(), mesh.end(), [&](const MeshBox& box)
		{
			return BoxUnavailable(box, room);
		}), mesh.end());
}

DirectX::XMFLOAT2 PropMeshGenerator::PlacePropInBox(MeshBox box, const Prop& prop)
{
	float min_x = box.pos.x;
	float max_x = box.pos.x + box.size.x - prop.size.x;
	float min_y = box.pos.y;
	float max_y = box.pos.y + box.size.y - prop.size.y;

	float x = RNG::RandFloatInRange(min_x, max_x);
	float y = RNG::RandFloatInRange(min_y, max_y);

	return DirectX::XMFLOAT2(x,y);
}

bool PropMeshGenerator::BoxUnavailable(MeshBox box, const GeneratedRoom& room)
{
	// Delete boxes overlapping with another props
	for (const auto& prop : room.props)
	{
		DirectX::XMFLOAT2 prop_pos_2f(prop.position.x, prop.position.y);
		DirectX::XMFLOAT2 prop_size_2f(prop.size.x, prop.size.y);

		if (GeometryUtils::BoxesIntersect(box.pos, box.size, prop_pos_2f, prop_size_2f))
			return true;
	}

	// Delete boxes blocking doors or stairs
	for (const auto& link : room.links)
	{
		// Clear box area in front of the doors
		if (link.orientation == XZ)
		{
			DirectX::XMFLOAT2 door_box_pos(link.pos.x, link.pos.y == room.pos.y ? link.pos.y : link.pos.y - RoomContentConfig::MIN_PROP_OFFSET);
			DirectX::XMFLOAT2 door_box_size(1, RoomContentConfig::MIN_PROP_OFFSET);

			if (GeometryUtils::BoxesIntersect(box.pos, box.size, door_box_pos, door_box_size))
				return true;
		}
		if (link.orientation == YZ)
		{
			DirectX::XMFLOAT2 door_box_pos(link.pos.x == room.pos.x ? link.pos.x : link.pos.x - RoomContentConfig::MIN_PROP_OFFSET, link.pos.y);
			DirectX::XMFLOAT2 door_box_size(RoomContentConfig::MIN_PROP_OFFSET, 1);

			if (GeometryUtils::BoxesIntersect(box.pos, box.size, door_box_pos, door_box_size))
				return true;
		}
		// Clear boxes colliding with stairs
		if (link.orientation == XYX)
		{
			DirectX::XMFLOAT2 stair_box_pos(link.pos.x, link.pos.y);
			DirectX::XMFLOAT2 stair_box_size(RoomLayoutConfig::verticalRoomLinkLength, RoomLayoutConfig::verticalRoomLinkWidth);

			if (GeometryUtils::BoxesIntersect(box.pos, box.size, stair_box_pos, stair_box_size))
				return true;
		}
		if (link.orientation == XYY)
		{
			DirectX::XMFLOAT2 stair_box_pos(link.pos.x, link.pos.y);
			DirectX::XMFLOAT2 stair_box_size(RoomLayoutConfig::verticalRoomLinkWidth, RoomLayoutConfig::verticalRoomLinkLength);

			if (GeometryUtils::BoxesIntersect(box.pos, box.size, stair_box_pos, stair_box_size))
				return true;
		}
	}
	return false;
}