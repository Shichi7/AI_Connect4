#include "stdafx.h"
#include "CityNode.h"
#include <algorithm>  

using namespace std;

CityNode::CityNode(int id, double x, double y, int cities_count)
{
	this->id = id;
	this->x = x;
	this->y = y;
	this->cities_count = cities_count;

	in_ranking = 0;

	distances = new double[cities_count];
	distances_ranking = new int[cities_count - 1];

	for (unsigned int i = 0; i < (cities_count - 1); i++)
	{
		distances_ranking[i] = 0;
	}

	distances[id - 1] = 0;
}

CityNode::~CityNode()
{
	delete distances;
	delete distances_ranking;
}

string CityNode::toString()
{
	return Misc::padString("ID: [" + Misc::numberToString(id) + "]", 15) + Misc::padString("POS: [" + Misc::numberToString(x) + ", " + Misc::numberToString(y) + "]", 25);
}

void CityNode::setDistance(CityNode* distanced_city)
{
	int array_index = distanced_city->id - 1;

	double x1 = distanced_city->x;
	double y1 = distanced_city->y;

	double x2 = this->x;
	double y2 = this->y;

	double distance = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));

	distances[array_index] = distance;
	putDistanceInRanking(distanced_city->id, distance);
}

void CityNode::putDistanceInRanking(int id, double value)
{
	bool done = false;
	unsigned int counter = 0;
	while ((!done)&&(counter<cities_count))
	{
		int distance_array_key = distances_ranking[counter] - 1;
		if (distances_ranking[counter] == 0)
		{
			distances_ranking[counter] = id;
			done = true;
		}
		else if (value < distances[distance_array_key])
		{
			for (unsigned i = in_ranking; i > counter; i--)
			{
				distances_ranking[i] = distances_ranking[i - 1];
			}
			distances_ranking[counter] = id;
			done = true;
		}

		counter++;
	}
	in_ranking++;
}

void CityNode::setDistance(CityNode* distanced_city, double value)
{
	distances[distanced_city->id - 1] = value;
	putDistanceInRanking(distanced_city->id, value);
}

double CityNode::getDistance(int distanced_city_id)
{
	return distances[distanced_city_id - 1];
}


