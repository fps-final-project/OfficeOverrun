#pragma once
#include "SubgraphMatcher.h"

namespace WorldGenerator
{
	template <typename T>
	class GraphProduction
	{
	private:
		Graph<T> l_graph;
		Graph<T> r_graph;

	public:
		// Returns where the production can be applied
		std::vector<std::vector<int>> Match(const Graph<T> &G);

		void Apply(Graph<T>& G, std::vector<int> match);
	};

	template<typename T>
	inline std::vector<std::vector<int>> GraphProduction<T>::Match(const Graph<T> &G)
	{
		return SubgraphMatcher::MatchSubgraph(l_graph, G);
	}
	template<typename T>
	inline void GraphProduction<T>::Apply(Graph<T>& G, std::vector<int> match)
	{
		// Relabel vertices
		for (int i = 0; i < match.size(); i++)
			G[match[i]].label = r_graph[i].label;

		// Modify edges
		for (int i = 0; i < match.size(); i++)
		{
			for (int j = 0; j < match.size(); j++)
			{
				if(r_graph.HasEdge(i,j))
					G.AddEdge(match[i], match[j]);
				else
					G.DeleteEdge(match[i], match[j]);
			}
		}
	}
}