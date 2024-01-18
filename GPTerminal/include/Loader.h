#pragma once

#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <codecvt>
#include <locale>

class Loader : Component
{
public:
	enum Style
	{
		DOTS,
		ARC,
		BAR
	};

	Loader(Style style, std::string text) {

		// Convert narrow string to wide string
		this->text = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(text);
			
		switch (style)
		{
		case Loader::DOTS:
			copy(begin(this->frames_dots), end(this->frames_dots), back_inserter(this->frames));
			break;
		case Loader::ARC:
			copy(begin(this->frames_arc), end(this->frames_arc), back_inserter(this->frames));
			break;
		case Loader::BAR:
			copy(begin(this->frames_bar), end(this->frames_bar), back_inserter(this->frames));
			break;
		default:
			break;
		}

	};

	std::string draw() {
		throw std::logic_error("Use `wdraw()` instead (outputs a std::wstring instead of std::string)");
	}

	std::wstring wdraw() {
		std::wstring output = L"";

		output.append(L"\r"); // carriage return
		output.append(L"\x1b[2K"); //clear line

		std::this_thread::sleep_for(std::chrono::milliseconds(80));

		this->index++;
		if (this->index >= this->frames.size()) {
			this->index = 0;
		}

		output.append(this->frames[this->index]);
		output.append(L" ");
		output.append(this->text);

		return output;
	}

private:

	size_t index = 0;
	std::wstring text;

	std::vector<std::wstring> frames;

	const std::wstring frames_dots[10] = {
		L"⠋",
		L"⠙",
		L"⠹",
		L"⠸",
		L"⠼",
		L"⠴",
		L"⠦",
		L"⠧",
		L"⠇",
		L"⠏"
	};

	const std::wstring frames_arc[6] = {
		L"◜",
		L"◠",
		L"◝",
		L"◞",
		L"◡",
		L"◟" 
	};

	const std::wstring frames_bar[4] = {
		L"|",
		L"/",
		L"-",
		L"\\"
	};

};

//---------------------------------------------
//void exampleUse{
//
//	Loader loader(Loader::BALL, "Loading...");
//
//	while (RESULT == false) {
//		std::cout << loader.draw();
//	}
//	std::cout << "\r\x1b[2K"; //carriage return and clear line
//}
