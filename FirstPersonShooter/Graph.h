#pragma once

#include <vector>
#include "Node.h"

namespace WorldGenerator
{
	template <typename T>
	class Graph
	{
	private:
		void MakeAdMatrix(int size);
		void InitGraph(std::vector<T>& values, int maxSize);
		int maxSize;
	public:
		std::vector<Node<T>> nodes;
		std::vector<std::vector<int>> adMatrix;

		Graph(std::vector<T>& values);
		Graph(std::vector<T>& values, int maxSize);
		Graph() = default;

		bool HasEdge(int from, int to);
		void AddEdge(int from, int to);
		void AddUndirectedEdge(int from, int to);
		void DeleteEdge(int from, int to);
	};

	template<typename T>
	inline void Graph<T>::MakeAdMatrix(int size)
	{
		adMatrix = std::vector<std::vector<int>>(size, std::vector<int>(size));
	}

	template<typename T>
	inline void Graph<T>::InitGraph(std::vector<T>& values, int maxSize)
	{
		this->maxSize = maxSize;
		MakeAdMatrix(maxSize);
		nodes = std::vector<Node<T>>();
		for (auto& val : values)
		{
			nodes.push_back(Node<T>(&val));
		}
	}

	template<typename T>
	inline Graph<T>::Graph(std::vector<T>& values)
	{
		InitGraph(values, values.size());
	}
	template<typename T>
	inline Graph<T>::Graph(std::vector<T>& values, int maxSize)
	{
		InitGraph(values, maxSize);
	}
	template<typename T>
	inline bool Graph<T>::HasEdge(int from, int to)
	{
		return adMatrix[from][to] == 1;
	}
	template<typename T>
	inline void Graph<T>::AddEdge(int from, int to)
	{
		adMatrix[from][to] = 1;
	}
	template<typename T>
	inline void Graph<T>::AddUndirectedEdge(int from, int to)
	{
		adMatrix[from][to] = 1;
		adMatrix[to][from] = 1;
	}
	template<typename T>
	inline void Graph<T>::DeleteEdge(int from, int to)
	{
		adMatrix[from][to] = 0;
	}
}