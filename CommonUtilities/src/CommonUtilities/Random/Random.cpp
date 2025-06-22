#include <CommonUtilities/Random/Random.hpp>

#include <queue>

static std::queue<std::uint64_t> locSeeds;

void CommonUtilities::PushSeed(std::uint64_t seed)
{
	locSeeds.push(priv::Seed);

	priv::Seed = seed;
	dre.seed(priv::Seed);
}

void CommonUtilities::PopSeed()
{
	std::uint64_t seed = locSeeds.front();
	locSeeds.pop();

	priv::Seed = seed;
	dre.seed(priv::Seed);
}
