#include <CommonUtilities/Random/PerlinNoise.h>

#include <CommonUtilities/Random/Random.hpp>

#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>

// Thanks to https://github.com/rajabala/CurlNoise/tree/master for implementation details

using namespace CommonUtilities;

inline constexpr std::array<int, 512> locKenPerlinHash =
{
	151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
	8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
	35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,
	134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
	55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
	18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
	250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
	189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167,
	43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
	97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
	107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
	
	151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
	8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
	35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,
	134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
	55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
	18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
	250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
	189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167,
	43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
	97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
	107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
};

inline constexpr Vector3f locGradients3D[] =
{
	Vector3f(1.0f, 1.0f, 0.0f),
	Vector3f(-1.0f, 1.0f, 0.0f),
	Vector3f(1.0f,-1.0f, 0.0f),
	Vector3f(-1.0f,-1.0f, 0.0f),
	Vector3f(1.0f, 0.0f, 1.0f),
	Vector3f(-1.0f, 0.0f, 1.0f),
	Vector3f(1.0f, 0.0f,-1.0f),
	Vector3f(-1.0f, 0.0f,-1.0f),
	Vector3f(0.0f, 1.0f, 1.0f),
	Vector3f(0.0f,-1.0f, 1.0f),
	Vector3f(0.0f, 1.0f,-1.0f),
	Vector3f(0.0f,-1.0f,-1.0f),

	// Repeat some to skew distribution
	Vector3f(1.0f, 1.0f, 0.0f),
	Vector3f(-1.0f, 1.0f, 0.0f),
	Vector3f(0.0f,-1.0f, 1.0f),
	Vector3f(0.0f,-1.0f,-1.0f)
};

PerlinNoise::PerlinNoise() 
	: myTable({ locKenPerlinHash.begin(), locKenPerlinHash.end() })
	, mySeed(0)
{

}

// Generate a new permutation vector based on the value of seed
PerlinNoise::PerlinNoise(unsigned int seed)
	: myTable({ locKenPerlinHash.begin(), locKenPerlinHash.end() })
	, mySeed(0)
{
	SetSeed(seed);
}

unsigned PerlinNoise::GetSeed() const
{
	return mySeed;
}

void PerlinNoise::SetSeed(unsigned aSeed)
{
	if (mySeed == aSeed)
		return;

	mySeed = aSeed;

	if (aSeed == 0)
	{
		myTable = { locKenPerlinHash.begin(), locKenPerlinHash.end() };

		return;
	}

	myTable.resize(TABLE_SIZE);

	// Fill p with values from 0 to 255
	std::iota(myTable.begin(), myTable.end(), 0);

	// Initialize a random engine with seed
	std::default_random_engine engine(mySeed);

	// Suffle  using the above random engine
	std::shuffle(myTable.begin(), myTable.end(), engine);

	// Duplicate the permutation vector
	myTable.insert(myTable.end(), myTable.begin(), myTable.end());

	cu::PushSeed(mySeed);

	for (std::size_t i = 0; i < TABLE_SIZE; ++i)
	{
		myGradients[i] = cu::RandomDirection3D<float>();
	}

	for (std::size_t i = 0; i < TABLE_SIZE; ++i)
	{
		myGradients2D[i] = cu::RandomDirection2D<float>();
	}

	for (std::size_t i = 0; i < TABLE_SIZE; ++i)
	{
		myGradients1D[i] = cu::Random(-1.0f, 1.0f);
	}

	cu::PopSeed(); // reset to previous
}

