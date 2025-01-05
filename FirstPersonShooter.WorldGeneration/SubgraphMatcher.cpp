#include "pch.h"
#include "SubgraphMatcher.h"

using namespace WorldGenerator;

void WorldGenerator::SubgraphMatcher::ExploreBranch(std::vector<unsigned long long int>& M, const std::vector<unsigned long long int>& A, const std::vector<unsigned long long int>& B, 
	const std::vector<std::vector<int>>& ad_list, std::vector<bool> F, std::vector<int> H, int d, std::vector<std::vector<int>> &matches)
{
	std::vector<unsigned long long int> M_d = M; // Copy

	for (int k = 0; k < B.size(); k++)
	{
		if((bool)BIT_CHECK(M_d[d], k) && !F[k])
		{
			M_d[d] = BIT_SET(0, k);
			if (Refine(M_d, A, B, ad_list))
			{
				H[d] = k;
				if (d == A.size() - 1)
				{
					matches.push_back(H);
				}
				else
				{
					F[k] = true;
					ExploreBranch(M_d, A, B, ad_list, F, H, d + 1, matches);
					F[k] = false;
				}
			}
			M_d = M;
		}
	}
}

bool WorldGenerator::SubgraphMatcher::Refine(std::vector<unsigned long long int>& M, const std::vector<unsigned long long int>& A, const std::vector<unsigned long long int>& B, const std::vector<std::vector<int>> & ad_list)
{
	int elim = 0; // number of removed 1 from M
	
	do
	{
		elim = 0;
		for (int i = 0; i < A.size(); i++)
		{
			for (int j = 0; j < B.size(); j++)
			{
				if (BIT_CHECK(M[i], j))
				{
					for (int x : ad_list[i])
					{
						if ((M[x] & B[j]) == 0)
						{
							M[i] = BIT_CLEAR(M[i], j);
							elim++;
						}
					}
				}
			}
			if (M[i] == 0)
				return false;
		}
	} while (elim > 0);

	return true;
}
