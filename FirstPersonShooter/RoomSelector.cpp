#include "pch.h"
#include "RoomSelector.h"
#include "NotImplementedExecption.h"
#include <cmath>
#include <RngUtils.h>

using namespace WorldGenerator;

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
	std::vector<int> path = GenerateRandomPath();

	// Step 3
	ConstructGFromPath(path);

	return G;
}


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

void WorldGenerator::RoomSelector::RandomDfs(int v, std::vector<bool>& visited, std::vector<int> &path, int tr_f)
{
	visited[v] = true;
	path.push_back(v);
	if (path.size() == N)
		return;

	auto neighbours = H.GetNeighbours(v);
	neighbours = RngUtils::ShuffleVector<int>(neighbours);

	for (int i = 0; i < neighbours.size(); i++)
	{
		int u = neighbours[i];
		if (visited[u] == true)
			continue;

		bool up = H.nodes[v].value->IsBelow(*H.nodes[u].value);

		// If below the minimal room number on the floor, try to find floor on the same floor
		if (up && tr_f < MIN_F)
		{
			for (int j = i + 1; j < neighbours.size(); j++)
			{
				if (!visited[neighbours[j]] && H.nodes[v].value->IsSameLevel(*H.nodes[neighbours[j]].value))
				{
					u = neighbours[j];
				}
			}
		}

		RandomDfs(u, visited, path, up? 1 : tr_f + 1);
		return;
	}
	return;
}

std::vector<int> WorldGenerator::RoomSelector::GenerateRandomPath()
{
	int n = H.nodes.size();
	std::vector<bool> visited(n);
	std::vector<int> path;

	RandomDfs(s, visited, path, 1);

	return path;
}

void WorldGenerator::RoomSelector::ConstructGFromPath(std::vector<int> P)
{
	G = Graph<GeneratedRoom>(H.Size());
	
	for (int i = 0; i < P.size(); i++)
	{
		int v = P[i];
		auto node = H[v];
		G.AddNode(node);

		if (i > 0)
			G.AddUndirectedEdge(i - 1, i);

		G_H_map.push_back(v);
	}
}
