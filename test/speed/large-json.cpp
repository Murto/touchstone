#include "touchstone.hpp"

#include <chrono>
#include <fstream>
#include <iostream>

int main() {
	using namespace touchstone::parsing;
	std::ifstream fs("test/speed/large-json.json");
	auto start = std::chrono::steady_clock::now();
	parseJSON(fs);
	auto end = std::chrono::steady_clock::now();
	std::cout << "Time elapsed:\n";
	std::cout << "--Seconds elapsed:      " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << '\n';
	std::cout << "--Milliseconds elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << '\n';
	std::cout << "--Microseconds elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << '\n';
	std::cout << "--Nanoseconds elapsed:  " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << '\n';
	std::cout.flush();
}

