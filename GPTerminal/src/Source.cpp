#include <memory>
#include <iostream>
#include <windows.h>
#include <vector>
#include <thread>
#include <filesystem>
#include <fstream>

#include <optional>

#include "Result.h"

#include "TabSelector.h"
#include "Loader.h"
#include "HLine.h"
#include "AiCompletionService.h"
#include "TextManipulation.h"
#include "Frame.h"

#define COLOR_RESET   "\033[0m"
#define COLOR_BLACK   "\033[30m"      /* Black */
#define COLOR_RED     "\033[31m"      /* Red */
#define COLOR_GREEN   "\033[32m"      /* Green */
#define COLOR_YELLOW  "\033[33m"      /* Yellow */
#define COLOR_BLUE    "\033[34m"      /* Blue */
#define COLOR_MAGENTA "\033[35m"      /* Magenta */
#define COLOR_CYAN    "\033[36m"      /* Cyan */
#define COLOR_WHITE   "\033[37m"      /* White */
#define COLOR_DARKGRAY   "\033[90m"      /* GRAY */
#define COLOR_BOLD	  "\033[1m"		  /* Bold */
#define COLOR_UNDERLINE "\033[1m"		  /* Underline */

#define ENV_VAR_MAX_SIZE 200
#define ERROR_MESSAGE_MAX_SIZE 100

std::optional<int> chat(char* model, char* host, char* apiKey);
std::optional<int> powershellHelp(std::string prompt, char* model, char* host, char* apiKey);
void copyToClipboard(std::string textToCopy);
void getEnvVariable(const char* envVarName, char* destination, char* error);
Result<std::string, std::string> getHistory();

int main(int argc, char** argv) {

	// help section
	if (argc == 2) {
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
			std::string help_message = R"(GPTerminal - your terminal AI assistant

USAGE: GPTerminal [-h | --help] [-c | --chat] <command description>
	- <command description> - Generate a command based on the provided description.
		- eg.: GPTerminal list all markdown files in current directory
	- [-c | --chat] - Enter chat mode
		- Chat mode will also be entered if GPTerminal is called without providing any command.
	- [-h | --help] - Show this help message

CONFIGURATIONS: Set the following environment variables
	- GPTERMINAL_LLM_API_KEY - Your API key (for OpenAI API or for the Google Gemini API)
	- GPTERMINAL_LLM_API_HOST - (optional) The host address (url) you want to use. Defaults to the OpenAI host address.
	- GPTERMINAL_MODEL - (optional) The LLM model you want to use. Defaults to gpt-3.5-turbo.
	- GPTERMINAL_CHAT_MODEL - (optional) The LLM model you want to use in chat mode. Defaults to the same model you choose in the GPTERMINAL_MODEL environment variable.

If you find any issue while using GPTerminal or would like to see some extra feature, feel free to reach out here: 
- https://github.com/franciscopower/GPTerminal/issues
)";
			std::cout << help_message << std::endl;
			return 0;
		}
	}

	//get settings
	char model[ENV_VAR_MAX_SIZE] = "";
	char model_chat[ENV_VAR_MAX_SIZE] = "";
	char host[ENV_VAR_MAX_SIZE] = "";
	char apiKey[ENV_VAR_MAX_SIZE] = "";

	const char ENV_LLM_API_KEY[] = "GPTERMINAL_LLM_API_KEY";
	const char ENV_OPENAI_API_KEY[] = "OPENAI_API_KEY";
	const char ENV_LLM_API_HOST[] = "GPTERMINAL_LLM_API_HOST";
	const char ENV_LLM_MODEL[] = "GPTERMINAL_MODEL";
	const char ENV_LLM_MODEL_CHAT[] = "GPTERMINAL_CHAT_MODEL";

	char err_llmKey[ERROR_MESSAGE_MAX_SIZE];
	char err_openaiKey[ERROR_MESSAGE_MAX_SIZE];
	getEnvVariable(ENV_LLM_API_KEY, apiKey, err_llmKey);
	getEnvVariable(ENV_OPENAI_API_KEY, apiKey, err_openaiKey);
	if (apiKey[0] == '\0') {
		std::cout << COLOR_RED << "[ERROR] - Could not find Api Key.\nPlease set an environment variable called GPTERMINAL_LLM_API_KEY to your api key." << COLOR_RESET << std::endl;
		return 1;
	}

	char error[ERROR_MESSAGE_MAX_SIZE];
	getEnvVariable(ENV_LLM_API_HOST, host, error);
	getEnvVariable(ENV_LLM_MODEL, model, error);
	getEnvVariable(ENV_LLM_MODEL_CHAT, model_chat, error);
	if (model_chat[0] == '\0') {
		strncpy(model_chat, model, ENV_VAR_MAX_SIZE);
	}
	

	// Set console code page to UTF-8 so console known how to interpret string data
	UINT oldcp = GetConsoleOutputCP();  //what is the current code page? store for later
	SetConsoleOutputCP(CP_UTF8);
	// Enable buffering to prevent VS from chopping up UTF-8 byte sequences
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	// get user input
	std::optional<int> returned_error;
	if ((argc == 2 && (strcmp(argv[1],"--chat") == 0 || strcmp(argv[1],"-c") == 0 )) || argc < 2) {
		returned_error = chat(model_chat, host, apiKey);
	} 
	else if (argc == 2 && (strcmp(argv[1],"--fix") == 0 || strcmp(argv[1],"-f") == 0 )) {
		//read previous command
		Result<std::string, std::string> prevCommand_r = getHistory();
		if (!prevCommand_r.is_ok) {
			std::cerr << COLOR_RED << "[ERROR] - " << prevCommand_r.error << COLOR_RESET << std::endl;
			return 1;
		}
		std::cout << prevCommand_r.value << std::endl;

		//returned_error = powershellHelp(prompt, model, host, apiKey);
	} 
	else {
		std::string prompt = "";
		for (int i = 1; i < argc; i++) {
			prompt.append(argv[i]);
			prompt.append(" ");
		}
		std::string fullPrompt = "Keeping in mind that the current working directory is '";
		fullPrompt.append(std::filesystem::current_path().string());
		fullPrompt.append("', given the following request, create a Windows Powershell command that can solve it (your reply must only contain the command, nothing else, and do not wrap it in a markdown code block. For example: Request: 'List all items in the current directory'; Reply: 'Get-ChildItem'): ");
		fullPrompt.append(prompt);
		returned_error = powershellHelp(fullPrompt, model, host, apiKey);
	}
		
	std::cout << COLOR_RESET;

	SetConsoleOutputCP(oldcp);
	if (returned_error.has_value())
		return 1;
	else
		return 0;
}
 
