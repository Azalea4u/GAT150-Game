#pragma once
#include <random>

namespace kiko
{
	inline void seedRandom(unsigned int seed) { srand(seed); }
	
	inline int random() { return rand(); }
	inline int random(int max) { return rand() % max; } // 0 - (max - 1)
	inline int random(int min, int max) { return min + random((max - min) + 1); } // min - max

	inline float randomf() { return random() / (float)RAND_MAX; }
	inline float randomf(float max) { return randomf() * max; } // 0.0f - max
	inline float randomf(float min, float max) { return min + randomf(max - min); } // min - max
}