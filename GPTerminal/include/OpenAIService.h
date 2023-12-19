#pragma once
#include <curl/curl.h>
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

	json openAiResponse;
	json openAiRequest = {
		{"model", "gpt-3.5-turbo"},
		{"messages", {
			{
				{"role",  "user"},
				{"content", "Who won the world series in 2020?"}
			},
			{
				{"role", "assistant"},
				{"content", "The Los Angeles Dodgers won the World Series in 2020."}
			},
			{
				{"role",  "user"},
				{"content", "Where was it played?"}
			}
		}}
	};

};

