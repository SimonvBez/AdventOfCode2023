#include <iostream>
#include <vector>
#include <algorithm>
#include "common.hpp"


struct range {
    uint64_t start;
    uint64_t length;

    uint64_t end() const {
        return start + length;
    }

    range intersect(range& other) {
        uint64_t in_end = other.end();
        uint64_t this_end = end();
        if (other.start < start) {
            if (in_end < start) {
                return { 0, 0 };
            }
            return { start, std::min(in_end, this_end) - start };
        }
        else {
            if (other.start >= this_end) {
                return { 0, 0 };
            }
            return { other.start, std::min(in_end, this_end) - other.start };
        }
    }

    bool operator<(const range& right) {
        return start < right.start;
    }
};


struct conversion_map {
    uint64_t dest_start;
    range source_range;

    inline bool is_in_range(uint64_t in_val) const {
        return in_val >= source_range.start && in_val < source_range.end();
    }
};


class Part1Solution {
private:
    std::vector<uint64_t> seed_values;
    std::vector<char> changed_values_mask;

public:
    Part1Solution(std::vector<uint64_t>& values) :
        seed_values(values),
        changed_values_mask(seed_values.size(), 0)
    {}

    void end_conversion() {
        std::fill(changed_values_mask.begin(), changed_values_mask.end(), 0);
    }

    void process_map(conversion_map& map) {
        for (int i = 0; i < seed_values.size(); ++i) {
            if (!changed_values_mask[i] && map.is_in_range(seed_values[i])) {
                seed_values[i] = seed_values[i] - map.source_range.start + map.dest_start;
                changed_values_mask[i] = 1;
            }
        }
    }

    uint64_t get_minimum() {
        return *std::min_element(seed_values.begin(), seed_values.end());
    }
};


class Part2Solution {
private:
    std::vector<range> current_ranges;
    std::vector<range> new_ranges;

public:
    Part2Solution(std::vector<uint64_t>& values) {
        for (int i = 0; i < values.size(); i += 2) {
            current_ranges.emplace_back(values[i], values[i+1]);
        }
    }

    void end_conversion() {
        // Add the remaining current ranges to the new ranges
        for (range& current_range : current_ranges) {
            if (current_range.length) {
                new_ranges.push_back(current_range);
            }
        }
        // Turn new_ranges into current_ranges, and clear new_ranges
        std::swap(current_ranges, new_ranges);
        new_ranges.clear();
    }

    void process_map(conversion_map& map) {
        for (int i = 0; i < current_ranges.size(); ++i) {
            range& current_range = current_ranges[i];
            if (!current_range.length) {
                continue;
            }
            range intersection_range = map.source_range.intersect(current_range);
            if (intersection_range.length) {
                // Remove the intersecting range from the current ranges
                if (current_range.start < intersection_range.start) {
                    if (current_range.end() > intersection_range.end()) {
                        current_ranges.emplace_back(intersection_range.end(), current_range.end() - intersection_range.end());
                    }
                    current_range.length = intersection_range.start - current_range.start;
                }
                else {
                    if (current_range.end() > intersection_range.end()) {
                        current_range.start = intersection_range.end();
                    }
                    current_range.length -= intersection_range.length;
                }

                // Add the intersecting values to the new ranges
                new_ranges.emplace_back(intersection_range.start - map.source_range.start + map.dest_start, intersection_range.length);
            }
        }
    }

    uint64_t get_minimum() {
        return (*std::min_element(current_ranges.begin(), current_ranges.end())).start;
    }
};


int main() {
    std::ifstream file_in = open_input(5);
    Stopwatch sw;

    std::vector<uint64_t> values;
    std::string line;
    std::getline(file_in, line); // Read the first line with the seeds

    const char* line_end = line.c_str() + line.size();
    const char* number_start = std::find(line.c_str(), line_end, ' ') + 1;

    // Read the seed values
    while (number_start < line_end) {
        uint64_t value;
        number_start = std::from_chars(number_start, line_end, value).ptr + 1;
        values.push_back(value);
    }

    Part1Solution part1_solver(values);
    Part2Solution part2_solver(values);

    file_in.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Skip the blank line
    while (file_in) {
        file_in.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Skip the line saying what is being converted
        while (std::getline(file_in, line) && line.size()) {
            number_start = line.c_str();
            line_end = line.c_str() + line.size();

            // Parse the conversion map
            conversion_map map;
            number_start = std::from_chars(number_start, line_end, map.dest_start).ptr + 1;
            number_start = std::from_chars(number_start, line_end, map.source_range.start).ptr + 1;
            std::from_chars(number_start, line_end, map.source_range.length);

            // Process the map
            part1_solver.process_map(map);
            part2_solver.process_map(map);
        }
        // Let the solvers know this map section is over
        part1_solver.end_conversion();
        part2_solver.end_conversion();
    }
    sw.stop();
    
    std::cout << "Part 1: " << part1_solver.get_minimum() << std::endl;
    std::cout << "Part 2: " << part2_solver.get_minimum() << std::endl;
    std::cout << "Took " << sw.get_us() << " us" << std::endl;
}
