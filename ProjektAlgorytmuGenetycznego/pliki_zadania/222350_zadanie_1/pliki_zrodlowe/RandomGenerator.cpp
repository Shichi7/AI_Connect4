#include "stdafx.h"
#include "RandomGenerator.h"


RandomGenerator::RandomGenerator(): rng(random_device()())
{
}


RandomGenerator::~RandomGenerator()
{
}


int RandomGenerator::generate(int min, int max)
{
	int multiplier = 10000;
	int multiplified_min = min*multiplier;
	int multiplified_max = (max+1)*multiplier-1;

	uniform_int_distribution<int> uni(multiplified_min, multiplified_max);

	int result = ((double)(uni(rng))) / multiplier;

	return result;
}