#pragma once
#include "Graph.h"


namespace WorldGenerator
{
	// Implementation of Ullmann, Julian R. (1976), "An algorithm for subgraph isomorphism", Journal of the ACM, 23 (1): 31–42
	class __declspec(dllexport) SubgraphMatcher
	{
	public:
		// Subgraph matcher finds the mapping of pattern G1 to subgraph of G2 with respect to isomorphism and vertex labelling stored in nodes
		static std::vector<int> MatchSubgraph(Graph<int> &G1, Graph<int> &G2);
	private:
		static std::vector<unsigned long> GenerateM0(Graph<int>& G1, Graph<int>& G2);
	};
}

