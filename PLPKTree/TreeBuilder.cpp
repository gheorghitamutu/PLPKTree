// KFrameworkProject.cpp : Defines the entry point for the console application.
//

#include "IncludeHeader.h"
#include "InputFile.h"

bool BuildTree(const char* file_name)
{
	InputFile input_file;
	input_file.ReadFile(file_name);
	input_file.ParseInput();

    return true;
}

