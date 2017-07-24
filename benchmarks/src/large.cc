#include "file_map.hh"
#include "json_node.hh"
#include "parsing.hh"

#include <chrono>
#include <fstream>
#include <iostream>

void print_time_elapsed(const std::chrono::time_point<std::chrono::steady_clock>& start, const std::chrono::time_point<std::chrono::steady_clock>& end);

int main() {
	using namespace touchstone;
	file_map mapping("benchmarks/json/large.json");
	auto start = std::chrono::steady_clock::now();
	json_node node = parse(mapping.cbegin(), mapping.cend());
	auto end = std::chrono::steady_clock::now();
	std::cout << "Time elapsed for file parse:\n";
	print_time_elapsed(start, end);
	std::ofstream ofs("/dev/null");
	start = std::chrono::steady_clock::now();
	ofs << node;
	end = std::chrono::steady_clock::now();
	std::cout << "\nTime elapsed for file write:\n";
	print_time_elapsed(start, end);
	std::cout << "\nPress enter to quit." << std::endl;
	std::cin.get();
}

void print_time_elapsed(const std::chrono::time_point<std::chrono::steady_clock>& start, const std::chrono::time_point<std::chrono::steady_clock>& end) {
	std::cout << "--Seconds elapsed:      " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << '\n';
	std::cout << "--Milliseconds elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << '\n';
	std::cout << "--Microseconds elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << '\n';
	std::cout << "--Nanoseconds elapsed:  " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << '\n';
}


