#pragma once
using namespace std;

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace Helpers
{
	template<class T>
	static string numberToString(T x)
	{
		stringstream stream;
		stream << x;
		return stream.str();
	}

	static int stringToInt(string to_convert)
	{
		return stoi(to_convert);
	}

	static double stringToDouble(string to_convert)
	{
		return stod(to_convert);
	}

	template<>
	static string numberToString(double x)
	{
		stringstream stream;
		stream << fixed << setprecision(2) << x;
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

	static void printComm(string comm, bool pause = false)
	{
		if (!Helpers::emptyString(comm))
		{
			cout << comm << endl << endl;

			if (pause)
				system("pause");
		}
	}
};


