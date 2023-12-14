#include <iostream>
#include <algorithm>
#include "common.hpp"


bool is_not_space(char c) {
    return c != ' ';
}


int calculate_possible_wins(uint64_t race_time, uint64_t distance_record) {
    double hold_time_low = (race_time - sqrt(race_time * race_time - 4 * distance_record)) / 2;
    double minimum_hold_time = floor(hold_time_low) + 1;
    return (int)((race_time / 2.0 - minimum_hold_time) * 2) + 1;
}


uint64_t pow_10(size_t exp) {
    uint64_t result = 1;
    for (size_t i = 0; i < exp; ++i) {
        result *= 10;
    }
    return result;
}


int main() {
    std::ifstream file_in = open_input(6);
    Stopwatch sw;

    const int race_count = 4;
    int race_times[race_count];
    int distance_records[race_count];
    uint64_t part2_race_time = 0;
    uint64_t part2_distance_record = 0;

    std::string line;
    std::getline(file_in, line); // Read the times
    const char* line_end = line.c_str() + line.size();
    const char* digit_end = line.c_str() + 10;
    for (int i = 0; i < race_count; ++i) {
        const char* digit_begin = std::find_if(digit_end, line_end, is_not_space);
        digit_end = std::from_chars(digit_begin, line_end, race_times[i]).ptr;
        part2_race_time *= pow_10(digit_end - digit_begin);
        part2_race_time += race_times[i];
    }

    std::getline(file_in, line); // Read the record distances
    line_end = line.c_str() + line.size();
    digit_end = line.c_str() + 10;
    for (int i = 0; i < race_count; ++i) {
        const char* digit_begin = std::find_if(digit_end, line_end, is_not_space);
        digit_end = std::from_chars(digit_begin, line_end, distance_records[i]).ptr;
        part2_distance_record *= pow_10(digit_end - digit_begin);
        part2_distance_record += distance_records[i];
    }

    int ways_product = 1;
    for (int i = 0; i < race_count; ++i) {
        ways_product *= calculate_possible_wins(race_times[i], distance_records[i]);
    }
    int part2_ways = calculate_possible_wins(part2_race_time, part2_distance_record);
    sw.stop();

    std::cout << "Part 1: " << ways_product << std::endl;
    std::cout << "Part 2: " << part2_ways << std::endl;
    std::cout << "Took " << sw.get_us() << " us" << std::endl;
}
