#include "pch.h"
#include "SudokuCell.h"

SudokuCell::SudokuCell()
{
}

SudokuCell::~SudokuCell()
{
}

void SudokuCell::setCell(int x, int y, int value)
{
	setupCell(x, y);
	this->value = value;
	defined = true;

	domains.push_back(value);
}

void SudokuCell::setCell(int x, int y)
{
	setupCell(x, y);
	defined = false;

	for (int i = 1; i < 10; i++)
	{
		domains.push_back(i);
	}
}

void SudokuCell::setupCell(int x, int y)
{
	domains.clear();
	this->x = x;
	this->y = y;
		
	area = (y / 3) * 3 + (x / 3);
}

string SudokuCell::getCellString()
{
	string to_return;
	if (defined)
	{
		to_return = Helpers::numberToString(value);
	}
	else
	{
		to_return = ".";
	}
	return to_return;
}

bool SudokuCell::filterDomains(int value)
{
	bool filtered = false;
	if (!defined)
	{
		int counter = 0;
		while ((counter< domains.size())&&(!filtered))
		{
			if (domains.at(counter) == value)
			{
				filtered = true;
				domains.erase(domains.begin() + counter);
			}
			counter++;
		}
	}

	return filtered;
}

void SudokuCell::unfilterDomains(int value)
{
	domains.push_back(value);
	sort(domains.begin(), domains.end());
}


int SudokuCell::countDefinedConstraints()
{
	int count = 0;
	for (int i = 0; i < constrained_cells.size(); i++)
	{
		if (constrained_cells.at(i)->defined)
		{
			count++;
		}
	}
	return count;
}

string SudokuCell::getConstraintCellString()
{
	string to_return = "pos: ["+ Helpers::numberToString(x) +", "+ Helpers::numberToString(y)+"]\n";
	to_return += "constrained: \n";

	for (int i = 0; i < constrained_cells.size(); i++)
	{
		 to_return += "[" + Helpers::numberToString(constrained_cells.at(i)->x) + ", " + Helpers::numberToString(constrained_cells.at(i)->y) + "]\n";
	}

	return to_return;
}




