#pragma once

#include <vector>
namespace WorldGenerator
{
	template <typename T>
	class Node
	{
	public:
		T* value;
		int label;
		Node(T* value);
	};

	template<typename T>
	inline Node<T>::Node(T* value)
		: value(value)
	{
	}
}