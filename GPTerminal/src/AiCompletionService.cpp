#include <memory>
#include <optional>
#include "OpenAiApi.h"
#include "GeminiApi.h"
#include <algorithm>
#include "ClaudeApi.h"
#include "Result.h"

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

	std::string selected_service;

	for (const auto& [key, value] : this->models) {
		auto it = find(value.begin(), value.end(), model);
		if (it != value.end()) {
			selected_service = key;
			break;
		}
	}
	
	if (selected_service == "open_ai" || model == "") { // default model provider
		auto openai_service = std::make_shared<OpenAiApi>();
		return openai_service;
	}
	else if (selected_service == "gemini") {
		auto gemini_service = std::make_shared<GeminiApi>();
		return gemini_service;
	}
	else if (selected_service == "claude") {
		auto claude_service = std::make_shared<ClaudeApi>();
		return claude_service;
	}
	else {
		return NULL;
	}

}
