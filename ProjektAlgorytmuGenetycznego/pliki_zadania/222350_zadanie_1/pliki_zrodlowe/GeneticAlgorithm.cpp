#include "stdafx.h"
#include "GeneticAlgorithm.h"


GeneticAlgorithm::GeneticAlgorithm()
{
	resetRecords();
	csv_string = Misc::csv_data_header + "\n";
	debug_string = "";
}

GeneticAlgorithm::~GeneticAlgorithm()
{
	deletePopulation(&population);
}

void GeneticAlgorithm::deletePopulation(vector<Individual*>* population_pointer)
{
	for (unsigned int i = 0; i < population_pointer->size(); i++)
	{
		if (population_pointer->at(i) != NULL)
		{
			delete population_pointer->at(i);
			//cout << "USUWAM: " << i<<endl;
		}
	}

	population_pointer->clear();
}

void GeneticAlgorithm::resetRecords()
{
	current_best = INT_MAX;
	current_worst = -current_best;
}

void GeneticAlgorithm::runRandom(int population_count)
{
	this->population_size = population_count;
	for (int i = 0; i < population_count; i++)
	{
		population.push_back(new Individual(data, &generator));
		population.at(i)->fillDataRandom();
		checkRecords(population.at(i)->getFitness(), i);
	}

	calculatePopulationMeanScore();
}

void GeneticAlgorithm::run()
{
	updateStrings(0);
	
	for (int i = 0; i < iterations; i++)
	{
		resetRecords();
		crossPopulation();
		mutatePopulation();
		updateStrings(i + 1);
	}
}

void GeneticAlgorithm::updateStrings(int iteration)
{
	double current_mean_fitness = calculatePopulationMeanScore();

	if (full_debug)
	{
		string intro_text = Misc::newline_string + Misc::iteration_keyword + " " + Misc::numberToString(iteration) + Misc::newline_string;
		if (iteration == 0)
		{
			intro_text = Misc::original_population_string + Misc::newline_string;
		}
		debug_string += intro_text + returnPopulationString() + Misc::newline_string + getStringOfIndex(current_best_index) + Misc::newline_string + getStringOfIndex(current_worst_index) + Misc::newline_string + Misc::newline_string;
	}

	csv_string += Misc::numberToString(iteration) + Misc::csv_delimeter + Misc::numberToString(population[current_best_index]->getFitness()) + Misc::csv_delimeter + Misc::numberToString(current_mean_fitness) + Misc::csv_delimeter + Misc::numberToString(population[current_worst_index]->getFitness()) + "\n";
}


void GeneticAlgorithm::mutatePopulation()
{
	bool inverse = false;
	if (mutation_type.compare("inverse") == 0) inverse = true;

	for (int i = 0; i < population_size; i++)
	{
		population.at(i)->mutate(mutation_probability, inverse);
		checkRecords(population.at(i)->getFitness(), i);
	}
}


pair<int, int> GeneticAlgorithm::randSegmentBounds()
{
	pair<int, int> segment_bounds;

	segment_bounds.first = generator.generate(0, data->dimension - 1);
	segment_bounds.second = generator.generate(0, data->dimension - 1);

	if (segment_bounds.first > segment_bounds.second)
	{
		int temp = segment_bounds.first;
		segment_bounds.first = segment_bounds.second;
		segment_bounds.second = temp;
	}

	return segment_bounds;
}

void GeneticAlgorithm::crossIndividuals(pair<int, int> parents, vector<Individual*>* new_population, bool crossing)
{
	bool ox_type = true;
	if (crossing_type.compare("PMX") == 0) ox_type = false;

	if (crossing)
	{
		pair<int, int> bounds = randSegmentBounds();

		if (full_debug) debug_string += Misc::crossing_string + Misc::numberToString(parents.first) + ";" + Misc::numberToString(parents.second) + Misc::newline_string + Misc::crossing_index_string + Misc::numberToString(bounds.first) + "-" + Misc::numberToString(bounds.second) + Misc::newline_string;
		
		new_population->push_back(population.at(parents.first)->cross(population.at(parents.second), bounds.first, bounds.second, ox_type));
		checkRecords(new_population->back()->getFitness(), new_population->size() - 1);
		if (new_population->size() != population_size)
		{
			new_population->push_back(population.at(parents.second)->cross(population.at(parents.first), bounds.first, bounds.second, ox_type));
			checkRecords(new_population->back()->getFitness(), new_population->size()-1);
		}
		/*cout << "segment: " << bounds.first << "-" << bounds.second;
		cout << endl;
		cout << "parent1" << population.at(parents.first)->getString() << endl;
		cout << "parent2" << population.at(parents.second)->getString() << endl;

		cout << "child1" << new_population->at(new_population->size() - 2) -> getString() << endl;
		cout << "child2" << new_population->at(new_population->size() - 1) -> getString() << endl;
		system("pause");*/
	}
	else
	{
		if (full_debug) debug_string += Misc::no_crossing_string + Misc::numberToString(parents.first) + ";" + Misc::numberToString(parents.second) + Misc::newline_string;

		new_population->push_back(new Individual(*population.at(parents.first)));
		checkRecords(new_population->back()->getFitness(), new_population->size() - 1);
		if (new_population->size() != population_size)
		{
			new_population->push_back(new Individual(*population.at(parents.second)));
			checkRecords(new_population->back()->getFitness(), new_population->size() - 1);
		}
	}
}

