#include <cstdlib>
#include <iostream>

#include "OpenAIService.h"
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

    if (!createOpenAiRequest(prompt))
        return "[ERROR] - Could not create request";

    const std::string data_from_json = ((this->openAiRequest).dump());
    const char* data = data_from_json.c_str();
    std::cout << "Request: " << data << std::endl;

    curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, data);

    // perform request
    this->res = curl_easy_perform(this->curl);

    if (this->res!= CURLE_OK) {
        std::cout << "[ERROR] - " << curl_easy_strerror(this->res) << std::endl;
    }

    std::string reply;

    try
    {
        this->openAiResponse = json::parse(this->api_reply);
        reply = std::string(json::parse(this->api_reply)["choices"][0]["message"]["content"]); 
    }
    catch (const std::exception&)
    {
        std::cout << R"(
[ERROR] - Could not parse the reply. 
The replay might contain special characters. 
Try repeating your request and asking the model not to return any special characters.
)" << std::endl;
    }

    return reply;
}

bool OpenAIService::createOpenAiRequest(std::string prompt) {

    this->chat.push_back({ "user", prompt });
    
    this->openAiRequest["model"] = this->model;

    json json_messages = json::array();
    for (ChatEntry c : this->chat) {
        json_messages.push_back({ {"role", c.role}, {"content", c.content} });
    }
    this->openAiRequest["messages"] = json_messages;
    
    return true;
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
