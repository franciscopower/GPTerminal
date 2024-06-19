#pragma once
#include <memory>
#include <optional>
#include "ChatApi.h"
#include <unordered_map>


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

	const std::unordered_map<std::string, std::vector<std::string>> models = {
		{"open_ai", {
			  "gpt-4o",
			  "gpt-4",
			  "gpt-4-turbo",
			  "gpt-3.5-turbo",
			}
		},
		{"gemini", {
			  "gemini-1.5-pro",
			  "gemini-1.5-flash",
			  "gemini-1.0-pro",
			}
		}
	};

};
