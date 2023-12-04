#include <iostream>
#include <string>
#include "common.hpp"


/*
start: start index of this list of numbers in the input
i: index of the number that needs to be parsed
*/
int parse_num(std::string& line, int start, int i) {
    char digit_l = line[start + i * 3] - '0';
    char digit_r = line[start + 1 + i * 3] - '0';
    if (digit_l == ' ' - '0') {
        return digit_r;
    }
    else {
        return digit_l * 10 + digit_r;
    }
}


int main() {
    std::ifstream file_in = open_input(4);

    int point_sum = 0;
    int total_scratchcards = 0;

    int scatchcards_copies[200] = { 0 };
    int game = 0;
    std::string line;
    while (std::getline(file_in, line)) {
        // Create a lookup table of all the winning numbers
        bool winning_numbers[100] = { 0 };
        for (int i = 0; i < 10; ++i) {
            winning_numbers[parse_num(line, 10, i)] = true;
        }

        // Check how many numbers are winning numbers
        int winning_count = 0;
        for (int i = 0; i < 25; ++i) {
            winning_count += winning_numbers[parse_num(line, 42, i)];
        }

        int card_count = scatchcards_copies[game] + 1;
        total_scratchcards += card_count;

        int points = 0;
        for (int i = 0; i < winning_count; ++i) {
            if (points) {
                points *= 2;
            }
            else {
                points = 1;
            }

            scatchcards_copies[game + 1 + i] += card_count;
        }

        point_sum += points;
        game++;
    }

    std::cout << "Part 1: " << point_sum << std::endl;
    std::cout << "Part 2: " << total_scratchcards << std::endl;
}
