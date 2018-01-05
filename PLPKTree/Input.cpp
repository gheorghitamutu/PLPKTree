#include "IncludeHeader.h"
#include "Input.h"

Input::Input()
{
	this->buffer_input = NULL;
}


Input::~Input()
{
	if (this->buffer_input != NULL)
	{
		delete this->buffer_input;
	}
}

void Input::ParseTree()
{
	this->tree.ParseTree();
}

bool Input::ParseInput()
{
	for (buffer_index = 0; buffer_index < this->size_input; buffer_index++)
	{
		switch (this->buffer_input[buffer_index])
		{
		case '#':
			tree.AddBranch(CreateToken());
			break;
		case '`':
			tree.AddBranch(CreateExpression());
			break;
		default:
			break;
		}
	}

	tree.ResetCurrentExp();
	return true;
}

std::vector<std::vector<Expression*>> Input::GetTree()
{
	return tree.GetTree();
}
