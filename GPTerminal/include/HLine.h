#pragma once
#include <string>
#include <iostream>
#include "Component.h"

#include <sys/ioctl.h>
#include <unistd.h>

class HLine : Component
{
public:
	HLine(std::string title);
	HLine();
	~HLine();

	void setTitle(std::string title);

	std::string draw();


private:

	std::string title;
    const std::string hline = "─";

};

HLine::HLine()
{
	this->title = "";
}

HLine::HLine(std::string title)
{
	this->title = title;
}

HLine::~HLine()
{
}

void HLine::setTitle(std::string title) {
	this->title = title;
}

std::string HLine::draw() {
	std::string output;

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int winColumns = w.ws_col;

	int restOflineLenght = winColumns - 1;

	output.append(hline);
	// add title
	if (this->title != "") {
		output.append(" ");
		output.append(this->title);
		output.append(" ");
		restOflineLenght = restOflineLenght - 2 - int(this->title.length());
	}

	//draw start line
	for (int i = 0; i < restOflineLenght; i++) {
		output.append(hline);
	}

	return output;
}

// EXAMPLE USE
//HLine hlinetitle("Some title here");
//std::cout << hlinetitle.draw() << std::endl;
//std::cout << "testContent" << std::endl;
//HLine hline("");
//std::cout << hline.draw() << std::endl;
