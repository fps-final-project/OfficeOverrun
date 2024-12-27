#pragma once
#include "GraphGrammar.h"
#include "GraphProductionReader.h"
#include <filesystem>
#include <fstream>
#include <iostream>

using directory_iterator = std::filesystem::directory_iterator;

namespace WorldGenerator
{
	class GraphGrammarReader
	{
	private:
	public:
		template<typename T>
		static GraphGrammar<T> ReadGraphGrammar(std::string dir_path);
	};

	template<typename T>
	inline GraphGrammar<T> GraphGrammarReader::ReadGraphGrammar(std::string dir_path)
	{
		std::vector<GraphProduction<T>> productions;
		for (const auto& dirEntry : directory_iterator(dir_path))
		{
			try
			{
				GraphProduction<T> prod = GraphProductionReader::ReadGraphProduction<T>((dirEntry.path()).string());
				productions.push_back(prod);
			}
			catch (std::exception ex)
			{
			}
		}
		return GraphGrammar<T>(productions);
	}
}