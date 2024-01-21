#include <iostream>
#include <exception>
#include <optional>

#include "Result.h"
#include "OpenAiApi.h"

auto OpenAiApi::createPrompt(std::string prompt) -> Result<std::string, std::string> {
		
    this->chat.push_back({ "user", prompt });
    nlohmann::json api_request;
    
    try
    {
		api_request["model"] = this->model;

		nlohmann::json json_messages = nlohmann::json::array();
		for (ChatEntry c : this->chat) {
			json_messages.push_back({ {"role", c.role}, {"content", c.content} });
		}
		api_request["messages"] = json_messages;
    }
    catch (const std::exception& e)
    {
        return Result<std::string, std::string>::Err(e.what());
    }
    
    return Result<std::string, std::string>::Ok(api_request.dump()); 

}
auto OpenAiApi::decodeReply(std::string api_reply) -> Result<std::string, std::string> {

    std::string reply;
    std::string role;

    try
    {
        reply = std::string(nlohmann::json::parse(api_reply)["choices"][0]["message"]["content"]); 
        role = std::string(nlohmann::json::parse(api_reply)["choices"][0]["message"]["role"]); 
    }
    catch (const std::exception& e)
    {
        return Result<std::string, std::string>::Err(e.what());
    }

    this->chat.push_back({ role, reply });

    return Result<std::string, std::string>::Ok(reply);
}

Result<ServiceHeaders*, std::string> OpenAiApi::setServiceHeaders() {
    strncpy_s(this->service_headers.url, "https://api.openai.com/v1/chat/completions", URL_MAX_SIZE);

	size_t sz = 0;
	char* bearerToken = nullptr;
	errno_t resEnv = _dupenv_s(&bearerToken, &sz, "OPENAI_API_KEY");
	if (resEnv != 0 || bearerToken == nullptr) {
		free(bearerToken);
        return Result<ServiceHeaders*, std::string>::Err("Could not find API Key");
	}
    strncpy_s(this->service_headers.bearer_token, bearerToken, BEARER_TOKEN_MAX_SIZE);
	free(bearerToken);

    return Result<ServiceHeaders*, std::string>::Ok(& (this->service_headers));
}

OpenAiApi::OpenAiApi(char* model) {
    this->model = model;
}
OpenAiApi::OpenAiApi() {
    this->model = "gpt-3.5-turbo";
}

ServiceHeaders* OpenAiApi::getServiceHeaders() {
    return &(this->service_headers);
}

OpenAiApi::~OpenAiApi() {}
