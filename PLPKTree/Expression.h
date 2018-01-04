#pragma once
#include "IncludeHeader.h"
class Expression
{
public:
	Expression();
	void SetIdentifier(char identifier);
	char GetIdentifier();
	void SetChildrenCount();
	int GetFreeChildrenCount();
	Expression* GetCurrentChild();
	Expression* GetFather();
	void SetFather(Expression* expression);
	void SetExpressionName(char* exp_name);
	char* GetExpressionName();
	std::vector<Expression*> GetChildren();
	void AddChild(Expression* expression);
	~Expression();
private:
	char identifier;
	char* exp_name;
	Expression* father;
	std::vector<Expression*> children;
	int count_children;
	int current_child;

};

