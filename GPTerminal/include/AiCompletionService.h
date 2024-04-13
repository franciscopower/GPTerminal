#pragma once
#include <curl/curl.h>
#include <optional>
#include "OpenAiApi.h"

class AiCompletionService
{
public:
	AiCompletionService();
	~AiCompletionService();

	Result<std::string, std::string> createCompletion(std::string user_input);
	std::optional<std::string> init(char* model, char* host, char* apiKey);

private:

	static size_t decodeCompletion(char* data, size_t size, size_t nmemb, void* userdata);
	OpenAiApi openai_chat_api;

	std::string api_reply;

    CURL* curl;
	CURLcode res;
};

