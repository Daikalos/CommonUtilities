#pragma once

#pragma once
#include <vector>

// THIS CLASS IS A TRANSLATION TO C++11 FROM THE REFERENCE
// JAVA IMPLEMENTATION OF THE IMPROVED PERLIN FUNCTION (see http://mrl.nyu.edu/~perlin/noise/)
// THE ORIGINAL JAVA IMPLEMENTATION IS COPYRIGHT 2002 KEN PERLIN

// I ADDED AN EXTRA METHOD THAT GENERATES A NEW PERMUTATION VECTOR (THIS IS NOT PRESENT IN THE ORIGINAL IMPLEMENTATION)

namespace CommonUtilities
{
    class PerlinNoise
    {
    public:
        // Initialize with the reference values for the permutation vector
        PerlinNoise();

        // Generate a new permutation vector based on the value of seed
        PerlinNoise(unsigned int seed);

        // Get a noise value, for 2D images z can have any value
        double Noise(double x, double y, double z);

    private:
        double Fade(double t);
        double Lerp(double t, double a, double b);
        double Grad(int hash, double x, double y, double z);

        // The permutation vector
        std::vector<int> p;
    };
}