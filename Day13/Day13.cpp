#include <iostream>
#include <vector>
#include <algorithm>
#include "common.hpp"


void add_row_bitmask(std::vector<uint32_t>& row_bitmasks, std::string& line) {
    uint32_t row_mask = 0;
    for (uint32_t i = 0; i < line.size(); ++i) {
        if (line[i] == '#') {
            row_mask |= 1 << i;
        }
    }
    row_bitmasks.push_back(row_mask);
}


void add_col_bitmask(std::vector<uint32_t>& col_bitmasks, std::string& line, int row_number) {
    for (uint32_t i = 0; i < col_bitmasks.size(); ++i) {
        if (line[i] == '#') {
            col_bitmasks[i] |= 1 << row_number;
        }
    }
}


bool has_single_bit_set(uint32_t num) {
    return num && !(num & (num - 1));
}


void find_mirror(std::vector<uint32_t>& bitmasks, uint32_t& clean_mirror, uint32_t& smudged_mirror) {
    for (uint32_t mirror_pos = 1; mirror_pos < bitmasks.size(); ++mirror_pos) {
        uint32_t compare_count = std::min(mirror_pos, (uint32_t)bitmasks.size() - mirror_pos);
        auto left_it = bitmasks.rend() - mirror_pos;
        auto right_it = bitmasks.begin() + mirror_pos;
        bool is_clean_mirror = true;
        bool is_smudged_mirror = false;
        for (uint32_t i = 0; i < compare_count; ++i) {
            if (*left_it != *right_it) {
                is_clean_mirror = false;
                if (is_smudged_mirror) {
                    // A second smudge was detected. This is neither a clean nor a smudged mirror
                    is_smudged_mirror = false;
                    break;
                }
                if (has_single_bit_set(*left_it ^ *right_it)) {
                    is_smudged_mirror = true;
                }
                else {
                    // These two rows have more than one non-matching symbols
                    break;
                }
            }
            ++left_it;
            ++right_it;
        }
        if (is_clean_mirror) {
            clean_mirror = mirror_pos;
        }
        if (is_smudged_mirror) {
            smudged_mirror = mirror_pos;
        }
        if (clean_mirror && smudged_mirror) {
            return;
        }
    }
}


int main() {
    std::ifstream file_in = open_input(13);

    uint32_t summary_clean = 0;
    uint32_t summary_smudged = 0;
    std::vector<uint32_t> row_bitmasks;
    std::vector<uint32_t> col_bitmasks;
    std::string line;
    while (file_in) {
        int row_count = 0;
        // Read the first line of a pattern to know how many columns it has
        std::getline(file_in, line);
        col_bitmasks.insert(col_bitmasks.begin(), line.size(), 0);
        do {
            // For each line, read the pattern and add it to the row and column bitmasks
            add_row_bitmask(row_bitmasks, line);
            add_col_bitmask(col_bitmasks, line, row_count);

            row_count++;
        } while (std::getline(file_in, line) && line.size());

        // Reached the end of the pattern

        // Check if there are any clean or smudged horizontal mirrors
        uint32_t clean_mirror = 0;
        uint32_t smudged_mirror = 0;
        find_mirror(row_bitmasks, clean_mirror, smudged_mirror);
        summary_clean += clean_mirror * 100;
        summary_smudged += smudged_mirror * 100;

        if (!clean_mirror || !smudged_mirror) {
            // Check if there are any clean or smudged mirrors vertical mirrors
            clean_mirror = 0;
            smudged_mirror = 0;
            find_mirror(col_bitmasks, clean_mirror, smudged_mirror);
            summary_clean += clean_mirror;
            summary_smudged += smudged_mirror;
        }

        row_bitmasks.clear();
        col_bitmasks.clear();
    }

    std::cout << "Part 1: " << summary_clean << std::endl;
    std::cout << "Part 2: " << summary_smudged << std::endl;
}
