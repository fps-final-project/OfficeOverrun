#include "pch.h"
#include "RoomSelector.h"
#include "NotImplementedExecption.h"

using namespace WorldGenerator;

void WorldGenerator::RoomSelector::RemoveUpDownEdges()
{
	for (int i = 0; i < H.nodes.size(); i++)
	{
		Node<GeneratedRoom>& v = H.nodes[i];
		for (int j = 0; j < H.nodes.size(); j++)
		{
			Node<GeneratedRoom>& u = H.nodes[j];
			if (H.HasEdge(i,j) && v.value->IsAbove(*u.value))
			{
				H.DeleteEdge(i, j);
			}
		}
	}
}

Graph<GeneratedRoom> WorldGenerator::RoomSelector::SelectRooms()
{
	throw NotImplemented();
}
