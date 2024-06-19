#pragma once
#include "ChatApi.h"
#include "Result.h"
#include "curl/curl.h"

class GeminiApi : public ChatApi
{
public:
	GeminiApi();
	~GeminiApi();

	std::optional<std::string> init(char* model, char* host, char* apiKey) override;
	Result<std::string, std::string> requestCompletion(std::string user_input) override;

	std::string getModel() override {
		return this->model;
	}


private:

	Result<std::string, std::string> createPrompt(std::string user_input) override;
	static size_t curlWriteFunction(char* data, size_t size, size_t nmemb, void* userdata);
	Result<std::string, std::string> parseReply(std::string api_reply) override;

	std::string api_reply;

    CURL* curl;
	CURLcode res;

};
