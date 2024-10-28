#pragma once

#include <vector>
#include "Node.h"

namespace WorldGenerator
{
	template <typename T>
	class Graph
	{
	public:
		std::vector<Node<T>> nodes;

		Graph(std::vector<T>& values);
		Graph() = default;

		void DeleteEdge(int from, int to);
	};

	template<typename T>
	inline Graph<T>::Graph(std::vector<T>& values)
	{
		nodes = std::vector<Node<T>>();
		for (auto& val : values)
		{
			nodes.push_back(Node<T>(&val));
		}
	}
	template<typename T>
	inline void Graph<T>::DeleteEdge(int from, int to)
	{

	}
}