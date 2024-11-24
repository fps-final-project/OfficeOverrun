#pragma once

#include <vector>
#include <functional>
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

		Graph(int maxSize);
		Graph(std::vector<T>& values);
		Graph(std::vector<T>& values, int maxSize);
		Graph() = default;

		int Size();
		void AddNode(Node<T> value);
		void AddNodes(std::vector<Node<T>> values);
		virtual bool HasEdge(int from, int to);
		void AddEdge(int from, int to);
		void AddUndirectedEdge(int from, int to);
		std::vector<int> GetNeighbours(int v);
		std::vector<int> GetIngoingNeighbours(int v);
		void DeleteEdge(int from, int to);
		std::vector<int> GetVerticesIf(std::function<bool(T)> predicate);

		Node<T> operator [](int i) const;
		Node<T>& operator [](int i);
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
	inline Graph<T>::Graph(int maxSize)
	{
		InitGraph(std::vector<T>(), maxSize);
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
	inline int Graph<T>::Size()
	{
		return nodes.size();
	}
	template<typename T>
	inline void Graph<T>::AddNode(Node<T> value)
	{
		if (Size() + 1 > maxSize)
			throw std::exception("Error: Exceed graph max size");
		nodes.push_back(value);
	}
	template<typename T>
	inline void Graph<T>::AddNodes(std::vector<Node<T>> values)
	{
		if (Size() + values.size() > maxSize)
			throw std::exception("Error: Exceed graph max size");
		nodes.insert(nodes.end(), values.begin(), values.end());
	}
	template<typename T>
	inline bool Graph<T>::HasEdge(int from, int to)
	{
		return adMatrix[from][to] > 0;
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
	inline std::vector<int> Graph<T>::GetNeighbours(int v)
	{
		std::vector<int> neighbours;
		for (int i = 0; i < adMatrix[v].size(); i++)
		{
			if (adMatrix[v][i] > 0)
				neighbours.push_back(i);
		}
		return neighbours;
	}
	template<typename T>
	inline std::vector<int> Graph<T>::GetIngoingNeighbours(int v)
	{
		std::vector<int> neighbours;
		for (int i = 0; i < adMatrix[v].size(); i++)
		{
			if (adMatrix[i][v] > 0)
				neighbours.push_back(i);
		}
		return neighbours;
	}
	template<typename T>
	inline void Graph<T>::DeleteEdge(int from, int to)
	{
		adMatrix[from][to] = 0;
	}
	template<typename T>
	inline std::vector<int> Graph<T>::GetVerticesIf(std::function<bool(T)> predicate)
	{
		std::vector<int> valid_vs;
		for (int v = 0; v < Size(); v++)
			if (predicate(*nodes[v].value))
				valid_vs.push_back(v);

		return valid_vs;
	}
	template<typename T>
	inline Node<T> Graph<T>::operator[](int i) const
	{
		return nodes[i];
	}
	template<typename T>
	inline Node<T>& Graph<T>::operator[](int i)
	{
		return nodes[i];
	}
}