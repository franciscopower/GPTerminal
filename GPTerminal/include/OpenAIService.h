#pragma once
#include <curl/curl.h>

class OpenAIService
{
public:
	OpenAIService();
	~OpenAIService();

	std::string createCompletion(std::string prompt);


private:

    CURL* curl;
    CURLcode res;

	static size_t decodeCompletion(char* ptr, size_t size, size_t nmemb, void* userdata);

};

