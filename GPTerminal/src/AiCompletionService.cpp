#include <cstdlib>
#include <iostream>

#include "AiCompletionService.h"
#include "json.hpp"


AiCompletionService::AiCompletionService(char* model)
{
    ServiceHeaders* service_headers;

    openai_chat_api = OpenAiApi(model);
    service_headers = openai_chat_api.getServiceHeaders();

    // CURL initialization
    curl_global_init(CURL_GLOBAL_ALL);
    this->curl = curl_easy_init();
    if (!this->curl) {
        std::cout << "[ERROR] - Cannot initialize CURL" << std::endl;
        return; //TODO: Return an error
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

}

std::string AiCompletionService::createCompletion(std::string user_input) {
    this->api_reply = "";

    std::string prompt = this->openai_chat_api.createPrompt(user_input);
    const char* data = prompt.c_str();
    //const char* data = R"({"messages":[{"content":"hey","role":"user"}],"model":"gpt-3.5-turbo"})";
    curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, data);

    // perform request
    this->res = curl_easy_perform(this->curl);

    if (this->res!= CURLE_OK) {
        std::cout << "[ERROR] - " << curl_easy_strerror(this->res) << std::endl;
    }

    std::string reply = this->openai_chat_api.decodeReply(this->api_reply);

    return reply;
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
