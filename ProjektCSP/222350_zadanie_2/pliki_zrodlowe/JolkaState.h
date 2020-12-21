#pragma once
#include <string>
#include <vector>
#include "JolkaNode.h"
#include "helpers.h"

using namespace std;
#include "JolkaConstraint.h"

class JolkaState
{
public:
	JolkaState(int id, int row_len, int col_len, vector<string> words);
	~JolkaState();

	static JolkaState* createJolka(int id, vector<string> data, vector<string> words);
	static void createVerticalNodes(JolkaState* jolka_state, vector<string> data, vector<string> words);
	static void createHorizontalNodes(JolkaState* jolka_state, vector<string> data, vector<string> words);
	static void createConstraints(JolkaState* jolka_state, JolkaNode* new_node, int first_vertical);
	static void fillDomains(JolkaNode* new_node, vector<string> words);

	string heuristic_variable;
	string heuristic_value;
	string algorithm;

	string solve(string algorithm, string heuristics_variable, string heuristic_value);
	string getJolkaStatsString();

	string runAlgorithm(string winning_solutions);

	JolkaNode* getNode();
	JolkaNode* getNextNode();
	JolkaNode* getMostConstrainedNode();

	void filterConstrainedDomains(JolkaNode* node, bool* empty_found, vector<pair<int, vector<string>>>* filtered);
	void unfilterConstrainedDomains(JolkaNode* node, vector<pair<int, vector<string>>>* filtered);

	bool checkConstraints(JolkaNode* node);

	string getJolkaNodesString();
	string getJolkaString();

	int solutions;
	int returns;
	int returns_to_first_solution;
	int visited_nodes;
	int visited_to_first_solution;
	double time_taken;

	int id;
	int row_len;
	int col_len;

	vector<string> words;
	vector<JolkaNode*> nodes;

};

