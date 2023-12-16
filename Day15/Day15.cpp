#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include "common.hpp"


struct lens {
    std::string_view label;
    uint32_t focal_length;
};


std::vector<lens>::iterator find_lens(std::vector<lens>& box, std::string_view label) {
    return std::find_if(box.begin(), box.end(), [&label](lens& boxed_lens) { return boxed_lens.label == label; });
}



int main() {
    std::ifstream file_in = open_input(15);
    Stopwatch sw;

    std::string line;
    std::getline(file_in, line);

    uint32_t hash_sum = 0;
    std::array<std::vector<lens>, 256> boxes;

    const char* label_start = line.c_str();
    const char* char_ptr = line.c_str();
    size_t label_length = 0;
    uint8_t current_hash = 0;
    while (uint8_t c = *char_ptr++) {
        if (c == ',') {
            hash_sum += current_hash;
            label_start = char_ptr;
            label_length = 0;
            current_hash = 0;
            continue;
        }
        else if (c == '-') {
            auto existing_label_it = find_lens(boxes[current_hash], std::string_view(label_start, label_length));
            if (existing_label_it != boxes[current_hash].end()) {
                boxes[current_hash].erase(existing_label_it); // Remove the lens
            }
        }
        else if (c == '=') {
            std::string_view label(label_start, label_length);
            uint32_t focal_length = *char_ptr - '0';
            auto existing_label_it = find_lens(boxes[current_hash], label);
            if (existing_label_it != boxes[current_hash].end()) {
                existing_label_it->focal_length = focal_length; // Swap out the lens
            }
            else {
                boxes[current_hash].push_back({ label, focal_length }); // Add the new lens
            }
        }
        label_length++;
        current_hash += c;
        current_hash *= 17;
    }
    hash_sum += current_hash;

    uint32_t focusing_power_sum = 0;
    for (uint32_t b = 0; b < boxes.size(); ++b) {
        for (uint32_t l = 0; l < boxes[b].size(); ++l) {
            focusing_power_sum += (b + 1) * (l + 1) * boxes[b][l].focal_length;
        }
    }
    sw.stop();

    std::cout << "Part 1: " << hash_sum << std::endl;
    std::cout << "Part 2: " << focusing_power_sum << std::endl;
    std::cout << "Took " << sw.get_us() << " us" << std::endl;
}
