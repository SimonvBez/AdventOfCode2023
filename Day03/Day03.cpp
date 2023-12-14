#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <ranges>
#include "common.hpp"


struct Gear {
    int parts_attached;
    int number;
};


bool is_symbol(char c) {
    return c != '.' && (c < '0' || c > '9');
}


/*
Returns true if a character is a symbol. If the symbol is also a gear, the part number is connected to the gear
*/
bool symbol_check(char c, int char_index, int part_number, std::unordered_map<int, Gear>& gears) {
    bool result = false;
    if (is_symbol(c)) {
        result = true;
        if (c == '*') {
            if (gears.contains(char_index)) {
                gears[char_index].parts_attached++;
                gears[char_index].number *= part_number;
            }
            else {
                gears[char_index] = Gear{ 1, part_number };
            }
        }
    }
    return result;
}


/*
Returns true if a part number has any adjacent symbols. If the part number is also next to any gears, the gear will be connected to this part
*/
bool check_for_adjacent_symbols(
    std::string& previous_line,
    std::string& current_line,
    std::string& next_line,
    std::unordered_map<int, Gear>& previous_line_gears,
    std::unordered_map<int, Gear>& current_line_gears,
    std::unordered_map<int, Gear>& next_line_gears,
    int part_number,
    int number_begin_i,
    int number_end_i)
{
    bool can_look_left = number_begin_i;
    bool can_look_right = number_end_i < current_line.size();
    int begin_i = can_look_left ? number_begin_i - 1 : number_begin_i;
    int end_i = can_look_right ? number_end_i + 1 : number_end_i;

    bool result = false;

    // Check all the adjacent characters for any symbols
    if (previous_line.size()) { // Check if there is a previous line
        for (int i = begin_i; i < end_i; ++i) {
            char c = previous_line[i];
            if (symbol_check(c, i, part_number, previous_line_gears)) {
                result = true;
            }
        }
    }
    if (can_look_left && symbol_check(current_line[begin_i], begin_i, part_number, current_line_gears)) {
        result = true;
    }
    if (can_look_right && symbol_check(current_line[end_i - 1], end_i - 1, part_number, current_line_gears)) {
        result = true;
    }
    if (next_line.size()) { // Check if there is a next line
        for (int i = begin_i; i < end_i; ++i) {
            char c = next_line[i];
            if (symbol_check(c, i, part_number, next_line_gears)) {
                result = true;
            }
        }
    }
    return result;
}


int main() {
    std::ifstream file_in = open_input(3);
    Stopwatch sw;

    std::string previous_line;
    std::string current_line;
    std::string next_line;
    std::unordered_map<int, Gear> previous_line_gears(20);
    std::unordered_map<int, Gear> current_line_gears(20);
    std::unordered_map<int, Gear> next_line_gears(20);
    std::getline(file_in, current_line);
    std::getline(file_in, next_line);

    int part_number_sum = 0;
    int gear_ratio_sum = 0;
    while (current_line.size()) {
        const char* line_search_begin = current_line.c_str();
        const char* line_end = line_search_begin + current_line.size();
        while (true) {
            const char* digit_begin = std::find_if(line_search_begin, line_end, isdigit); // Get begin of part number
            if (digit_begin >= line_end) {
                break; // No more part numbers were found on this line
            }

            int part_number;
            const char* digit_end = std::from_chars(digit_begin, line_end, part_number).ptr; // Read the part number

            int number_length = (int)(digit_end - digit_begin); // The amount of characters the number takes up
            int begin_i = (int)(digit_begin - current_line.c_str()); // Begin index of the number in the line
            int end_i = (int)(digit_end - current_line.c_str()); // End index of the number in the line
            // Check if this part number is next to any symbols, and connect it to any adjacent gears
            if (check_for_adjacent_symbols(previous_line, current_line, next_line, previous_line_gears, current_line_gears, next_line_gears, part_number, begin_i, end_i)) {
                part_number_sum += part_number;
            }

            line_search_begin = digit_end; // Continue searching after this part number
        }

        // Iterate over all the completed gears on the previous line
        for (Gear gear : std::views::values(previous_line_gears)) {
            if (gear.parts_attached == 2) {
                gear_ratio_sum += gear.number;
            }
        }

        // Read the new line and move all the lines up
        previous_line.clear(); // Clear the new line in case file_in has reached the EOF
        std::getline(file_in, previous_line); // Read the new line in previous_line to avoid constructing a new string
        std::string temp = std::move(previous_line); // Store the new line in a temp veriable to later put it into next_line
        previous_line = std::move(current_line);
        current_line = std::move(next_line);
        next_line = std::move(temp);

        // Move the gears up by a line
        previous_line_gears = std::move(current_line_gears);
        current_line_gears = std::move(next_line_gears);
        next_line_gears = std::unordered_map<int, Gear>(20); // Constructing a new empty unordered_map is faster than clearing and reusing the old previous_line_gears one
    }
    // Iterate over all the completed gears on the current line
    for (Gear gear : std::views::values(current_line_gears)) {
        if (gear.parts_attached == 2) {
            gear_ratio_sum += gear.number;
        }
    }
    sw.stop();

    std::cout << "Part 1: " << part_number_sum << std::endl;
    std::cout << "Part 2: " << gear_ratio_sum << std::endl;
    std::cout << "Took " << sw.get_us() << " us" << std::endl;
}
