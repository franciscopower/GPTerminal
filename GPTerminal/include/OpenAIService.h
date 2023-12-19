#pragma once
#include <curl/curl.h>
#include <vector>
#include <json.hpp>


using json = nlohmann::json;

class OpenAIService
{
public:
	OpenAIService();
	~OpenAIService();

	std::string createCompletion(std::string prompt);

private:

    CURL* curl;
    CURLcode res;
	std::string api_reply;
	static size_t decodeCompletion(char* ptr, size_t size, size_t nmemb, void* userdata);

	struct ChatEntry {
		std::string role;
		std::string content;
	};
	std::vector<ChatEntry> chat;

	json openAiResponse;
	json openAiRequest;
	bool createOpenAiRequest(std::string prompt);

	char model[30] = "gpt-3.5-turbo";
};

