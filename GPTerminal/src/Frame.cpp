#include "Frame.h"
#include <Windows.h>
#include <iostream>

Frame::Frame(std::string title) {
	this->title = title;
}

void Frame::setContent(std::string content) {
	this->content = content;
}

std::string Frame::draw() {
	std::string output;

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int winColumns, winRows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	winColumns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	winRows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	//draw start line
	output.append(ulcorner);
	output.append(" ");
	output.append(title);
	output.append(" ");
	for (int i = 0; i < winColumns-2-title.length()-2; i++) {
		output.append(hline);
	}
	output.append(urcorner);
	
	//split string into lines
	std::vector<std::string> contentLines = split(content, "\n");

	//split lines into words
	size_t lineLengh = 0;
	std::vector<std::string> contentWords;
	for (std::string l : contentLines) {
		contentWords = split(l, " ");

		lineLengh = 0;
		output.append(vline);
		for (std::string w : contentWords) {
			lineLengh += w.length();

			if (lineLengh > winColumns - 4) {
				while (lineLengh-w.length() < winColumns - 2) {
					output.append(" ");
					lineLengh++;
				}
				output.append(vline);
				output.append("\n");
				output.append(vline);
				output.append(" ");
				lineLengh = w.length() + 1;
			}
			else {
				output.append(" ");
				lineLengh++;
			}
			output.append(w);
		}
		while (lineLengh < winColumns-2) {
			output.append(" ");
			lineLengh++;
		}

		output.append(vline);
		output.append("\n");
	}

	//draw end line
	output.append(llcorner);
	for (int i = 0; i < winColumns-2; i++) {
		output.append(hline);
	}
	output.append(lrcorner);


 
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
