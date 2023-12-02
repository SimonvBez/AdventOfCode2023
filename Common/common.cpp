#include "common.hpp"
#include <format>


std::ifstream open_input(int day) {
	std::string path = std::format("../Inputs/input{:02}.txt", day);
	std::ifstream input_stream;

	input_stream.exceptions(std::ios::failbit | std::ios::badbit); // Enable exceptions for opening the file
	input_stream.open(path);
	input_stream.exceptions(std::ios::goodbit); // Disable exceptions

	return input_stream;
}


std::vector<std::string> read_lines(std::ifstream& file_in) {
	std::vector<std::string> result;
	std::string line;
	while (std::getline(file_in, line)) {
		result.push_back(line);
	}
	return result;
}
