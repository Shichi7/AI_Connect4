#pragma once
#include <string>
#include "helpers.h"
#include "SudokuCell.h"
#include <chrono> 

using namespace std;

class SudokuState
{
public:
	SudokuState(int id, double difficulty);
	~SudokuState();

	int solutions;
	int returns;
	int returns_to_first_solution;
	int visited_nodes;
	int visited_to_first_solution;
	double time_taken;

	double difficulty;
	int id;
	SudokuCell cells[9][9];
	vector<SudokuCell*> cellsByArea[9];

	string heuristic_variable;
	string heuristic_value;
	string algorithm;

	string solve(string algorithm, string heuristics_variable, string heuristic_value);

	string getSudokuStatsString();
	string getSudokuString();
	string getSudokuConstraintString();

	string runAlgorithm(string winning_solutions);

	SudokuCell* getCell();
	SudokuCell* getNextCell();
	SudokuCell* getMostConstrainedCell();
	bool checkConstraints(SudokuCell* cell);
	void filterConstrainedDomains(SudokuCell* cell, bool* empty_found, vector<int>* filtered);
	void unfilterConstrainedDomains(SudokuCell* cell, vector<int>* filtered);

	static SudokuState* createSudoku(string loaded_data);
	static string getNextArgument(string loaded_data, int* counter);
};

