#include <iostream>
#include <vector>
#include <algorithm>
#include "common.hpp"


struct potential_group {
    size_t start;
    std::vector<uint32_t> sizes;
};


class SpringRow {
private:
    std::string springs;
    std::vector<uint32_t> groups;
    Array2D<uint64_t> recursion_cache;

    std::vector<uint32_t> get_possible_group_sizes(size_t group_start_index) {
        std::vector<uint32_t> possible_groups;
        possible_groups.reserve(springs.size());
        int current_size = 0;
        for (size_t i = group_start_index; i < springs.size(); ++i) {
            char spring = springs[i];

            switch (spring) {
            case '#':
                break;
            case '?':
                possible_groups.push_back(current_size);
                break;
            case '.':
                if (current_size) {
                    possible_groups.push_back(current_size);
                }
                return possible_groups;
            }
            current_size++;
        }
        if (current_size) {
            possible_groups.push_back(current_size);
        }
        return possible_groups;
    }

    std::vector<size_t> get_possible_starts(size_t i) {
        std::vector<size_t> possible_starts;
        possible_starts.reserve(springs.size());
        bool is_in_group = false;
        for (; i < springs.size(); ++i) {
            char spring = springs[i];

            switch (spring) {
            case '#':
                if (!is_in_group) {
                    possible_starts.push_back(i);
                }
                is_in_group = true;
                break;
            case '?':
                if (!is_in_group) {
                    possible_starts.push_back(i);
                }
                is_in_group = false;
                break;
            case '.':
                is_in_group = false;
                break;
            }
        }

        return possible_starts;
    }

    uint64_t count_arrangements_cached(std::vector<potential_group>& potential_groups, size_t potential_groups_used = 0, size_t groups_filled = 0) {
        uint64_t cached_result = recursion_cache.get(potential_groups_used, groups_filled);
        if (cached_result != -1) {
            return cached_result;
        }

        uint64_t result = count_arrangements(potential_groups, potential_groups_used, groups_filled);
        recursion_cache.set(potential_groups_used, groups_filled, result);
        return result;
    }

    uint64_t count_arrangements(std::vector<potential_group>& potential_groups, size_t potential_groups_used, size_t groups_filled) {
        if (groups_filled == groups.size()) {
            return std::all_of(potential_groups.begin() + potential_groups_used, potential_groups.end(), [](potential_group& pot_group) { return pot_group.sizes[0] == 0; });
        }
        if (potential_groups.size() - potential_groups_used < groups.size() - groups_filled) {
            return 0; // Not enough potential groups remaining to fill the remaining groups
        }

        uint64_t result = 0;
        uint32_t next_group_size = groups[groups_filled];
        potential_group& pot_group = potential_groups[potential_groups_used];
        if (std::find(pot_group.sizes.begin(), pot_group.sizes.end(), next_group_size) != pot_group.sizes.end()) {
            // This potential group fits the next group size

            // Find the next potential group that starts after this one ends
            size_t group_end = pot_group.start + next_group_size;
            size_t next_potential_group = potential_groups_used + 1;
            while (next_potential_group < potential_groups.size()) {
                if (potential_groups[next_potential_group].start > group_end) {
                    break;
                }
                next_potential_group++;
            }
            result += count_arrangements_cached(potential_groups, next_potential_group, groups_filled + 1);
        }
        if (pot_group.sizes[0] == 0) {
            // This potential group may be skipped
            result += count_arrangements_cached(potential_groups, potential_groups_used + 1, groups_filled);
        }
        return result;
    }

public:
    SpringRow(std::string& line) {
        size_t space_i = line.find(' ');
        springs = line.substr(0, space_i);

        int group;
        const char* group_begin = line.c_str() + space_i + 1;
        const char* line_end = line.c_str() + line.size();
        while (group_begin < line_end) {
            group_begin = std::from_chars(group_begin, line_end, group).ptr + 1;
            groups.push_back(group);
        }
    }

    uint64_t get_arrangement_count() {
        std::vector<potential_group> potential_groups;

        for (size_t start : get_possible_starts(0)) {
            potential_groups.emplace_back(start, get_possible_group_sizes(start));
        }

        recursion_cache.set_size(potential_groups.size() + 1, groups.size() + 1);
        recursion_cache.fill(-1);

        return count_arrangements_cached(potential_groups);
    }

    void unfold() {
        size_t springs_count = springs.size();
        size_t group_count = groups.size();
        springs.reserve(springs_count * 5 + 5);
        groups.reserve(group_count * 5);
        for (int i = 0; i < 4; ++i) {
            springs += '?';
            springs.insert(springs.end(), springs.begin(), springs.begin() + springs_count);
            groups.insert(groups.end(), groups.begin(), groups.begin() + group_count);
        }
    }
};


int main() {
    std::ifstream file_in = open_input(12);

    std::vector<SpringRow> spring_rows;
    std::string line;
    while (std::getline(file_in, line)) {
        spring_rows.emplace_back(line);
    }

    uint64_t arrangement_sum1 = 0;
    uint64_t arrangement_sum2 = 0;
    for (SpringRow& row : spring_rows) {
        arrangement_sum1 += row.get_arrangement_count();
        row.unfold();
        arrangement_sum2 += row.get_arrangement_count();
    }

    std::cout << "Part 1: " << arrangement_sum1 << std::endl;
    std::cout << "Part 2: " << arrangement_sum2 << std::endl;
}
