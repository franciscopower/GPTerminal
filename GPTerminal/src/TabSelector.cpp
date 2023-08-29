#include "TabSelector.h"
#include "conio.h"

TabSelector::TabSelector(std::vector<std::string> options) {
	this->options = options;
}

std::string TabSelector::draw() {
	std::string output = "";

	output.append("\r"); // carriage return
	output.append("\x1b[2K"); //clear line

	for (int i = 0; i < 4; i++) {
		if (this->selectedOption == i) {
			output.append("\x1b[7m"); // reverse terminal colors
			output.append("> ");
			output.append(this->options[i]);
			output.append("\t");
			output.append("\x1b[0m");
		}
		else {
			output.append("> ");
			output.append(this->options[i]);
			output.append("\t");
		}
	}


	return output;
}

int TabSelector::getInput() {
	int key;
	int c = _getch();

	if (!c || c == 224) { // check if they're extended char

		switch (key = _getch()) {
		case 77: // arrow left
			this->selectedOption = (this->selectedOption + 1) % 4;
			break;
		case 75: // arrow right
			this->selectedOption = (this->selectedOption -1) % 4;
			if (this->selectedOption < 0) { this->selectedOption = 4; }
			break;
		}
	
	}
	else {
		switch (c) {
		case '\t':
			this->selectedOption = (this->selectedOption + 1) % 4;
			break;
		case '\r':
			return this->selectedOption;
		}
	}

	//return this->selectedOption;
	return -1; // if an option has not been selected yet
}
