#include "stdafx.h"
#include "Loader.h"

using namespace std;

Loader::Loader()
{
}

Loader::~Loader()
{
}

void Loader::setFilepathBase(string filepath_base)
{
	this->filepath_base = filepath_base;
}

int Loader::getDataStartingLine()
{
	unsigned int current_line = 0;
	int starting_line = -1;

	while (current_line < data_lines.size()&&(starting_line == -1))
	{
		trim_front(&(data_lines.at(current_line)));
		string first_phrase = getNextPhrase(data_lines.at(current_line));
		if (Misc::isStringNumber(first_phrase, true, true))
		{
			if (std::stoi(first_phrase) == 1)
			{
				starting_line = current_line;
			}
		}
		current_line++;
	}
	return starting_line;
}

string Loader::getNextPhrase(string line)
{
	string to_return = "";
	unsigned int counter = 0;

	while ((counter < line.length()) && ((isalnum(line.at(counter))) || (line.at(counter) == '.') || (line.at(counter) == '-') || (line.at(counter) == '+')))
	{
		to_return += line.at(counter);
		counter++;
	}
	return to_return;
}

string Loader::loadData(ProblemData* data)
{
	string filepath = filepath_base + data->name + "." + Misc::problem_files_extension;
	string comm = loadStringFromFile(filepath);

	if (Misc::emptyString(comm))
	{
		int errors = 0;
		unsigned int next_line = getDataStartingLine();

		if (next_line != -1)
		{
			unsigned int city_count = 1;

			while ((!isEOF(next_line)) && (next_line < data_lines.size()))
			{
				errors += loadCityData(data, city_count, &next_line);
				city_count++;
			}
			city_count--;

			if (errors > 0)
			{
				comm = data->name + ": " + Misc::loading_data_corrupted + Misc::numberToString(errors);
			}
			else if (city_count != data->dimension)
			{
				comm = data->name + ": " + Misc::loading_data_incomplete;
			}
		}
		else
		{
			comm = data->name + ": " + Misc::loading_data_error;
		}
	}

	data_lines.clear();

	return comm;
}

bool Loader::isEOF(int current_line)
{
	return (data_lines.at(current_line).compare("EOF") == 0);
}

int Loader::loadCityData(ProblemData* data, int city_count, unsigned int* next_line)
{
	int error = 0;

	string line = data_lines.at(*next_line);

	trim_front(&line);

	string id = getNextPhrase(line);

	line.erase(0, id.length());
	trim_front(&line);

	string x_coord = getNextPhrase(line);

	line.erase(0, x_coord.length());
	trim_front(&line);

	string y_coord = getNextPhrase(line);
	line.erase(0, y_coord.length());
	trim_front(&line);

	error = 1;

	//cout << "loaded id: [" << id << "] x: [" + x_coord + "] y: [" + y_coord + "]\n";

	if ((line.length() == 0) && (id.length() != 0)&& (x_coord.length() != 0) && (y_coord.length() != 0))
	{
		if ((Misc::isStringNumber(id, true, true)) && (Misc::isStringNumber(x_coord)) && (Misc::isStringNumber(y_coord)))
		{
			int converted_id = atoi(id.c_str());
			double converted_x = atof(x_coord.c_str());
			double converted_y = atof(y_coord.c_str());

			if (converted_id == city_count)
			{
				CityNode* new_node = new CityNode(converted_id, converted_x, converted_y, data->dimension);
				data->pushNewCity(new_node);
				error = 0;
			}
		}
	}

	(*next_line)++;
	return error;
}

void Loader::trim_front(string* line)
{
	while ((line->length() > 0) && (!isalnum(line->at(0))) && ((line->at(0)) != '-'))
	{
		line->erase(0, 1);
	}
}

string Loader::loadStringFromFile(string filename)
{
	string loaded_string = "";
	ifstream file_stream(filename);

	if (file_stream.is_open())
	{
		string new_line;
		while (getline(file_stream, new_line))
		{
			data_lines.push_back(new_line);
		}
		file_stream.close();
	}
	else
	{
		loaded_string = Misc::loading_file_error;
	}

	return loaded_string;
}