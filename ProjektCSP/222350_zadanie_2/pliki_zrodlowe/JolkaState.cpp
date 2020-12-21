#include "pch.h"
#include "JolkaState.h"


JolkaState::JolkaState(int id, int row_len, int col_len, vector<string> words)
{
	this->id = id;
	this->words = words;
	this->row_len = row_len;
	this->col_len = col_len;

	this->visited_nodes = 0;
	this->visited_to_first_solution = -1;
	this->returns = 0;
	this->returns_to_first_solution = -1;
	this->solutions = 0;
}

JolkaState::~JolkaState()
{
	for (int i = 0; i < nodes.size(); i++)
	{
		delete nodes.at(i);
	}
	nodes.clear();
}

string JolkaState::solve(string algorithm, string heuristics_variable, string heuristic_value)
{
	time_t start, end;

	string result = "Solutions: \n";
	this->algorithm = algorithm;
	this->heuristic_variable = heuristics_variable;
	this->heuristic_value = heuristic_value;

	start = clock();

	ios_base::sync_with_stdio(false);

	result += runAlgorithm("");

	end = clock();

	time_taken = double(end - start) / double(CLOCKS_PER_SEC);

	result += getJolkaStatsString();
	return result;
}

string JolkaState::runAlgorithm(string winning_solutions)
{
	JolkaNode* currentNode = getNode();

	if (currentNode == NULL)
	{
		solutions++;
		winning_solutions += getJolkaString() + "\n\n";
		if (visited_to_first_solution == -1)
		{
			returns_to_first_solution = returns;
			visited_to_first_solution = visited_nodes;
		}
	}
	else
	{
		vector<string> temp_domains = currentNode->domains;
		if (heuristic_value.compare("random") == 0)
		{
			random_shuffle(temp_domains.begin(), temp_domains.end());
		}

		for (int i = 0; i < currentNode->domains.size(); i++)
		{
			currentNode->value = temp_domains.at(i);
			visited_nodes++;

			if (checkConstraints(currentNode))
			{
				currentNode->defined = true;

				if (algorithm.compare("forward") == 0)
				{
					bool empty_found = false;
					vector<pair<int, vector<string>>> filtered;
					filterConstrainedDomains(currentNode, &empty_found, &filtered);

					if (!empty_found)
					{
						winning_solutions = runAlgorithm(winning_solutions);
					}

					unfilterConstrainedDomains(currentNode, &filtered);
				}
				else if (algorithm.compare("back") == 0)
				{
					winning_solutions = runAlgorithm(winning_solutions);
				}

				returns++;

				currentNode->defined = false;
			}
		}
	}

	return winning_solutions;
}

void JolkaState::filterConstrainedDomains(JolkaNode* node, bool* empty_found, vector<pair<int, vector<string>>>* filtered)
{
	int counter = 0;
	while ((counter < node->constraints.size()) && (!(*empty_found)))
	{
		vector<string> old_domains = node->constraints.at(counter)->filterDomains(node->value);

		if (old_domains.size()!=0)
		{
			int indeks = counter;
			filtered->push_back(make_pair(indeks, old_domains));
			if (node->constraints.at(counter)->constrained_node->domains.size() == 0)
			{
				*empty_found = true;
			}
		}
		counter++;
	}
}

void JolkaState::unfilterConstrainedDomains(JolkaNode* node, vector<pair<int, vector<string>>>* filtered)
{
	for (int i = 0; i < filtered->size(); i++)
	{
		int index = filtered->at(i).first;
		vector<string> old_domains = filtered->at(i).second;
		node->constraints.at(index)->unfilterDomains(old_domains);
	}
}

bool JolkaState::checkConstraints(JolkaNode* node)
{
	string value = node->value;
	bool constraint_broken = false;

	int counter = 0;
	while ((!constraint_broken) && (counter < node->constraints.size()))
	{
		JolkaConstraint* constraint = node->constraints.at(counter);
		if (constraint->constrained_node->defined)
		{
			char conflicting_char = value.at(constraint->constrained_pos2);
			char conflicting_char_2 = constraint->constrained_node->value.at(constraint->constrained_pos1);

			if (conflicting_char != conflicting_char_2)
			{
				constraint_broken = true;
			}
		}
		counter++;
	}
	return !constraint_broken;
}

JolkaNode* JolkaState::getNode()
{
	JolkaNode* currentNode = NULL;

	if (heuristic_variable.compare("as_declared") == 0)
	{
		currentNode = getNextNode();
	}
	else if (heuristic_variable.compare("most_constrained") == 0)
	{
		currentNode = getMostConstrainedNode();
	}
	return currentNode;
}

JolkaNode* JolkaState::getNextNode()
{
	JolkaNode* node = NULL;
	int i = 0;

	while ((i < nodes.size())&&(node == NULL))
	{
		if (!(nodes.at(i)->defined))
		{
			node = nodes.at(i);
		}
		i++;
	}

	return node;
}

JolkaNode* JolkaState::getMostConstrainedNode()
{
	int record = -1;
	JolkaNode* node = NULL;

	for (int i = 0; i < nodes.size(); i++)
	{
		if (!(nodes.at(i)->defined))
		{
			int count = nodes.at(i)->countDefinedConstraints();

			if (count > record)
			{
				record = count;
				node = nodes.at(i);
			}
		}
	}

	return node;
}

