#include "IncludeHeader.h"
#include "KToken.h"


KToken::KToken()
{
	this->type = NULL;
	this->value = NULL;
	SetIdentifier('#');
	SetExpressionName("KToken");
}


KToken::~KToken()
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

char * KToken::GetTokenType()
{
	return this->type;
}

char * KToken::GetTokenValue()
{
	return this->value;
}

void KToken::SetTokenType(char* type)
{
	this->type = (char*)malloc(MAX_TOKEN_SIZE);
	strcpy(this->type, type);
}

void KToken::SetTokenValue(char * value)
{
	this->value = (char*)malloc(MAX_TOKEN_SIZE);
	strcpy(this->value, value);
}

KToken KToken::operator=(KToken rhs)
{
	if (this != &rhs)
	{
		this->type = rhs.GetTokenType();
		this->value = rhs.GetTokenValue();
	}
	
	return *this;
}
