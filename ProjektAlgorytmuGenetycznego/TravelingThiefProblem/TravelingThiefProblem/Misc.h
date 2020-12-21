#pragma once
using namespace std;

#include <string>
#include <sstream>
#include <iomanip>
namespace Misc
{
	const string separator_line = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
	const string newline_string = "\n";
	const string double_newline_string = "\n\n";
	const string problem_files_extension = "tsp";
	const string comm_header_string = "COMM: ";

	const string loading_file_error= "Wrong filepath!";
	const string loading_data_corrupted = "Number of corrupted data lines: ";
	const string loading_data_error = "Cities data starting line not found!";
	const string loading_data_incomplete = "Dimension not matching data lines!";

	const string quitting = "QUITTING...";

	//start of HEADERS
	const string csv_data_header = "iteration;best;avg;worst;";
	const string csv_delimeter = ";";

	const string inner_problem_cities_header = "CITIES DATA";
	//end of HEADERS

	const string original_population_string = "ORIGINAL POPULATION: ";
	const string crossing_string = "CROSSING OF FOLLOWING INDIVIDUALS: ";
	const string crossing_index_string = "SEGMENT: ";
	const string no_crossing_string = "LACK OF CROSSING OF FOLLOWING INDIVIDUALS: ";

	const string data_keyword = "DATA: ";
	const string fitness_keyword = "FITNESS: ";
	const string iteration_keyword = "ITERATION: ";
	const string greedy_keyword = "GREEDY SOLUTION: ";

	const int fitness_string_length = 5;

	template<class T>
	static string numberToString(T x)
	{
		stringstream stream;
		stream << x;
		return stream.str();
	}

	template<>
	static string numberToString(double x)
	{
		stringstream stream;
		stream <<fixed<<setprecision(2) << x;
		return stream.str();
	}

	static bool emptyString(string comm)
	{
		return (comm.compare("") == 0);
	}

	static bool isStringNumber(string to_check, bool only_positive = false, bool only_integer = false)
	{
		bool result = true;
		unsigned int counter = 0;
		bool delimeter_found = false;
		bool e_found = false;
		bool plus_found = false;
		bool negative = false;

		if (!emptyString(to_check))
		{
			while (counter < to_check.length() && result)
			{
				if (!isdigit(to_check.at(counter)))
				{
					if ((to_check.at(counter) == '.') && (!only_integer) && (!delimeter_found) && (counter < (to_check.length() - 1)))
					{
						delimeter_found = true;
					}
					else if ((to_check.at(counter) == '+') && (!only_integer) && (!plus_found) && (counter < (to_check.length() - 1)))
					{
						plus_found = true;
					}
					else if ((to_check.at(counter) == 'e') && (!only_integer) && (!e_found) && (counter < (to_check.length() - 1)))
					{
						e_found = true;
					}
					else if ((to_check.at(counter) == '-') && (!only_positive) && (counter == 0))
					{
						negative = true;
					}
					else
					{
						result = false;
					}
				}
				counter++;
			}
		}
		else
		{
			result = false;
		}

		return result;
	}

	const int header_padding = 19;
	const int value_padding = 30;
	const int value_city_padding = 7;

	static string padString(string to_pad, int padding, bool front = false)
	{
		string helper = "";
		int number_of_spaces = padding - to_pad.length();
		while (number_of_spaces > 0)
		{
			helper += " ";
			number_of_spaces--;
		}
		if (front)
		{
			helper = helper + to_pad;
		}
		else
		{
			helper = to_pad + helper;
		}
		return helper;
	}

	static void printComm(string comm, bool pause = false)
	{
		if (!Misc::emptyString(comm))
		{
			cout << comm << endl << endl;

			if (pause)
				system("pause");
		}
	}
};


