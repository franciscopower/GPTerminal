#pragma once

#include <vector>
#include <string>
// #include <thread>
// #include <chrono>
#include <unistd.h>

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
		this->text = text;

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
		std::string output = "";

		output.append("\r"); // carriage return
		output.append("\x1b[2K"); //clear line

		// std::this_thread::sleep_for(std::chrono::milliseconds(80));
		usleep(80);

		this->index++;
		if (this->index >= this->frames.size()) {
			this->index = 0;
		}

		output.append(this->frames[this->index]);
		output.append(" ");
		output.append(this->text);

		return output;
	}

private:

	size_t index = 0;
	std::string text;

	std::vector<std::string> frames;

	const std::string frames_dots[10] = {
		"⠋",
		"⠙",
		"⠹",
		"⠸",
		"⠼",
		"⠴",
		"⠦",
		"⠧",
		"⠇",
		"⠏"
	};

	const std::string frames_arc[6] = {
		"◜",
		"◠",
		"◝",
		"◞",
		"◡",
		"◟" 
	};

	const std::string frames_bar[4] = {
		"|",
		"/",
		"-",
		"\\"
	};

};

//---------------------------------------------
//void exampleUse{
//
//	u8oader loader(Loader::BALL, "Loading...");
//
//	while (RESULT == false) {
//		std::cout << loader.draw();
//	}
//	std::cout << "\r\x1b[2K"; //carriage return and clear line
//}