NoiseSample1D PerlinNoise::Noise1D(float aPoint, float aFrequency) const
{
	float p = aPoint * aFrequency;

	int ix0 = (int)std::floorf(p);
	float tx0 = p - ix0;
	float tx1 = tx0 - 1.0f;
	ix0 &= 255;
	int ix1 = ix0 + 1;

	int h0 = myTable[ix0];
	int h1 = myTable[ix1];

	// gradients at each of the 4 lattice points
	float g000;
	float g100;

	if (mySeed == 0)
	{
		g000 = locGradients3D[myTable[h0] & 1].x;
		g100 = locGradients3D[myTable[h1] & 1].x;
	}
	else
	{
		g000 = myGradients1D[myTable[h0]];
		g100 = myGradients1D[myTable[h1]];
	}

	float v000 = g000 * tx0;
	float v100 = g100 * tx1;

	float dtx = FadeDerivate(tx0);
	float tx = Fade(tx0);

	float a = v000;
	float b = v100 - v000;

	float da = g000;
	float db = g100 - g000;

	NoiseSample1D sample;
	sample.value = a + b * tx;

	// calculate derivative analytically
	sample.derivative = da + db * tx;

	float delta = b * dtx;

	sample.derivative += delta;
	sample.derivative *= aFrequency;

	return sample;
}
NoiseSample1D PerlinNoise::Noise1D(float aPoint, float aFrequency, int aOctaves, float aOctaveMultiplier, float aOctaveScale) const
{
	NoiseSample sum = Noise1D(aPoint, aFrequency);

	float strength = 1.0f;
	float range = 1.0f;
	for (int i = 1; i < aOctaves; i++)
	{
		aFrequency	*= aOctaveScale;
		strength	*= aOctaveMultiplier;
		range		+= strength;
		sum			+= Noise1D(aPoint, aFrequency) * strength;
	}

	return sum * (1.0f / range);
}

NoiseSample2D PerlinNoise::Noise2D(const cu::Vector2f& aPoint, float aFrequency) const
{
	Vector2f p = aPoint * aFrequency;

	int ix0 = (int)std::floorf(p.x);
	int iy0 = (int)std::floorf(p.y);
	float tx0 = p.x - ix0;
	float ty0 = p.y - iy0;
	float tx1 = tx0 - 1.0f;
	float ty1 = ty0 - 1.0f;
	ix0 &= 255;
	iy0 &= 255;
	int ix1 = ix0 + 1;
	int iy1 = iy0 + 1;

	int h0	= myTable[ix0];
	int h1	= myTable[ix1];
	int h00 = myTable[h0 + iy0];
	int h10 = myTable[h1 + iy0];
	int h01 = myTable[h0 + iy1];
	int h11 = myTable[h1 + iy1];

	// gradients at each of the 4 lattice points
	Vector2f g000;
	Vector2f g100;
	Vector2f g010;
	Vector2f g110;

	if (mySeed == 0)
	{
		g000 = locGradients3D[myTable[h00] & 3].XY();
		g100 = locGradients3D[myTable[h10] & 3].XY();
		g010 = locGradients3D[myTable[h01] & 3].XY();
		g110 = locGradients3D[myTable[h11] & 3].XY();
	}
	else
	{
		g000 = myGradients2D[myTable[h00]];
		g100 = myGradients2D[myTable[h10]];
		g010 = myGradients2D[myTable[h01]];
		g110 = myGradients2D[myTable[h11]];
	}

	float v000 = Vector2f::Dot(g000, Vector2f(tx0, ty0));
	float v100 = Vector2f::Dot(g100, Vector2f(tx1, ty0));
	float v010 = Vector2f::Dot(g010, Vector2f(tx0, ty1));
	float v110 = Vector2f::Dot(g110, Vector2f(tx1, ty1));

	float dtx = FadeDerivate(tx0);
	float dty = FadeDerivate(ty0);
	float tx = Fade(tx0);
	float ty = Fade(ty0);

	float a = v000;
	float b = v100 - v000;
	float c = v010 - v000;
	float e = v110 - v010 - v100 + v000;

	Vector2f da = g000;
	Vector2f db = g100 - g000;
	Vector2f dc = g010 - g000;
	Vector2f de = g110 - g010 - g100 + g000;

	NoiseSample2D sample;
	sample.value = a + b * tx + (c + e * tx) * ty;

	// calculate derivative analytically
	sample.derivative = da + db * tx + (dc + de * tx) * ty;

	Vector2 delta = Vector2(
		(b + e * ty) * dtx,
		(c + e * tx) * dty);

	sample.derivative += delta;
	sample.derivative *= aFrequency;

	return sample;

}
NoiseSample2D PerlinNoise::Noise2D(const cu::Vector2f& aPoint, float aFrequency, int aOctaves, float aOctaveMultiplier, float aOctaveScale) const
{
	NoiseSample sum = Noise2D(aPoint, aFrequency);

	float strength = 1.0f;
	float range = 1.0f;
	for (int i = 1; i < aOctaves; i++)
	{
		aFrequency	*= aOctaveScale;
		strength	*= aOctaveMultiplier;
		range		+= strength;
		sum			+= Noise2D(aPoint, aFrequency) * strength;
	}

	return sum * (1.0f / range);
}

