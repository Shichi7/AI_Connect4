// ProblemPlecakowy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "ProblemData.h"
#include "GreedyAlgorithm.h"
#include "GeneticAlgorithm.h"
#include "Loader.h"
#include "CSVSaver.h"
#include "Misc.h"

#include <iostream>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//FILEPATH
#define FILEPATH "problem_files/"

//DEBUG SETTINGS
#define DEBUG_PROBLEM_DATA 0
#define DEBUG_GREEDY 0
#define DEBUG_RANDOM 0
#define DEBUG_GENETIC 0

//GREEDY SETTINGS
#define GREEDY_ON 1


//RANDOM SETTINGS
#define RANDOM_ON 1
#define RANDOM_ALGORITHM_POP 100

//GENETIC SETTINGS
#define SELECTION_TYPE "tournament" //roulette/tournament
#define CROSSING_TYPE "PMX" //OX/PMX
#define MUTATION_TYPE "inverse" //swap/inverse

#define CROSSING_PROBABILITY 500 //promiles
#define MUTATION_PROBABILITY 150 //promiles

#define POPULATION_SIZE 100
#define ITERATIONS 1000

#define TOURNAMENT_SIZE 5 //population percentage (min 2 individuals, to force 1 set 0)

#define GENETIC_REPEATS 1


//END OF SETTINGS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


using namespace std;

void defineProblems(vector<ProblemData*>& problems);
void freeProblemsMemory(vector<ProblemData*>& problems);
string getProblemsString(vector<ProblemData*> problems);
string loadProblems(vector<ProblemData*>& problems);

void defineProblems(vector<ProblemData*>& problems)
{
	//problems.push_back(new ProblemData("berlin11_modified", 11, "Easy", 4038));
	//problems.push_back(new ProblemData("berlin52", 52, "Easy", 7542));
	problems.push_back(new ProblemData("kroA100", 100, "Medium", 21282));
	//problems.push_back(new ProblemData("kroA150", 150, "Medium", 26524));
	//problems.push_back(new ProblemData("kroA200", 200, "Medium", 29368));
	//problems.push_back(new ProblemData("fl417", 417, "Hard", 11861));
}

int main()
{
	bool greedy_on = GREEDY_ON;
	bool random_on = RANDOM_ON;

	bool debug_problem_data = DEBUG_PROBLEM_DATA;
	bool debug_greedy = DEBUG_GREEDY;
	bool debug_random = DEBUG_RANDOM;
	bool debug_genetic = DEBUG_GENETIC;

	int random_pop = RANDOM_ALGORITHM_POP;

	string crossing_type = CROSSING_TYPE;
	string mutation_type = MUTATION_TYPE;
	string selection_type = SELECTION_TYPE;

	int crossing_prob = CROSSING_PROBABILITY;
	int mutation_prob = MUTATION_PROBABILITY;
	int population_size = POPULATION_SIZE;
	int iterations = ITERATIONS;

	int tournament_size = TOURNAMENT_SIZE;

	int genetic_repeats = GENETIC_REPEATS;

	string comm = "";
	bool errors = false;
	vector<ProblemData*> problems;

	defineProblems(problems);
	comm = loadProblems(problems);

	errors = !Misc::emptyString(comm);

	if (!errors)
	{
		if (debug_problem_data)
		{
			comm = getProblemsString(problems);
		}
	}

	Misc::printComm(comm, true);


	if (!errors)
	{
		for (unsigned int i = 0; i < problems.size(); i++)
		{
			GreedyAlgorithm greedy;
			GeneticAlgorithm random;
			if (greedy_on)
			{
				greedy.setData(problems.at(i));
				greedy.run();

				if (debug_greedy)
				{
					comm = greedy.returnGreedyResultString();
					Misc::printComm(comm, true);
				}
			}

			if (random_on)
			{
				random.setProblemData(problems.at(i));
				random.runRandom(random_pop);

				if (debug_random)
				{
					comm = random.returnRandomAlgorithmString();
					Misc::printComm(comm, true);
				}
			}

			vector<double> bests;
			for (int j = 0; j < genetic_repeats; j++)
			{
				GeneticAlgorithm genetic;
				genetic.setProblemData(problems.at(i));
				genetic.configMain(iterations, population_size, debug_genetic);

				genetic.configSelection(selection_type, tournament_size);
				genetic.configCrossing(crossing_type, crossing_prob);
				genetic.configMutation(mutation_type, mutation_prob);

				genetic.run();

				if (debug_genetic)
				{
					comm = genetic.getFullDebug();
					Misc::printComm(comm, true);
				}

				bests.push_back(genetic.getCurrentBest());

				CSVSaver csvSaver;
				GeneticAlgorithm* random_pointer = NULL;
				GreedyAlgorithm* greedy_pointer = NULL;
				if (greedy_on)
				{
					greedy_pointer = &greedy;
				}

				if (random_on)
				{
					random_pointer = &random;
				}

				csvSaver.setData(problems.at(i), &genetic, random_pointer, greedy_pointer, j);
				csvSaver.setRepeats(bests);
				csvSaver.writeToCSV();
			}
		}
	}

	system("cls");
	Misc::printComm(Misc::quitting, true);

	freeProblemsMemory(problems);
	
	return 0;
}

string loadProblems(vector<ProblemData*>& problems)
{
	string filepath = FILEPATH;
	Loader loader;
	string comm = "";

	loader.setFilepathBase(filepath);

	unsigned int i = 0;

	while ((Misc::emptyString(comm)) && (i < problems.size()))
	{
		comm = loader.loadData(problems.at(i));
		i++;
	}
	return comm;
}

void freeProblemsMemory(vector<ProblemData*>& problems)
{
	for (unsigned int i = 0; i < problems.size(); i++)
	{
		delete(problems.at(i));
	}

	problems.clear();
}

string getProblemsString(vector<ProblemData*> problems)
{
	string data_string = problems.size() == 0 ? "Brak wczytanych problemów" : "";

	if (problems.size() > 0)
	{
		for (unsigned int i = 0; i < problems.size(); i++)
		{
			data_string += problems.at(i)->baseDataToString();
		}
	}

	return data_string;
}




