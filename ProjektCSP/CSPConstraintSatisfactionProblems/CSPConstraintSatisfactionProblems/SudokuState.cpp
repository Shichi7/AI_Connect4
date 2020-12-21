#include "pch.h"
#include "SudokuState.h"


SudokuState::SudokuState(int id, double difficulty)
{
	this->id = id;
	this->difficulty = difficulty;

	this->visited_nodes = 0;
	this->visited_to_first_solution = -1;
	this->returns = 0;
	this->returns_to_first_solution = -1;
	this->solutions = 0;
}


SudokuState::~SudokuState()
{
}

string SudokuState::solve(string algorithm, string heuristics_variable, string heuristic_value)
{
	time_t start, end;

	string result = "Solutions: \n";
	this->algorithm = algorithm;
	this->heuristic_variable = heuristics_variable;
	this->heuristic_value = heuristic_value;

	start = clock();

	ios_base::sync_with_stdio(false);

	result += runAlgorithm("");

	end = clock();

	time_taken = double(end - start) / double(CLOCKS_PER_SEC);

	result += getSudokuStatsString();
	return result;
}

bool SudokuState::checkConstraints(SudokuCell* cell)
{
	int x = cell->x;
	int y = cell->y;
	int area = cell->area;
	int value = cell->value;
	bool constraint_broken = false;

	int counter = 0;
	while ((!constraint_broken)&&(counter<cell->constrained_cells.size()))
	{
		if (cell->constrained_cells.at(counter)->defined)
		{
			if (cell->value == cell->constrained_cells.at(counter)->value)
			{
				constraint_broken = true;
			}
		}
		counter++;
	}
	return !constraint_broken;
}

string SudokuState::runAlgorithm(string winning_solutions)
{
	SudokuCell* currentCell = getCell();

	if (currentCell == NULL)
	{
		solutions++;
		winning_solutions += getSudokuString() + "\n\n";
		if (visited_to_first_solution == -1)
		{
			returns_to_first_solution = returns;
			visited_to_first_solution = visited_nodes;
		}
	}
	else
	{
		vector<int> temp_domains = currentCell->domains;
		if (heuristic_value.compare("random") == 0)
		{
			random_shuffle(temp_domains.begin(), temp_domains.end());
		}

		for (int i = 0; i < currentCell->domains.size(); i++)
		{
			currentCell->value = temp_domains.at(i);
			visited_nodes++;

			if (checkConstraints(currentCell))
			{
				currentCell->defined = true;

				if (algorithm.compare("forward") == 0)
				{
					bool empty_found = false;
					vector<int> filtered;
					filterConstrainedDomains(currentCell, &empty_found, &filtered);

					if (!empty_found)
					{
						winning_solutions = runAlgorithm(winning_solutions);
					}
					unfilterConstrainedDomains(currentCell, &filtered);
				}
				else if (algorithm.compare("back") == 0)
				{
					winning_solutions = runAlgorithm(winning_solutions);
				}
				returns++;

				currentCell->defined = false;
			}
		}
	}

	return winning_solutions;
}

void SudokuState::filterConstrainedDomains(SudokuCell* cell, bool* empty_found, vector<int>* filtered)
{
	int counter = 0;
	while ((counter < cell->constrained_cells.size()) && (!(*empty_found)))
	{
		if (cell->constrained_cells.at(counter)->filterDomains(cell->value))
		{
			filtered->push_back(counter);
			if (cell->constrained_cells.at(counter)->domains.size() == 0)
			{
				*empty_found = true;
			}
		}
		counter++;
	}
}

void SudokuState::unfilterConstrainedDomains(SudokuCell* cell, vector<int>* filtered)
{
	for (int i = 0; i < filtered->size(); i++)
	{
		cell->constrained_cells.at(filtered->at(i))->unfilterDomains(cell->value);
	}
}

SudokuCell* SudokuState::getCell()
{
	SudokuCell* currentCell = NULL;
	if (heuristic_variable.compare("as_declared") == 0)
	{
		currentCell = getNextCell();
	}
	else if (heuristic_variable.compare("most_constrained") == 0)
	{
		currentCell = getMostConstrainedCell();
	}
	return currentCell;
}

SudokuCell* SudokuState::getNextCell()
{
	SudokuCell* cell = NULL;
	int y = 0;

	while ((y < 9) && (cell == NULL))
	{
		int x = 0;
		while ((x < 9) && (cell == NULL))
		{
			if (!cells[x][y].defined)
			{
				cell = &cells[x][y];
			}
			x++;
		}
		y++;
	}

	return cell;
}

