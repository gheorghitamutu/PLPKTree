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
	fseek(fp, 0, SEEK_SET);

	this->buffer_input = (char*)malloc(this->file_size);
	if (this->buffer_input == NULL)
	{
		exit(1);
	}

	this->buffer_index = 0;
	while ((this->buffer_input[this->buffer_index] = getc(fp)) != EOF)
	{
		this->buffer_index++;
	}
	this->buffer_index = 0;

	fclose(fp);
}

bool InputFile::AddToken()
{
	int index_token_member = 0;

	bool save = false;
	char* temp_param = NULL;
	int temp_param_index;

	while (this->buffer_input[this->buffer_index] != ')' &&
		this->buffer_index < this->file_size)
	{
		if (this->buffer_input[this->buffer_index] == '\"')
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
					tokens.emplace_back(new Token);
				}

				this->buffer_index++;
				continue;
			}
			else
			{
				temp_param[temp_param_index] = 0;
				printf("%s\n", temp_param);

				if (index_token_member == 0)
				{
					tokens.back()->SetTokenValue(temp_param);
					index_token_member++;
				}
				else
				{
					tokens.back()->SetTokenType(temp_param);
					index_token_member = 0;
				}
				delete temp_param;
				temp_param = NULL;
			}
		}

		if (save)
		{
			temp_param[temp_param_index] = this->buffer_input[this->buffer_index];
			temp_param_index++;
		}

		//printf("%c", this->buffer_input[this->buffer_index]); // printing token
		this->buffer_index++;
	}


	//printf("%c\n", this->buffer_input[this->buffer_index]);  // printing token

	return true;
}


InputFile::~InputFile()
{
	if (this->fp != NULL)
	{
		fclose(fp);
	}
}
