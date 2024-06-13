#pragma once
#include <vector>
#include <string>
#include <optional>
#include "Result.h"

#define URL_MAX_SIZE 100
#define BEARER_TOKEN_MAX_SIZE 300

class ChatApi
{
public:
	virtual std::optional<std::string> init(char* model, char* host, char* apiKey) = 0;
	virtual Result<std::string, std::string> requestCompletion(std::string user_input) = 0;

	const char* MODELS_LIST[4] = {
      "gpt-4o",
      "gpt-4",
      "gpt-4-turbo",
      "gpt-3.5-turbo",
	};

protected:

	struct ChatEntry {
		std::string role;
		std::string content;
	};
	std::vector<ChatEntry> chat;

	const char* model;

	virtual Result<std::string, std::string> createPrompt(std::string user_input) = 0;
	virtual Result<std::string, std::string> parseReply(std::string api_reply) = 0;
};
