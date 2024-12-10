#include "pch.h"
#include "RoomSelector.h"

using namespace WorldGenerator;

WorldGenerator::RoomSelector::RoomSelector(RoomSelectorArgs args)
{
	H = args.initialGraph;
	s = args.startVertex;
	N = std::floor(H.Size() * args.roomDensity);
	floors = args.floorCount;
	e_c = args.edgeDensityCoeff;
}

Graph<GeneratedRoom> WorldGenerator::RoomSelector::SelectRooms()
{
	// Step 1
	RemoveUpDownEdges();

	// Step 2
	std::vector<int> path = GenerateRandomPath();

	// Step 3
	ConstructGFromPath(path);

	// Step 4
	RemoveDownUpEdges();

	// Steps 5-6
	AddSpareVertices();

	// Step 7
	AddEdgesAtRandom();

	// Add rooftop room
	AddRooftopVertex();

	return G;
}


void WorldGenerator::RoomSelector::RemoveUpDownEdges()
{
	for (int i = 0; i < H.Size(); i++)
	{
		Node<GeneratedRoom>& v = H[i];
		for (int j = 0; j < H.Size(); j++)
		{
			Node<GeneratedRoom>& u = H[j];
			if (H.HasEdge(i,j) && v.value->IsAbove(*u.value))
			{
				H.DeleteEdge(i, j);
			}
		}
	}
}

void WorldGenerator::RoomSelector::RemoveDownUpEdges()
{
	for (int i = 0; i < H.Size(); i++)
	{
		Node<GeneratedRoom>& v = H[i];
		for (int j = 0; j < H.Size(); j++)
		{
			Node<GeneratedRoom>& u = H[j];
			if (H.HasEdge(i, j) && v.value->IsBelow(*u.value))
			{
				H.DeleteEdge(i, j);
			}
		}
	}
}

// Returns index of the first neighbour above the vertex v or -1 if no such exists
int WorldGenerator::RoomSelector::FindNeighbourAbove(Graph<GeneratedRoom>& graph, int v)
{
	std::vector<int> neighbours = graph.GetNeighbours(v);
	auto it = std::find_if(neighbours.begin(), neighbours.end(), [&](int u)
		{
			return graph[u].value->IsAbove(*graph[v].value);
		}
	);
	return it != neighbours.end() ? *it : -1;
}

// Selects random vertex with a neighbour above except s
// ISSUE: it will tend to find diagonal vertex always
int WorldGenerator::RoomSelector::SelectVertexWithNeighbourAbove(Graph<GeneratedRoom>& graph, std::vector<int> vertices, int s)
{
	std::vector<int> valid_vertices;
	std::copy_if(vertices.begin(), vertices.end(), std::back_inserter(valid_vertices), [&](int v)
		{return v != s && FindNeighbourAbove(graph, v) >= 0; }
	);

	// If no way up just take the furthest vertex
	if (valid_vertices.empty())
		valid_vertices = vertices;
	// Compute distances from s to all vertices
	std::vector<int> dist = PathFinding::ComputeGraphDistances(graph, s);

	// Find max distance
	int max_dist = dist[*std::max_element(valid_vertices.begin(), valid_vertices.end(),
		[&dist](int a, int b) 
		{ return dist[a] < dist[b]; }
	)];

	// Select vertices with max distance from s
	std::vector<int> max_dist_valid_vertices;
	std::copy_if(valid_vertices.begin(), valid_vertices.end(), std::back_inserter(max_dist_valid_vertices),
		[&dist, max_dist](int v) { return dist[v] == max_dist; });

	// Select random vertex amongst the valid ones with max distance
	return RNG::SelectRandomElement(max_dist_valid_vertices);
}

void WorldGenerator::RoomSelector::UpdatePathWithFloor(std::vector<int>& P, int z)
{
	// Select start vertex on the floor
	int s_floor = P[P.size() - 1];
	// Select other vertices on the z-floor
	std::vector<int> z_vertices = H.GetVerticesIf([&](GeneratedRoom room) 
		{return room.pos.z == z; }
	);
	// Select the target vertex on the floor that has a neigbhour above except the s_floor
	int t_floor = SelectVertexWithNeighbourAbove(H, z_vertices, s_floor);

	// Get induced graph for the floor
	auto [H_z, map] = GraphUtils::GenerateInducedGraph(H, z_vertices);

	// Construct weighted graph form H
	WeightedGraph<GeneratedRoom> H_w(H_z);

	// Set random weights
	for (WeightedEdge e : H_w.GetAllEdges())
	{
		e.weight = RNG::RandIntInRange(1, MAX_EDGE_WEIGHT);
		H_w.SetEdge(e);
	}

	// Select the start and target vertices in H_w
	int s_floor_H_w = GraphUtils::FindValueIndexInMap(map, s_floor);
	int t_floor_H_w = GraphUtils::FindValueIndexInMap(map, t_floor);

	// Find shotest s-t path in H_w
	auto s_t_p = PathFinding::FindShortestPathDijkstra(H_w, s_floor_H_w, t_floor_H_w);
	std::transform(s_t_p.begin() + 1, s_t_p.end(), std::back_inserter(P), [&](int v)
		{ return map[v]; }
	);

	// Add room above t to P
	s_floor = FindNeighbourAbove(H, t_floor);
	if(s_floor != H.Size() - 1) // if its not the roof
		P.push_back(s_floor);
}

