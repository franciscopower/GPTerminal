#include <cstdlib>
#include <iostream>

#include <optional>
#include "Result.h"

#include "AiCompletionService.h"
#include "json.hpp"


AiCompletionService::AiCompletionService() {}

auto AiCompletionService::init(char* model) -> std::optional<std::string> {

    this->openai_chat_api = OpenAiApi(model);
    
    ServiceHeaders* service_headers;
    auto service_headers_r = this->openai_chat_api.setServiceHeaders();
    if (service_headers_r.is_ok)
        service_headers = service_headers_r.value;
    else
        return service_headers_r.error;

    // CURL initialization
    curl_global_init(CURL_GLOBAL_ALL);
    this->curl = curl_easy_init();
    if (!this->curl) {
        return "Error initializing CURL";
    }

    // create headers
    struct curl_slist* headers = NULL;

    char curlBearerHeader[BEARER_TOKEN_MAX_SIZE] = "Authorization: Bearer ";
    strncat_s(curlBearerHeader, BEARER_TOKEN_MAX_SIZE, service_headers->bearer_token, BEARER_TOKEN_MAX_SIZE - 25);

    headers = curl_slist_append(headers, curlBearerHeader);
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept: application/json");

    // set CURL options
    curl_easy_setopt(this->curl, CURLOPT_URL, service_headers->url);
    curl_easy_setopt(this->curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(this->curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(this->curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, &AiCompletionService::decodeCompletion);
    curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, (void *)&(this->api_reply));

    return {};
}

auto AiCompletionService::createCompletion(std::string user_input) -> Result<std::string, std::string>{

//    std::string test_reply = R"(This is a sample text string containing various elements for testing.
//
//* It has different sentence structures (simple, complex). Some sentences are longer, much looooonger than others, like this one, for example.
//* It uses mixed case (uppercase and lowercase letters).
//* Numbers are included (123, 4.56).
//* Special characters are present (!@#$%^&*()_+-=[]{};':"\|,.<>/?).
//
//Here's some code example too:
//```python
//def main():
//    print("Hi mom!")
//```
//
//This allows you to test your function's ability to handle a range of text features.)";
//    return Result<std::string, std::string>::Ok(test_reply);

    this->api_reply = "";

    auto openai_chat_api_r = this->openai_chat_api.createPrompt(user_input);
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

    auto reply_r = this->openai_chat_api.decodeReply(this->api_reply);
    if (!reply_r.is_ok)
        return Result<std::string, std::string>::Err(reply_r.error);
        
    return Result<std::string, std::string>::Ok(reply_r.value);
}

size_t AiCompletionService::decodeCompletion(char* data, size_t size, size_t nmemb, void* userdata) {
    ((std::string*)userdata)->append(std::string(data));

    return size * nmemb;
}

AiCompletionService::~AiCompletionService()
{
    // Cleanup CULR
    curl_easy_cleanup(this->curl);
    curl_global_cleanup();
}
