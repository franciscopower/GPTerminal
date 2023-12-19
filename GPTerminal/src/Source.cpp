#include <iostream>
#include <windows.h>
#include <vector>
#include <thread>
#include <chrono>

#include "PowerTUI.h"
#include "OpenAIService.h"


int main(){

    OpenAIService aiService = OpenAIService();

    while (true) {

    std::cout << "Message: ";
    char prompt_c[1000];
    std::cin.getline(prompt_c, 1000);
    std::cout << std::endl;
    std::string prompt = prompt_c;

    if (prompt == "q" || prompt == "quit")
        break;

    std::string completion = aiService.createCompletion(prompt_c);

    std::cout << "Reply: " << completion << std::endl;

    

    }

    return 0;
}