#pragma once
#include "IncludeHeader.h"
#include "Expression.h"

class KList : public Expression
{
public:
	KList();
	~KList();

	void SetChildrenCount();
};

