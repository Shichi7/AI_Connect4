#pragma once
#include "ProblemData.h"
#include "RandomGenerator.h"

class Individual
{
public:
	Individual(Individual& to_copy);
	Individual(ProblemData* problem_data);
	Individual(ProblemData* problem_data, RandomGenerator* generator);
	~Individual();

	void evaluateFitness();

	void fillDataRandom();
	string getString();
	double getFitness();
	void setNewGenes(int* genes);

	void mutate(int propability, bool inverse = false);
	Individual* cross(Individual* partner, int segment_start, int segment_end, bool ox_type);

private:
	ProblemData* problem_data;
	RandomGenerator* generator;
	int* genes;
	int length;
	double fitness;

	Individual* crossOX(Individual* partner, int segment_start, int segment_end);
	Individual* crossPMX(Individual* partner, int segment_start, int segment_end);
	void mutateSwap(int propability);
	void mutateInverse(int propability);

	void configure(ProblemData* problem_data);
};

