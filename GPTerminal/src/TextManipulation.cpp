#include "TextManipulation.h"

#include <vector>
#include <string>
#include <Windows.h>
#include <iostream> //TODO remove

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

namespace ManipulateText {


	// Function to split a string into substrings based on a given delimiter
	std::vector<std::string> split(std::string& s, const std::string& delimiter) {
		std::vector<std::string> res;
		size_t start = 0, end = 0;

		// Handle empty delimiter case: Split string into individual characters
		if (delimiter.empty()) {
			for (char c : s) {
				res.push_back(std::string(1, c)); 
			}
			return res; 
		}

		// Find occurrences of the delimiter and split accordingly
		while ((end = s.find(delimiter, start)) != std::string::npos) {
			// Extract substring from starting position to delimiter
			res.push_back(s.substr(start, end - start));
			// Update starting position for the next search after the delimiter
			start = end + delimiter.length();
		}
		// Add the remaining part of the string (after the last delimiter or if no delimiter is found)
		res.push_back(s.substr(start));

		return res;
	}

	std::string wrap(std::string text) {

		std::string output = "";
		// get window size
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		int winColumns;

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


	// Function to color code text with code blocks and inline code highlighted
	std::string colorCode(std::string& text) {
		std::string res;

		const std::string CODE_BLOCK_DELIMITER = "```";
		const std::string CODE_LINE_DELIMITER = "`";

		std::vector<std::string> sub_sections = split(text, CODE_BLOCK_DELIMITER);

		// Flag to track if currently processing a code block (1) or not (0)
		int code_block = (text.find(CODE_BLOCK_DELIMITER) == 0);

		for (std::string sub : sub_sections) {

			// Handle code blocks
			if (code_block == 1) {
				size_t first_line_end = sub.find_first_of("\n");
				std::string language = sub.substr(0, first_line_end);
				res.append(COLOR_DARKGRAY);
				res.append(u8"\u2500");
				res.append(" " + language + " ");
				for (int i = 0; i + language.length() + 3 < 20; i++) {
					res.append(u8"\u2500");
				}
				res.append("\n");
				res.append(COLOR_YELLOW);
				res.append(sub.substr(first_line_end+1, sub.length()-2)); //append sub but removing last char, which is "\n"
				res.append(COLOR_RESET);
				res.append(COLOR_DARKGRAY);
				for (int i = 0; i < 20; i++) {
					res.append(u8"\u2500");
				}
				res.append(COLOR_RESET);
				code_block = 0;
			}
			else {
				// Handle text sections (outside code blocks)

				std::vector<std::string> sub_line = split(sub, CODE_LINE_DELIMITER);

				// Flag to track if currently processing an inline code section (1) or not (0)
				int code_line = (text.find(CODE_LINE_DELIMITER) == 0);

				// Loop through each sub-line within the current sub-section
				for (std::string s : sub_line) {
					// Handle inline code
					if (code_line == 1) {
						res.append(COLOR_YELLOW);
						res.append(s);
						res.append(COLOR_RESET);
						code_line = 0;
					}
					else {
						// Handle normal text within a sub-section
						res.append(s);
						code_line = 1;
					}
				}
				code_block = 1;
			}
		}
		return res;
	}

}
