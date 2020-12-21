#pragma once
#include <vector>;
#include "helpers.h";
#include <algorithm>

using namespace std;

class SudokuCell
{
public:
	SudokuCell();
	~SudokuCell();

	void setCell(int x, int y);
	void setCell(int x, int y, int value);
	string getCellString();
	string getConstraintCellString();
	bool filterDomains(int value);
	void unfilterDomains(int value);
	int countDefinedConstraints();

	int x;
	int y;

	int area;

	int value;
	bool defined;

	vector<int> domains;
	vector<SudokuCell*> constrained_cells;
private:
	void setupCell(int x, int y);

};

