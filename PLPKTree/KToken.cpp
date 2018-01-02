#include "IncludeHeader.h"
#include "KToken.h"


Token::Token()
{
	this->type = NULL;
	this->value = NULL;
}


Token::~Token()
{
	if (this->type != NULL)
	{
		delete type;
	}
	if (this->value != NULL)
	{
		delete value;
	}
}

char * Token::GetTokenType()
{
	return this->type;
}

char * Token::GetTokenValue()
{
	return this->value;
}

void Token::SetTokenType(char* type)
{
	this->type = (char*)malloc(MAX_TOKEN_SIZE);
	strcpy(this->type, type);
}

void Token::SetTokenValue(char * value)
{
	this->value = (char*)malloc(MAX_TOKEN_SIZE);
	strcpy(this->value, value);
}