void GeneticAlgorithm::crossPopulation()
{
	vector<Individual*> new_population;
	/*bool* reused_indexes = new bool[population_size];
	for (unsigned int i = 0; i < population_size; i++)
	{
		reused_indexes[i] = false;
	}*/

	while (new_population.size() != population_size)
	{
		int random_number = generator.generate(0, 999);
		pair<int, int> parents = selection();

		if (random_number < crossing_probability)
		{
			crossIndividuals(parents, &new_population);
		}
		else
		{
			crossIndividuals(parents, &new_population, false);
		}
	}

	deletePopulation(&population);
	population = new_population;
}

pair<int, int> GeneticAlgorithm::selection()
{
	pair<int, int> parents;

	if (selection_type.compare("roulette") == 0)
	{
		parents.first = runRoulette();
		parents.second = parents.first;
		while (parents.first == parents.second)
		{
			parents.second = runRoulette();
		}
	}
	else if (selection_type.compare("tournament") == 0)
	{
		parents.first = runTournament();
		parents.second = parents.first;
		while (parents.first == parents.second)
		{
			parents.second = runTournament();
		}
	}

	//cout << "Wybrano ostatecznie: " << parents.second << endl << "fitness: " << population[parents.second]->getFitness() << endl;
	//cout << "Wybrano ostatecznie: " << parents.first << endl << "fitness: " << population[parents.first]->getFitness() << endl;
	//system("pause");

	return parents;
}

int GeneticAlgorithm::runRoulette()
{
	return 0;
}

int GeneticAlgorithm::runTournament()
{
	int* indexes = new int[tournament_size];

	bool* possible_indexes = new bool[population_size];
	for (int i = 0; i < population_size; i++)
	{
		possible_indexes[i] = false;
	}

	for (int i = 0; i < tournament_size; i++)
	{
		bool taken = true;
		while (taken)
		{
			int candidate = generator.generate(0, population_size - 1);
			if (!possible_indexes[candidate])
			{
				taken = false;
				possible_indexes[candidate] = true;
				indexes[i] = candidate;
			}
			//cout << "Kandydat: " << candidate << endl<<"fitness: "<< population[candidate]->getFitness()<<endl;
		}
	}

	int best_parent_index = indexes[0];
	double best_fitness = population[indexes[0]]->getFitness();

	for (int i = 1; i < tournament_size; i++)
	{
		if (population[indexes[i]]->getFitness() < best_fitness)
		{
			best_parent_index = indexes[i];
			best_fitness = population[indexes[i]]->getFitness();
		}
	}

	delete indexes;
	delete possible_indexes;

	//cout << "Wybrano: " << best_parent_index << endl << "fitness: " << population[best_parent_index]->getFitness() << endl;

	return best_parent_index;
}


void GeneticAlgorithm::configSelection(string selection_type, int tournament_size)
{
	this->selection_type = selection_type;
	this->tournament_size = (tournament_size*population_size)/100;

	if (this->tournament_size < 2) this->tournament_size = 2;
	if (tournament_size == 0) this->tournament_size = 1;

	/*cout <<"["<< tournament_size<<"]";
	system("pause");*/
}

void GeneticAlgorithm::configMutation(string mutation_type, int mutation_probability)
{
	this->mutation_type = mutation_type;
	this->mutation_probability = mutation_probability;
}

void GeneticAlgorithm::configCrossing(string crossing_type, int crossing_probability)
{
	this->crossing_type = crossing_type;
	this->crossing_probability = crossing_probability;
}

void GeneticAlgorithm::configMain(int iterations, int population_size, bool debug)
{
	this->iterations = iterations;
	this->population_size = population_size;

	this->full_debug = debug;

	for (int i = 0; i < population_size; i++)
	{
		population.push_back(new Individual(data, &generator));
		population.at(i)->fillDataRandom();
		checkRecords(population.at(i)->getFitness(), i);
	}
}

