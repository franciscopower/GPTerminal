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

	Result<ServiceHeaders*, std::string> setServiceHeaders();
	ServiceHeaders* getServiceHeaders();

private:


};
