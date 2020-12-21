#pragma once
#include "Individual.h"

class GeneticAlgorithm
{
public:
	GeneticAlgorithm();
	~GeneticAlgorithm();

	void configMain(int iterations, int population_size, bool debug = false);
	void configSelection(string selection_type, int tournament_size);
	void configCrossing(string crossing_type, int crossing_probability);
	void configMutation(string mutation_type, int mutation_probability);
	void setProblemData(ProblemData* data);

	void runRandom(int population_count);

	string returnRandomAlgorithmString();
	string returnPopulationString();
	string getStringOfIndex(int index);

	string getFullDebug();
	string getCSVDebug();
	string getCSVRandomDebug();
	//string getAlgorithmInfo();
	string getAlgorithmCSVInfo();
	string getAlgorithmCSVSummary();
	void run();

	double getCurrentBest();

private:
	RandomGenerator generator;
	ProblemData* data;
	vector<Individual*> population;

	string selection_type;
	string crossing_type;
	string mutation_type;

	int iterations;
	int population_size;

	int crossing_probability;
	int mutation_probability;

	int tournament_size;

	double current_best;
	int current_best_index;

	double current_worst;
	int current_worst_index;

	double current_mean;

	void deletePopulation(vector<Individual*>* population);

	void resetRecords();
	void checkRecords(double value, int index);
	void updateStrings(int iteration);
	double calculatePopulationMeanScore();
	double calculatePopulationStandard();

	void mutatePopulation();

	void crossPopulation();
	void crossIndividuals(pair<int, int> parents, vector<Individual*>* new_population, bool crossing = true);
	pair<int, int> selection();
	pair<int, int> randSegmentBounds();

	int runTournament();
	int runRoulette();

	bool full_debug;
	string debug_string;
	string csv_string;
};

