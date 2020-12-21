#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include "helpers.h"
#include <vector>

using namespace std;

class Loader
{
public:
	Loader();
	~Loader();

	static string loadSudokuByID(string filepath, int id);
	static void loadJolkaDataByID(vector<string>* data_lines, string filebasepath, int id);
};

