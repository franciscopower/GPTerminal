#pragma once
#include <curl/curl.h>
#include "OpenAiApi.h"

class AiCompletionService
{
public:
	AiCompletionService(char* model);
	~AiCompletionService();

	std::string createCompletion(std::string prompt);

private:

	static size_t decodeCompletion(char* data, size_t size, size_t nmemb, void* userdata);
	OpenAiApi openai_chat_api;

	std::string api_reply;

    CURL* curl;
    CURLcode res;
};

