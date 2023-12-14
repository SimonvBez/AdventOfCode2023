#pragma once
#include <fstream>
#include <chrono>


std::ifstream open_input(int day);

std::vector<std::string> read_lines(std::ifstream& file);


template <typename T>
class Array2D {
private:
	size_t _height;
	size_t _width;
	size_t _length;
	T* _data;

public:
	Array2D(size_t height, size_t width): _height(height), _width(width), _length(height * width) {
		_data = new T[_length];
	}

	Array2D() : _data(nullptr) {}

	~Array2D() {
		if (_data) {
			delete[] _data;
		}
	}

	void set_size(size_t height, size_t width) {
		if (_data) {
			delete[] _data;
		}
		_height = height;
		_width = width;
		_length = height * width;
		_data = new T[_length];
	}

	T get(size_t y, size_t x) {
		return _data[y * _width + x];
	}

	void set(size_t y, size_t x, T val) {
		_data[y * _width + x] = val;
	}

	size_t height() {
		return _height;
	}

	size_t width() {
		return _width;
	}

	T* data() {
		return _data;
	}

	size_t total_size() {
		return _length;
	}

	T* row_data(size_t y) {
		return _data + y * _width;
	}

	void fill(T val) {
		std::fill(_data, _data + _length, val);
	}
};


class Stopwatch {
private:
	std::chrono::steady_clock::time_point tbegin;
	std::chrono::steady_clock::time_point tend;

public:
	Stopwatch() : tbegin(std::chrono::high_resolution_clock::now()) {}

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
