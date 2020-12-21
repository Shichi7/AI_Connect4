#pragma once
#include <utility>
#include <vector>
#include <string>
using namespace std;

class JolkaNode;

class JolkaConstraint
{
public:
	JolkaConstraint(JolkaNode* constrained_node, int pos1, int pos2);
	~JolkaConstraint();

	static pair<JolkaConstraint*, JolkaConstraint*> createConstraint(JolkaNode* node1, JolkaNode* node2);

	JolkaNode* constrained_node;

	int constrained_pos1;
	int constrained_pos2;

	vector<string> filterDomains(string value);
	void unfilterDomains(vector<string> old_domains);
};

