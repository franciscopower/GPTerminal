#include <iostream>
#include <windows.h>
#include <vector>
#include <thread>

#include "PowerTUI.h"
#include "AiCompletionService.h"

void chat(char* model);
void powershellHelp(std::string prompt, char* model);

int main(int argc, char** argv) {
    char model[] = "gpt-3.5-turbo";

	if ((argc == 2 && argv[1] == "chat") || argc == 1) {
		chat(model);
	} 
	else {
		std::string prompt = "";
		for (int i = 1; i < argc; i++) {
			prompt.append(argv[i]);
			prompt.append(" ");
		}
		powershellHelp(prompt, model);
	}

    return 0;
}

void powershellHelp(std::string prompt, char* model) {

    AiCompletionService aiService = AiCompletionService(model);
	bool responseComplete = false;

	// set TUI
	std::vector<std::string> options = {
		"Run",
		"Explain",
		"Improve",
		"Copy to Clipboard",
		"Quit"
	};
	TabSelector tabSelector(options);
	Frame outputFrame("GPT");
	Loader loader(Loader::BAR, "Generating...");

	std::string fullPrompt = "Given the following request, create a Windows Powershell command that can solve it (your reply must only contain the command, nothing else): ";
	fullPrompt.append(prompt);

	// main loop
	while (!responseComplete) {

		// create completion
		std::string completion;
		std::thread completion_thread([&](){
			completion = aiService.createCompletion(fullPrompt);
		});

		// loader animation
		while (completion == "") {
			std::cout << loader.draw();
		}
		std::cout << "\r\x1b[2K"; //carriage return and clear line

		completion_thread.join();

		outputFrame.setContent(completion);
		std::cout << outputFrame.draw() << std::endl << std::endl;
		
		int selectedOption = -1;
		while (selectedOption < 0 ) {
			std::cout << tabSelector.draw();
			selectedOption = tabSelector.getInput();
		}
		std::cout << std::endl << std::endl;

		switch (selectedOption)
		{
		case 0:
			// run code
			responseComplete = true;
			break;
		case 1:
			// explain code
			fullPrompt = "Explain the command you generated.";
			break;
		case 2:
			// improve
			std::cout << "How should I improve the command?\n-> ";
			char prompt_c[1000];
			std::cin.getline(prompt_c, 1000);
			fullPrompt = "Change the command you created according to the following (your reply must only contain the command, nothing else): ";
			fullPrompt.append(prompt_c);
			break;
		case 3:
			// copy to clipboard
			responseComplete = true;
			break;
		case 4:
			responseComplete = true;
			break;
		default:
			responseComplete = true;
			break;
		}
	}

}


void chat(char* model) {

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
			completion = aiService.createCompletion(prompt);
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
}
