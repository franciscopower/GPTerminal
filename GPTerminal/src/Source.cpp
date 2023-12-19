#include <iostream>
#include <windows.h>
#include <vector>
#include <thread>
#include <chrono>

#include "PowerTUI.h"
#include "AiCompletionService.h"


int main(){

    char model[] = "gpt-3.5-turbo";

    AiCompletionService aiService = AiCompletionService(model);

    while (true) {

		std::cout << "Message: ";
		char prompt_c[1000];
		std::cin.getline(prompt_c, 1000);
		std::string prompt = prompt_c;

		if (prompt == "q" || prompt == "quit")
			break;

		std::string completion = aiService.createCompletion(prompt_c);

		std::cout << "Reply: " << completion << std::endl << std::endl;    

    }

    return 0;
}