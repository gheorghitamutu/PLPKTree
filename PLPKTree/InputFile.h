#pragma once
#include "Input.h"
class InputFile :
	public Input
{
public:
	InputFile();
	~InputFile();

	void ReadFile(const char* filepath);
	bool AddToken();
public:
	FILE * fp;
	int file_size;
};

