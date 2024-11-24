#include "pch.h"
#include "Pathfinder.h"
#include "Room.hpp"
#include "PathNodeData.h"
#include "RoomLinkData.hpp"
#include "Path.h"

#include <stack>

void Pathfinder::AddRoomNodes(const Room& room)
{
	const float wallOffset = 0.5f;
	const float targetDistance = 0.8f;
	float height = room.getPosition().y;

	int nNodesX = (room.getSize().x - 2 * wallOffset) / targetDistance;
	int nNodesZ = (room.getSize().z - 2 * wallOffset) / targetDistance;

	float distanceX = room.getSize().x / nNodesX;
	float distanceZ = room.getSize().z / nNodesZ;


	for (int i = 0; i < nNodesX; i++)
	{
		for (int j = 0; j < nNodesZ; j++)
		{
			nodes.push_back({ room.getPosition().x + wallOffset + i * distanceX, height, room.getPosition().z + wallOffset + j * distanceZ });
			edges.push_back(std::set<int>());

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
	nodeToRoomLink[std::make_pair(link.roomId, roomIdx)] = nodes.size() - 1;
	edges.push_back(std::set<int>());

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
	edges.push_back(std::set<int>());

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
			EraseFromNeighbourSetIf(edges[neighbour], [&](int x) { return x == toBeDeleted; });
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
				EraseFromNeighbourSetIf(edges[neighbour], [&](int x)
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

int Pathfinder::FindRoomIdx(int nodeIdx) const
{
	// binary search
	int l = 0, r = roomNodeIndexPrefix.size() - 2;
	int mid = l + (r - l) / 2;
	while (l < r)
	{
		if (nodeIdx < roomNodeIndexPrefix[mid])
		{
			r = mid - 1;
		}
		else if (nodeIdx >= roomNodeIndexPrefix[mid + 1])
		{
			l = mid + 1;
		}
		else break;

		mid = l + (r - l) / 2;
	}

	return mid;
}

int Pathfinder::FindLinkNode(int room1, int room2) const
{
	if (room1 > room2)
	{
		int tmp = room1;
		room1 = room2;
		room2 = tmp;
	}

	auto link = nodeToRoomLink.find(std::make_pair(room1, room2));
	return link == nodeToRoomLink.end() ? -1 : link->second;
}

float Pathfinder::Dist(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2) const
{
	return sqrtf(DistSquared(p1, p2));
}

float Pathfinder::DistSquared(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2) const
{
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z);
}

float Pathfinder::H(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2) const
{
	return std::abs(p1.x - p2.x) + std::abs(p1.z - p2.z);
}

void Pathfinder::AddEdge(int u, int v)
{
	edges[u].insert(v);
	edges[v].insert(u);
}

void Pathfinder::EraseFromNeighbourSetIf(std::set<int>& set, const std::function<bool(int)>& predicate)
{
	for (auto it = set.begin(); it != set.end(); ) {
		if (predicate(*it)) {
			it = set.erase(it);
		}
		else {
			++it;
		}
	}
}

int Pathfinder::UpdateNode(DirectX::XMFLOAT3 position, int currIndex) const
{
	int result = currIndex;
	float minDist = DistSquared(position, nodes[currIndex]);
	for (int neighbour : edges[currIndex])
	{
		if (minDist > DistSquared(position, nodes[neighbour]))
		{
			minDist = DistSquared(position, nodes[neighbour]);
			result = neighbour;
		}
	}

	return result;
}

bool Pathfinder::NodesInNeighboringRooms(int n1, int n2) const
{
	int r1 = FindRoomIdx(n1);
	int r2 = FindRoomIdx(n2);

	if (r1 > r2)
	{
		int tmp = r1;
		r1 = r2;
		r2 = tmp;
	}

	return r1 == r2 || nodeToRoomLink.find(std::make_pair(r1, r2)) != nodeToRoomLink.end();
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
			if (fScore[elem.second] + Dist(nodes[elem.second], nodes[v]) < fScore[v])
			{
				auto it = pq.find(std::make_pair(fScore[v], v));
				if (it != pq.end())
				{
					pq.erase(it);
					prev[v] = elem.second;
					sol[v] = sol[elem.second] + Dist(nodes[elem.second], nodes[v]);
					fScore[v] = sol[v] + H(nodes[v], nodes[end]);
					pq.insert(std::make_pair(fScore[v], v));
				}
			}
		}
	}

	return prev;
}

std::vector<int> Pathfinder::AStarRoom(int start, int end, int roomId) const
{
	int n = roomNodeIndexPrefix[roomId + 1] - roomNodeIndexPrefix[roomId];
	int idxOffset = roomNodeIndexPrefix[roomId];

	float unreachable = 100000.f;
	std::vector<float> sol(n, unreachable);
	std::vector<float> fScore(n, unreachable);
	std::vector<int> prev(n, -1);
	std::set<std::pair<float, int>> pq;

	for (int i = roomNodeIndexPrefix[roomId]; i < roomNodeIndexPrefix[roomId + 1]; i++)
	{
		if (i == start)
		{
			pq.insert(std::make_pair(0, i));
		}
		else pq.insert(std::make_pair(unreachable, i));
	}

	sol[start - idxOffset] = 0;
	fScore[start - idxOffset] = H(nodes[start], nodes[end]);


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
			if (v >= roomNodeIndexPrefix[roomId] && v < roomNodeIndexPrefix[roomId + 1] 
				&& sol[elem.second - idxOffset] + Dist(nodes[elem.second], nodes[v]) < sol[v - idxOffset])
			{
				auto it = pq.find(std::make_pair(fScore[v - idxOffset], v));
				if (it != pq.end())
				{
					pq.erase(it);
					prev[v - idxOffset] = elem.second;
					sol[v - idxOffset] = sol[elem.second - idxOffset] + Dist(nodes[elem.second], nodes[v]);
					fScore[v - idxOffset] = sol[v - idxOffset] + H(nodes[v], nodes[end]);
					pq.insert(std::make_pair(fScore[v - idxOffset], v));
				}
			}
		}
	}

	return prev;
}

