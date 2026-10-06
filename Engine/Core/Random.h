#pragma once

#include <cstdlib>
#include <random>

namespace nu {

	inline std::mt19937& Generator() {
		static std::random_device randomDevice;
		static std::mt19937 generator(randomDevice());

		return generator;
	}

	inline void SeedRandom(unsigned int seed) {
		Generator().seed(seed);
	}

	inline int RandomInt() {
		std::uniform_int_distribution<> dist;
		return dist(Generator());
	}


	/// <summary>
	/// Get random integer value [0, max]
	/// </summary>
	/// <param name="max">exclusive max</param>
	/// <returns>Random number between 0 & max</returns>
	inline int RandomInt(int max) {
		std::uniform_int_distribution<> dist(0, max - 1);
		return dist(Generator());
	}


	/// <summary>
	/// Returns random integer value [min, max]
	/// </summary>
	/// <param name="min">inclusive min</param>
	/// <param name="max">inclusive max</param>
	/// <returns>Random number between min and max (inclusive)</returns>
	inline int RandomInt(int min, int max) {
		std::uniform_int_distribution<> dist(min, max - 1);
		return dist(Generator());
	}


	inline float RandomFloat() {
		std::uniform_real_distribution<float> dist(0.0f, 1.0f);
		return dist(Generator());
	}

	inline float RandomFloat(float max) {
		std::uniform_real_distribution<float> dist(0.0f, max);
		return dist(Generator());
	}

	inline float RandomFloat(float min, float max) {

		if (min > max) std::swap(min, max);

		std::uniform_real_distribution<float> dist(min, max);
		return dist(Generator());
	}

	inline bool RandomBool() {
		std::bernoulli_distribution dist(0.5f);
		return dist(Generator());
	}
}