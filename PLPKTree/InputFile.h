#pragma once
#include "Input.h"
class InputFile :
	public Input
{
public:
	InputFile();
	~InputFile();

	void ReadFile(const char* filepath);
	KToken* CreateToken();
	Expression* CreateExpression();
	KList* CreateKList();
public:
	FILE * fp;
	int file_size;
};