std::list<PathNodeData> Pathfinder::ConstructPath(const std::vector<int>& prev, int start, int idxOffset) const
{
	int currVertex = start;
	std::list<PathNodeData> result;

	while (currVertex != -1)
	{
		result.push_front(PathNodeData(currVertex, nodes[currVertex]));
		currVertex = prev[currVertex - idxOffset];
	}

	return result;
}

Pathfinder::Pathfinder(const std::vector<Room>& rooms, DirectX::XMFLOAT3 playerPos)
{
	roomNodeIndexPrefix.push_back(0);

	for (int i = 0; i < rooms.size(); i++)
	{
		AddRoomNodes(rooms[i]);

		// process stairs first, we ensure correct values in room links map after deletion of some nodes
		auto stairsIt = std::find_if(rooms[i].m_links.begin(), rooms[i].m_links.end(), [](const RoomLinkData& link) {
			return link.orientation == OrientationData::XZX || link.orientation == OrientationData::XZZ;
			});

		if (stairsIt != rooms[i].m_links.end())
		{
			if (stairsIt->roomId > i)
			{
				AdjustStairsNodes(*stairsIt, i);
			}
			else
			{
				DeleteStairsNodes(*stairsIt, i);
				AddStairsConnectionNode(*stairsIt, i, stairsIt->roomId);
			}
		}

		for (const auto& link : rooms[i].m_links)
		{
			// process only doors
			if ((link.orientation == OrientationData::XY || link.orientation == OrientationData::ZY) && link.roomId < i)
			{
				AddDoorNodes(link, i);
			}
		}

		roomNodeIndexPrefix.push_back(nodes.size() + 1);
	}

	playerNode = FindClosestNode(playerPos);
}

Path Pathfinder::FindPath(DirectX::XMFLOAT3 enemyPos) const
{
	int enemyNode = FindClosestNode(enemyPos);
	return FindPathFromNode(enemyNode);
}

Path Pathfinder::FindPathFromNode(int nodeIdx) const
{
	auto nodeParents = AStar(nodeIdx, playerNode);
	Path path;
	path.path = ConstructPath(nodeParents, playerNode);
	path.currentNode = nodeIdx;
	path.playerVisible = NodesInNeighboringRooms(nodeIdx, playerNode);
	return path;
}

Path Pathfinder::FindPathFromNodeFast(int nodeIdx) const
{
	int r1 = FindRoomIdx(nodeIdx);
	int r2 = FindRoomIdx(playerNode);

	int idxStart = nodeIdx;
	auto linkNode = FindLinkNode(r1, r2);

	if (r1 != r2)
	{
		for (int neighbour : edges[linkNode])
		{
			if (FindRoomIdx(neighbour) == r2)
			{
				idxStart = neighbour;
				break;
			}
		}
	}

	auto nodeParents = AStarRoom(idxStart, playerNode, r2);
	Path path;
	path.path = ConstructPath(nodeParents, playerNode, roomNodeIndexPrefix[r2]);
	if (linkNode != -1)
		path.path.push_front(PathNodeData(linkNode, nodes[linkNode]));

	path.currentNode = nodeIdx;

	if (path.path.size() > 1 && path.path.front().index == path.currentNode)
		path.path.pop_front();

	path.playerVisible = NodesInNeighboringRooms(nodeIdx, playerNode);
	return path;
}

void Pathfinder::UpdatePath(Path& path, DirectX::XMFLOAT3 currPos) const
{
	// check if enemy moved
	float cutoffThreshold = 0.1f;
	path.currentNode = UpdateNode(currPos, path.currentNode);

	if (
		path.path.size() > 1 && 
		DistSquared(nodes[path.path.front().index], currPos) < cutoffThreshold * cutoffThreshold)
	{
		path.path.pop_front();
	}

	// check if player moved
	if (playerNode != path.path.back().index)
	{
		path.playerVisible = NodesInNeighboringRooms(path.currentNode, playerNode);
		if (path.playerVisible)
		{
			path = FindPathFromNodeFast(path.currentNode);
		}
	}
}

void Pathfinder::UpdatePlayerNode(DirectX::XMFLOAT3 playerPos)
{
	playerNode = UpdateNode(playerPos, playerNode);
}

