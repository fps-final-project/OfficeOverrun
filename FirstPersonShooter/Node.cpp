#include "pch.h"
#include "Node.h"

using namespace WorldGenerator;

template<typename T>
bool Node<T>::IsConnectedTo(Node* neighbour)
{
	auto it = std::find(neighbours.begin(), neighbours.end(), neighbour);
	return it != neighbours..end();
}
