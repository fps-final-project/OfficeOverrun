#pragma once
#include "Graph.h"
#include "GeneratedRoom.h"

#define BIT_SET(byte,nbit)   ((byte) |  (1<<(nbit)))
#define BIT_CHECK(byte,nbit) ((byte) &   (1<<(nbit)))
#define BIT_CLEAR(byte,nbit) ((byte) &= ~(1<<(nbit)))


namespace WorldGenerator
{
	// Implementation of Ullmann, Julian R. (1976), "An algorithm for subgraph isomorphism", Journal of the ACM, 23 (1): 31–42
	// All bitmask representations of vectors are in little to big endian order
	class __declspec(dllexport) SubgraphMatcher
	{
	public:
		template <typename T>
		static std::vector<int> MatchSubgraph(Graph<T> &G1, Graph<T> &G2);

	private:
		template <typename T>
		static std::vector<unsigned long> GenerateM0(Graph<T>& G1, Graph<T>& G2);
		
		template <typename T>
		static std::vector<unsigned long> GenerateA(Graph<T> &G);
		
		template <typename T>
		static std::vector<unsigned long> GenerateB(Graph<T>& G);

		template <typename T>
		static std::vector<std::vector<int>> GenerateAdList(Graph<T>& G);

		static void ExploreBranch(std::vector<unsigned long>& M, const std::vector<unsigned long>& A, const std::vector<unsigned long>& B,
			const std::vector<std::vector<int>>& ad_list, std::vector<bool> F, std::vector<int> H, int d, std::vector<std::vector<int>> &matches);

		static bool Refine(std::vector<unsigned long>& M, const std::vector<unsigned long>& A, const std::vector<unsigned long>& B, const std::vector<std::vector<int>>& ad_list);
	};

	// Subgraph matcher finds the mapping of pattern G1 to subgraph of G2 with respect to isomorphism and vertex labelling stored in nodes
	template<typename T>
	inline std::vector<int> SubgraphMatcher::MatchSubgraph(Graph<T>& G1, Graph<T>& G2)
	{
		std::vector<unsigned long> M0 = GenerateM0(G1, G2); // Matrix where rows are bitmasks
		std::vector<unsigned long> A = GenerateA(G1);
		std::vector<unsigned long> B = GenerateB(G2);
		std::vector<std::vector<int>> ad_list = GenerateAdList(G1);
		std::vector< std::vector<int>> matches;

		std::vector<bool> F(G2.Size()); // Records which columns of M has been used
		std::vector<int> H(G1.Size(), -1); // Stores columns of M used at each depth

		if(!Refine(M0, A, B, ad_list))
			return std::vector<int>();

		ExploreBranch(M0, A, B, ad_list, F, H, 0, matches);

		return std::vector<int>();
	}

	//  M0 is a vector of int-bitmask
	template<typename T>
	inline std::vector<unsigned long> SubgraphMatcher::GenerateM0(Graph<T>& G1, Graph<T>& G2)
	{
		std::vector<unsigned long> M0(G1.Size());
		for (int i = 0; i < G1.Size(); i++)
		{
			for (int j = 0; j < G2.Size(); j++)
			{
				// If vertex in G2 has no less degree and bot vertices are of the same label
				if (G2.GetNeighbours(j).size() >= G1.GetNeighbours(i).size() && G2[j].label == G1[i].label)
					M0[i] = BIT_SET(M0[i], j);
			}
		}

		return M0;
	}


	// Generates vector of bitmasks representing each row of adjacency matrix Mat
	template<typename T>
	inline std::vector<unsigned long> SubgraphMatcher::GenerateA(Graph<T>& G)
	{
		std::vector<unsigned long> A(G.Size());
		for (int i = 0; i < G.Size(); i++)
		{
			for (int j = 0; j < G.Size(); j++)
			{
				if (G.HasEdge(i,j) > 0)
					A[i] = BIT_SET(A[i], j);
			}
		}
		return A;
	}

	// Generates vector of bitmasks representing each column of adjacency matrix Mat
	template<typename T>
	inline std::vector<unsigned long> SubgraphMatcher::GenerateB(Graph<T>& G)
	{
		std::vector<unsigned long> B(G.Size());
		for (int i = 0; i < G.Size(); i++)
		{
			for (int j = 0; j < G.Size(); j++)
			{
				if (G.HasEdge(j, i) > 0)
					B[i] = BIT_SET(B[i], j);
			}
		}
		return B;
	}

	// Generates adjacency lists for all vertices
	template<typename T>
	inline std::vector<std::vector<int>> SubgraphMatcher::GenerateAdList(Graph<T>& G)
	{
		std::vector<std::vector<int>> ad_list;

		for (int i = 0; i < G.Size(); i++)
			ad_list.push_back(G.GetNeighbours(i));

		return ad_list;
	}
}

