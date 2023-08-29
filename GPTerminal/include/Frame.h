#pragma once

#include <string>
#include <vector>

#include "Component.h"


class Frame : Component
{
public:

	Frame(std::string content);

	void setContent(std::string content);

	std::string draw();

private:

	std::string content;
	std::string title;

    const std::string hline     = "\xC4";
    const std::string vline     = "\xB3";
    const std::string ulcorner  = "\xDA";
    const std::string urcorner  = "\xBF";
    const std::string lrcorner  = "\xD9";
    const std::string llcorner  = "\xC0";

    std::vector<std::string> split(std::string s, std::string delimiter);

};

