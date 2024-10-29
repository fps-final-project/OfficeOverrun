#pragma once
#include <tuple>

namespace WorldGenerator
{
	class Utils
	{
	public:
		static bool LinesIntersect1D(int s1, int e1, int s2, int e2);
		static bool LinesIntersectByEnds1D(int s1, int e1, int s2, int e2);
		static std::tuple<int, int> ComputeLinesIntersection1D(int s1, int e1, int s2, int e2); // Returns start and length
	};
}