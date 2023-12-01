#pragma once
#include <fstream>
#include <chrono>


std::ifstream open_input(int day);

class Stopwatch {
private:
	std::chrono::steady_clock::time_point tbegin;
	std::chrono::steady_clock::time_point tend;

public:
	void start() {
		tbegin = std::chrono::high_resolution_clock::now();
	}

	void stop() {
		tend = std::chrono::high_resolution_clock::now();
	}

	double get_ms() {
		return ((std::chrono::duration<double, std::milli>)(tend - tbegin)).count();
	}

	double get_us() {
		return ((std::chrono::duration<double, std::micro>)(tend - tbegin)).count();
	}

	long long get_ns() {
		return ((std::chrono::duration<long long, std::nano>)(tend - tbegin)).count();
	}
};
