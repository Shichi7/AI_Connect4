#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include "GeneticAlgorithm.h"
#include "GreedyAlgorithm.h"

using namespace std;

class CSVSaver
{
public:
	CSVSaver();
	~CSVSaver();

	void writeToCSV();
	void setData(ProblemData* problem, GeneticAlgorithm* genetic, GeneticAlgorithm* random, GreedyAlgorithm* greedy, int repeat_no);
	void setRepeats(vector<double> bests, vector<int> bests_indexes);

private:
	ProblemData* problem;
	GeneticAlgorithm* genetic;
	GeneticAlgorithm* random;
	GreedyAlgorithm* greedy;

	string getCSVHeaderGeneticSummary();
	string getCSVHeaderGenetic();
	string getCSVHeader();

	string buildCSVString();

	int repeat;

	vector<double> bests;
	double best_best;
	double mean;
	double worst_best;
	double standard;
	string best_of_bests;
};