string JolkaState::getJolkaStatsString()
{
	string to_return = "ID: [" + Helpers::numberToString(id) + "]\n";
	to_return += "Algorithm: [" + algorithm + "]\n";
	to_return += "Variable heur: [" + heuristic_variable + "]\n";
	to_return += "Value heur: [" + heuristic_value + "]\n";
	to_return += "Solutions: [" + Helpers::numberToString(solutions) + "]\n";
	to_return += "Visited Nodes: [" + Helpers::numberToString(visited_nodes) + "]\n";
	to_return += "Visited Nodes To First: [" + Helpers::numberToString(visited_to_first_solution) + "]\n";
	to_return += "Returns: [" + Helpers::numberToString(returns) + "]\n";
	to_return += "Returns To First: [" + Helpers::numberToString(returns_to_first_solution) + "]\n";
	to_return += "Exec Time: [" + Helpers::numberToString(time_taken) + "s]\n";

	return to_return;
}

string JolkaState::getJolkaNodesString()
{
	string to_return = "";
	for (int i = 0; i < nodes.size(); i++)
	{
		to_return += "NODE [" + Helpers::numberToString(i) + "]\n" + nodes.at(i)->getNodeString();
	}
	return to_return;
}

string JolkaState::getJolkaString()
{
	string placeholder_line = "";
	for (int i = 0; i < row_len; i++)
	{
		placeholder_line += "#";
	}

	vector<string> to_return;
	to_return.resize(col_len, placeholder_line);

	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes.at(i)->defined)
		{
			string value = nodes.at(i)->value;
			int length = nodes.at(i)->length;
			bool vertical = nodes.at(i)->vertical;

			for (int j = 0; j < length; j++)
			{
				int x = nodes.at(i)->x;
				int y = nodes.at(i)->y;
				if (vertical)
				{
					y += j;
				}
				else
				{
					x += j;
				}
				to_return.at(y).at(x) = value.at(j);
			}
		}
	}
	string final_string = "";
	for (int i = 0; i < to_return.size(); i++)
	{
		final_string += to_return.at(i)+"\n";
	}

	return final_string;
}

//STATIC CREATORS
void JolkaState::createVerticalNodes(JolkaState* jolka_state, vector<string> data, vector<string> words)
{
	int row_len = data.at(0).size();

	int first_vertical = jolka_state->nodes.size();

	for (int i = 0; i < row_len; i++)
	{
		int new_node_len = 0;
		int x = i;
		int y = 0;
		for (int j = 0; j < data.size(); j++)
		{
			if (data.at(j).at(i) != '#')
			{
				new_node_len++;
			}
			else
			{
				if (new_node_len > 1)
				{
					JolkaNode* new_node = new JolkaNode(x, y, new_node_len, true);
					jolka_state->nodes.push_back(new_node);
					createConstraints(jolka_state, new_node, first_vertical);
					fillDomains(new_node, words);
				}
				y = j + 1;
				new_node_len = 0;
			}
		}
		if (new_node_len > 1)
		{
			JolkaNode* new_node = new JolkaNode(x, y, new_node_len, true);
			jolka_state->nodes.push_back(new_node);
			createConstraints(jolka_state, new_node, first_vertical);
			fillDomains(new_node, words);
		}
	}
}

void JolkaState::createConstraints(JolkaState* jolka_state, JolkaNode* new_node, int first_vertical)
{
	for (int i = 0; i < first_vertical; i++)
	{
		pair<JolkaConstraint*, JolkaConstraint*> constraints = JolkaConstraint::createConstraint(new_node, jolka_state->nodes.at(i));
		if (constraints.first != NULL)
		{
			new_node->constraints.push_back(constraints.first);
			jolka_state->nodes.at(i)->constraints.push_back(constraints.second);
		}
	}
}

void JolkaState::fillDomains(JolkaNode* new_node, vector<string> words)
{
	for (int i = 0; i < words.size(); i++)
	{
		if (words.at(i).length() == new_node->length)
		{
			new_node->domains.push_back(words.at(i));
		}
	}
}

void JolkaState::createHorizontalNodes(JolkaState* jolka_state, vector<string> data, vector<string> words)
{
	int row_len = data.at(0).size();

	for (int i = 0; i < data.size(); i++)
	{
		int new_node_len = 0;
		int x = 0;
		int y = i;
		for (int j = 0; j < row_len; j++)
		{
			if (data.at(i).at(j) != '#')
			{
				new_node_len++;
			}
			else
			{
				if (new_node_len > 1)
				{
					JolkaNode* new_node = new JolkaNode(x, y, new_node_len);
					jolka_state->nodes.push_back(new_node);
					fillDomains(new_node, words);
				}
				x = j + 1;
				new_node_len = 0;
			}
		}
		if (new_node_len > 1)
		{
			JolkaNode* new_node = new JolkaNode(x, y, new_node_len);
			jolka_state->nodes.push_back(new_node);
			fillDomains(new_node, words);
		}
	}
}

JolkaState* JolkaState::createJolka(int id, vector<string> data, vector<string> words)
{
	int row_len = data.at(0).length();
	int col_len = data.size();

	JolkaState* jolka_state = new JolkaState(id, row_len, col_len, words);

	createHorizontalNodes(jolka_state, data, words);
	createVerticalNodes(jolka_state, data, words);

	return jolka_state;
}