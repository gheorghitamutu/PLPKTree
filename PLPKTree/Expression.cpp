#include "Expression.h"

Expression::Expression()
{
	this->identifier = '`';
	this->count_children = 0;
	this->current_child = 0;
	this->exp_name = NULL;
	this->father = NULL;
}

void Expression::SetIdentifier(char identifier)
{
	this->identifier = identifier;
}

char Expression::GetIdentifier()
{
	return this->identifier;
}

void Expression::SetChildrenCount()
{
	if (this->exp_name == NULL)
	{
		return;
	}

	if (!strcmp(this->exp_name, "{}"))
	{
		this->count_children = 1;
		return;
	}

	if (strstr(this->exp_name, ".List{"))
	{
		this->count_children = 1;
		return;
	}

	for (int index = 0; index < strlen(this->exp_name); index++)
	{
		if (this->exp_name[index] == '_')
		{
			this->count_children++;
		}
	}
}

int Expression::GetFreeChildrenCount()
{
	return this->count_children - this->current_child;
}

Expression* Expression::GetCurrentChild()
{
	return this->children[current_child];
}

Expression * Expression::GetFather()
{
	return this->father;
}

void Expression::SetFather(Expression * expression)
{
	this->father = expression;
}

void Expression::SetExpressionName(char * exp_name)
{
	this->exp_name = new char[strlen(exp_name)];

	if (this->exp_name == NULL)
	{
		exit(1);
	}

	strcpy(this->exp_name, exp_name);
}

char * Expression::GetExpressionName()
{
	return this->exp_name;
}

std::vector<Expression*> Expression::GetChildren()
{
	return this->children;
}

void Expression::AddChild(Expression * expression)
{
	this->children.emplace_back(expression);
	this->current_child++;

	if (expression->GetFather() != NULL)
	{
		printf("Kast tree is invalid!\n");
		exit(1);
	}
	else
	{
		expression->SetFather(this);
	}
}


Expression::~Expression()
{

}