NoiseSample3D PerlinNoise::Noise3D(const cu::Vector3f& aPoint, float aFrequency) const
{
	Vector3f p = aPoint * aFrequency;

	int ix0 = (int)std::floorf(p.x);
	int iy0 = (int)std::floorf(p.y);
	int iz0 = (int)std::floorf(p.z);
	float tx0 = p.x - ix0;
	float ty0 = p.y - iy0;
	float tz0 = p.z - iz0;
	float tx1 = tx0 - 1.0f;
	float ty1 = ty0 - 1.0f;
	float tz1 = tz0 - 1.0f;
	ix0 &= 255;
	iy0 &= 255;
	iz0 &= 255;
	int ix1 = ix0 + 1;
	int iy1 = iy0 + 1;
	int iz1 = iz0 + 1;

	int h0	= myTable[ix0];
	int h1	= myTable[ix1];
	int h00 = myTable[h0 + iy0];
	int h10 = myTable[h1 + iy0];
	int h01 = myTable[h0 + iy1];
	int h11 = myTable[h1 + iy1];
	
	// gradients at each of the 8 lattice points
	Vector3f g000;
	Vector3f g100;
	Vector3f g010;
	Vector3f g110;
	Vector3f g001;
	Vector3f g101;
	Vector3f g011;
	Vector3f g111;

	if (mySeed == 0)
	{
		 g000 = locGradients3D[myTable[h00 + iz0] & 15];
		 g100 = locGradients3D[myTable[h10 + iz0] & 15];
		 g010 = locGradients3D[myTable[h01 + iz0] & 15];
		 g110 = locGradients3D[myTable[h11 + iz0] & 15];
		 g001 = locGradients3D[myTable[h00 + iz1] & 15];
		 g101 = locGradients3D[myTable[h10 + iz1] & 15];
		 g011 = locGradients3D[myTable[h01 + iz1] & 15];
		 g111 = locGradients3D[myTable[h11 + iz1] & 15];
	}
	else
	{
		g000 = myGradients[myTable[h00 + iz0]];
		g100 = myGradients[myTable[h10 + iz0]];
		g010 = myGradients[myTable[h01 + iz0]];
		g110 = myGradients[myTable[h11 + iz0]];
		g001 = myGradients[myTable[h00 + iz1]];
		g101 = myGradients[myTable[h10 + iz1]];
		g011 = myGradients[myTable[h01 + iz1]];
		g111 = myGradients[myTable[h11 + iz1]];
	}

	float v000 = Vector3f::Dot(g000, Vector3f(tx0, ty0, tz0));
	float v100 = Vector3f::Dot(g100, Vector3f(tx1, ty0, tz0));
	float v010 = Vector3f::Dot(g010, Vector3f(tx0, ty1, tz0));
	float v110 = Vector3f::Dot(g110, Vector3f(tx1, ty1, tz0));
	float v001 = Vector3f::Dot(g001, Vector3f(tx0, ty0, tz1));
	float v101 = Vector3f::Dot(g101, Vector3f(tx1, ty0, tz1));
	float v011 = Vector3f::Dot(g011, Vector3f(tx0, ty1, tz1));
	float v111 = Vector3f::Dot(g111, Vector3f(tx1, ty1, tz1));

	float dtx = FadeDerivate(tx0);
	float dty = FadeDerivate(ty0);
	float dtz = FadeDerivate(tz0);
	float tx = Fade(tx0);
	float ty = Fade(ty0);
	float tz = Fade(tz0);

	float a = v000;
	float b = v100 - v000;
	float c = v010 - v000;
	float d = v001 - v000;
	float e = v110 - v010 - v100 + v000;
	float f = v101 - v001 - v100 + v000;
	float g = v011 - v001 - v010 + v000;
	float h = v111 - v011 - v101 + v001 - v110 + v010 + v100 - v000;

	Vector3f da = g000;
	Vector3f db = g100 - g000;
	Vector3f dc = g010 - g000;
	Vector3f dd = g001 - g000;
	Vector3f de = g110 - g010 - g100 + g000;
	Vector3f df = g101 - g001 - g100 + g000;
	Vector3f dg = g011 - g001 - g010 + g000;
	Vector3f dh = g111 - g011 - g101 + g001 - g110 + g010 + g100 - g000;

	NoiseSample3D sample;
	sample.value = a + b * tx + (c + e * tx) * ty + (d + f * tx + (g + h * tx) * ty) * tz;
	
	// calculate derivative analytically
	sample.derivative = da + db * tx + (dc + de * tx) * ty + (dd + df * tx + (dg + dh * tx) * ty) * tz;

	Vector3 delta = Vector3((b + e * ty + (f + h * ty) * tz) * dtx,
							(c + e * tx + (g + h * tx) * tz) * dty,
							(d + f * tx + (g + h * tx) * ty) * dtz);

	sample.derivative += delta;
	sample.derivative *= aFrequency;

	return sample;
}

