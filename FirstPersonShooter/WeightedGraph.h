#include "Graph.h"

#pragma once

namespace WorldGenerator
{
	struct WeightedEdge
	{
		int from;
		int to;
		int weight;

		WeightedEdge(int from, int to, int weight)
			: from(from), to(to), weight(weight)
		{
		}
	};

	template <typename T>
	class WeightedGraph : Graph<T>
	{
	public:
		WeightedGraph(Graph<T> graph);
		
		void SetEdge(WeightedEdge e);
		std::vector<WeightedEdge> GetAllEdges();
		std::vector<WeightedEdge> GetOutgoingEdges(int v);
	};
	template<typename T>
	inline WeightedGraph<T>::WeightedGraph(Graph<T> graph) : Graph<T>(graph)
	{
	}
	template<typename T>
	inline void WeightedGraph<T>::SetEdge(WeightedEdge e)
	{
		adMatrix[e.from][e.to] = e.weight;
	}
	template<typename T>
	inline std::vector<WeightedEdge> WeightedGraph<T>::GetAllEdges()
	{
		std::vector<WeightedEdge> edges;
		for(int v = 0; v < Size(); v++)
			for (int u = 0; u < Size(); u++)
				if (adMatrix[v][u] > 0)
					edges.push_back(WeightedEdge(v, u, adMatrix[v][u]));
		return edges;
	}
	template<typename T>
	inline std::vector<WeightedEdge> WeightedGraph<T>::GetOutgoingEdges(int v)
	{
		std::vector<WeightedEdge> edges;
		for (int u = 0; u < Size(); u++)
			if (adMatrix[v][u] > 0)
				edges.push_back(WeightedEdge(v, u, adMatrix[v][u]));
		return edges;
	}
}