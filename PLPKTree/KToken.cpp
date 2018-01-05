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
	for(auto exp : this->children)
	{
		if (exp != NULL)
		{
			delete exp;
		}
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

	this->count_children++;
	Expression* exp = new Expression;
	exp->SetExpressionName(this->type);
	AddChild(exp);
}

void KToken::SetTokenValue(char * value)
{
	this->value = (char*)malloc(MAX_TOKEN_SIZE);
	strcpy(this->value, value);

	this->count_children++;
	Expression* exp = new Expression;
	exp->SetExpressionName(this->value);
	AddChild(exp);
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