NoiseSample3D PerlinNoise::Noise3D(const cu::Vector3f& aPoint, float aFrequency, int aOctaves, float aOctaveMultiplier, float aOctaveScale) const
{
	NoiseSample sum = Noise3D(aPoint, aFrequency);

	float strength = 1.0f;
	float range = 1.0f;
	for (int i = 1; i < aOctaves; i++)
	{
		aFrequency	*= aOctaveScale;
		strength	*= aOctaveMultiplier;
		range		+= strength;
		sum			+= Noise3D(aPoint, aFrequency) * strength;
	}

	return sum * (1.0f / range);
}

Vector3f PerlinNoise::Curl(const cu::Vector3f& aPoint, float aFrequency, const Vector3f& aStrength, NoiseQuality aQuality) const
{
	cu::Vector3f samplePos = aPoint * 0.05f;

	Vector3f curl;

	if (aQuality == NoiseQuality::High)
	{ 
		NoiseSample nsX = Noise3D(samplePos, aFrequency);
		NoiseSample nsY = Noise3D(samplePos + Vector3f(31.341f, -43.23f, 12.34f), aFrequency);
		NoiseSample nsZ = Noise3D(samplePos + Vector3f(-231.341f, 124.23f, -54.34f), aFrequency);

		nsX = nsX * aStrength.x;
		nsY = nsY * aStrength.y;
		nsZ = nsZ * aStrength.z;

		curl = Vector3f(
			nsZ.derivative.y - nsY.derivative.z,
			nsX.derivative.z - nsZ.derivative.x,
			nsY.derivative.x - nsX.derivative.y);
	}
	else if (aQuality == NoiseQuality::Medium)
	{
		NoiseSample nsX = Noise2D(samplePos.XY(), aFrequency);
		NoiseSample nsY = Noise2D(samplePos.XY() + Vector2f(31.341f, -43.23f), aFrequency);
		NoiseSample nsZ = Noise2D(cu::Vector2f(0.0f, samplePos.z) + Vector2f(-231.341f, 124.23f), aFrequency);

		nsX = nsX * aStrength.x;
		nsY = nsY * aStrength.y;
		nsZ = nsZ * aStrength.z;

		curl = Vector3f(
			nsZ.derivative.y,
			nsX.derivative.x,
			nsY.derivative.x - nsX.derivative.y);
	}
	else 
	{
		NoiseSample nsX = Noise1D(samplePos.x, aFrequency);
		NoiseSample nsY = Noise1D(samplePos.y + 31.341f, aFrequency);
		NoiseSample nsZ = Noise1D(samplePos.z - 231.341f, aFrequency);

		nsX = nsX * aStrength.x;
		nsY = nsY * aStrength.y;
		nsZ = nsZ * aStrength.z;

		curl = Vector3f(
			nsZ.derivative,
			-nsX.derivative,
			nsY.derivative);
	}

	return curl;
}

