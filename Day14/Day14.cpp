#include <iostream>
#include <vector>
#include <algorithm>
#include "common.hpp"


uint32_t get_north_beam_load(Array2D<char>& rocks) {
    uint32_t result = 0;
    for (size_t y = 0; y < rocks.height(); ++y) {
        for (size_t x = 0; x < rocks.width(); ++x) {
            if (rocks.get(y, x) == 'O') {
                result += (uint32_t)(rocks.height() - y);
            }
        }
    }
    return result;
}


void slide_rocks_up(Array2D<char>& rocks) {
    for (size_t x = 0; x < rocks.width(); ++x) {
        size_t new_rock_pos = 0;
        for (size_t y = new_rock_pos; y < rocks.height(); ++y) {
            switch (rocks.get(y, x)) {
            case 'O':
                rocks.set(y, x, '.');
                rocks.set(new_rock_pos, x, 'O');
                new_rock_pos++;
                break;
            case '#':
                new_rock_pos = y + 1;
                break;
            }
        }
    }
}


void slide_rocks_down(Array2D<char>& rocks) {
    for (int x = 0; x < rocks.width(); ++x) {
        int new_rock_pos = (uint32_t)rocks.height() - 1;
        for (int y = new_rock_pos; y >= 0; --y) {
            switch (rocks.get(y, x)) {
            case 'O':
                rocks.set(y, x, '.');
                rocks.set(new_rock_pos, x, 'O');
                new_rock_pos--;
                break;
            case '#':
                new_rock_pos = y - 1;
                break;
            }
        }
    }
}


void slide_rocks_left(Array2D<char>& rocks) {
    for (size_t y = 0; y < rocks.height(); ++y) {
        size_t new_rock_pos = 0;
        for (size_t x = new_rock_pos; x < rocks.width(); ++x) {
            switch (rocks.get(y, x)) {
            case 'O':
                rocks.set(y, x, '.');
                rocks.set(y, new_rock_pos, 'O');
                new_rock_pos++;
                break;
            case '#':
                new_rock_pos = x + 1;
                break;
            }
        }
    }
}


void slide_rocks_right(Array2D<char>& rocks) {
    for (int y = 0; y < rocks.height(); ++y) {
        int new_rock_pos = (int)rocks.width() - 1;
        for (int x = new_rock_pos; x >= 0; --x) {
            switch (rocks.get(y, x)) {
            case 'O':
                rocks.set(y, x, '.');
                rocks.set(y, new_rock_pos, 'O');
                new_rock_pos--;
                break;
            case '#':
                new_rock_pos = x - 1;
                break;
            }
        }
    }
}


void do_tilt_cycle(Array2D<char>& rocks) {
    slide_rocks_up(rocks);
    slide_rocks_left(rocks);
    slide_rocks_down(rocks);
    slide_rocks_right(rocks);
}


int main() {
    std::ifstream file_in = open_input(14);
    Stopwatch sw;
    uint32_t cycles_before_loop = 96;

    std::vector<std::string> lines = read_lines(file_in);

    // Copy the lines in a contiguous memory array
    Array2D<char> rocks(lines.size(), lines[0].size());
    for (size_t y = 0; y < rocks.height(); ++y) {
        std::copy(lines[y].begin(), lines[y].end(), rocks.row_data(y));
    }

    // Slide the rocks up, get the beam load and complete the cycle
    slide_rocks_up(rocks);
    uint32_t north_beam_load_part1 = get_north_beam_load(rocks);
    slide_rocks_left(rocks);
    slide_rocks_down(rocks);
    slide_rocks_right(rocks);

    // Make a total of cycles_before_loop cycles to reach the inside of the repeating loop
    for (uint64_t i = 0; i < cycles_before_loop - 1; ++i) {
        do_tilt_cycle(rocks);
    }
    // Make a copy of the current rocks
    std::vector<char> snapshot(rocks.data(), rocks.data() + rocks.total_size());

    // Keep doing cycles until the exact same rock layout has returned
    uint32_t cycle_size = 0;
    while (true) {
        do_tilt_cycle(rocks);
        cycle_size++;
        if (std::equal(snapshot.begin(), snapshot.end(), rocks.data())) {
            break;
        }
    }
    // Calculate how many cycles there actually still have to be done to get the result of 1000000000 cycles
    uint32_t cycles_to_go = (1000000000 - cycles_before_loop - cycle_size) % cycle_size;
    for (uint32_t i = 0; i < cycles_to_go; ++i) {
        do_tilt_cycle(rocks);
    }
    // Get the beam load
    uint32_t north_beam_load_part2 = get_north_beam_load(rocks);
    sw.stop();

    std::cout << "Part 1: " << north_beam_load_part1 << std::endl;
    std::cout << "Part 2: " << north_beam_load_part2 << std::endl;
    std::cout << "Took " << sw.get_us() << " us" << std::endl;
}
