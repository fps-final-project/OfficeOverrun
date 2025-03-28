#pragma once
#include "GraphProduction.h"
#include <utility>
#include <algorithm>
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
			std::vector<int> match;

			void Execute(Graph<T>& G)
			{
				production.Apply(G, match);
			}

			ProductionExecution(GraphProduction<T>& production, std::vector<int>& match) : production(production), match(match)
			{
			}
		};

		std::vector<GraphProduction<T>> productions;
		std::unique_ptr<ProductionExecution> MatchProduction(Graph<T>& G);

	public:
		void Apply(Graph<T>& G);

		GraphGrammar(std::vector<GraphProduction<T>> productions);
	};


	template<typename T>
	inline void GraphGrammar<T>::Apply(Graph<T>& G)
	{
		std::unique_ptr<ProductionExecution> toPerform = MatchProduction(G);
		while (toPerform != nullptr)
		{
			toPerform->Execute(G);

			toPerform = MatchProduction(G);
		}
	}

	template <typename T>
	std::unique_ptr<typename GraphGrammar<T>::ProductionExecution> GraphGrammar<T>::MatchProduction(Graph<T>& G)
	{
		for (auto& P : productions)
		{
			std::vector<std::vector<int>> matches = P.Match(G);

			if (!matches.empty())
			{
				auto match = RNG::SelectRandomElement(matches);
				auto execution = std::make_unique<ProductionExecution>(P, match);
				return execution;
			}
		}
		return nullptr;
	}

	template<typename T>
	inline GraphGrammar<T>::GraphGrammar(std::vector<GraphProduction<T>> productions)
	{
		// Sort by priority ascending
		std::sort(std::begin(productions), std::end(productions), [](GraphProduction<T>& p1, GraphProduction<T>& p2)
			{
				return p1.Priority() < p2.Priority();
			});

		this->productions = productions;
	}
}