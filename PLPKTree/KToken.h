#pragma once
#include "Expression.h"
class Token
{
public:
	Token();
	~Token();

	char* GetTokenType();
	char* GetTokenValue();
	void SetTokenType(char* type);
	void SetTokenValue(char* value);
private:
	char* type;
	char* value;
};

