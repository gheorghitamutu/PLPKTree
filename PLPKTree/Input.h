#pragma once
#include "IncludeHeader.h"
#include "KToken.h"

class Input
{
public:
	Input();
	~Input();
	virtual bool AddToken() = 0;
	bool ParseInput();

public:
	char* buffer_input;
	int buffer_index;
	int size_input;
	std::vector<Token*> tokens;
};

 