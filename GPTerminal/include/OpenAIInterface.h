#pragma once
#include <string>
#include <vector>

class OpenAiInterface
{

private:

	struct Message {
		std::string role;
		std::string content;
	};

	struct Choices {
		std::string finish_reason;
		int index;
		Message message;
	};

	struct Usage
	{
		int completion_tokens;
		int prompt_tokens;
		int total_tokens;
	};

	struct Response
	{
		std::string id;
		std::vector<Choices> choices;
		int created;
		std::string model;
		std::string object;
		Usage usage;
	} response;

	struct Request
	{
		std::vector<Message> messages;
		std::string model = "gpt-3.5-turbo";
		float frequency_penalty = 0;
		int max_tokens = 500;
		int n = 1; //nr of choices to generate
		float temperature = 0.2;
		std::string user;
	} request;

public:
	OpenAiInterface();
	void setRequest(std::string message);
	std::string getResponseString();

};

void OpenAiInterface::setRequest(std::string message)
{
	this->request.messages.push_back({"user", message});
}

std::string OpenAiInterface::getResponseString() {
	return this->response.choices[0].message.content;
}




