#pragma once
#include <iostream>
#include "CityNode.h"
#include "Misc.h"
#include "vector"

using namespace std;

class ProblemData
{
public:
	ProblemData(string name, int dimension, string level, double optimal_score = -1);
	~ProblemData();

	double evaluteIndividual(int* individual_data);

	string baseDataToCSVString();
	string baseDataToString();
	string citiesDataToString();

	void pushNewCity(CityNode* city_pointer);
	CityNode* getCityById(int id);

	string name;
	int dimension;
	string level;
	string filepath;
	double optimal_score;

private:
	vector<CityNode*> cities;
};