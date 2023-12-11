#include <iostream>
#include <windows.h>
#include <vector>
#include <thread>
#include <chrono>

#include "PowerTUI.h"
#include "OpenAIService.h"


int main(){

    std::string prompt;
    std::cout << "Enter a prompt: ";
    std::cin >> prompt;
    std::cout << std::endl;

    OpenAIService aiService = OpenAIService();

    std::string completion = aiService.createCompletion(prompt);

    std::cout << "Completion: " << completion << std::endl;

    return 0;
}