#include "stdafx.h"
#include "Individual.h"
#include <vector>
#include <algorithm>


using namespace std;

Individual::Individual(Individual& to_copy)
{
	this->generator = to_copy.generator;
	this->problem_data = to_copy.problem_data;
	length = this->problem_data->dimension;

	genes = new int[length];
	fitness = to_copy.fitness;

	for (int i = 0; i < length; i++)
	{
		genes[i] = to_copy.genes[i];
	}
}

Individual::Individual(ProblemData* problem)
{
	configure(problem);
}

Individual::Individual(ProblemData* problem, RandomGenerator* generator)
{
	configure(problem);
	this->generator = generator;
}

void Individual::configure(ProblemData* problem_data)
{
	this->problem_data = problem_data;
	length = this->problem_data->dimension;

	genes = new int[length];
}

Individual* Individual::crossOX(Individual* partner, int segment_start, int segment_end)
{
	Individual* child = new Individual(problem_data, generator);
	vector<int> used_values;

	for (int i = segment_start; i <= segment_end; i++)
	{
		child->genes[i] = genes[i];
		used_values.push_back(genes[i]);
	}

	int i = (segment_end + 1) % length;
	int j = i;

	while (i != segment_start)
	{
		while (find(used_values.begin(), used_values.end(), partner->genes[j]) != used_values.end())
		{
			j = (j + 1) % length;
		}
		child->genes[i] = partner->genes[j];
		j = (j + 1) % length;
		i = (i + 1) % length;
	}

	child->evaluateFitness();

	return child;
}

Individual* Individual::crossPMX(Individual* partner, int segment_start, int segment_end)
{
	//cout << this->getString() <<endl<<partner->getString() <<endl<<"["<<segment_start<<"] - ["<<segment_end<<"]"<<endl;

	Individual* child = new Individual(problem_data, generator);
	int* map = new int[length];
	for (int i = 0; i < length; i++)
	{
		map[i] = -1;
	}

	for (int i = segment_start; i <= segment_end; i++)
	{
		int parent1_gene = this->genes[i] - 1;
		int parent2_gene = partner->genes[i] - 1;

		if ((map[parent1_gene] == -1) && (map[parent2_gene] == -1))
		{
			map[parent1_gene] = parent2_gene;
			map[parent2_gene] = parent1_gene;
		}
	}

	for (int i = 0; i < length; i++)
	{
		int gene_value = this->genes[i];
		if (map[gene_value - 1] != -1)
		{
			gene_value = map[gene_value - 1] + 1;
		}

		child->genes[i] = gene_value;
		//cout << "[" << gene_value << "]";
	}
	//cout << endl;
	//system("pause");
	child->evaluateFitness();

	delete map;
	return child;
}


Individual* Individual::cross(Individual* partner, int segment_start, int segment_end, bool ox_type)
{
	Individual *child;
	if (ox_type) child = crossOX(partner, segment_start, segment_end);
	else child = crossPMX(partner, segment_start, segment_end);

	return child;
}

void Individual::mutate(int propability, bool inverse)
{
	if (!inverse)
	{
		mutateSwap(propability);
	}
	else
	{
		mutateInverse(propability);
	}
}


void Individual::mutateSwap(int propability)
{
	for (int i = 0; i < length; i++)
	{
		if (generator->generate(0, 999) < propability)
		{
			int to_swap_index = generator->generate(0, length - 1);
			int temp = genes[i];
			genes[i] = genes[to_swap_index];
			genes[to_swap_index] = temp;
		}
	}
	evaluateFitness();
}

void Individual::mutateInverse(int propability)
{
	if (generator->generate(0, 999) < propability)
	{
		int first_index = generator->generate(0, length - 1);
		int second_index = generator->generate(0, length - 1);

		if (first_index > second_index)
		{
			int temp = first_index;
			first_index = second_index;
			second_index = temp;
		}

		while (first_index < second_index)
		{
			int temp = genes[first_index];
			genes[first_index] = genes[second_index];
			genes[second_index] = temp;
			first_index++;
			second_index--;
		}
	}
	evaluateFitness();
}

void Individual::fillDataRandom()
{
	for (int i = 0; i < length; i++)
	{
		genes[i] = -1;
	}

	for (int i = 1; i <= problem_data->dimension; i++)
	{
		bool repeat = true;
		while (repeat)
		{
			int index = generator->generate(0, length - 1);
			if (genes[index] == (-1))
			{
				repeat = false;
				genes[index] = i;
			}
		}
	}

	evaluateFitness();
}

void Individual::setNewGenes(int* genes)
{
	for (int i = 0; i < length; i++)
	{
		this->genes[i] = genes[i];
	}
	evaluateFitness();
}

string Individual::getString()
{
	string to_return = "";
	string fitness_string = Misc::numberToString(fitness);
	int loop_len = Misc::fitness_string_length - fitness_string.length();

	for (int i = 0; i < loop_len; i++)
	{
		fitness_string = "0" + fitness_string;
	}

	to_return += "[" + Misc::fitness_keyword + fitness_string + "]" + " ----> " + "[" + Misc::data_keyword;

	for (int i = 0; i < length; i++)
	{
		to_return += Misc::numberToString(genes[i]);
		if (i < length - 1)
		{
			to_return += ", ";
		}
	}

	to_return += "]";
	return to_return;
}

double Individual::getFitness()
{
	return fitness;
}

void Individual::evaluateFitness()
{
	fitness = problem_data->evaluteIndividual(genes);
}

Individual::~Individual()
{
	delete genes;
}