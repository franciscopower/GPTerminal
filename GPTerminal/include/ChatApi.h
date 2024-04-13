#pragma once
#include <vector>
#include <string>
#include <optional>
#include <json.hpp>
#include "Result.h"

#define URL_MAX_SIZE 100
#define BEARER_TOKEN_MAX_SIZE 300

class ChatApi
{
protected:

	struct ChatEntry {
		std::string role;
		std::string content;
	};
	std::vector<ChatEntry> chat;

	const char* model;

	
public:

	virtual Result<std::string, std::string> createPrompt(std::string user_input) = 0;
	virtual Result<std::string, std::string> decodeReply(std::string api_reply) = 0;
};
