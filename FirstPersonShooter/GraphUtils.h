#include "Graph.h"
#include <utility>

#pragma once

namespace WorldGenerator
{
	class GraphUtils
	{
	public:
		// Returns pair of graph induced with vertices and vector mapping new graph vertices to G vertices
		template<typename T>
		static std::pair<Graph<T>, std::vector<int>> GenerateInducedGraph(Graph<T> G, std::vector<int> vertices);
		static int FindValueIndexInMap(std::vector<int> map, int value);
	};

	template<typename T>
	inline std::pair<Graph<T>, std::vector<int>> GraphUtils::GenerateInducedGraph(Graph<T> G, std::vector<int> vertices)
	{
		Graph<GeneratedRoom> C(G.Size());
		std::vector<int> C_G_map;
		std::vector<int> G_C_map(G.Size(), -1);

		// Adding vertices and updating map
		for (int v : vertices)
		{
			C.AddNode(G[v]);
			C_G_map.push_back(v);
			G_C_map[v] = C.Size() - 1;
		}

		// Adding edges
		for (int v_C = 0; v_C < C.Size(); v_C++)
		{
			int v_G = C_G_map[v_C];
			for (int u_G : G.GetNeighbours(v_G))
			{
				int u_C = G_C_map[u_G];
				if (u_C == -1) // u_G not in C
					continue;
				C.AddEdge(v_C, u_C);
			}
		}

		return std::pair(C, C_G_map);
	}
}

