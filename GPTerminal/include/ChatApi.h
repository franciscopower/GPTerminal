#pragma once
#include <vector>
#include <string>
#include <optional>
#include <json.hpp>
#include "Result.h"

#define URL_MAX_SIZE 100
#define BEARER_TOKEN_MAX_SIZE 300

// QUESTION: How can I use this struct definition instead of the bottom one? Which one is better?
//struct ServiceHeaders {
//	const char* url;
//	const char* bearer_token;
//};

struct ServiceHeaders {
	char url[URL_MAX_SIZE];
	char bearer_token[BEARER_TOKEN_MAX_SIZE];
};


class ChatApi
{
protected:

	struct ChatEntry {
		std::string role;
		std::string content;
	};
	std::vector<ChatEntry> chat;

	const char* model;

	ServiceHeaders service_headers;
	virtual Result<ServiceHeaders*, std::string> setServiceHeaders() = 0;
	
public:

	virtual Result<std::string, std::string> createPrompt(std::string user_input) = 0;
	virtual Result<std::string, std::string> decodeReply(std::string api_reply) = 0;

	virtual ServiceHeaders* getServiceHeaders() = 0;

};

// QUESTION: Why does this not work????
//ChatApi::ChatApi(char* model)
//{
//	this->model = model;
//	setServiceHeaders();
//}
//ChatApi::ChatApi()
//{
//	this->model = "gpt-3.5-turbo";
//	setServiceHeaders();
//}

//ServiceHeaders* ChatApi::getServiceHeaders() {
//	return &(this->service_headers);
//}

//ChatApi::~ChatApi()
//{
//}
