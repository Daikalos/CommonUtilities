#include <CommonUtilities/Random/Random.hpp>

void CommonUtilities::Seed(std::uint64_t seed)
{
	dre.seed(seed);
}