std::vector<int> WorldGenerator::RoomSelector::GenerateRandomPath()
{
	std::vector<int> P; // desired path
	P.push_back(s);

	// For all floors
	for (int z = 0; z < floors; z++)
		UpdatePathWithFloor(P, z);

	return P;
}

void WorldGenerator::RoomSelector::ConstructGFromPath(std::vector<int> P)
{
	G = Graph<GeneratedRoom>(H.Size());
	H_G_map = std::vector<int>(H.Size(), -1);
	
	for (int i = 0; i < P.size(); i++)
	{
		AddHVertexToG(P[i]);

		if (i > 0)
			G.AddUndirectedEdge(i - 1, i);
	}
	s_G = 0;
	t_G = G.Size() - 1;
}

void WorldGenerator::RoomSelector::AddHVertexToG(int v)
{
	auto node = H[v];
	G.AddNode(node);
	G_H_map.push_back(v);
	H_G_map[v] = G.Size() - 1;
}

void WorldGenerator::RoomSelector::AddSpareVertices()
{
	int G_N = RNG::RandIntInRange(max(N, G.Size()), H.Size()); // desired number of rooms in the level
	
	// H[G] subgraph neighbourhood
	std::vector<int> neigh_G = ComputeNeighbourhood();
	
	while (G.Size() < G_N)
	{
		if (neigh_G.size() == 0)
			return; // sometimes if rooms are on many different levels some rooms may become unreachable

		int v = RNG::SelectRandomElement(neigh_G);
		AddHVertexToG(v);

		std::vector<int> connected = H.GetIngoingNeighbours(v);
		auto is_in_G = [&](int w) { return H_G_map[w] >= 0; };
		int u = *std::find_if(connected.begin(), connected.end(), is_in_G);
		G.AddUndirectedEdge(H_G_map[u], H_G_map[v]);

		UpdateNeighbourhood(neigh_G, v);
	}
}


std::vector<int> WorldGenerator::RoomSelector::ComputeNeighbourhood()
{
	std::vector<int> neigh_G;
	for (int i = 0; i < G.Size(); i++)
	{
		int v = G_H_map[i];
		for (int u : H.GetNeighbours(v))
		{
			// u is in not in G and not already in neigh_G
			if (H_G_map[u] == -1 && std::find(neigh_G.begin(), neigh_G.end(), u) == neigh_G.end())
			{
				neigh_G.push_back(u);
			}
		}
	}
	
	return neigh_G;
}

void WorldGenerator::RoomSelector::UpdateNeighbourhood(std::vector<int> &neigh_G, int v) // v is vertex in H
{
	neigh_G.erase(std::remove(neigh_G.begin(), neigh_G.end(), v), neigh_G.end());
	for (int u : H.GetNeighbours(v))
	{
		// u is in not in G and not already in neigh_G
		if (H_G_map[u] == -1 && std::find(neigh_G.begin(), neigh_G.end(), u) == neigh_G.end())
		{
			neigh_G.push_back(u);
		}
	}
}

void WorldGenerator::RoomSelector::AddRooftopVertex()
{
	AddHVertexToG(H.Size() - 1);
	G.AddUndirectedEdge(t_G, G.Size() - 1);
}

void WorldGenerator::RoomSelector::AddEdgesAtRandom()
{
	for (int v_H = 0; v_H < H.Size(); v_H++)
	{
		int v_G = H_G_map[v_H];
		if (v_G == -1) // skip if v not in G
			continue;
		std::vector<int> neighbours = H.GetNeighbours(v_H);
		for (int u_H : neighbours)
		{
			int u_G = H_G_map[u_H];
			if (u_H == -1) // skip if u not in G
				continue;
			if (u_G <= v_G) // iterate through edge only once
				continue; 
			if (G.HasEdge(u_G, v_G)) // skip if edge already exists
				continue;

			bool shouldAddEdge = RNG::RandBoolWithProbabilty(e_c);
			if (shouldAddEdge)
			{
				G.AddUndirectedEdge(v_G, u_G);
			}
		}
	}
}