std::optional<int> powershellHelp(std::string prompt, char* model, char* host, char* apiKey) {

	AiCompletionServiceFactory llm_service_factory;
	auto llm_service = llm_service_factory.getService(model);
	if (llm_service == NULL) {
		std::cerr << COLOR_RED << "[ERROR] - " << model << " is currently not supported by GPTerminal. Change the defined model in the environment variable GPTERMINAL_MODEL." << COLOR_RESET << std::endl;
		return 1;
	}
    AiCompletionService aiService = AiCompletionService(llm_service);

	aiService.init(model, host, apiKey);

	bool responseComplete = false;

	// set TUI
	enum options_enum {
		COPY,
		EXPLAIN,
		IMPROVE,
		RUN,
		QUIT
	};
	std::vector<std::string> options = {
		"Copy to Clipboard",
		"Explain",
		"Improve",
		"Run",
		"Quit"
	};
	TabSelector tabSelector(options);
	Frame outputFrame("Command");
	Loader loader(Loader::DOTS, "Generating...");


	std::string generatedCommand = "";
	int selectedOption = -1;

	// main loop
	while (!responseComplete) {

		// create completion
		Result<std::string, std::string> completion_r;
		std::thread completion_thread([&]() {
			completion_r = aiService.createCompletion(prompt);
			});

		// loader animation
		std::cout << COLOR_YELLOW;
		while (completion_r.value == "" && completion_r.error == "") {
			std::cout << loader.draw() << std::flush;
		}
		std::cout << COLOR_RESET << "\r\x1b[2K"; //carriage return and clear line

		completion_thread.join();

		// display result
		if (!completion_r.is_ok) {
			std::cerr << COLOR_RED << "[ERROR] - " << completion_r.error << COLOR_RESET << std::endl;
			return 1;
		}

		if (selectedOption == -1 || selectedOption == IMPROVE) { 
			generatedCommand = completion_r.value; 
			generatedCommand = ManipulateText::removeCodeBlocks(generatedCommand);
		}
		outputFrame.setContent(completion_r.value);
		std::cout << outputFrame.draw() << std::endl << std::endl;

		// get next action 
		selectedOption = -1;
		while (selectedOption < 0 ) {
			std::cout << tabSelector.draw() << std::flush;
			selectedOption = tabSelector.getInput();
		}
		std::cout << std::endl << std::endl;

		std::string full_command = "powershell ";
		full_command.append(generatedCommand);

		switch (selectedOption)
		{
		case COPY:
			// copy to clipboard
			copyToClipboard(generatedCommand);
			responseComplete = true;
			break;
		case EXPLAIN:
			// explain code
			prompt = "Explain the command you generated.";
			outputFrame.setTitle("Explanation");
			break;
		case IMPROVE:
			// improve
			std::cout << "How should I improve the command?" << std::endl;
			std::cout << COLOR_YELLOW << u8"\u21aa " << COLOR_RESET;
			char prompt_c[1000];
			std::cin.getline(prompt_c, 1000);
			prompt = "Change the command you created according to the following (your reply must only contain the command, nothing else): ";
			prompt.append(prompt_c);
			outputFrame.setTitle("Command");
			break;
		case RUN:
			// run code
			system(full_command.c_str());
			responseComplete = true;
			break;
		case QUIT:
			responseComplete = true;
			break;
		default:
			responseComplete = true;
			break;
		}
	}
	return {};
}


