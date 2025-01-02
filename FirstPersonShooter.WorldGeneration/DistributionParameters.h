#pragma once

namespace WorldGenerator
{
	struct DistributionParameters
	{
		int poisson_lambda = 2;
		int binomial_t = 4;
		double binomial_p = 0.5;
	};
}