#include "Common.h"

bool startswith(std::string input, std::string compar)
{
	bool starts = true;
	size_t len0 = input.size();
	size_t len1 = compar.size();

	if (len0 > 0 && len1 > 0 && len1 <= len0)
	{
		for (int i = 0; i < len1 && starts; i++)
		{
			if (input[i] != compar[i])
			{
				starts = false;
			}
		}
	}
	else
	{
		starts = false;
	}

	return starts;
}

bool requiresparentheses(std::string name)
{
	return startswith(name, "v4f") || startswith(name, "v3f") || startswith(name, "v2f")
		|| startswith(name, "v2u") || startswith(name, "lst");
}

std::string & removeparentheses(std::string & input)
{
	if (input.empty()) return input;

	if (input[0] == '(')
	{
		input = input.erase(0, 1);

		int last = input.size() - 1;

		if (input[last] == ')')
		{
			input = input.erase(last, 1);
		}
	}

	return input;
}

std::string & removequotes(std::string & input)
{
	if (input.empty()) return input;

	if (input[0] == '\"')
	{
		input = input.erase(0, 1);

		int last = input.size() - 1;

		if (input[last] == '\"')
		{
			input = input.erase(last, 1);
		}
	}

	return input;
}

std::string & stolower(std::string & input)
{
	for (int i = 0; i < input.length(); i++)
	{
		input[i] = tolower(input[i]);
	}
	return input;
}

std::string & ltrim(std::string & input)
{
	auto it2 = std::find_if(input.begin(), input.end(), [](char ch) { return !std::isspace<char>(ch, std::locale::classic()); });
	input.erase(input.begin(), it2);
	return input;
}

std::string & rtrim(std::string & input)
{
	auto	it1 = std::find_if(input.rbegin(), input.rend(), [](char ch) { return !std::isspace<char>(ch, std::locale::classic()); });
	input.erase(it1.base(), input.end());
	return input;
}

std::string & trim(std::string & input)
{
	return ltrim(rtrim(input));
}

std::string sclone(std::string* input)
{
	char* strSection = new char[input->size() + 1];
	strcpy(strSection, input->c_str());
	std::string clone(strSection);
	return clone;
}