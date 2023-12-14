#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <numeric>
#include "common.hpp"


struct Node {
    int left_i;
    int right_i;
};


int main() {
    std::ifstream file_in = open_input(8);
    Stopwatch sw;

    std::string instructions;
    std::getline(file_in, instructions);
    file_in.ignore(1); // Skip the empty line

    std::unordered_map<std::string, int> node_to_index_map;

    std::vector lines = read_lines(file_in);
    // Read all the nodes, and store them in a map so the index can be looked up later
    int line_counter = 0;
    for (std::string& line : lines) {
        size_t node_end = line.find(' ');
        std::string node_name = line.substr(0, node_end);
        node_to_index_map.emplace(std::move(node_name), line_counter);
        line_counter++;
    }

    // Go over all the nodes again, this time storing what the indexes of each left and right nodes are
    std::vector<Node> network(node_to_index_map.size());
    line_counter = 0;
    for (std::string& line : lines) {
        size_t node_left_begin = line.find('(') + 1;
        size_t node_left_end = line.find(',', node_left_begin);
        int left_i = node_to_index_map[line.substr(node_left_begin, node_left_end - node_left_begin)];
        size_t node_right_begin = node_left_end + 2;
        size_t node_right_end = line.size() - 1;
        int right_i = node_to_index_map[line.substr(node_right_begin, node_right_end - node_right_begin)];
        network[line_counter].left_i = left_i;
        network[line_counter].right_i = right_i;
        line_counter++;
    }

    // Solve part 1
    int step_part1 = 0;
    int current_node = node_to_index_map["AAA"];
    int target_node = node_to_index_map["ZZZ"];
    while (current_node != target_node) {
        if (instructions[step_part1 % instructions.size()] == 'L') {
            current_node = network[current_node].left_i;
        }
        else {
            current_node = network[current_node].right_i;
        }
        step_part1++;
    }

    // Solve part 2
    // Get all start and end positions
    std::vector<int> start_positions;
    std::vector<int> target_positions;
    for (const auto& [name, index] : node_to_index_map) {
        if (name.back() == 'A') {
            start_positions.push_back(index);
        }
        else if (name.back() == 'Z') {
            target_positions.push_back(index);
        }
    }

    // Get the length of the cycle that each node path has
    std::vector<uint64_t> cycles;
    for (int current_position : start_positions) {
        int step = 0;
        while (true) {
            bool is_left = instructions[step % instructions.size()] == 'L';
            if (std::find(target_positions.begin(), target_positions.end(), current_position) != target_positions.end()) {
                cycles.push_back(step);
                break;
            }

            if (is_left) {
                current_position = network[current_position].left_i;
            }
            else {
                current_position = network[current_position].right_i;
            }
            step++;
        }
    }

    // Shrink the cycle lengths as much as possible, so multiplying them won't cause an integer overflow
    uint64_t max_cycle = *std::max_element(cycles.begin(), cycles.end());
    uint64_t common_divisor = 1;
    bool has_shrunk;
    do {
        has_shrunk = false;
        for (uint64_t divisor = 2; divisor <= max_cycle / 2; divisor++) {
            bool success = true;
            for (uint64_t shrink_check : cycles) {
                if (shrink_check % divisor) {
                    success = false; // Not all cycles can be divided by this divisor
                    break;
                }
            }
            if (success) {
                // Every cycle can be divided by this divisor, so shrink all the cycles
                for (uint64_t& cycle : cycles) {
                    cycle /= divisor;
                }
                max_cycle /= divisor;
                common_divisor *= divisor;
                has_shrunk = true;
                break;
            }
        }
    } while (has_shrunk);

    // Get the product of all the shrunk cycle lengths, to get the first step number where all ghosts are on an end node
    uint64_t common_multiple = std::accumulate(cycles.begin(), cycles.end(), 1ULL, std::multiplies());
    common_multiple *= common_divisor;
    sw.stop();

    std::cout << "Part 1: " << step_part1 << std::endl;
    std::cout << "Part 2: " << common_multiple << std::endl;
    std::cout << "Took " << sw.get_us() << " us" << std::endl;
}
