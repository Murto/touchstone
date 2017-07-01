#include "touchstone.hpp"

#include <chrono>
#include <fstream>
#include <iostream>

void printTimeElapsed(const std::chrono::time_point<std::chrono::steady_clock>& start, const std::chrono::time_point<std::chrono::steady_clock>& end);

int main() {
	using namespace touchstone;
	std::ifstream ifs("benchmarks/json/large.json");
	auto start = std::chrono::steady_clock::now();
	JSONNode node = parseJSON(ifs);
	auto end = std::chrono::steady_clock::now();
	std::cout << "Time elapsed for file parse:\n";
	printTimeElapsed(start, end);
	std::ofstream ofs("/dev/null");
	start = std::chrono::steady_clock::now();
	ofs << node;
	end = std::chrono::steady_clock::now();
	std::cout << "\nTime elapsed for file write:\n";
	printTimeElapsed(start, end);
	std::cout << "\nPress enter to quit." << std::endl;
	std::cin.get();
}

void printTimeElapsed(const std::chrono::time_point<std::chrono::steady_clock>& start, const std::chrono::time_point<std::chrono::steady_clock>& end) {
	std::cout << "--Seconds elapsed:      " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << '\n';
	std::cout << "--Milliseconds elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << '\n';
	std::cout << "--Microseconds elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << '\n';
	std::cout << "--Nanoseconds elapsed:  " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << '\n';
}


