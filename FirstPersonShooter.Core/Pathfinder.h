#pragma once
#include <vector>
#include <DirectXMath.h>
#include <list>
#include <set>
#include <map>
#include <functional>
#include <unordered_set>
#include <queue>

class Room;
struct PathNodeData;
struct RoomLinkData;
struct Path;

class __declspec(dllexport) Pathfinder
{
	int playerNode;
	bool playerNodeChanged;

	std::vector<DirectX::XMFLOAT3> nodes;
	std::vector<bool> nodeMask;
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> targetNodes;
	std::vector<std::set<int>> edges;

	// a connection (r1, r2), r1 < r2 has a corresponding node
	std::map<std::pair<int, int>, int> nodeToRoomLink;

	// we can identify that node indexes that belong to room i 
	// are between roomNodeIndexPrefix[i] and roomNodeIndexPrefix[i + 1]
	std::vector<int> roomNodeIndexPrefix;

	void AddRoomNodes(const Room& room, std::function<bool(DirectX::XMFLOAT3)> pred = [](DirectX::XMFLOAT3 pos) { return true; });
	void AddDoorNodes(const RoomLinkData& link, int roomIdx);
	void AddStairsConnectionNode(const RoomLinkData& link, int roomIdx1, int roomIdx2);
	void AdjustStairsNodes(const RoomLinkData& link, int roomIdx);
	float GetAdjustedHeight(const RoomLinkData& link, DirectX::XMFLOAT3 position);

	void DeleteStairsNodes(const RoomLinkData& link, int roomIdx);



	int FindClosestNodeInARoom(DirectX::XMFLOAT3 position, int roomIdx) const;
	int FindClosestNode(DirectX::XMFLOAT3 position) const;
	int FindRoomIdx(int nodeIdx) const;
	int FindLinkNode(int room1, int room2) const;

	float Dist(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2) const;
	float DistSquared(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2) const;
	float H(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2) const;

	void AddEdge(int u, int v);
	void EraseFromNeighbourSetIf(std::set<int>& set, const std::function<bool(int)>& predicate);
	int UpdateNode(DirectX::XMFLOAT3 position, int currIndex) const;

	bool NodesInNeighboringRooms(int n1, int n2) const;

	std::vector<int> AStar(int start, int end) const;
	std::vector<int> AStarRoom(int start, int end, int roomId) const;
	std::list<PathNodeData> ConstructPath(const std::vector<int>& prev, int start, int idxOffset = 0) const;
public:
	Pathfinder(const std::vector<Room>& rooms, DirectX::XMFLOAT3 playerPos);
	Path FindPath(DirectX::XMFLOAT3 enemyPos);
	Path FindPathFromNode(int nodeIdx);
	Path FindPathFromNodeFast(int nodeIdx);
	void UpdatePath(Path& path, DirectX::XMFLOAT3 currPos);
	void UpdatePlayerNode(DirectX::XMFLOAT3 playerPos, int currentNodeIndex);
	void UpdateNodesCloseToPlayer(int playerNode, float attackRange = 0.7f);
};