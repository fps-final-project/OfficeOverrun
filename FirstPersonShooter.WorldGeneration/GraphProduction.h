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
		int priority;
	public:
		int Priority() { return priority; }
		Graph<T> L_graph() { return l_graph; };
		Graph<T> R_graph() { return r_graph; };

		// Returns where the production can be applied
		std::vector<std::vector<int>> Match(Graph<T> &G);

		void Apply(Graph<T>& G, const std::vector<int> &match);

		GraphProduction(int priority, const Graph<T>& l_graph, const Graph<T>& r_graph)
			: priority(priority), l_graph(l_graph), r_graph(r_graph)
		{
		}

		GraphProduction(const GraphProduction<T>& other)
			: priority(other.priority), l_graph(other.l_graph), r_graph(other.r_graph)
		{
		}

		GraphProduction<T>& operator=(const GraphProduction<T>& other)
		{
			return GraphProduction(other);
		}
	};

	template<typename T>
	inline std::vector<std::vector<int>> GraphProduction<T>::Match(Graph<T> &G)
	{
		return SubgraphMatcher::MatchSubgraph(l_graph, G);
	}

	template<typename T>
	inline void GraphProduction<T>::Apply(Graph<T>& G,const std::vector<int> &match)
	{
		if (l_graph.Size() != r_graph.Size())
			throw std::exception("Error: Production containg vertex addition/deletion not implemented");

		// Relabel vertices
		for (int i = 0; i < match.size(); i++)
			G[match[i]].label = LabelHelpers::TransformLabel(G[match[i]].label, r_graph[i].label);

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