#include "pch.h"
#include "Pathfinder.h"
#include "Room.hpp"
#include "PathNodeData.h"
#include "RoomLinkData.hpp"

#include <stack>
#include <set>

void Pathfinder::AddRoomNodes(const Room& room)
{
	const float wallOffset = 0.5f;
	const float targetDistance = 1.f;
	float height = room.getPosition().y + 1.f;
	
	int nNodesX = (room.getSize().x - 2 * wallOffset) / targetDistance;
	int nNodesZ = (room.getSize().z - 2 * wallOffset) / targetDistance;
	
	float distanceX = room.getSize().x / nNodesX;
	float distanceZ = room.getSize().z / nNodesZ;


	for (int i = 0; i < nNodesX; i++)
	{
		for (int j = 0; j < nNodesZ; j++)
		{
			nodes.push_back({ room.getPosition().x + wallOffset + i * distanceX, height, room.getPosition().z + wallOffset + j * distanceZ });
			edges.push_back(std::vector<int>());

			if (j)
			{
				AddEdge(edges.size() - 1, edges.size() - 2);
			}

			if (i)
			{
				AddEdge(edges.size() - 1, edges.size() - 1 - nNodesZ);
			}

			if (i && j)
			{
				AddEdge(edges.size() - 1, edges.size() - 2 - nNodesZ);
			}
		}
	}
}

void Pathfinder::AddDoorNodes(const RoomLinkData& link, int roomIdx)
{
	DirectX::XMFLOAT3 nodePos = { link.pos.x + link.size.x / 2, link.pos.y, link.pos.z + link.size.z / 2 };

	int closestRoom1 = FindClosestNodeInARoom(nodePos, roomIdx);
	int closestRoom2 = FindClosestNodeInARoom(nodePos, link.roomId);

	nodes.push_back(nodePos);
	edges.push_back(std::vector<int>());

	AddEdge(closestRoom1, nodes.size() - 1);
	AddEdge(closestRoom2, nodes.size() - 1);
}

void Pathfinder::AddStairsConnectionNode(const RoomLinkData& link, int roomIdx1, int roomIdx2)
{
	float xOffset = link.orientation == OrientationData::XZX ? link.size.x : link.size.x / 2;
	float zOffset = link.orientation == OrientationData::XZZ ? link.size.z : link.size.z / 2;

	DirectX::XMFLOAT3 nodePosition = { link.pos.x + xOffset, link.pos.y + link.size.y, link.pos.z + zOffset };
	int node1 = FindClosestNodeInARoom(nodePosition, roomIdx1);
	int node2 = FindClosestNodeInARoom(nodePosition, roomIdx2);

	nodes.push_back(nodePosition);
	edges.push_back(std::vector<int>());

	AddEdge(node1, nodes.size() - 1);
	AddEdge(node2, nodes.size() - 1);
}

void Pathfinder::DeleteStairsNodes(const RoomLinkData& link, int roomIdx)
{
	std::stack<int> markedForDeletion;
	for (int i = roomNodeIndexPrefix[roomIdx] + 1; i < nodes.size(); i++)
	{
		auto pos = nodes[i];
		if (
			pos.x >= link.pos.x && pos.x <= link.pos.x + link.size.x &&
			pos.z >= link.pos.z && pos.z <= link.pos.z + link.size.z)
		{
			markedForDeletion.push(i);
		}
	}
	
	while (!markedForDeletion.empty())
	{
		int toBeDeleted = markedForDeletion.top();

		for (int neighbour : edges[toBeDeleted])
		{
			std::remove_if(edges[neighbour].begin(), edges[neighbour].end(), [&](int x) { return x == toBeDeleted; });
		}

		nodes.erase(nodes.begin() + toBeDeleted);
		edges.erase(edges.begin() + toBeDeleted);

		markedForDeletion.pop();
	}
}

void Pathfinder::AdjustStairsNodes(const RoomLinkData& link, int roomIdx)
{
	for (int i = roomNodeIndexPrefix[roomIdx] + 1; i < nodes.size(); i++)
	{
		auto pos = nodes[i];
		if (
			pos.x >= link.pos.x && pos.x <= link.pos.x + link.size.x &&
			pos.z >= link.pos.z && pos.z <= link.pos.z + link.size.z)
		{
			nodes[i].y = GetAdjustedHeight(link, nodes[i]);

			for (int neighbour : edges[i])
			{
				std::remove_if(edges[neighbour].begin(), edges[neighbour].end(), [&](int x) 
					{ return x == i && (nodes[x].y != link.pos.y || nodes[x].y <= nodes[i].y); });
			}
		}
	}
}

float Pathfinder::GetAdjustedHeight(const RoomLinkData& link, DirectX::XMFLOAT3 position)
{
	if (link.orientation == OrientationData::XZX)
		return position.y + ((position.x - link.pos.x) / link.size.x) * link.size.y;
	else if (link.orientation == OrientationData::XZZ)
		return position.y + ((position.z - link.pos.z) / link.size.z) * link.size.y;
	else return position.y;
}

int Pathfinder::FindClosestNodeInARoom(DirectX::XMFLOAT3 position, int roomIdx) const
{
	int closest_idx = roomNodeIndexPrefix[roomIdx];
	float min_dist = DistSquared(position, nodes[closest_idx]);
	int upper_bound = roomIdx == roomNodeIndexPrefix.size() - 1 ? nodes.size() : roomNodeIndexPrefix[roomIdx + 1];
	for (int i = roomNodeIndexPrefix[roomIdx] + 1; i < upper_bound; i++)
	{
		float dist = DistSquared(position, nodes[i]);
		if (dist < min_dist)
		{
			min_dist = dist;
			closest_idx = i;
		}
	}

	return closest_idx;
}

