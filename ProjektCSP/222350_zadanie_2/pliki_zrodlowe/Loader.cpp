#include "pch.h"
#include "Loader.h"


Loader::Loader()
{
}


Loader::~Loader()
{
}


string Loader::loadSudokuByID(string filepath, int id)
{
	string loaded_string = "";
	ifstream file_stream(filepath);

	if (file_stream.is_open())
	{
		int i = 0;
		bool finished = false;
		while (!finished)
		{
			getline(file_stream, loaded_string);
			if (i == id)
			{
				finished = true;
			}
			i++;
		}
		file_stream.close();
	}

	return loaded_string;
}

void Loader::loadJolkaDataByID(vector<string>* data_lines, string filebasepath, int id)
{
	string filepath = filebasepath + Helpers::numberToString(id);
	string loaded_string = "";
	ifstream file_stream(filepath);

	if (file_stream.is_open())
	{
		int i = 0;
		bool finished = false;
		string loaded_string;
		while (getline(file_stream, loaded_string))
		{
			data_lines->push_back(loaded_string);
		}
		file_stream.close();
	}
}
