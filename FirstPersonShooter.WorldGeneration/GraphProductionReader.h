#pragma once
#include "GraphProduction.h"
#include <fstream>
#include <algorithm>

namespace WorldGenerator
{
	class GraphProductionReader
	{
	private:
		template<typename T>
		static Graph<T> ReadGraph(std::ifstream& file);
	public:
		template<typename T>
		static GraphProduction<T> ReadGraphProduction(std::string path);
	};

	template<typename T>
	inline Graph<T> GraphProductionReader::ReadGraph(std::ifstream& file)
	{
		// Read graph nodes size
		int n;
		file >> n;

		// Read node labels
		std::vector<int> labels;
		int label;
		for (int i = 0; i < n; i++)
		{
			file >> label;
			labels.push_back(label);
		}

		Graph<T> graph(n);

		// Add Nodes
		std::vector<Node<T>> nodes;
		std::transform(labels.begin(), labels.end(), std::back_inserter(nodes),
			[](int l) 
			{ 
				Node<T> node(nullptr);
				node.label = l;
				return node;
			});

		graph.AddNodes(nodes);

		// Read edges
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (file.eof())
				{
					throw new std::exception("Invalid file.");
					file.close();
					return false;
				}

				int adj;
				file >> adj;
				if (adj)
					graph.AddEdge(i, j);
			}
		}

		return graph;
	}

	template<typename T>
	inline GraphProduction<T> GraphProductionReader::ReadGraphProduction(std::string path)
	{
		try
		{
			std::ifstream file(path, std::ios::in);

			if (!file.is_open())
				throw new std::exception("Failed to open file.");
			
			int priority;
			file >> priority;
			
			Graph<T> G_L = ReadGraph<T>(file);
			Graph<T> G_R = ReadGraph<T>(file);

			file.close();

			return GraphProduction(priority, G_L, G_R);
		}
		catch (const std::exception& ex)
		{
			std::string err_msg = "Error: " + std::string(ex.what()) + " While reading file " + path;
			throw new std::exception(err_msg.c_str());
		}
	}
}