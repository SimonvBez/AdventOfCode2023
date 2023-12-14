#include <iostream>
#include <vector>
#include <algorithm>
#include "common.hpp"


struct position {
    uint16_t y;
    uint16_t x;

    bool operator==(position const&) const = default;
};


enum direction {
    up,
    down,
    right,
    left
};


enum tile_type {
    empty,
    empty_not_enclosed,
    pipe
};


struct pos_dir {
    position pos;
    direction dir;
};


class PipeMap {
private:
    std::vector<std::string>& lines;
    Array2D<char> pipe_map;
    uint32_t pipe_count;
    uint32_t enclosed_count;

    void map_loop(position start_position) {
        // Find valid direction, and get next position
        pipe_count = 1;
        pipe_map.set(start_position.y, start_position.x, pipe);

        pos_dir current = get_start_pos_dir(start_position);
        get_next_pipe(current); // Move to the connecting pipe

        while (current.pos != start_position) {
            pipe_count++;
            pipe_map.set(current.pos.y, current.pos.x, pipe);
            get_next_pipe(current);
        }
    }

    pos_dir get_start_pos_dir(position start_position) {
        pos_dir result{ .pos = start_position };
        if (start_position.y > 0) {
            const char valid_chars[] = "|F7";
            const char* valid_chars_end = valid_chars + 3;
            result.dir = up;
            if (std::find(valid_chars, valid_chars_end, get_facing_tile(result)) != valid_chars_end) {
                return result;
            }
        }
        if (start_position.y < lines.size() - 1) {
            const char valid_chars[] = "|JL";
            const char* valid_chars_end = valid_chars + 3;
            result.dir = down;
            if (std::find(valid_chars, valid_chars_end, get_facing_tile(result)) != valid_chars_end) {
                return result;
            }
        }
        // Couldn't go up or down, so the valid options have to be left and right, just choose right
        result.dir = right;
        return result;
    }

    void mark_not_enclosed() {
        for (uint16_t y = 0; y < pipe_map.height(); ++y) {
            for (uint16_t x = 0; x < pipe_map.width(); ++x) {
                if (pipe_map.get(y, x) == empty) {
                    bool is_enclosed = tile_is_enclosed({ y, x });
                    if (is_enclosed) {
                        enclosed_count++;
                    }
                    else {
                        pipe_map.set(y, x, empty_not_enclosed);
                    }
                }
            }
        }
    }

    bool tile_is_enclosed(position pos) {
        bool is_enclosed = false;
        bool was_going_up;
        while (0 < pos.x) {
            pos.x--;

            tile_type tile = (tile_type)pipe_map.get(pos.y, pos.x);
            if (tile == empty_not_enclosed) {
                return is_enclosed;
            }
            if (tile == pipe) {
                switch (get_tile(pos)) {
                case '|':
                    is_enclosed = !is_enclosed;
                    break;
                case '7':
                    was_going_up = true;
                    break;
                case 'J':
                    was_going_up = false;
                    break;
                case 'L':
                    if (was_going_up) {
                        is_enclosed = !is_enclosed;
                    }
                    break;
                case 'F':
                    if (!was_going_up) {
                        is_enclosed = !is_enclosed;
                    }
                    break;
                }
            }
        }
        
        return is_enclosed;
    }

    char get_tile(position& pos) {
        return lines[pos.y][pos.x];
    }

    char get_facing_tile(pos_dir& current) {
        switch (current.dir) {
        case up:
            return lines[current.pos.y - 1][current.pos.x];
        case down:
            return lines[current.pos.y + 1][current.pos.x];
        case right:
            return lines[current.pos.y][current.pos.x + 1];
        case left:
            return lines[current.pos.y][current.pos.x - 1];
        }
    }

    void get_next_pipe(pos_dir& current) {
        switch (current.dir) {
        case up:
            // | 7 F
            current.pos.y--;
            switch (get_tile(current.pos)) {
            case '7':
                current.dir = left;
                break;
            case 'F':
                current.dir = right;
                break;
            }
            break;
        case down:
            // | L J
            current.pos.y++;
            switch (get_tile(current.pos)) {
            case 'L':
                current.dir = right;
                break;
            case 'J':
                current.dir = left;
                break;
            }
            break;
        case right:
            // - J 7
            current.pos.x++;
            switch (get_tile(current.pos)) {
            case 'J':
                current.dir = up;
                break;
            case '7':
                current.dir = down;
                break;
            }
            break;
        case left:
            // - L F
            current.pos.x--;
            switch (get_tile(current.pos)) {
            case 'L':
                current.dir = up;
                break;
            case 'F':
                current.dir = down;
                break;
            }
            break;
        }
    }

public:
    PipeMap(std::vector<std::string>& lines) : lines(lines), pipe_map(lines.size(), lines[0].size()) {
        position start_position;
        for (int i = 0; i < lines.size(); ++i) {
            int start_x = (int)lines[i].find('S');
            if (start_x != std::string::npos) {
                start_position.y = i;
                start_position.x = start_x;
                break;
            }
        }
        pipe_map.fill(empty);
        map_loop(start_position);
        mark_not_enclosed();
    }

    int get_pipe_length() {
        return pipe_count;
    }

    int get_enclosed_count() {
        return enclosed_count;
    }
};


int main() {
    std::ifstream file_in = open_input(10);
    Stopwatch sw;

    std::vector<std::string> lines = read_lines(file_in);
    PipeMap pipe_map(lines);

    int farthest_distance = pipe_map.get_pipe_length() / 2;
    int enclosed_count = pipe_map.get_enclosed_count();
    sw.stop();

    std::cout << "Part 1: " << farthest_distance << std::endl;
    std::cout << "Part 2: " << enclosed_count << std::endl;
    std::cout << "Took " << sw.get_us() << " us" << std::endl;
}
