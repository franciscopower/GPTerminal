#include <cstdlib>
#include <iostream>

#include "OpenAIService.h"
#include "OpenAIInterface.h"

#include "json.hpp"


OpenAIService::OpenAIService()
{
    // CURL initialization

    curl_global_init(CURL_GLOBAL_ALL);
    this->curl = curl_easy_init();
    if (!this->curl) {
        std::cout << "[ERROR] - Cannot initialize CURL" << std::endl;
        return; //TODO: Return an error
    }

    const char* openAIPostUrl = "https://api.openai.com/v1/chat/completions";

    // create headers
    struct curl_slist* headers = NULL;

    char* openAIBearerToken = nullptr;
    size_t sz = 0;
    errno_t resEnv = _dupenv_s(&openAIBearerToken, &sz, "OPENAI_API_KEY");
    if (resEnv != 0 || openAIBearerToken == nullptr) {
        std::cout << "[ERROR] - Could not find API Key" << std::endl;
        free(openAIBearerToken);
        return; //TODO: Return an error
    }

    char curlBearerHeader[100] = "Authorization: Bearer ";
    strncat_s(curlBearerHeader, 100, openAIBearerToken, 75);
    free(openAIBearerToken);

    headers = curl_slist_append(headers, curlBearerHeader);
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept: application/json");

    // set CURL options
    curl_easy_setopt(this->curl, CURLOPT_URL, openAIPostUrl);
    curl_easy_setopt(this->curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(this->curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(this->curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, &OpenAIService::decodeCompletion);
    curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, (void *)&(this->api_reply));

}

std::string OpenAIService::createCompletion(std::string prompt) {

    // set data for call
    const char* data = "{\n    \"model\": \"gpt-3.5-turbo\",\n    \"messages\": [\n        {\n            \"role\": \"user\",\n            \"content\": \"Who won the world series in 2020?\"\n        },\n        {\n            \"role\": \"assistant\",\n            \"content\": \"The Los Angeles Dodgers won the World Series in 2020.\"\n        },\n        {\n            \"role\": \"user\",\n            \"content\": \"Where was it played?\"\n        }\n    ],\n    \"temperature\": 1,\n    \"top_p\": 1,\n    \"n\": 1,\n    \"stream\": false,\n    \"max_tokens\": 250,\n    \"presence_penalty\": 0,\n    \"frequency_penalty\": 0\n}";
    curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, data);

    // perform request
    this->res = curl_easy_perform(this->curl);

    if (this->res!= CURLE_OK) {
        std::cout << "[ERROR] - " << curl_easy_strerror(this->res) << std::endl;
    }

    return this->api_reply;
}

size_t OpenAIService::decodeCompletion(char* data, size_t size, size_t nmemb, void* userdata) {
    ((std::string*)userdata)->append(std::string(data));

    return size * nmemb;
}

OpenAIService::~OpenAIService()
{
    // Cleanup CULR
    curl_easy_cleanup(this->curl);
    curl_global_cleanup();
}
