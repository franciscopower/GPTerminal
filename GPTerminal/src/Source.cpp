#include <iostream>
#include <windows.h>
#include <vector>
#include <thread>
#include <chrono>

#include "PowerTUI.h"
#include "OpenAIService.h"


int main(){

    std::cout << "Enter a prompt: ";
    char prompt_c[1000];
    std::cin.getline(prompt_c, 1000);
    
    std::cout << std::endl;
    std::string prompt = prompt_c;
    std::cout << "Your prompt:" << prompt << std::endl;

    OpenAIService aiService = OpenAIService();

    std::string completion = aiService.createCompletion(prompt_c);

    std::cout << "Completion: " << completion << std::endl;

    return 0;
}