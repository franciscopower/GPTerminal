#pragma once

#include <string>
#include <vector>

#include "Component.h"

class Frame : Component
{
public:

	Frame(std::string content);
	Frame();

	void setContent(std::string content);

	std::string draw();

private:

	std::string content;
	std::string title;

    std::vector<std::string> split(std::string s, std::string delimiter);
};

