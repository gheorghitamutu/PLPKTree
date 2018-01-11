#ifndef INPUTFILE_H
#define INPUTFILE_H

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

#endif
