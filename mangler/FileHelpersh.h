#ifndef FILEHELPERS_H
#define FILEHELPERS_H

#include <string>
#include <algorithm> 
#include <cctype>
#include <locale>

inline bool nullorempty(
	const std::string& input
)
{
	const std::string* ptr = &input;
	if(ptr == nullptr) return true;
	return input.empty();
}

inline const std::string& clone(
	const std::string& input
)
{
	std::string tmp(input);
	return tmp;
}

inline const char* clone(
	const char* cstr
)
{
	std::string a_input = std::string(cstr);
	std::string tmp(a_input);
	return tmp.c_str();
}

inline void split(
	std::string& input, 
	const std::string& delimiter, 
	std::vector<std::string*>* outTokens, 
	bool ignoreEmpty = false
)
{
	std::vector<std::string*>* tokens;
	std::string s = std::string(input);
	size_t pos = 0;
	std::string token;

	tokens = new std::vector<std::string*>();

	while ((pos = s.find(delimiter)) != std::string::npos) 
	{
		token = s.substr(0, pos);
		
		if(ignoreEmpty)
		{
			if(!nullorempty(token))
			{
				tokens->push_back(&token);
			}
		}
		else
		{
			tokens->push_back(&token);
		}

		s.erase(0, pos + delimiter.length());
	}

	outTokens = tokens;
}

inline void remove(
	std::string& input, 
	int startIndex, 
	int endIndex
)
{
	input.erase(input.begin() + startIndex, input.begin() + endIndex);
}

inline void ltrim(
	std::string &s
) 
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
}

inline void rtrim(
	std::string &s
) 
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

inline void trim(
	std::string &s
) 
{
	ltrim(s);
	rtrim(s);
}

inline std::string ltrim_copy(
	const std::string input
) 
{
	std::string s = input;
	ltrim(s);
	return s;
}

inline std::string rtrim_copy(
	const std::string input
) 
{
	std::string s = input;
	rtrim(s);
	return s;
}

inline std::string trim_copy(
	const std::string input
) 
{
	std::string s = input;
	trim(s);
	return s;
}

inline bool startswith(
	const std::string& input, 
	const std::string& prefix)
{
	return (!input.compare(0, prefix.size(), prefix));
}

inline bool startswith(
	const std::string& input, 
	char* cstrPrefix
)
{
	const std::string a_prefix = std::string(cstrPrefix);
	return startswith(input, a_prefix);
}

inline bool startswith(
	const char* input, 
	char* cstrPrefix
)
{
	const std::string a_input = std::string(input);
	const std::string a_prefix = std::string(cstrPrefix);
	return startswith(a_input, a_prefix);
}

inline bool startswith(
	std::string& input, 
	std::string& prefix
)
{
	return (!input.compare(0, prefix.size(), prefix));
}

inline bool int_try_parse(
	std::string& input, 
	int& output
)
{
	try
	{
		output = std::stoi(input.c_str());
		return true;
	}
	catch (const std::invalid_argument& ia)
	{
		return false;
	}
}

inline bool float_try_parse(
	std::string& input, 
	float& output
)
{
	try
	{
		output = std::stof(input.c_str());
		return true;
	}
	catch (const std::invalid_argument& ia)
	{
		return false;
	}
}

#endif