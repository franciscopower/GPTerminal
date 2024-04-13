#pragma once
#include <optional>

#include "ChatApi.h"
#include "Result.h"

class OpenAiApi : public ChatApi
{
public:
	OpenAiApi(char* model);
	OpenAiApi();
	~OpenAiApi();
	Result<std::string, std::string> createPrompt(std::string user_input);
	Result<std::string, std::string> decodeReply(std::string api_reply);

	const char* MODELS_LIST[16] = {
      "gpt-4-0613",
      "gpt-4",
      "gpt-4-1106-preview",
      "gpt-4-turbo",
      "gpt-4-turbo-2024-04-09",
      "gpt-3.5-turbo-16k",
      "gpt-3.5-turbo-0613",
      "gpt-3.5-turbo-0301",
      "gpt-3.5-turbo-instruct-0914",
      "gpt-3.5-turbo-0125",
      "gpt-3.5-turbo-instruct",
      "gpt-4-0125-preview",
      "gpt-3.5-turbo",
      "gpt-3.5-turbo-16k-0613",
      "gpt-4-turbo-preview",
      "gpt-3.5-turbo-1106",
	};

};
