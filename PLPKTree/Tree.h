#ifndef TREE_H
#define TREE_H

#include "Expression.h"

class Tree
{
public:
	Tree();
	~Tree();
	void AddBranch(Expression* expression);
	void ParseTree(); // BFS
	void ResetCurrentExp();
	std::vector<std::vector<Expression*>> GetTree();
private:
	Expression* current_exp;
	Expression* root_exp;
	std::vector<Expression*> nodes_to_visit; // use for BFS
	std::vector<std::vector<Expression*>> tree; // use for actual drawing
};

#endif
