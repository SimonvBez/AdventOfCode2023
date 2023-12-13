#include <iostream>
#include <vector>
#include <algorithm>
#include "common.hpp"


struct position {
    uint32_t y;
    uint32_t x;
};


void expand_galaxies(std::vector<position>& galaxies, uint32_t& max_y, uint32_t& max_x, uint32_t expansion_amount) {
    for (uint32_t y = 0; y < max_y; ++y) {
        if (std::find_if(galaxies.begin(), galaxies.end(), [y](position galaxy) { return galaxy.y == y; }) == galaxies.end()) {
            // This row does not have any galaxies
            for (position& galaxy : galaxies) {
                if (galaxy.y > y) {
                    galaxy.y += expansion_amount;
                }
            }
            max_y += expansion_amount;
            y += expansion_amount; // Skip the rows which were just created by the expansion
        }
    }
    for (uint32_t x = 0; x < max_x; ++x) {
        if (std::find_if(galaxies.begin(), galaxies.end(), [x](position galaxy) { return galaxy.x == x; }) == galaxies.end()) {
            // This column does not have any galaxies
            for (position& galaxy : galaxies) {
                if (galaxy.x > x) {
                    galaxy.x += expansion_amount;
                }
            }
            max_x += expansion_amount;
            x += expansion_amount; // Skip the columns which were just created by the expansion
        }
    }
}


uint64_t get_galaxy_distance_sum(std::vector<position>& galaxies) {
    uint64_t distance_sum = 0;
    for (size_t i = 0; i < galaxies.size(); ++i) {
        for (size_t j = i + 1; j < galaxies.size(); ++j) {
            distance_sum += llabs((int64_t)galaxies[i].x - galaxies[j].x) + llabs((int64_t)galaxies[i].y - galaxies[j].y);
        }
    }
    return distance_sum;
}


int main() {
    std::ifstream file_in = open_input(11);

    // Parse the galaxy locations
    std::vector<position> galaxies;
    uint32_t max_y = 0;
    uint32_t max_x = 0;
    uint32_t line_number = 0;
    std::string line;
    while (std::getline(file_in, line)) {
        auto galaxy_it = line.begin();
        while ((galaxy_it = std::find(galaxy_it, line.end(), '#')) != line.end()) {
            uint32_t row = (uint32_t)(galaxy_it - line.begin());
            max_y = std::max(max_y, line_number);
            max_x = std::max(max_x, row);
            galaxies.emplace_back(line_number, row);
            galaxy_it++;
        }
        line_number++;
    }

    // Expand the space
    expand_galaxies(galaxies, max_y, max_x, 1);
    uint64_t distance_sum_1 = get_galaxy_distance_sum(galaxies);

    // Expand the space again. 499,999 is used to account for the previous expansion
    expand_galaxies(galaxies, max_y, max_x, 499'999);
    uint64_t distance_sum_2 = get_galaxy_distance_sum(galaxies);

    std::cout << "Part 1: " << distance_sum_1 << std::endl;
    std::cout << "Part 2: " << distance_sum_2 << std::endl;
}