std::optional<int> chat(char* model, char* host, char* apiKey) {

	Loader loader(Loader::DOTS, "Generating...");

	AiCompletionServiceFactory llm_service_factory;
	auto llm_service = llm_service_factory.getService(model);
	if (llm_service == NULL) {
		std::cerr << COLOR_RED << "[ERROR] - " << model << " is currently not supported by GPTerminal. Change the defined model in the environment variable GPTERMINAL_CHAT_MODEL or GPTERMINAL_MODEL." << COLOR_RESET << std::endl;
		return 1;
	}
    AiCompletionService aiService = AiCompletionService(llm_service);

	std::optional<std::string> ai_init_error = aiService.init(model, host, apiKey);

	std::cout << "Hi! I'm " << aiService.getModel() << ". Let's chat! And when you want to quit, just type 'quit' or 'q' :)" << std::endl;

	if (ai_init_error.has_value()) {
		std::cerr << COLOR_RED << "[ERROR] - " << ai_init_error.value() << COLOR_RESET << std::endl;
		return 1;
	}

	while (true) {

		std::cout << COLOR_DARKGRAY << "You: " << COLOR_RESET;
		char prompt_c[1000];
		std::cin.getline(prompt_c, 1000);
		std::string prompt = prompt_c;

		if (prompt == "q" || prompt == "quit")
			break;

		Result<std::string, std::string> completion_r;
		std::thread completion_thread([&]() {
			completion_r = aiService.createCompletion(prompt);
		});

		// loader animation
		std::cout << COLOR_YELLOW;
		while (completion_r.value == "" && completion_r.error == "") {
			std::cout << loader.draw() << std::flush; //TODO make it work without the endl
		}
		std::cout << COLOR_RESET << "\r\x1b[2K"; //carriage return and clear line

		completion_thread.join();

		if (completion_r.is_ok) {
			HLine hlinetitle("GPT");
			HLine hline("");
			std::cout << COLOR_DARKGRAY << hlinetitle.draw() << COLOR_RESET << std::endl;
			std::cout << ManipulateText::wrap(ManipulateText::colorCode(completion_r.value));
			std::cout << COLOR_DARKGRAY << hline.draw() << std::endl << COLOR_RESET << std::endl;
		}
		else {
			std::cerr << COLOR_RED << "[ERROR] - " <<completion_r.error << COLOR_RESET << std::endl;
			return 1;
		}

    }
	return {};
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

	std::cout << "Copied command." << std::endl;
}

void getEnvVariable(const char* envVarName, char* destination, char* error) {

	size_t sz = 0;
	char* rawEnvVarValue = nullptr;
	errno_t resEnv = _dupenv_s(&rawEnvVarValue, &sz, envVarName);
	if (resEnv != 0 || rawEnvVarValue == nullptr) {
		free(rawEnvVarValue);
		strncpy(error, "Could not find environemnt variable " , ERROR_MESSAGE_MAX_SIZE);
		strncat(error, envVarName, ERROR_MESSAGE_MAX_SIZE-40);
		return;
	}
	strncpy(destination, rawEnvVarValue, ENV_VAR_MAX_SIZE);
	free(rawEnvVarValue);

	return;

}


Result<std::string, std::string> getHistory() {
	const char* history_file_name = "hist.tmp";

	system("powershell \"Get-History -count 1 | ForEach-Object {echo $_.CommandLine}\"");
	//system("powershell \"Get-History -count 1 | ForEach-Object {echo $_.CommandLine} > hist.tmp\"");
	//std::ifstream historyFile(history_file_name);

	//if (!historyFile.is_open()) {
	//	return Result<std::string, std::string>::Err("Error opening history file.");
	//}

	//std::string lines;
	//std::string line;
	//while (std::getline(historyFile, line)) {
	//	std::cout << line << std::endl;
	//	lines.append(line);
	//	lines.append("\n");
	//}
	////lines.pop_back(); //remove trailing \n

	//historyFile.close();

	////remove(history_file_name);
	
	//Return Result<std::string, std::string>::Ok(lines);
	return Result<std::string, std::string>::Ok("test");
}

