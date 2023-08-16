#include <iostream>
#include <windows.h>
#include <vector>

#include "TabSelector.h"


int main(){



    // Select followup actions
    std::vector<std::string> options = {
        "Run",
        "Explain",
        "Improve",
        "Quit"
    };
    TabSelector tabSelector(options);

    // wait option selection
    int selectedOption = -1;
    while (selectedOption < 0 ) {
        std::cout << tabSelector.draw() << std::endl;
        selectedOption = tabSelector.getInput();
    }

    std::cout << std::endl; // leave empty space before end of program
    return 0;
}