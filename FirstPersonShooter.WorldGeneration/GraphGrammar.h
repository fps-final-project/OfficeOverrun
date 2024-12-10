#pragma once
#include "GraphProduction.h"
#include <utility>
#include "RNG.h"

namespace WorldGenerator
{
	template <typename T>
	class __declspec(dllexport) GraphGrammar
	{
	private:
		class ProductionExecution
		{
		public:
			GraphProduction<T>& production;
			std::vector<int>& match;

			void Execute(Graph<T>& G)
			{
				production.Apply(G, match);
			}
		};

		std::vector<GraphProduction<T>> productions;
		std::vector<ProductionExecution> MatchProductions(Graph<T>& G);

	public:
		void Apply(Graph<T>& G);
	};

	template<typename T>
	inline std::vector<GraphGrammar<T>::ProductionExecution> GraphGrammar<T>::MatchProductions(Graph<T>& G)
	{
		std::vector<ProductionExecution> possibles;
		for (auto P : productions)
		{
			std::vector<std::vector<int>> matches = P.Match(G);
			if (matches.empty())
				continue;

			match = RNG::SelectRandomElement(matches);
			ProductionExecution pe = { P, match };
			possibles.push_back(pe);
		}
		return possibles;
	}

	template<typename T>
	inline void GraphGrammar<T>::Apply(Graph<T>& G)
	{
		std::vector<ProductionExecution> toPerform = MatchProductions(G);
		while (toPerform.size() > 0)
		{
			ProductionExecution pe = RNG::SelectRandomElement(toPerform);
			pe.Execute(G);

			toPerform = MatchProductions(G);
		}
	}
}