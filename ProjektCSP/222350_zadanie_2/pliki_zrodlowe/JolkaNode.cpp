#include "pch.h"
#include "JolkaNode.h"


JolkaNode::JolkaNode(int x, int y, int length, bool vertical)
{
	this->x = x;
	this->y = y;
	this->length = length;
	this->vertical = vertical;

	defined = false;
	value = "";
}


JolkaNode::~JolkaNode()
{
	for (int i = 0; i < constraints.size(); i++)
	{
		delete constraints.at(i);
	}
}


string JolkaNode::getNodeString()
{
	string to_return;
	if (defined)
	{
		to_return = "value: [" + value +"]\n";
	}

	string orientation = vertical ? "vertical" : "horizontal";

	to_return += "orientation: [" + orientation + "]/";
	to_return += "start: [" + Helpers::numberToString(x) +", "+ Helpers::numberToString(y) +"]/";
	to_return += "length: [" + Helpers::numberToString(length) + "]\n";
	to_return += "domains: ";
	for (int i = 0; i < domains.size(); i++)
	{
		to_return += "[" + domains.at(i) + "]";
	}

	to_return += "\nconstraints:\n";

	for (int i = 0; i < constraints.size(); i++)
	{
		JolkaConstraint* constraint = constraints.at(i);
		string constrained_orientation = constraint->constrained_node->vertical ? "vertical" : "horizontal";
		to_return += "     constrained at ["+Helpers::numberToString(constraint->constrained_pos2)+"] with [" + Helpers::numberToString(constraint->constrained_node->x) +", "+ Helpers::numberToString(constraint->constrained_node->y)+ ", "+ constrained_orientation +  "] at ["+ Helpers::numberToString(constraint->constrained_pos1) +"]\n";
	}
	return to_return;
}

int JolkaNode::countDefinedConstraints()
{
	int count = 0;
	for (int i = 0; i < constraints.size(); i++)
	{
		if (constraints.at(i)->constrained_node->defined)
		{
			count++;
		}
	}
	return count;
}
