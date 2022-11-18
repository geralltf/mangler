#include "GameConfParser.h"


bool ConfParser::GetSection(std::string line, std::string & sectionName)
{
	if (IsComment(line))
	{
		return false;
	}

	std::smatch		matches;
	std::regex		expr("\\[([^\\[\\]]+)\\]");

	std::regex_search(trim(line), matches, expr);

	if (matches.size() == 2)
	{
		sectionName = matches[1];

		stolower(sectionName);

		return true;
	}
	else
	{
		return false;
	}
}

bool ConfParser::IsComment(std::string line)
{
	if (!line.empty())
	{
		std::string		clone = sclone(&line);

		clone = trim(clone);

		if (clone[0] == ';' || clone[0] == '#')
		{
			return true;
		}
	}
	return false;
}

bool ConfParser::GetPair(std::string line, std::string & key, std::string & value)
{
	if (IsComment(line))
	{
		key = "";
		value = "";
		return false;
	}

	char * pch = strtok((char*)line.c_str(), "=");
	if (pch != nullptr)
	{
		key = std::string(pch);
		key = removequotes(trim(key));
		key = stolower(key);
		pch = strtok(NULL, "\0");
	}
	else
	{
		key = "";
		value = "";
		return false;
	}

	if (pch != nullptr)
	{
		value = std::string(pch);
		value = removequotes(trim(value));
		if (requiresparentheses(key))
		{
			value = removeparentheses(value);
		}

		return true;
	}
	else
	{
		value = "";
	}

	return false;
}

void ConfParser::ConfParser_tests()
{
	std::string								sectionName;
	std::string								key, value;

	// Test cases.
	std::string test_comment0 = "; Athena Engine";
	std::string test_comment1 = "# Athena Engine";
	std::string test_comment2 = "   # Athena Engine"; // with spaces
	std::string test_comment3 = "		 # Athena Engine"; // with some tabs
	std::string test_section = "[BUILD1]";
	std::string test_section2 = "       [BUILD2]"; // with spaces
	std::string test_section3 = "					[BUILD3]"; // with some tabs

	std::string test_kp0 = "OUTPUT=\"c:\\bin\\GameTestProv_AE.ini; BINDINGTYPE=CString;\"";
	std::string test_kp1 = "\"Test Key\"=c:\\bin\\GameTestProv_AE.ini; BINDINGTYPE=CString;";
	std::string test_kp2 = "OUTPUT=c:\\bin\\GameTestProv_AE.ini; BINDINGTYPE=CString;";
	std::string test_kp3 = "iResolutionPrimary=2048";
	std::string test_kp4 = "fPostLoadUpdateTimeMS=256.12";
	std::string test_kp5 = "bAllowScreenshot=1";
	std::string test_kp6 = "VERSION = 1.17.0920.5;";
	std::string test_kp7 = "SGeneralWarning=\"One two three four five six seven eight nine ten eleven twelve thirteen\"";
	std::string test_kp8 = "SGeneralError=One two three four five six seven eight nine ten eleven twelve thirteen";

	// Test Assertions.
	assert(ConfParser::GetSection(test_section, sectionName) == true);
	assert(sectionName == "build1");
	assert(ConfParser::GetSection(test_section2, sectionName) == true);
	assert(sectionName == "build2");
	assert(ConfParser::GetSection(test_section3, sectionName) == true);
	assert(sectionName == "build3");

	assert(ConfParser::IsComment(test_comment0) == true);
	assert(ConfParser::IsComment(test_comment1) == true);
	assert(ConfParser::IsComment(test_comment2) == true);
	assert(ConfParser::IsComment(test_comment3) == true);
	assert(ConfParser::IsComment(test_section) == false);

	assert(ConfParser::GetPair(test_kp0, key, value) == true);
	assert(key == "output" && value == "c:\\bin\\GameTestProv_AE.ini; BINDINGTYPE=CString;");
	assert(ConfParser::GetPair(test_kp1, key, value) == true);
	assert(key == "test key" && value == "c:\\bin\\GameTestProv_AE.ini; BINDINGTYPE=CString;");
	assert(ConfParser::GetPair(test_kp2, key, value) == true);
	assert(key == "output" && value == "c:\\bin\\GameTestProv_AE.ini; BINDINGTYPE=CString;");
	assert(ConfParser::GetPair(test_kp3, key, value) == true);
	assert(key == "iresolutionprimary" && value == "2048");
	assert(ConfParser::GetPair(test_kp4, key, value) == true);
	assert(key == "fpostloadupdatetimems" && value == "256.12");
	assert(ConfParser::GetPair(test_kp5, key, value) == true);
	assert(key == "ballowscreenshot" && value == "1");
	assert(ConfParser::GetPair(test_kp6, key, value) == true);
	assert(key == "version" && value == "1.17.0920.5;");
	assert(ConfParser::GetPair(test_kp7, key, value) == true);
	assert(key == "sgeneralwarning" && value == "One two three four five six seven eight nine ten eleven twelve thirteen");
	assert(ConfParser::GetPair(test_kp8, key, value) == true);
	assert(key == "sgeneralerror" && value == "One two three four five six seven eight nine ten eleven twelve thirteen");
	assert(ConfParser::GetPair(test_comment3, key, value) == false);
	assert(ConfParser::GetPair(test_comment0, key, value) == false);
	assert(ConfParser::GetPair(test_section2, key, value) == false);

}