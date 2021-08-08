#pragma once
#include <string>
class Option
{
private:

	std::string optionString;

public:

	// Write in the option here.
	Option(std::string&& specification);
	~Option();
};