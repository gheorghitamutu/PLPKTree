#pragma once
#include "Expression.h"
class KToken : public Expression
{
public:
	KToken();
	~KToken();

	char* GetTokenType();
	char* GetTokenValue();
	void SetTokenType(char* type);
	void SetTokenValue(char* value);

	KToken operator=(const KToken rhs);
private:
	char* type;
	char* value;
};

