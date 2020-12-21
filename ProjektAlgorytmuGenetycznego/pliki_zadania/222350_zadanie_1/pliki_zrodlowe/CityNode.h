#pragma once
#include <iostream>
#include "Misc.h"
#include <vector>

class CityNode
{
public:
	CityNode(int id, double x, double y, int cities_count);
	~CityNode();

	int id;
	double x;
	double y;

	double* distances;
	int* distances_ranking;
	int cities_count;
	int in_ranking;

	string toString();
	void setDistance(CityNode* distanced_city);
	void setDistance(CityNode* distanced_city, double value);
	double getDistance(int distanced_city_id);

private:
	void putDistanceInRanking(int id, double value);
};

