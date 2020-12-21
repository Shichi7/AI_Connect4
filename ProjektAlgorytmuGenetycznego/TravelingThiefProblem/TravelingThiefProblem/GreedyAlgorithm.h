#pragma once
#include "Individual.h"

class GreedyAlgorithm
{
public:
	GreedyAlgorithm();
	~GreedyAlgorithm();

	void setData(ProblemData* sack_data);
	void run();
	string returnGreedyResultString();
	string returnGreedyResultCSV();
	double returnFitness();

	bool data_set_flag;
	double final_fitness;

	ProblemData* data;
	vector<Individual*> results;

	double best;
	double worst;
	double standard;
};

