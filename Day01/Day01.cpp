#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctype.h>
#include "common.hpp"


int main() {
    std::ifstream file_in = open_input(1);

    std::vector<std::string> written_digits{ "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

    uint32_t part1_sum = 0;
    uint32_t part2_sum = 0;
    std::string line;
    while (std::getline(file_in, line)) {
        auto first_digit_it = std::find_if(line.begin(), line.end(), isdigit); // Get position of first digit
        auto last_digit_it = std::find_if(line.rbegin(), line.rend(), isdigit).base() - 1; // Get position of last digit
        int first_digit = *first_digit_it - '0';
        int last_digit = *last_digit_it - '0';
        part1_sum += first_digit * 10 + last_digit;

        std::string_view first_written_digit_view(line.begin(), first_digit_it); // View from the start to the first digit
        std::string_view last_written_digit_view(last_digit_it + 1, line.end()); // View from the last digit to the end
        // Go over all the written digits to find the first and last one
        for (int i = 0; i < written_digits.size(); ++i) {
            std::string& written_digit = written_digits[i];
            size_t new_end = first_written_digit_view.find(written_digit);
            if (new_end != std::string::npos) {
                // Found a digit that was before the previous one, store it and reduce the view
                first_digit = i;
                new_end += written_digit.size() - 1;
                first_written_digit_view = { line.begin(), line.begin() + new_end };
            }

            size_t new_begin = last_written_digit_view.rfind(written_digit);
            if (new_begin != std::string::npos) {
                // Found a digit that was after the previous one, store it and reduce the view
                last_digit = i;
                last_written_digit_view = { last_written_digit_view.begin() + new_begin, last_written_digit_view.end() };
            }
        }
        part2_sum += first_digit * 10 + last_digit;
    }

    std::cout << "Part 1: " << part1_sum << std::endl;
    std::cout << "Part 2: " << part2_sum << std::endl;
}
