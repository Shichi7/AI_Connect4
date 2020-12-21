#include "stdafx.h"
#include "GreedyAlgorithm.h"


GreedyAlgorithm::GreedyAlgorithm()
{
	data_set_flag = false;
	best = INT_MAX;
	worst = -INT_MAX;
}

GreedyAlgorithm::~GreedyAlgorithm()
{
	if (data_set_flag)
	{
		for (unsigned int i = 0; i < results.size(); i++)
		{
			delete results.at(i);
			results.clear();
		}
	}
}

void GreedyAlgorithm::setData(ProblemData* problem_data)
{
	this->data = problem_data;
	data_set_flag = true;
}

void GreedyAlgorithm::run()
{
	for (int i = 0; i < data->dimension; i++)
	{
		int* genes = new int[data->dimension];
		bool* used_ids = new bool[data->dimension];

		for (int j = 0; j < data->dimension; j++)
		{
			used_ids[j] = false;
		}

		genes[0] = i + 1;
		used_ids[i] = true;

		for (int j = 1; j < data->dimension; j++)
		{
			int nearest_id;
			int* ranking = data->getCityById(genes[j - 1])->distances_ranking;

			bool used = true;
			unsigned int counter = 0;

			while (used)
			{
				nearest_id = ranking[counter];
				used = used_ids[nearest_id - 1];

				counter++;
			}

			used_ids[nearest_id - 1] = true;
			genes[j] = nearest_id;
		}
		Individual* new_result = new Individual(data);
		new_result->setNewGenes(genes);
		results.push_back(new_result);

		delete genes;
		delete used_ids;
	}

	final_fitness = 0;
	for (unsigned int i = 0; i < results.size(); i++)
	{
		results.at(i)->evaluateFitness();
		double fitness = results.at(i)->getFitness();
		if (fitness < best) best = fitness;
		if (fitness > worst) worst = fitness;
		final_fitness += fitness;
	}
	final_fitness /= results.size();

	for (unsigned int i = 0; i < results.size(); i++)
	{
		double fitness = results.at(i)->getFitness();
		standard += pow(fitness - final_fitness, 2);
	}

	standard = sqrt(standard / (results.size() - 1));
}

double GreedyAlgorithm::returnFitness()
{
	return final_fitness;
}

string GreedyAlgorithm::returnGreedyResultString()
{
	string info = "";
	info += data->name + "    optimal: [" + Misc::numberToString(data->optimal_score) + "]" + Misc::newline_string;
	info += Misc::greedy_keyword + Misc::newline_string;
	for (unsigned int i = 0; i < results.size(); i++)
	{
		info += results.at(i)->getString() + Misc::newline_string;
	}
	info += "Average fitness: [" + Misc::numberToString(final_fitness) + "]" + Misc::newline_string;
	return info;
}

string GreedyAlgorithm::returnGreedyResultCSV()
{
	return "GREEDY" + Misc::csv_delimeter + Misc::numberToString(final_fitness) +"\n";
}
