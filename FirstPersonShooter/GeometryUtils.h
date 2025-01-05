#pragma once
#include <tuple>
#include <DirectXMath.h>

namespace WorldGenerator
{
	class __declspec(dllexport) GeometryUtils
	{
	public:
		static bool LinesIntersect1D(int s1, int e1, int s2, int e2);
		static bool LinesIntersectByEnds1D(int s1, int e1, int s2, int e2);
		static std::tuple<int, int> ComputeLinesIntersection1D(int s1, int e1, int s2, int e2); // Returns start and length
		static bool BoxesIntersect(DirectX::XMFLOAT2 pos1, DirectX::XMFLOAT2 size1, DirectX::XMFLOAT2 pos2, DirectX::XMFLOAT2 size2);
		static float ComputeNormalVectorAngleIn4Axis(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
		static DirectX::XMFLOAT3 AdjustPropSizeToOrientation(DirectX::XMFLOAT3 orientation, DirectX::XMFLOAT3 size);
		static DirectX::XMFLOAT3 AdjustPropPositionToOrientation(DirectX::XMFLOAT3 orientation, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 pos);
	};
}