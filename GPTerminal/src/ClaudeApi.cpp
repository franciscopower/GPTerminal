
#include <iostream>
#include <exception>
#include <optional>

#include "Result.h"
#include "ClaudeApi.h"
#include "json.hpp"

ClaudeApi::ClaudeApi() {}

std::optional<std::string> ClaudeApi::init(char* model, char* host, char* apiKey) {
	if (model[0] == '\0') {
        this->model = "claude-3-5-sonnet-20240620";
	}
    else {
        this->model = model;
    }

	if (host[0] == '\0') {
		strcpy_s(host, 200, "https://api.anthropic.com/v1/messages");
	}

    // CURL initialization
    curl_global_init(CURL_GLOBAL_ALL);
    this->curl = curl_easy_init();
    if (!this->curl) {
        return "Error initializing CURL";
    }

    // create headers
    struct curl_slist* headers = NULL;

    char curlBearerHeader[BEARER_TOKEN_MAX_SIZE] = "x-api-key: ";
    strncat_s(curlBearerHeader, BEARER_TOKEN_MAX_SIZE, apiKey, BEARER_TOKEN_MAX_SIZE - 12);

    headers = curl_slist_append(headers, curlBearerHeader);
    headers = curl_slist_append(headers, "anthropic-version: 2023-06-01");
    headers = curl_slist_append(headers, "Accept: application/json");

    // set CURL options
    curl_easy_setopt(this->curl, CURLOPT_URL, host);
    curl_easy_setopt(this->curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(this->curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(this->curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, &ClaudeApi::curlWriteFunction);
    curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, (void *)&(this->api_reply));

    return {};
}

Result<std::string, std::string> ClaudeApi::requestCompletion(std::string user_input) {

//    std::string test_reply = R"(This is a sample text string containing various elements for testing.
//
//* It has different sentence structures (simple, complex). Some sentences are longer, much looooonger than others, like this one, for example.
//* It uses mixed case (uppercase and lowercase letters).
//* Numbers are included (123, 4.56).
//
//Here's some inline code: `int test = 1`
//
//Here's some block code example too:
//```C++
//auto openai_chat_api_r = this->openai_chat_api.createPrompt(user_input);
//    std::string prompt;
//    if (openai_chat_api_r.is_ok) {
//        prompt = openai_chat_api_r.value;
//    } else {
//        return Result<std::string, std::string>::Err(openai_chat_api_r.error);
//    }
//```
//
//This allows you to test your function's ability to handle a range of text features.)";
    //return Result<std::string, std::string>::Ok(test_reply);

    this->api_reply = "";

    auto openai_chat_api_r = this->createPrompt(user_input);
    std::string prompt;
    if (openai_chat_api_r.is_ok) {
        prompt = openai_chat_api_r.value;
    } else {
        return Result<std::string, std::string>::Err(openai_chat_api_r.error);
    }

    const char* data = prompt.c_str();
    //const char* data = R"({"messages":[{"content":"hey","role":"user"}],"model":"gpt-3.5-turbo"})";
    curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, data);

    // perform request
    this->res = curl_easy_perform(this->curl);

    if (this->res!= CURLE_OK)
        return Result<std::string, std::string>::Err(curl_easy_strerror(this->res));

    auto reply_r = this->parseReply(this->api_reply);
    if (!reply_r.is_ok)
        return Result<std::string, std::string>::Err(reply_r.error);
        
    return Result<std::string, std::string>::Ok(reply_r.value);
}

auto ClaudeApi::createPrompt(std::string prompt) -> Result<std::string, std::string> {
		
    this->chat.push_back({ "user", prompt });
    nlohmann::json api_request;
    
    try
    {
		api_request["model"] = this->model;
        api_request["max_tokens"] = 1024;

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

size_t ClaudeApi::curlWriteFunction(char* data, size_t size, size_t nmemb, void* userdata) {
    size_t result_size = size * nmemb;

    ((std::string*)userdata)->append(data, result_size);

    return result_size;
}

auto ClaudeApi::parseReply(std::string api_reply) -> Result<std::string, std::string> {

    std::string reply;
    std::string role;

    try
    {
        reply = std::string(nlohmann::json::parse(api_reply)["content"][0]["text"]); 
        role = std::string(nlohmann::json::parse(api_reply)["role"]); 
    }
    catch (const std::exception&)
    {
        try
        {
            reply = std::string(nlohmann::json::parse(api_reply)["stop_reason"]);
            if (reply == "max_tokens") {
			    return Result<std::string, std::string>::Err("Sorry, I cannot complete your request as I've reached the maximum nr of tokens for this request.");
            }
            else {
			    return Result<std::string, std::string>::Err("Sorry. For reasons not even I understand, I could not complete your request.");
            }
        }
        catch (const std::exception&)
        {
			try {
				reply = std::string(nlohmann::json::parse(api_reply)["error"]["message"]); 
				return Result<std::string, std::string>::Err(reply);
			}
			catch (const std::exception& e) {
				return Result<std::string, std::string>::Err(e.what() + std::string("\n\n") + api_reply);
			}
        }
    }

    this->chat.push_back({ role, reply });

    return Result<std::string, std::string>::Ok(reply);
}

ClaudeApi::~ClaudeApi() 
{
    // Cleanup CULR
    curl_easy_cleanup(this->curl);
    curl_global_cleanup();
}
