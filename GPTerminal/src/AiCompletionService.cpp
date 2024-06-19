#include <memory>
#include <optional>
#include "Result.h"
#include "OpenAiApi.h"
#include "GeminiApi.h"

#include "AiCompletionService.h"


AiCompletionService::AiCompletionService(std::shared_ptr<ChatApi> chat_api_service)
	: chat_api(chat_api_service) {}

auto AiCompletionService::init(char* model, char* host, char* apiKey) -> std::optional<std::string> 
{
	return this->chat_api->init(model, host, apiKey);
}

auto AiCompletionService::createCompletion(std::string user_input) -> Result<std::string, std::string>
{
	return this->chat_api->requestCompletion(user_input);
}

std::string AiCompletionService::getModel() {
	return this->chat_api->getModel();
}


AiCompletionService::~AiCompletionService() {}


std::shared_ptr<ChatApi> AiCompletionServiceFactory::getService(std::string model) {

	
	
	auto gemini_service = std::make_shared<GeminiApi>();

	return gemini_service;
}
