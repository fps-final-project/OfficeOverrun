#include "pch.h"
#include "PropMeshGenerator.h"
#include <algorithm>
#include "RoomContentConfig.h"
#include "GeometryUtils.h"

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
	return false;
}