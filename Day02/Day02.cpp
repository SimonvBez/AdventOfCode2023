#include <iostream>
#include <string>
#include "common.hpp"

int main() {
    std::ifstream file_in = open_input(2);
    Stopwatch sw;

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
        const char* line_end = line.c_str() + line.size();
        const char* number_start = std::find(line.c_str() + 5, line_end, ' ') + 1; // Move over the second ' ' to get to the start of the first block number
        while (number_start < line_end) {
            int number;
            const char* number_end = std::from_chars(number_start, line_end, number).ptr; // Read the number of blocks
            const char* color_start = number_end + 1;

            char color = *color_start; // Get the first char of the color, 'r', 'g' or 'b'
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

            number_start = std::find(color_start, line_end, ' ') + 1;  // Find the ' ' before the start of the next number, then +1 to get to the number
        }

        if (game_is_possible) {
            id_sum += game_id;
        }
        power_sum += min_red * min_green * min_blue;
        game_id++;
    }
    sw.stop();

    std::cout << "Part 1: " << id_sum << std::endl;
    std::cout << "Part 2: " << power_sum << std::endl;
    std::cout << "Took " << sw.get_us() << " us" << std::endl;
}
