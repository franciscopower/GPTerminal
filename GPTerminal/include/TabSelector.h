#pragma once
#include "Component.h"
#include <string>
#include <vector>

class TabSelector :
    public Component
{
private:
	std::vector<std::string> options;
	int selectedOption = 0;
public:
	TabSelector(std::vector<std::string> options);
	std::string draw();
	int getInput();
	Dimention getDim();
};

