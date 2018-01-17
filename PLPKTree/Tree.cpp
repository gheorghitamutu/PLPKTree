#include "Tree.h"

Tree::Tree()
{
	this->current_exp = NULL;
}


Tree::~Tree()
{
	// not deleting anymore any expressions as they are used later after this class is deleted
}

void Tree::AddBranch(Expression* expression)
{
	if (this->current_exp == NULL) // if no root make root
	{
		this->root_exp = expression;
		this->current_exp = expression;
	}
	else // if root exists
	{
		if (this->current_exp->GetFreeChildrenCount() == 0) // if exp has no children nodes left
		{
			if (this->current_exp->GetFather() == NULL) // if this is root kast tree is invalid
			{
				printf("Invalid kast tree!\n");
			}
			else // go up one level in the tree
			{
				Expression* temp_exp = this->current_exp->GetFather();
				this->current_exp = temp_exp;
				AddBranch(expression);
			}
		}
		else
		{
			this->current_exp->AddChild(expression);
			this->current_exp = expression;
		}
	}
}

void Tree::ParseTree()
{
	if (this->root_exp == NULL)
	{
		printf("Empty Tree!\n");
	}
	else
	{
		if (this->current_exp == NULL)
		{
			this->current_exp = this->root_exp;
			nodes_to_visit.emplace_back(this->current_exp);
		}
		
        int v_size = nodes_to_visit.size();
		while (nodes_to_visit.size() > 0)
		{
			for (auto exp : nodes_to_visit)
			{
				printf("%s ", exp->GetExpressionName());
			}

			printf("\n");

			std::vector<Expression*> temp_vector;

			for (auto exp : nodes_to_visit)
			{
				for (auto exp_child : exp->GetChildren())
				{
					temp_vector.emplace_back(exp_child);
				}
			}

			this->nodes_to_visit.clear();

			for (auto exp : temp_vector)
			{
				this->nodes_to_visit.emplace_back(exp);
			}

			temp_vector.clear();

			v_size = nodes_to_visit.size();
		}
	}

	this->current_exp = NULL;
}

void Tree::ResetCurrentExp()
{
	this->current_exp = NULL;
}

std::vector<std::vector<Expression*>> Tree::GetTree()
{
	if (this->root_exp == NULL)
	{
		return this->tree;
	}
	else
	{
		std::vector<Expression*> temp_vector;

		if (this->current_exp == NULL)
		{
			this->current_exp = this->root_exp;
			nodes_to_visit.emplace_back(this->current_exp);
		}

		int v_size = nodes_to_visit.size();
		while (nodes_to_visit.size() > 0)
		{
			for (auto exp : nodes_to_visit)
			{
				temp_vector.emplace_back(exp);
			}

			this->tree.emplace_back(temp_vector);
			temp_vector.clear();

			for (auto exp : nodes_to_visit)
			{
				for (auto exp_child : exp->GetChildren())
				{
					temp_vector.emplace_back(exp_child);
				}
			}

			this->nodes_to_visit.clear();

			for (auto exp : temp_vector)
			{
				this->nodes_to_visit.emplace_back(exp);
			}

			temp_vector.clear();

			v_size = nodes_to_visit.size();
		}
	}

	this->current_exp = NULL;

	return this->tree;
}
