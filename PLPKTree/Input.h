#pragma once
#include "IncludeHeader.h"
#include "Expression.h"
#include "Tree.h"
#include "KToken.h"
#include "KList.h"

class Input
{
public:
	Input();
	~Input();
	virtual KToken* CreateToken() = 0;
	virtual Expression* CreateExpression() = 0;
	virtual Expression* CreateKList() = 0;
	void ParseTree();
	bool ParseInput();
	std::vector<std::vector<Expression*>> GetTree();

public:
	char* buffer_input;
	int buffer_index;
	int size_input;
	Tree tree;
};

 