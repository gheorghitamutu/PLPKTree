#include "IncludeHeader.h"
#include "InputFile.h"


InputFile::InputFile()
{
	this->fp = NULL;
}

void InputFile::ReadFile(const char * filepath)
{
	fp = fopen(filepath, "r");
	if (fp == NULL)
	{
		exit(1);
	}

	fseek(fp, 0L, SEEK_END);
	this->file_size = ftell(fp);
	this->size_input = this->file_size;
	if (this->file_size <= 2)
	{
		exit(1); // can t be a kast tree with such a small size
	}
	fseek(fp, 0, SEEK_SET);

	this->buffer_input = (char*)malloc(this->file_size + 1); // + 1 - fix warning for buffer overflow
	if (this->buffer_input == NULL)
	{
		exit(1);
	}

	for(this->buffer_index = 0; this->buffer_index < this->size_input; this->buffer_index++)
	{
		this->buffer_input[this->buffer_index] = getc(fp);
	}
	this->buffer_input[this->buffer_index] = '/0'; // make sure that all the methods you'll use won t access bad memory
	this->buffer_index = 0;

	fclose(fp);
	fp = NULL; // bug fix in destructor
}

KToken* InputFile::CreateToken()
{
	int index_token_member = 0;

	bool save = false;
	char* temp_param = NULL;
	int temp_param_index;

	KToken* ktoken = NULL;

	while (this->buffer_input[this->buffer_index] != ')' &&
		this->buffer_index < this->file_size) // check against buffer size also
	{
		if (this->buffer_input[this->buffer_index] == '\"' && 
			this->buffer_input[this->buffer_index - 1] != '\\') // fix escaped characters bug
		{
			save = !save;

			if (temp_param == NULL)
			{
				temp_param = (char*)malloc(MAX_TOKEN_SIZE);
				if (temp_param == NULL)
				{
					exit(1);
				}

				temp_param_index = 0;

				if (index_token_member == 0)
				{
					ktoken = new KToken;
				}

				this->buffer_index++;
				continue;
			}
			else
			{
				temp_param[temp_param_index] = 0;

				if (index_token_member == 0)
				{
					ktoken->SetTokenValue(temp_param);
					index_token_member++;
				}
				else
				{
					ktoken->SetTokenType(temp_param);
					index_token_member = 0;
				}
				free(temp_param);
				temp_param = NULL;
			}
		}

		if (save)
		{
			temp_param[temp_param_index] = this->buffer_input[this->buffer_index];
			temp_param_index++;
		}

		this->buffer_index++;
	}

	return ktoken;
}

Expression * InputFile::CreateExpression()
{
	Expression* expression = NULL;

	this->buffer_index++; // jump over first `

	char* temp_param = (char*)malloc(MAX_EXPRESSION_NAME_SIZE);
	if (temp_param == NULL)
	{
		exit(1);
	}
	memset(temp_param, 0, MAX_EXPRESSION_NAME_SIZE);

	int temp_param_index = 0;

	while (this->buffer_input[this->buffer_index] != '`' && 
		this->buffer_index < this->file_size) // check against buffer size also
	{
		temp_param[temp_param_index] = this->buffer_input[this->buffer_index];

		temp_param_index++;
		this->buffer_index++;
	}

	expression = new Expression;
	expression->SetExpressionName(temp_param);
	expression->SetChildrenCount();

	free(temp_param);

	return expression;
}

KList * InputFile::CreateKList()
{
	KList* k_list = NULL;

	this->buffer_index++; // jump over first `

	char* temp_param = (char*)malloc(MAX_EXPRESSION_NAME_SIZE);
	if (temp_param == NULL)
	{
		exit(1);
	}
	memset(temp_param, 0, MAX_EXPRESSION_NAME_SIZE);

	int temp_param_index = 0;

	while (this->buffer_input[this->buffer_index] != ')' &&
		this->buffer_index < this->file_size) // check against buffer size also
	{
		temp_param[temp_param_index] = this->buffer_input[this->buffer_index];

		temp_param_index++;
		this->buffer_index++;
	}

	k_list = new KList;
	k_list->SetExpressionName(temp_param);
	k_list->SetChildrenCount();

	free(temp_param);

	return k_list;
}


InputFile::~InputFile()
{
	if (this->fp != NULL)
	{
		fclose(fp);
	}
}
