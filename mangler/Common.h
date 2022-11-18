#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <algorithm>
#include <regex>
#include <sstream>

bool startswith(std::string input, std::string compar);

bool requiresparentheses(std::string name);

std::string & removeparentheses(std::string & input);

std::string & removequotes(std::string & input);

std::string & stolower(std::string & input);

std::string & ltrim(std::string & input);

std::string & rtrim(std::string & input);

std::string & trim(std::string & input);

std::string sclone(std::string* input);

inline std::string FloatToString(float f) {// HACK: scrappy, remove or put elsewhere
	std::stringstream ss;
	ss << f;
	return ss.str();
}

#endif