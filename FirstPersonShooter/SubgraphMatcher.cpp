#include "pch.h"
#include "SubgraphMatcher.h"

#define BITSET(byte,nbit)   ((byte) |=  (1<<(nbit)))

std::vector<int> WorldGenerator::SubgraphMatcher::MatchSubgraph(Graph<int>& G1, Graph<int>& G2)
{



	return std::vector<int>();
}

// M0 is a vector of int-bitmask
std::vector<unsigned long> WorldGenerator::SubgraphMatcher::GenerateM0(Graph<int>& G1, Graph<int>& G2)
{
	std::vector<unsigned long> M0(G1.Size());
	for (int i = 0; i < G1.Size(); i++)
	{
		for (int j = 0; j < G2.Size(); j++)
		{
			// If vertex in G2 has no less degree and bot vertices are of the same label
			if (G2.GetNeighbours(j).size() >= G1.GetNeighbours(i).size() && *G2[j].value == *G1[i].value)
				BITSET(M0[i], j);
		}
	}

	return M0;
}
