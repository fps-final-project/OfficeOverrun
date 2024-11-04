#include "Graph.h"
#include "WeightedGraph.h"
#include <limits.h>

#pragma once

namespace WorldGenerator
{
	class PathFinding
	{
	private:
		int DijkstraFindMinDistance(std::vector<int>& dist, std::vector<bool>& vis);
	public:
		// Uses dijkstra algorithm to find shortest path, returns the shortest path, algorithm https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
		template <typename T>
		static std::vector<int> FindShortestPathsDijkstra(WeightedGraph<T> G, int s, int t);
	};

	inline int PathFinding::DijkstraFindMinDistance(std::vector<int>& dist, std::vector<bool>& vis)
	{
		int min_dist = INT_MAX, min_idx = -1;

		for (int v = 0; v < dist.size(); v++)
			if (!vis[v] && dist[v] < min_dist)
				min_dist = dist[v], min_idx = v;

		return min_idx;
	}

	template<typename T>
	inline std::vector<int> PathFinding::FindShortestPathsDijkstra(WeightedGraph<T> G, int s, int t)
	{
		// Initialize distance vector
		std::vector<int> dist(G.Size(), INT_MAX);
		dist[s] = 0;
		// Initialize visited vector
		std::vector<bool> vis(G.Size());
		vis[s] = true;
		// Initialize came_from
		std::vector<int> came_from(G.Size(), -1);
		
		// Repeat N - 1 times
		for (int i = 0; i < G.Size() - 1; i++)
		{
			// Get min distance vertex
			int u = DijkstraFindMinDistance(dist, vis);

			if (u == -1) // No reachable vertex
				break;
			
			vis[u] = true;

			// Update neighbours if better path is available from u
			for (WeightedEdge e : G.GetOutgoingEdges(u))
				if (dist[e.from] + e.weight < dist[e.to])
					dist[e.to] = dist[e.from] + e.weight, came_from[e.to] = e.from;
		}
		
		// If unreachable
		if(dist[t] == INT_MAX)
			return std::vector<int>().push_back(-1);
		
		// Retrieve s-t path
		std::vector<int> path;
		int cur = t;
		while (cur != -1)
		{
			path.insert(path.begin(), cur);
			cur = came_from[cur];
		}
		return path;
	}
}