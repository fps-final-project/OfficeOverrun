#pragma once
#include <vector>
#include <DirectXMath.h>
#include <list>
#include <set>
#include <functional>

class Room;
struct PathNodeData;
struct RoomLinkData;

class Pathfinder
{
	int playerNode;

	std::vector<DirectX::XMFLOAT3> nodes;
	std::vector<std::set<int>> edges;

	// we can identify that node indexes that belong to room i 
	// are between roomNodeIndexPrefix[i] and roomNodeIndexPrefix[i + 1]
	std::vector<int> roomNodeIndexPrefix;

	void AddRoomNodes(const Room& room);
	void AddDoorNodes(const RoomLinkData& link, int roomIdx);
	void AddStairsConnectionNode(const RoomLinkData& link, int roomIdx1, int roomIdx2);
	void AdjustStairsNodes(const RoomLinkData& link, int roomIdx);
	float GetAdjustedHeight(const RoomLinkData& link, DirectX::XMFLOAT3 position);
	void DeleteStairsNodes(const RoomLinkData& link, int roomIdx);

	int FindClosestNodeInARoom(DirectX::XMFLOAT3 position, int roomIdx) const;
	int FindClosestNode(DirectX::XMFLOAT3 position) const;

	float Dist(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2) const;
	float DistSquared(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2) const;

	void AddEdge(int u, int v);
	void EraseFromNeighbourSetIf(std::set<int>& set, const std::function<bool(int)>& predicate);

	std::vector<int> AStar(int start, int end) const;
	std::list<PathNodeData> ConstructPath(const std::vector<int>& prev, int start) const;
public:
	Pathfinder(const std::vector<Room>& rooms, DirectX::XMFLOAT3 playerPos);
	std::list<PathNodeData>	FindPath(DirectX::XMFLOAT3 enemyPos) const;
	void UpdatePath(std::list<PathNodeData>& path, DirectX::XMFLOAT3 currPos) const;
	void UpdatePlayerNode(DirectX::XMFLOAT3 playerPos);

	
};