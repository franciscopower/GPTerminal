#include <cstdlib>
#include <iostream>
#include <stdio.h>

#include <optional>
#include "Result.h"

#include "AiCompletionService.h"
#include "json.hpp"


AiCompletionService::AiCompletionService() {}

auto AiCompletionService::init(char* model, char* host, char* apiKey) -> std::optional<std::string> { 

    this->openai_chat_api = OpenAiApi(model);

    //check what provider the model is from
    int bValidModel = 0;
    for (const char* m : this->openai_chat_api.MODELS_LIST) {
        if (strcmp(m, model) == 0) {
            bValidModel = 1;
            exit;
        }
    }
    if (bValidModel == 0) {
        return "The selected LLM is not supported by GPTerminal. Try setting a different one.\nFor more information, check the help (-h or --help) section";
    }

    this->openai_chat_api = OpenAiApi(model);
    
    // CURL initialization
    curl_global_init(CURL_GLOBAL_ALL);
    this->curl = curl_easy_init();
    if (!this->curl) {
        return "Error initializing CURL";
    }

    // create headers
    struct curl_slist* headers = NULL;

    char curlBearerHeader[BEARER_TOKEN_MAX_SIZE] = "Authorization: Bearer ";
    strncat_s(curlBearerHeader, BEARER_TOKEN_MAX_SIZE, apiKey, BEARER_TOKEN_MAX_SIZE - 25);

    headers = curl_slist_append(headers, curlBearerHeader);
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept: application/json");

    // set CURL options
    curl_easy_setopt(this->curl, CURLOPT_URL, host);
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
    size_t result_size = size * nmemb;

    ((std::string*)userdata)->append(data, result_size);

    return result_size;
}

AiCompletionService::~AiCompletionService()
{
    // Cleanup CULR
    curl_easy_cleanup(this->curl);
    curl_global_cleanup();
}
