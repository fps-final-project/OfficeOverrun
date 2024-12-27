#pragma once
#include <string>

namespace WorldGenerator
{
	struct __declspec(dllexport) RoomTypesConfig
	{
		const std::string PRODUCTIONS_DIR = R"(.\FirstPersonShooter_WorldGeneration\Grammar Configuration\Productions)";

		std::string productionsDir() { return PRODUCTIONS_DIR; }
	};

}