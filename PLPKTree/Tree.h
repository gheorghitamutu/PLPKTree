#pragma once
#include "Expression.h"

class Tree
{
public:
	Tree();
	~Tree();
	void AddBranch(Expression* expression);
	void ParseTree(); // BFS
	void ResetCurrentExp();
private:
	Expression* current_exp;
	Expression* root_exp;
	std::vector<Expression*> nodes_to_visit; // use for BFS
};

