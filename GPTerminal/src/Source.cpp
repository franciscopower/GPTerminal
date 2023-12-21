#include <iostream>
#include <windows.h>
#include <vector>
#include <thread>
#include <filesystem>

#include "PowerTUI.h"
#include "AiCompletionService.h"

void chat(char* model);
void powershellHelp(std::string prompt, char* model);
void copyToClipboard(std::string textToCopy);

int main(int argc, char** argv) {
    char model[] = "gpt-3.5-turbo";

	if ((argc == 2 && strcmp(argv[1],"chat") == 0) || argc < 2) {
		std::cout << "Let's chat! When you want to quit, just type 'quit'." << std::endl;
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
	enum options_enum {
		COPY,
		EXPLAIN,
		IMPROVE,
		//RUN,
		QUIT
	};
	std::vector<std::string> options = {
		"Copy to Clipboard",
		"Explain",
		"Improve",
		//"Run",
		"Quit"
	};
	TabSelector tabSelector(options);
	Frame outputFrame("GPT");
	Loader loader(Loader::BAR, "Generating...");

	std::string fullPrompt = "Keeping in mind that the current working directory is '";
	fullPrompt.append(std::filesystem::current_path().string());
	fullPrompt.append("', given the following request, create a Windows Powershell command that can solve it (your reply must only contain the command, nothing else): ");
	fullPrompt.append(prompt);

	std::string generatedCommand = "";
	int selectedOption = -1;

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

		// display result
		if (selectedOption == -1 || selectedOption == IMPROVE) { generatedCommand = completion; }
		outputFrame.setContent(completion);
		std::cout << outputFrame.draw() << std::endl << std::endl;
		
		// get next action 
		selectedOption = -1;
		while (selectedOption < 0 ) {
			std::cout << tabSelector.draw();
			selectedOption = tabSelector.getInput();
		}
		std::cout << std::endl << std::endl;

		switch (selectedOption)
		{
		case COPY:
			// copy to clipboard
			copyToClipboard(generatedCommand);
			responseComplete = true;
			break;
		case EXPLAIN:
			// explain code
			fullPrompt = "Explain the command you generated.";
			break;
		case IMPROVE:
			// improve
			std::cout << "How should I improve the command?\n-> ";
			char prompt_c[1000];
			std::cin.getline(prompt_c, 1000);
			fullPrompt = "Change the command you created according to the following (your reply must only contain the command, nothing else): ";
			fullPrompt.append(prompt_c);
			break;
		//case RUN:
		//	// run code
		//	system(completion.c_str());
		//	responseComplete = true;
		//	break;
		case QUIT:
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

void copyToClipboard(std::string textToCopy) {
	const char* text = textToCopy.c_str();
	const size_t len = strlen(text) + 1;

	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
	memcpy(GlobalLock(hMem), text, len);

	OpenClipboard(0);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
}
