#pragma once
namespace WorldGenerator
{
	struct __declspec(dllexport) Vector3
	{
		int x;
		int y;
		int z;

		Vector3()
		{

		}
		Vector3(int x, int y, int z) : x(x), y(y), z(z)
		{

		}
	};
}