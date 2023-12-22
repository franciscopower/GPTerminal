# GPTerminal - Your AI Powershell assistant

*Use natural Language to generate Windows Powershell commands (using the OpenAI chat-gpt-3.5 API)*

## Instalation

1. Download the latest release from the [Releases page on Github](https://github.com/franciscopower/GPTerminal/releases);
2. Create an account in [OpenAI](https://openai.com/blog/openai-api);
3. Create an API key for the OpenAI API;
4. In your computer, create an **Environmental Variable* called "OPENAI_API_KEY" and paste your API key in the value field;
5. Still in the environmental variables editor, add the folder where you placed the GPTerminal.exe file to the path;

## Usage

### Powershell help

Start by describing the command you want to create. 
Eg.

```ps
GPTerminal.exe list all files in the current directory that have the .txt extension
```
GPT will generate a command to fullfill that request.
Once the code is generated, use the *tab* or *arrow* keys to chose an option (copy to clipboard, improve, explain or quit).

### Chat
To have a normal chat with chat-gpt, just use the command 
```ps
GPTerminal.exe chat
```

## Contributing
Feel free to contribute to the project, report bugs, request features or even review my code. 
I created this project to learn more about C++, so you may find many better ways to do certain things, and I'm happy to learn!

## Dependencies

- [nlohmann/json](https://github.com/nlohmann/json)
- [libcurl](https://curl.se/libcurl/)

