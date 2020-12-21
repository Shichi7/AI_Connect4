#include "stdafx.h"
#include "ProblemData.h"
#include <math.h>

ProblemData::ProblemData(string name, int dimension, string level, double optimal_score)
{
	this->name = name;
	this->dimension = dimension;
	this->level = level;
	this->optimal_score = optimal_score;
}


ProblemData::~ProblemData()
{
	for (unsigned int i = 0; i < cities.size(); i++)
	{
		delete cities.at(i);
	}
}


string ProblemData::baseDataToString()
{
	string optimal_string = optimal_score == -1 ? "Brak danych" : Misc::numberToString(optimal_score);

	string info = Misc::separator_line + Misc::newline_string + Misc::padString("Name: ", Misc::header_padding) + Misc::padString("[" + name + "]", Misc::value_padding, true)+"\n";
	info += Misc::padString("Dimension: ", Misc::header_padding) + Misc::padString("[" + Misc::numberToString(dimension) + "]", Misc::value_padding, true) + "\n";
	info += Misc::padString("Level: ", Misc::header_padding) + Misc::padString("[" + level + "]", Misc::value_padding, true) + "\n";
	info += Misc::padString("Optimal: ", Misc::header_padding) + Misc::padString("[" + optimal_string + "]", Misc::value_padding, true) + "\n";
	info += citiesDataToString();
	info += Misc::separator_line;
	return info;
}

string ProblemData::baseDataToCSVString()
{
	string info = "Problem name" + Misc::csv_delimeter + name + "\n";;
	info += "Level" + Misc::csv_delimeter + Misc::numberToString(level) + "\n";
	info += "Dimension" + Misc::csv_delimeter + Misc::numberToString(dimension) + "\n";;
	info += "Optimal score" + Misc::csv_delimeter + Misc::numberToString(optimal_score) + "\n";

	return info;
}

string ProblemData::citiesDataToString()
{
	string info = Misc::newline_string + Misc::inner_problem_cities_header + Misc::double_newline_string;
	for (unsigned int i = 0; i < cities.size(); i++)
	{
		info += cities.at(i)->toString() + Misc::newline_string;
	}
	return info;
}

CityNode* ProblemData::getCityById(int id)
{
	return cities.at(id-1);
}

void ProblemData::pushNewCity(CityNode* city_pointer)
{
	for (unsigned int i = 0; i < cities.size(); i++)
	{
		cities.at(i)->setDistance(city_pointer);
		city_pointer->setDistance(cities.at(i), cities.at(i)->getDistance(city_pointer->id));
	}

	cities.push_back(city_pointer);
}


double ProblemData::evaluteIndividual(int* individual_data)
{
	double total_distance = 0;
	double length = 0;

	for (int i = 0; i < dimension; i++)
	{
		CityNode* current_city = getCityById(individual_data[i]);
		CityNode* next_city;

		int next_city_id = (i < (dimension - 1)) ? i + 1 : 0;

		next_city = getCityById(individual_data[next_city_id]);
		double distance = current_city->getDistance(next_city->id);

		total_distance += distance;
	}
	return total_distance;
}