void GeneticAlgorithm::checkRecords(double value, int index)
{
	if (value < current_best)
	{
		current_best = value;
		current_best_index = index;
	}
	else if (value > current_worst)
	{
		current_worst = value;
		current_worst_index = index;
	}
}

void GeneticAlgorithm::setProblemData(ProblemData* data)
{
	this->data = data;
}

double GeneticAlgorithm::calculatePopulationMeanScore()
{
	double mean_score = 0;
	for (unsigned int i = 0; i < population.size(); i++)
	{
		mean_score += population.at(i)->getFitness();
	}
	mean_score /= population.size();
	return mean_score;
}

double GeneticAlgorithm::calculatePopulationStandard()
{
	double standard = 0;
	double mean = calculatePopulationMeanScore();
	for (unsigned int i = 0; i < population.size(); i++)
	{
		double fitness = population.at(i)->getFitness();
		standard += pow(fitness - mean, 2);
	}

	standard = sqrt(standard / (population.size() - 1));
	return standard;
}

string GeneticAlgorithm::returnRandomAlgorithmString()
{
	string info = "";

	for (unsigned int i = 0; i < population.size(); i++)
	{
		info += getStringOfIndex(i);
		info += Misc::newline_string;
	}

	info += data->name + "    optimal: [" + Misc::numberToString(data->optimal_score) + "]" + Misc::newline_string;
	info += "RANDOM SOLUTION" + Misc::newline_string;
	info += "Average fitness: "+ Misc::numberToString(calculatePopulationMeanScore()) + Misc::newline_string;

	return info;
}

string GeneticAlgorithm::returnPopulationString()
{
	string info = "";

	for (unsigned int i = 0; i < population.size(); i++)
	{
		info += getStringOfIndex(i);
		info += Misc::newline_string;
	}

	return info;
}

string GeneticAlgorithm::getStringOfIndex(int index)
{
	return Misc::numberToString(index) + " . " + population.at(index)->getString();
}


string GeneticAlgorithm::getFullDebug()
{
	return debug_string;
}

string GeneticAlgorithm::getCSVDebug()
{
	return csv_string;
}

string GeneticAlgorithm::getCSVRandomDebug()
{
	return Misc::numberToString(current_best) + Misc::csv_delimeter + Misc::numberToString(calculatePopulationMeanScore()) + Misc::csv_delimeter + Misc::numberToString(current_worst) + Misc::csv_delimeter + Misc::numberToString(calculatePopulationStandard()) + Misc::csv_delimeter + Misc::numberToString(population_size) + Misc::csv_delimeter;
}

string GeneticAlgorithm::getAlgorithmCSVInfo()
{
	return Misc::numberToString(iterations) + Misc::csv_delimeter + Misc::numberToString(population_size) + Misc::csv_delimeter + selection_type + Misc::csv_delimeter + Misc::numberToString(tournament_size) + Misc::csv_delimeter + crossing_type + Misc::csv_delimeter + Misc::numberToString(crossing_probability) + Misc::csv_delimeter + mutation_type + Misc::csv_delimeter + Misc::numberToString(mutation_probability) + Misc::csv_delimeter;
}

string GeneticAlgorithm::getAlgorithmCSVSummary()
{
	return Misc::numberToString(population.at(current_best_index)->getFitness()) + Misc::csv_delimeter + Misc::numberToString(calculatePopulationMeanScore()) + Misc::csv_delimeter + Misc::numberToString(population.at(current_worst_index)->getFitness()) + Misc::csv_delimeter;
}

double GeneticAlgorithm::getCurrentBest()
{
	return current_best;
}

/*string GeneticAlgorithm::getAlgorithmInfo()
{
	string info = "";

	info += "ALGORITHM SETTINGS:\n";
	info += Misc::padString("Iterations:", 25) + Misc::padString("[" + Misc::numberToString(iteration_number) + "]", 7, true) + "\n";
	info += Misc::padString("Population Size: ", 25) + Misc::padString("["+ Misc::numberToString(population_size) + "]", 7, true) + "\n";
	info += Misc::padString("Cross Probability: ", 25) + Misc::padString("[" + Misc::numberToString(crossing_probability) + "%]", 7, true) + "\n";
	info += Misc::padString("Mutation Probability: ", 25) + Misc::padString("[" + Misc::numberToString(mutation_probability) + "%]", 7, true) + "\n";
	info += Misc::padString("Tournament Size: ", 25) + Misc::padString("[" + Misc::numberToString(tournament_size) + "]", 7, true) + "\n";
	return info;
}*/