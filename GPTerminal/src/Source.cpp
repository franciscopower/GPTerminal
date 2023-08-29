#include <iostream>
#include <windows.h>
#include <vector>

#include "PowerTUI.h"


int main(){

    Frame outputFrame(std::string("this is a test string"));

    outputFrame.setContent("Lorem Ipsum is simply dummy text of the printing and typesetting industry.\nLorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.");

    std::cout << outputFrame.draw() << std::endl;

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
        std::cout << tabSelector.draw();
        selectedOption = tabSelector.getInput();
    }

    std::cout << std::endl; // leave empty space before end of program
    return 0;
}