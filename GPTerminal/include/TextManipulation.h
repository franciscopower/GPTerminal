#pragma once

#include <vector>
#include <string>

namespace ManipulateText {

	std::vector<std::string> split(std::string& s, const std::string& delimiter);

	std::string wrap(std::string text);

	std::string colorCode(std::string &text);

}
