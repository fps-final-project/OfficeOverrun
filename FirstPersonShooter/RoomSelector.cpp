#include "pch.h"
#include "RoomSelector.h"
#include "NotImplementedExecption.h"
#include <cmath>
#include <RngUtils.h>
#include <algorithm>

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

	// Steps 5-6
	AddSpareVertices();

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
		AddHVertexToG(P[i]);

		if (i > 0)
			G.AddUndirectedEdge(i - 1, i);
	}
}

void WorldGenerator::RoomSelector::AddHVertexToG(int v)
{
	int i = G.Size();
	
	auto node = H[v];
	G.AddNode(node);
	G_H_map.push_back(v);
	H_G_map[v] = i;
}

void WorldGenerator::RoomSelector::AddSpareVertices()
{
	ComputeNeighbourhood();
	while (G.Size() < N)
	{
		if (neigh_G.size() == 0)
			return; // sometimes if rooms are on many different levels some rooms may become unreachable

		int v = RngUtils::SelectRandomElement(neigh_G);
		AddHVertexToG(v);

		std::vector<int> connected = H.GetIngoingNeighbours(v);
		auto is_in_G = [&](int w) { return H_G_map[w] > 0; };
		int u = *std::find_if(connected.begin(), connected.end(), is_in_G);
		G.AddUndirectedEdge(H_G_map[u], H_G_map[v]);

		UpdateNeighbourhood(v);
	}
}

void WorldGenerator::RoomSelector::ComputeNeighbourhood()
{
	for (int i = 0; i < G.Size(); i++)
	{
		int v = G_H_map[i];
		for (int u : H.GetNeighbours(v))
		{
			// u is in not in G and not already in neigh_G
			if (H_G_map[u] == -1 && std::find(neigh_G.begin(), neigh_G.end(), u) == neigh_G.end())
			{
				neigh_G.push_back(u);
			}
		}
	}
}

void WorldGenerator::RoomSelector::UpdateNeighbourhood(int v) // v is vertex in H
{
	neigh_G.erase(std::remove(neigh_G.begin(), neigh_G.end(), v), neigh_G.end());
	for (int u : H.GetNeighbours(v))
	{
		// u is in not in G and not already in neigh_G
		if (H_G_map[u] == -1 && std::find(neigh_G.begin(), neigh_G.end(), u) == neigh_G.end())
		{
			neigh_G.push_back(u);
		}
	}
}
