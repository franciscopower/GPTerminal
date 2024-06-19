#pragma once
#include <memory>
#include <optional>
#include "ChatApi.h"


class AiCompletionService
{
public:
	AiCompletionService(std::shared_ptr<ChatApi> chat_api_service);
	~AiCompletionService();

	Result<std::string, std::string> createCompletion(std::string user_input);
	std::optional<std::string> init(char* model, char* host, char* apiKey);
	std::string getModel();

private:
	std::shared_ptr<ChatApi> chat_api;
};

class AiCompletionServiceFactory
{
public:
	std::shared_ptr<ChatApi> getService(std::string model);
};
