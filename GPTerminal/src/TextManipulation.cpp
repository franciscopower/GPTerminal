#include "TextManipulation.h"

#include <vector>
#include <string>
#include <Windows.h>
#include <iostream> //TODO remove

namespace ManipulateText {


	std::vector<std::string> split(std::string s, std::string delimiter) {
		std::vector<std::string> res;
		size_t start = 0, end = 0, delim_len = delimiter.length();

		while (end < s.length()) {
			if (s.substr(end, delim_len) == delimiter) {
				res.push_back(s.substr(start, end-start));
				end += delim_len;
				start = end;
			}
			else {
				end++;
			}
		}
		res.push_back(s.substr(start, end-start));

		return res;
	}

	std::string justify(std::string text) {

		std::string output = "";
		// get window size
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		int winColumns, winRows;

		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		winColumns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		//winRows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
			
		std::vector<std::string> contentLines = split(text, "\n");

		//split lines into words
		size_t lineLengh = 0;
		std::vector<std::string> contentWords;
		for (std::string l : contentLines) {
			contentWords = split(l, " ");

			lineLengh = 0;
			for (std::string w : contentWords) {
				lineLengh += w.length();

				// if word exceeds line lenght, fill the rest of the line with 
				// blank space and place the word in a new line
				if (lineLengh > winColumns) {
					while (lineLengh-w.length() < winColumns) { 
						output.append(" ");
						lineLengh++;
					}
					output.append("\n");
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

			output.append("\n");
		}

		return output;
	}

}
