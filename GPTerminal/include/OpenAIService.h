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

	//struct memory {
	//	char* response;
	//	size_t size;
	//} api_reply;
	std::string api_reply;

	static size_t decodeCompletion(char* ptr, size_t size, size_t nmemb, void* userdata);

};

