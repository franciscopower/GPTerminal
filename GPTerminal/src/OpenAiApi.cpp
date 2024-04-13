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
        try {
            reply = std::string(nlohmann::json::parse(api_reply)["error"]["message"]); 
            return Result<std::string, std::string>::Err(reply);
        }
        catch (const std::exception& e) {
            return Result<std::string, std::string>::Err(e.what());
        }
    }

    this->chat.push_back({ role, reply });

    return Result<std::string, std::string>::Ok(reply);
}

OpenAiApi::OpenAiApi(char* model) {
    this->model = model;
}
OpenAiApi::OpenAiApi() {
    this->model = "gpt-3.5-turbo";
}

OpenAiApi::~OpenAiApi() {}