Vector3f PerlinNoise::Curl(const cu::Vector3f& aPoint, float aFrequency, const Vector3f& aStrength, int aOctaves, float aOctaveMultiplier, float aOctaveScale, NoiseQuality aQuality) const
{
	cu::Vector3f samplePos = aPoint * 0.05f;

	Vector3f curl;

	if (aQuality == NoiseQuality::High)
	{ 
		NoiseSample nsX = Noise3D(samplePos, aFrequency, aOctaves, aOctaveMultiplier, aOctaveScale);
		NoiseSample nsY = Noise3D(samplePos + Vector3f(31.341f, -43.23f, 12.34f), aFrequency, aOctaves, aOctaveMultiplier, aOctaveScale);
		NoiseSample nsZ = Noise3D(samplePos + Vector3f(-231.341f, 124.23f, -54.34f), aFrequency, aOctaves, aOctaveMultiplier, aOctaveScale);

		nsX = nsX * aStrength.x;
		nsY = nsY * aStrength.y;
		nsZ = nsZ * aStrength.z;

		curl = Vector3f(
			nsZ.derivative.y - nsY.derivative.z,
			nsX.derivative.z - nsZ.derivative.x,
			nsY.derivative.x - nsX.derivative.y);
	}
	else if (aQuality == NoiseQuality::Medium)
	{
		NoiseSample nsX = Noise2D(samplePos.XY(), aFrequency, aOctaves, aOctaveMultiplier, aOctaveScale);
		NoiseSample nsY = Noise2D(samplePos.XY() + Vector2f(31.341f, -43.23f), aFrequency, aOctaves, aOctaveMultiplier, aOctaveScale);
		NoiseSample nsZ = Noise2D(cu::Vector2f(0.0f, samplePos.z) + Vector2f(-231.341f, 124.23f), aFrequency, aOctaves, aOctaveMultiplier, aOctaveScale);

		nsX = nsX * aStrength.x;
		nsY = nsY * aStrength.y;
		nsZ = nsZ * aStrength.z;

		curl = Vector3f(
			nsZ.derivative.y,
			nsX.derivative.x,
			nsY.derivative.x - nsX.derivative.y);
	}
	else 
	{
		NoiseSample nsX = Noise1D(samplePos.x, aFrequency, aOctaves, aOctaveMultiplier, aOctaveScale);
		NoiseSample nsY = Noise1D(samplePos.y + 31.341f, aFrequency, aOctaves, aOctaveMultiplier, aOctaveScale);
		NoiseSample nsZ = Noise1D(samplePos.z - 231.341f, aFrequency, aOctaves, aOctaveMultiplier, aOctaveScale);

		nsX = nsX * aStrength.x;
		nsY = nsY * aStrength.y;
		nsZ = nsZ * aStrength.z;

		curl = Vector3f(
			nsZ.derivative,
			-nsX.derivative,
			nsY.derivative);
	}


	return curl;
}

float PerlinNoise::Fade(float t) const
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float PerlinNoise::FadeDerivate(float t) const
{
	return 30.0f * t * t * (t * (t - 2.0f) + 1.0f);
}