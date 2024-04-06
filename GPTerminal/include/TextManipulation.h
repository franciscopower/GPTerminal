#pragma once

#include <vector>
#include <string>

namespace ManipulateText {

	std::vector<std::string> split(std::string& s, const std::string& delimiter);

	std::string justify(std::string text);

}
