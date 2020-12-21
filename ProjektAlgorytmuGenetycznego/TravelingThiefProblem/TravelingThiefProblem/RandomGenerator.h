#pragma once
#include <random>

using namespace std;


class RandomGenerator
{
public:
	RandomGenerator();
	~RandomGenerator();

	int generate(int min, int max);

private:
	mt19937 rng;
};

