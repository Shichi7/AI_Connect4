#include "pch.h"
#include <iostream>
#include "SudokuState.h"
#include "JolkaState.h"
#include "Loader.h"
#include "helpers.h"

#define SUDOKU_FILEPATH "Sudoku.csv";
#define SUDOKU_ID 45;
#define JOLKA_ID 4;

#define HEURISTICS_VARIABLE "as_declared"   //as_declared/most_constrained
#define HEURISTICS_VALUE "as_declared"   //as_declared/random
#define ALGORITHM "forward"   //forward/back

int main()
{
	int jolka_id = JOLKA_ID;
	int sudoku_id = SUDOKU_ID;
	string sudoku_filepath = SUDOKU_FILEPATH;
	string heuristics_variable = HEURISTICS_VARIABLE;
	string heuristic_value = HEURISTICS_VALUE;

	vector<string> jolka_words;
	vector<string> jolka_data;
	Loader::loadJolkaDataByID(&jolka_words, "Jolka/words", jolka_id);
	Loader::loadJolkaDataByID(&jolka_data, "Jolka/puzzle", jolka_id);

	JolkaState* jolka_1 = JolkaState::createJolka(jolka_id, jolka_data, jolka_words);
	Helpers::printComm(jolka_1->getJolkaNodesString(), true);

	Helpers::printComm(jolka_1->solve("back", "most_constrained", "as_declared"), true);


	string sudoku_data = Loader::loadSudokuByID(sudoku_filepath, sudoku_id);
	SudokuState* sudoku_1 = SudokuState::createSudoku(sudoku_data);
	Helpers::printComm(sudoku_1->getSudokuString(), true);

	Helpers::printComm(sudoku_1->solve("back", "as_declared", "as_declared"), true);


	delete sudoku_1
	delete jolka_1;
}

