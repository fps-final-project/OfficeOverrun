#pragma once

#include <vector>
namespace WorldGenerator
{
	template <typename T>
	class Node
	{
	public:
		T* value;
		std::vector<Node<T>*> neighbours;
		Node(T* value);
		bool IsConnectedTo(Node* neighbour);
	};

	template<typename T>
	inline Node<T>::Node(T* value)
		: value(value)
	{
		neighbours = std::vector<Node*>();
	}
	template<typename T>
	inline bool Node<T>::IsConnectedTo(Node* neighbour)
	{
		auto it = std::find(neighbours.begin(), neighbours.end(), neighbour);
		return it != neighbours.end();
	}
}