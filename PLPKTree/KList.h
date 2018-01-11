#ifndef KLIST_H
#define KLIST_H

#include "IncludeHeader.h"
#include "Expression.h"

class KList : public Expression
{
public:
	KList();
	~KList();

	void SetChildrenCount();
};

#endif
