#pragma once
#include <iostream>
#include "Misc.h"
#include "ProblemData.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class Loader
{
public:
	Loader();
	~Loader();

	vector<string> data_lines;

	string loadData(ProblemData* data);
	string loadStringFromFile(string filename);
	void setFilepathBase(string filepath_base);


private: 
	string filepath_base;

	int getDataStartingLine();

	int loadCityData(ProblemData* data, int city_count, unsigned int* next_line);

	bool isEOF(int current_line);
	string getNextPhrase(string line);
	void trim_front(string* line);

};

