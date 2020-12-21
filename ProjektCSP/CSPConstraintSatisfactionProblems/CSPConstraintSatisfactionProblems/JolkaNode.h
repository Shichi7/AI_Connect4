#pragma once
#include <vector>;
#include "helpers.h";
#include <algorithm>
#include "JolkaConstraint.h";

class JolkaNode
{
public:
	JolkaNode(int x, int y, int length, bool vertical = false);
	~JolkaNode();

	string value;
	bool defined;
	bool vertical;
	int x;
	int y;
	int length;

	int countDefinedConstraints();

	vector<string> domains;
	vector<JolkaConstraint*> constraints;

	string getNodeString();
};