SudokuCell* SudokuState::getMostConstrainedCell()
{
	int record = 0;
	SudokuCell* cell = NULL;

	for (int y = 0; y < 9; y++)
	{
		for (int x = 0; x < 9; x++)
		{
			if (!cells[x][y].defined)
			{
				int count = cells[x][y].countDefinedConstraints();
				if (count > record)
				{
					record = count;
					cell = &cells[x][y];
				}
			}
		}
	}

	return cell;
}

SudokuState* SudokuState::createSudoku(string loaded_data)
{
	int id;
	double difficulty;
	int counter = 0;
	string values_string;

	id = Helpers::stringToInt(SudokuState::getNextArgument(loaded_data, &counter));
	difficulty = Helpers::stringToDouble(SudokuState::getNextArgument(loaded_data, &counter));
	values_string = SudokuState::getNextArgument(loaded_data, &counter);

	SudokuState* new_sudoku = new SudokuState(id, difficulty);

	for (int i = 0; i < 81; i++)
	{
		int x = i % 9;
		int y = i / 9;
		if (values_string.at(i) == '.')
		{
			new_sudoku->cells[x][y].setCell(x, y);
		}
		else
		{
			int value = values_string.at(i) - 48;
			new_sudoku->cells[x][y].setCell(x, y, value);
		}

		int area_no = new_sudoku->cells[x][y].area;


		for (int j = 0; j < (x - (x % 3)); j++)
		{
			new_sudoku->cells[x][y].constrained_cells.push_back(&new_sudoku->cells[j][y]);
			new_sudoku->cells[j][y].constrained_cells.push_back(&new_sudoku->cells[x][y]);
		}

		for (int j = 0; j < (y - (y % 3)); j++)
		{
			new_sudoku->cells[x][y].constrained_cells.push_back(&new_sudoku->cells[x][j]);
			new_sudoku->cells[x][j].constrained_cells.push_back(&new_sudoku->cells[x][y]);
		}

		for (int j = 0; j < new_sudoku->cellsByArea[area_no].size(); j++)
		{
			new_sudoku->cells[x][y].constrained_cells.push_back(new_sudoku->cellsByArea[area_no].at(j));
			new_sudoku->cellsByArea[area_no].at(j)->constrained_cells.push_back(&new_sudoku->cells[x][y]);
		}

		new_sudoku->cellsByArea[area_no].push_back(&new_sudoku->cells[x][y]);
	}

	return new_sudoku;
}

string SudokuState::getNextArgument(string loaded_data, int* counter)
{
	string new_data_fragment = "";
	bool separator_found = false;
	while (!separator_found)
	{
		if (loaded_data.at(*counter) == ';')
		{
			separator_found = true;
		}
		else
		{
			new_data_fragment += loaded_data.at(*counter);
		}

		(*counter)++;
	}
	return new_data_fragment;
}

string SudokuState::getSudokuString()
{
	string to_return = "ID: [" + Helpers::numberToString(id) + "]\n";
	to_return += "Difficulty: [" + Helpers::numberToString(difficulty) + "]\n";

	for (int y = 0; y < 9; y++)
	{
		if (y % 3 == 0)
		{
			to_return += "\n";
		}
		to_return += "\n";

		for (int x = 0; x < 9; x++)
		{
			if (x % 3 == 0)
			{
				to_return += " ";
			}
			to_return += cells[x][y].getCellString();
		}
	}

	return to_return;

}

string SudokuState::getSudokuStatsString()
{
	string to_return = "ID: [" + Helpers::numberToString(id) + "]\n";
	to_return += "Difficulty: [" + Helpers::numberToString(difficulty) + "]\n";
	to_return += "Algorithm: [" + algorithm + "]\n";
	to_return += "Variable heur: [" + heuristic_variable + "]\n";
	to_return += "Value heur: [" + heuristic_value + "]\n";
	to_return += "Solutions: [" + Helpers::numberToString(solutions) + "]\n";
	to_return += "Visited Nodes: [" + Helpers::numberToString(visited_nodes) + "]\n";
	to_return += "Visited Nodes To First: [" + Helpers::numberToString(visited_to_first_solution) + "]\n";
	to_return += "Returns: [" + Helpers::numberToString(returns) + "]\n";
	to_return += "Returns To First: [" + Helpers::numberToString(returns_to_first_solution) + "]\n";
	to_return += "Exec Time: [" + Helpers::numberToString(time_taken) + "s]\n";

	return to_return;
}


string SudokuState::getSudokuConstraintString()
{
	string to_return = "";
	for (int y = 0; y < 9; y++)
	{
		for (int x = 0; x < 9; x++)
		{
			to_return += cells[x][y].getConstraintCellString();
		}
	}

	return to_return;

}