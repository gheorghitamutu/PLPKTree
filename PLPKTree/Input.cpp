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

bool Input::ParseInput()
{
	for (buffer_index = 0; buffer_index < this->size_input; buffer_index++)
	{
		switch (this->buffer_input[buffer_index])
		{
		case '#':
			AddToken();
			break;
		default:
			break;
		}
	}
	return true;
}
