#pragma once
#include "ChatApi.h"

class OpenAiApi : public ChatApi
{
public:
	OpenAiApi(char* model);
	OpenAiApi();
	~OpenAiApi();
	std::string createPrompt(std::string user_input);
	std::string decodeReply(std::string);

	void setServiceHeaders();
	ServiceHeaders* getServiceHeaders();

private:


};