int Pathfinder::FindClosestNode(DirectX::XMFLOAT3 position) const
{
	// can be optimized
	float minDist = DistSquared(position, nodes[0]);
	int closest = 0;
	for (int i = 1; i < nodes.size(); i++)
	{
		if (minDist > DistSquared(position, nodes[i]))
		{
			minDist = DistSquared(position, nodes[i]);
			closest = i;
		}
	}
	return closest;
}

float Pathfinder::Dist(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2) const
{
	return sqrtf(DistSquared(p1, p2));
}

float Pathfinder::DistSquared(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2) const
{
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z);
}

void Pathfinder::AddEdge(int u, int v)
{
	edges[u].push_back(v);
	edges[v].push_back(u);
}

std::vector<int> Pathfinder::AStar(int start, int end) const
{
	int n = nodes.size();
	float unreachable = 100000.f;
	std::vector<float> sol(n, unreachable);
	std::vector<float> fScore(n, unreachable);
	std::vector<int> prev(n, -1);
	std::set<std::pair<float, int>> pq;

	for (int i = 0; i < n; i++)
	{
		if (i == start)
		{
			pq.insert(std::make_pair(0, i));
		}
		else pq.insert(std::make_pair(unreachable, i));
	}

	sol[start] = 0;
	fScore[start] = Dist(nodes[start], nodes[end]);


	while (!pq.empty())
	{
		auto elem = *pq.begin();
		pq.erase(pq.begin());
		if (elem.first == unreachable || elem.second == end)
		{
			break;
		}

		for (auto v : edges[elem.second])
		{
			if (sol[elem.second] + Dist(nodes[elem.second], nodes[v]) < sol[v])
			{
				auto it = pq.find(std::make_pair(fScore[v], v));
				if (it != pq.end())
				{
					pq.erase(it);
					prev[v] = elem.second;
					sol[v] = sol[elem.second] + Dist(nodes[elem.second], nodes[v]);
					fScore[v] = sol[v] + Dist(nodes[v], nodes[end]);
					pq.insert(std::make_pair(fScore[v], v));
				}
			}
		}
	}

	return prev;
}

std::list<PathNodeData> Pathfinder::ConstructPath(const std::vector<int>& prev, int start) const
{
	int currVertex = start;
	std::list<PathNodeData> result;

	while (currVertex != -1)
	{
		result.push_front(PathNodeData(currVertex, nodes[currVertex]));
		currVertex = prev[currVertex];
	}

	return result;
}

Pathfinder::Pathfinder(const std::vector<Room>& rooms, DirectX::XMFLOAT3 playerPos)
{
	roomNodeIndexPrefix.push_back(0);

	for (int i = 0; i < rooms.size(); i++)
	{
		AddRoomNodes(rooms[i]);
		for (const auto& link : rooms[i].m_links)
		{
		
			if (link.orientation == OrientationData::XZX || link.orientation == OrientationData::XZZ)
			{
				if (link.roomId > i)
				{
					AdjustStairsNodes(link, i);
				}
				else
				{
					DeleteStairsNodes(link, i);
					AddStairsConnectionNode(link, i, link.roomId);
				}

			}
			else if (link.roomId < i)
			{
				AddDoorNodes(link, i);
			}
		}

		roomNodeIndexPrefix.push_back(nodes.size() + 1);
	}

	playerNode = FindClosestNode(playerPos);
}

std::list<PathNodeData> Pathfinder::FindPath(DirectX::XMFLOAT3 enemyPos) const
{
	int enemyNode = FindClosestNode(enemyPos);
	auto nodeParents = AStar(enemyNode, playerNode);
	return ConstructPath(nodeParents, playerNode);
}

void Pathfinder::UpdatePath(std::list<PathNodeData>& path, DirectX::XMFLOAT3 currPos) const
{
	if (path.size() == 1)
		return;

	// check if enemy moved
	auto currNode = path.begin();
	auto nextNode = std::next(currNode);

	if (DistSquared(currNode->position, currPos) > DistSquared(nextNode->position, currPos))
	{
		path.pop_front();
	}

	// check if player moved
	if (playerNode != path.back().index)
	{
		auto lastNode = std::prev(path.end());
		auto secondLastNode = std::prev(lastNode);

		if (DistSquared(lastNode->position, nodes[playerNode]) > DistSquared(secondLastNode->position, nodes[playerNode]))
		{
			path.pop_back();
			if (!(secondLastNode->index == playerNode))
			{
				path.push_back(PathNodeData(playerNode, nodes[playerNode]));
			}
		}
		else
		{
			path.push_back(PathNodeData(playerNode, nodes[playerNode]));
		}
	}
}

void Pathfinder::UpdatePlayerNode(DirectX::XMFLOAT3 playerPos)
{
	float minDist = DistSquared(playerPos, nodes[playerNode]);
	for (int neighbour : edges[playerNode])
	{
		if (minDist > DistSquared(playerPos, nodes[neighbour]))
		{
			minDist = DistSquared(playerPos, nodes[neighbour]);
			playerNode = neighbour;
		}
	}
}
