#include "pch.h"
#include "JolkaConstraint.h"
#include "JolkaNode.h"

JolkaConstraint::JolkaConstraint(JolkaNode* constrained_node, int pos1, int pos2)
{
	this->constrained_node = constrained_node;
	constrained_pos1 = pos1;
	constrained_pos2 = pos2;
}


JolkaConstraint::~JolkaConstraint()
{
}

pair<JolkaConstraint*, JolkaConstraint*> JolkaConstraint::createConstraint(JolkaNode* node1, JolkaNode* node2)
{
	JolkaConstraint* constraint1 = NULL;
	JolkaConstraint* constraint2 = NULL;

	JolkaNode* vertical = node1;
	JolkaNode* horizontal = node2;

	if (node2->vertical)
	{
		vertical = node2;
		horizontal = node1;
	}
	
	for (int i = 0; i < horizontal->length; i++)
	{
		for (int j = 0; j < vertical->length; j++)
		{
			int horizontal_x = horizontal->x + i;
			int horizontal_y = horizontal->y;

			int vertical_x = vertical->x;
			int vertical_y = vertical->y + j;
			
			if ((horizontal_x == vertical_x) && (horizontal_y == vertical_y))
			{
				constraint1 = new JolkaConstraint(horizontal, i, j);
				constraint2 = new JolkaConstraint(vertical, j, i);
				if (node2->vertical)
				{
					JolkaConstraint* temp = constraint1;
					constraint1 = constraint2;
					constraint2 = temp;
				}
			}
		}
	}

	return make_pair(constraint1, constraint2);
}

vector<string> JolkaConstraint::filterDomains(string value)
{
	bool filtered = false;
	bool defined = constrained_node->defined;
	vector<string> old_domains = constrained_node->domains;

	if (!defined)
	{
		int counter = 0;
		int internal_counter = 0;
		while (counter < old_domains.size())
		{
			if (constrained_node->domains.at(internal_counter).at(constrained_pos1) != value.at(constrained_pos2))
			{
				filtered = true;
				constrained_node->domains.erase(constrained_node->domains.begin() + internal_counter);
			}
			else
			{
				internal_counter++;
			}
			counter++;
		}
	}

	if (!filtered)
	{
		old_domains.clear();
	}

	return old_domains;
}

void JolkaConstraint::unfilterDomains(vector<string> old_domains)
{
	constrained_node->domains = old_domains;
	//sort(domains.begin(), domains.end());
}

