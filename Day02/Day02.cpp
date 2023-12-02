#include <iostream>
#include <string>
#include "common.hpp"

int main() {
    std::ifstream file_in = open_input(2);

    // Maximum number of blocks for a game to be possible
    int red_max_possible = 12;
    int green_max_possible = 13;
    int blue_max_possible = 14;

    int id_sum = 0;
    int power_sum = 0;
    int game_id = 1;
    std::string line;
    while (std::getline(file_in, line)) {
        bool game_is_possible = true;
        int min_red = 0;
        int min_green = 0;
        int min_blue = 0;
        size_t number_start_i = line.find(": ", 6) + 2; // Move over the ": " to get to the start of the first block number
        size_t number_end_i;
        while (true) {
            number_end_i = line.find(' ', number_start_i); // Find the end of the block number
            int number;
            std::from_chars(line.data() + number_start_i, line.data() + number_end_i, number); // Read the number of blocks

            char color = line[number_end_i + 1]; // Get the first char of the color, 'r', 'g' or 'b'
            int max_possible;
            // Set the max possible blocks of this color, and set the minimum amount of blocks that is at least in the bag
            switch (color) {
            case 'r':
                max_possible = red_max_possible;
                min_red = std::max(number, min_red);
                break;
            case 'g':
                max_possible = green_max_possible;
                min_green = std::max(number, min_green);
                break;
            case 'b':
                max_possible = blue_max_possible;
                min_blue = std::max(number, min_blue);
                break;
            }

            if (number > max_possible) {
                game_is_possible = false;
            }

            number_start_i = line.find(' ', number_end_i + 1); // Find the ' ' before the start of the next number
            if (number_start_i == std::string::npos) {
                break; // End of this line
            }
            else {
                number_start_i++; // Set to the start of the next number
            }
        }

        if (game_is_possible) {
            id_sum += game_id;
        }
        power_sum += min_red * min_green * min_blue;
        game_id++;
    }

    std::cout << "Part 1: " << id_sum << std::endl;
    std::cout << "Part 2: " << power_sum << std::endl;
}
