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
	N = std::floor(H.Size() * args.roomDensity);
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

	// Step 4
	RemoveDownUpEdges();

	// Step 5

	return G;
}


void WorldGenerator::RoomSelector::RemoveUpDownEdges()
{
	for (int i = 0; i < H.Size(); i++)
	{
		Node<GeneratedRoom>& v = H[i];
		for (int j = 0; j < H.Size(); j++)
		{
			Node<GeneratedRoom>& u = H[j];
			if (H.HasEdge(i,j) && v.value->IsAbove(*u.value))
			{
				H.DeleteEdge(i, j);
			}
		}
	}
}


void WorldGenerator::RoomSelector::RemoveDownUpEdges()
{
	for (int i = 0; i < H.Size(); i++)
	{
		Node<GeneratedRoom>& v = H[i];
		for (int j = 0; j < H.Size(); j++)
		{
			Node<GeneratedRoom>& u = H[j];
			if (H.HasEdge(i, j) && v.value->IsBelow(*u.value))
			{
				H.DeleteEdge(i, j);
			}
		}
	}
}

std::vector<int> WorldGenerator::RoomSelector::GenerateRandomPath()
{
	int n = H.Size();
	std::vector<bool> visited(n);
	std::vector<int> path;

	RandomDfs(s, visited, path, 1);

	return path;
}

void WorldGenerator::RoomSelector::RandomDfs(int v, std::vector<bool>& visited, std::vector<int>& path, int tr_f)
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

		bool up = H[v].value->IsBelow(*H[u].value);

		// If below the minimal room number on the floor, try to find floor on the same floor
		if (up && tr_f < MIN_F)
		{
			for (int j = i + 1; j < neighbours.size(); j++)
			{
				if (!visited[neighbours[j]] && H[v].value->IsSameLevel(*H[neighbours[j]].value))
				{
					u = neighbours[j];
				}
			}
		}

		RandomDfs(u, visited, path, up ? 1 : tr_f + 1);
		return;
	}
	return;
}


void WorldGenerator::RoomSelector::ConstructGFromPath(std::vector<int> P)
{
	G = Graph<GeneratedRoom>(H.Size());
	H_G_map = std::vector<int>(H.Size(), -1);
	
	for (int i = 0; i < P.size(); i++)
	{
		int v = P[i];
		auto node = H[v];
		G.AddNode(node);

		if (i > 0)
			G.AddUndirectedEdge(i - 1, i);

		G_H_map.push_back(v);
		H_G_map[v] = i;
	}
}

std::vector<int> WorldGenerator::RoomSelector::ComputeNeighbourhood()
{
	std::vector<int> N_G;
	for (int i = 0; i < G.Size(); i++)
	{
		int v = G_H_map[i];

	}
}