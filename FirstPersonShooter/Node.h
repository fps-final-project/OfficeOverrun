#pragma once

#include <vector>
namespace WorldGenerator
{
	template <typename T>
	class Node
	{
	public:
		T* value;
		std::vector<int> neighbours;
		Node(T* value);
		bool IsConnectedTo(int neighbour);
	};

	template<typename T>
	inline Node<T>::Node(T* value)
		: value(value)
	{
		neighbours = std::vector<int>();
	}
	template<typename T>
	inline bool Node<T>::IsConnectedTo(int neighbour)
	{
		auto it = std::find(neighbours.begin(), neighbours.end(), neighbour);
		return it != neighbours.end();
	}
}