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
