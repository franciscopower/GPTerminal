#include "Frame.h"
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

#define COLOR_RESET   "\033[0m"
#define COLOR_BLACK   "\033[30m"      /* Black */
#define COLOR_RED     "\033[31m"      /* Red */
#define COLOR_GREEN   "\033[32m"      /* Green */
#define COLOR_YELLOW  "\033[33m"      /* Yellow */
#define COLOR_BLUE    "\033[34m"      /* Blue */
#define COLOR_MAGENTA "\033[35m"      /* Magenta */
#define COLOR_CYAN    "\033[36m"      /* Cyan */
#define COLOR_WHITE   "\033[37m"      /* White */
#define COLOR_DARKGRAY   "\033[90m"      /* GRAY */
#define COLOR_BOLD	  "\033[1m"		  /* Bold */
#define COLOR_UNDERLINE "\033[1m"		  /* Underline */

#define HLINE     "─"
#define VLINE     "│"
#define ULCORNER  "┌"
#define URCORNER  "┐"
#define LRCORNER  "┘"
#define LLCORNER  "└"

Frame::Frame(std::string title) {
	this->title = title;
}
Frame::Frame() {
	this->title = "";
}

void Frame::setContent(std::string content) {
	this->content = content;
}

std::string Frame::draw() {

	const char border_color[] = COLOR_DARKGRAY;

	std::string output;

	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int winColumns = w.ws_col;


	output.append(border_color);
	output.append(ULCORNER);
	int restOfLineLenght = winColumns - 2; 
	// add title
	if (this->title != "") {
		output.append(" ");
		output.append(this->title);
		output.append(" ");
		restOfLineLenght = restOfLineLenght - 2 - int(this->title.length());
	}
	//draw start line
	for (int i = 0; i < restOfLineLenght; i++) {
		output.append(HLINE);
	}
	output.append(URCORNER);
	output.append(COLOR_RESET);
	
	//split string into lines
	std::vector<std::string> contentLines = split(content, "\n");

	//split lines into words
	size_t lineLengh = 0;
	std::vector<std::string> contentWords;
	for (std::string l : contentLines) {
		contentWords = split(l, " ");

		lineLengh = 0;
		output.append(border_color);
		output.append(VLINE); // vertical line at start of line
		output.append(COLOR_RESET);
		for (std::string w : contentWords) {
			lineLengh += w.length();

			// if word exceeds line lenght, fill the rest of the line with 
			// blank space and place the word in a new line
			if (lineLengh > winColumns - 4) {
				while (lineLengh-w.length() < winColumns - 2) { 
					output.append(" ");
					lineLengh++;
				}
				output.append(border_color);
				output.append(VLINE); 
				output.append("\n");
				output.append(VLINE);
				output.append(COLOR_RESET);
				output.append(" "); // blank space at the beginning
				lineLengh = w.length() + 1;
			}
			else {
				output.append(" "); //blank space between words and at the beginning
				lineLengh++;
			}
			output.append(w); //append word
		}
		// fill in rest of line with blank space
		while (lineLengh < winColumns-2) {
			output.append(" ");
			lineLengh++;
		}

		output.append(border_color);
		output.append(VLINE); // vertical line at the end of line
		output.append(COLOR_RESET);
		output.append("\n");
	}

	//draw end line
	output.append(border_color);
	output.append(LLCORNER);
	for (int i = 0; i < winColumns-2; i++) {
		output.append(HLINE);
	}
	output.append(LRCORNER);
	output.append(COLOR_RESET);


 
	return output;
}

std::vector<std::string> Frame::split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}


//--------------------------

//void exampleUse() {
//
//	Frame outputFrame(std::string("this is a test string"));
//	std::cout << outputFrame.draw() << std::endl;
//
//
//	outputFrame.setContent("Lorem Ipsum is simply dummy text of the printing and typesetting industry.\nLorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.");
//	std::cout << outputFrame.draw() << std::endl;
