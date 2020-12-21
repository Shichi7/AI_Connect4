#include "stdafx.h"
#include "CSVSaver.h"
#include "Misc.h"

using namespace std;

CSVSaver::CSVSaver()
{
	mean = 0;
	standard = 0;
	worst_best = -INT_MAX;
	best_best = INT_MAX;
}


CSVSaver::~CSVSaver()
{
}

void CSVSaver::setData(ProblemData* problem, GeneticAlgorithm* genetic, GeneticAlgorithm* random, GreedyAlgorithm* greedy, int repeat_no)
{
	this->problem = problem;
	this->genetic = genetic;
	this->random = random;
	this->greedy = greedy;
	this->repeat = repeat_no;
}

string CSVSaver::getCSVHeader()
{
	string header = "NAME" + Misc::csv_delimeter + "LEVEL" + Misc::csv_delimeter + "DIMENSION" + Misc::csv_delimeter + "OPTIMAL"  + Misc::csv_delimeter;
	header += "GREEDY BEST" + Misc::csv_delimeter + "GREEDY MEAN" + Misc::csv_delimeter + "GREEDY WORST" + Misc::csv_delimeter + "GREEDY STANDARD" + Misc::csv_delimeter + "RANDOM BEST" + Misc::csv_delimeter + "RANDOM MEAN" + Misc::csv_delimeter + "RANDOM WORST" + Misc::csv_delimeter + "RANDOM STANDARD" + Misc::csv_delimeter + "RANDOM POPULATION";
	header += "\n";
	return header;
}

string CSVSaver::getCSVHeaderGenetic()
{
	string header = "GENETIC" + Misc::csv_delimeter + "\n";
	header += "ITERATIONS" + Misc::csv_delimeter + "POPULATION SIZE" + Misc::csv_delimeter + "SELECTION TYPE" + Misc::csv_delimeter + "TOURNAMENT SIZE" + Misc::csv_delimeter + "CROSSING TYPE" + Misc::csv_delimeter + "CROSSING PROB ‰" + Misc::csv_delimeter + "MUTATION TYPE" + Misc::csv_delimeter + "MUTATION PROB ‰" + Misc::csv_delimeter;
	header += "\n";
	return header;
}

string CSVSaver::getCSVHeaderGeneticSummary()
{
	string header = "LAST BEST" + Misc::csv_delimeter + "LAST AVG" + Misc::csv_delimeter + "LAST WORST" +Misc::csv_delimeter + "\n";
	return header;
}

void CSVSaver::setRepeats(vector<double> bests, vector<int> bests_indexes)
{
	this->bests = bests;
	for (int i = 0; i < bests.size(); i++)
	{
		double value = bests.at(i);
		if (value < best_best)
		{
			best_best = value;
			best_of_bests = genetic->getStringOfIndex(genetic->getCurrentBestIndex());
		}
		if (value > worst_best)
		{
			worst_best = value;
		}
		mean += value;
	}
	mean /= bests.size();

	for (int i = 0; i < bests.size(); i++)
	{
		double value = bests.at(i);
		standard += pow(value - mean, 2);
	}
	standard = sqrt(standard / bests.size() - 1);
}

string CSVSaver::buildCSVString()
{
	string final_csv = getCSVHeader();

	final_csv += problem->name + Misc::csv_delimeter + problem->level + Misc::csv_delimeter + Misc::numberToString(problem->dimension) + Misc::csv_delimeter + Misc::numberToString(problem->optimal_score);
	final_csv += Misc::csv_delimeter;
	if (greedy != NULL)
	{
		final_csv += Misc::numberToString(greedy->best) + Misc::csv_delimeter + Misc::numberToString(greedy->final_fitness) + Misc::csv_delimeter + Misc::numberToString(greedy->worst) + Misc::csv_delimeter + Misc::numberToString(greedy->standard);
	}
	else
	{
		final_csv += "-";
	}
	final_csv += Misc::csv_delimeter;
	if (random != NULL)
	{
		final_csv += random->getCSVRandomDebug();
	}
	else
	{
		final_csv += "-" + Misc::csv_delimeter + "-" + Misc::csv_delimeter;
	}

	final_csv += Misc::double_newline_string;
	final_csv += getCSVHeaderGenetic();
	final_csv += genetic->getAlgorithmCSVInfo();

	final_csv += Misc::double_newline_string;
	final_csv += getCSVHeaderGeneticSummary();
	final_csv += genetic->getAlgorithmCSVSummary();

	final_csv += Misc::double_newline_string;
	final_csv += "BEST BEST; MEAN BEST; WORST BEST; STANDARD;\n";
	final_csv += Misc::numberToString(best_best) + Misc::csv_delimeter + Misc::numberToString(mean) + Misc::csv_delimeter + Misc::numberToString(worst_best) + Misc::csv_delimeter + Misc::numberToString(standard) + Misc::csv_delimeter;
	
	final_csv += Misc::double_newline_string;
	final_csv += "BESTS\n";
	for (int i = 0; i < bests.size(); i++)
	{
		final_csv += Misc::numberToString(bests.at(i)) + "\n";
	}

	final_csv += Misc::double_newline_string;
	final_csv += best_of_bests;

	final_csv += Misc::double_newline_string;
	final_csv += genetic->getCSVDebug();

	return final_csv;
}


void CSVSaver::writeToCSV()
{
	string to_write = buildCSVString();
	time_t raw_time = time(0);
	struct tm time_info;

	localtime_s(&time_info, &raw_time);

	char buffer[80];
	strftime(buffer, sizeof(buffer), "%d-%m-%Y_%H-%M-%S", &time_info);

	string date_string(buffer);

	string file_name = "results/res_" + date_string + "[" + Misc::numberToString(repeat)+"]"+ ".csv";

	ofstream myfile;
	myfile.open(file_name);

	myfile << to_write;

	myfile.close();
}
