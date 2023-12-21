#include <iostream>
#include <windows.h>
#include <vector>
#include <thread>

#include "PowerTUI.h"
#include "AiCompletionService.h"


int main(int argc, char **argv) {

    char model[] = "gpt-3.5-turbo";

    AiCompletionService aiService = AiCompletionService(model);


    while (true) {

		std::cout << "Message: ";
		char prompt_c[1000];
		std::cin.getline(prompt_c, 1000);
		std::string prompt = prompt_c;

		if (prompt == "q" || prompt == "quit")
			break;

		std::string completion;
		std::thread completion_thread([&](){
			completion = aiService.createCompletion(prompt_c);
		});

		// loader animation
		Loader loader(Loader::BAR, "Generating...");
		while (completion == "") {
			std::cout << loader.draw();
		}
		std::cout << "\r\x1b[2K"; //carriage return and clear line

		completion_thread.join();

		//Frame outputFrame("GPT");
		//outputFrame.setContent(completion);

		HLine hlinetitle("GPT");
		HLine hline("");
		std::cout << hlinetitle.draw() << std::endl;
		std::cout << completion << std::endl;
		std::cout << hline.draw() << std::endl << std::endl;
    }

    return 0;
}