#include "pch.h"
#include "RoomSelector.h"
#include "NotImplementedExecption.h"
#include <cmath>

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


WorldGenerator::RoomSelector::RoomSelector(RoomSelectorArgs args)
{
	H = args.initialGraph;
	s = args.startVertex;
	N = std::floor(H.nodes.size() * args.roomDensity);
	P = std::floor(N * args.pathLengthCoeff);
	e_c = args.edgeDensityCoeff;
}

Graph<GeneratedRoom> WorldGenerator::RoomSelector::SelectRooms()
{
	// Step 1
	RemoveUpDownEdges();

	// Step 2

	throw NotImplemented();
}
