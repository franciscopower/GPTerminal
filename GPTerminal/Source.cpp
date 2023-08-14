#include <iostream>
#include <windows.h>
#include <vector>

#include "TabSelector.h"

enum Level {
    GENERATE,
    CHOOSE_ACTIONS,
    USER_INPUT,
    EXECUTE,
};


int main(){

    std::vector<std::string> options = {
        "Run",
        "Explain",
        "Improve",
        "Quit"
    };
    TabSelector tabSelector(options);

    int selectedOption = 0;
    int c = 0;
    int key = 0;
    Level level = GENERATE;

    // MAIN LOOP
    bool RUN = true;
    while (RUN) {

        std::cout << tabSelector.draw() << std::endl;
        selectedOption = tabSelector.getInput();

		switch (selectedOption) {
		case 0:
			level = EXECUTE;
			break;
		case 1:
			level = GENERATE;
			break;
		case 2:
			level = USER_INPUT;
			break;
		case 3:
			return 0;
		}

    }

    std::cout << std::endl; // leave empty space before end of program
    return 0;
}