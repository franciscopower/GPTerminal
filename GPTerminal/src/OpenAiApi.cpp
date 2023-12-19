#include <iostream>

#include "OpenAiApi.h"

std::string OpenAiApi::createPrompt(std::string prompt) {
		
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
    catch (const std::exception&)
    {
        return ""; //TODO: Return error code
    }
    
    return api_request.dump(); //TODO: Return error code

}
std::string OpenAiApi::decodeReply(std::string api_reply) {

    std::string reply;
    std::string role;

    try
    {
        reply = std::string(nlohmann::json::parse(api_reply)["choices"][0]["message"]["content"]); 
        role = std::string(nlohmann::json::parse(api_reply)["choices"][0]["message"]["role"]); 
    }
    catch (const std::exception&)
    {
        std::cout << R"(
[ERROR] - Could not parse the reply. 
The replay might contain special characters. 
Try repeating your request and asking the model not to return any special characters.
)";
        return "";
    }

    this->chat.push_back({ role, reply });

    return reply;
}

void OpenAiApi::setServiceHeaders() {
    strncpy_s(this->service_headers.url, "https://api.openai.com/v1/chat/completions", URL_MAX_SIZE);

	size_t sz = 0;
	char* bearerToken = nullptr;
	errno_t resEnv = _dupenv_s(&bearerToken, &sz, "OPENAI_API_KEY");
	if (resEnv != 0 || bearerToken == nullptr) {
		std::cout << "[ERROR] - Could not find API Key" << std::endl;
		free(bearerToken);
		return; //TODO: Return an error
	}
    strncpy_s(this->service_headers.bearer_token, bearerToken, BEARER_TOKEN_MAX_SIZE);
	free(bearerToken);
}

OpenAiApi::OpenAiApi(char* model) {
    this->model = model;
    setServiceHeaders(); 
}
OpenAiApi::OpenAiApi() {
    this->model = "gpt-3.5-turbo";
    setServiceHeaders(); 
}

ServiceHeaders* OpenAiApi::getServiceHeaders() {
    return &(this->service_headers);
}

OpenAiApi::~OpenAiApi() {}
