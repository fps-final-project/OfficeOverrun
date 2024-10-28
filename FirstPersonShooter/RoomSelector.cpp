#include "pch.h"
#include "RoomSelector.h"
#include "NotImplementedExecption.h"

using namespace WorldGenerator;

void WorldGenerator::RoomSelector::RemoveUpDownEdges()
{
	for (int i = 0; i < H.nodes.size(); i++)
	{
		Node<GeneratedRoom>& v = H.nodes[i];
		for (auto& k : v.neighbours)
		{
			Node<GeneratedRoom>& u = H.nodes[k];
			if (v.value->IsAbove(*u.value))
			{
				H.DeleteEdge(k, i);
			}
		}
	}
}

Graph<GeneratedRoom> WorldGenerator::RoomSelector::SelectRooms()
{
	throw NotImplemented();
}
