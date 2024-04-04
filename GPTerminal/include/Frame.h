#pragma once

#include <string>
#include <vector>

#include "Component.h"

class Frame : Component
{
public:

	Frame();
	Frame(std::string content);

	void setContent(std::string content);
	void setTitle(std::string title);

	std::string draw();

private:

	std::string content;
	std::string title;

};

