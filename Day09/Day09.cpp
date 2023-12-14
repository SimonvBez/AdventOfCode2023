#include <iostream>
#include <vector>
#include <numeric>
#include "common.hpp"


int rminus(int left, int right) {
    return right - left;
}


int main() {
    std::ifstream file_in = open_input(9);
    Stopwatch sw;

    int extrapolated_future_sum = 0;
    int extrapolated_history_sum = 0;
    std::string line;
    std::vector<int> numbers;
    while (std::getline(file_in, line)) {
        numbers.clear();
        // Parse the numbers
        const char* number_start = line.c_str();
        const char* line_end = line.c_str() + line.size();
        while (number_start < line_end) {
            int num;
            number_start = std::from_chars(number_start, line_end, num).ptr + 1;
            numbers.push_back(num);
        }

        // Derive the numbers until they are all 0
        size_t original_size = numbers.size();
        int levels = 0;
        bool is_all_zero;
        do {
            levels++;
            is_all_zero = true;
            numbers.push_back(numbers[original_size-1]); // Remember the previous last number for the next step
            for (size_t i = original_size-1; i >= levels; --i) {
                numbers[i] = numbers[i] - numbers[i - 1];
                if (numbers[i]) {
                    is_all_zero = false;
                }
            }
        } while (!is_all_zero);

        extrapolated_future_sum += std::reduce(numbers.end() - levels, numbers.end()); // Sum up the last numbers of each seqence
        extrapolated_history_sum += std::accumulate(numbers.rend() - levels, numbers.rend(), 0, rminus); // Subtract the first numbers of each sequence with the previous history
    }
    sw.stop();

    std::cout << "Part 1: " << extrapolated_future_sum << std::endl;
    std::cout << "Part 2: " << extrapolated_history_sum << std::endl;
    std::cout << "Took " << sw.get_us() << " us" << std::endl;
}
