#include <CommonUtilities/Utility/Random.hpp>

using namespace CommonUtilities;

void rn::Seed(std::uint64_t seed)
{
	rn::dre.seed(seed);
}
