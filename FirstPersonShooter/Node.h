#pragma once

#include <vector>

template <typename T>
class Node
{
public:
	T value;
	std::vector<Node*> neighbours;
	Node(T value);
};

template<typename T>
inline Node<T>::Node(T value)
	: value(value)
{
	neighbours = std::vector<Node*>();
}