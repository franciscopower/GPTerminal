#pragma once
#include "ChatApi.h"
#include "Result.h"

class OpenAiApi : public ChatApi
{
public:
	OpenAiApi(char* model);
	OpenAiApi();
	~OpenAiApi();
	Result<std::string, std::string> createPrompt(std::string user_input);
	std::string decodeReply(std::string);

	void setServiceHeaders();
	ServiceHeaders* getServiceHeaders();

private:


};
