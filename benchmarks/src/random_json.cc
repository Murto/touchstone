#include "random_json.hh"

#include <ctime>
#include <fstream>
#include <ios>
#include <iostream>
#include <random>

void create_json(const char* path, unsigned long records) {
	std::fstream fs{path, std::ios_base::out | std::ios_base::trunc};
	if (!fs.good())
		throw std::runtime_error{"Could not open file."};
	
	fs << '[';
	std::mt19937 number_gen(std::time(nullptr));
	while (records > 0) {
		fs << "\n\t{\n\t\t\"string\":" << '\"';
		for (int i = 0; i < 16; ++i)
			fs << (char)((number_gen() % 26) + 97);

		fs << "\",\n\t\t\"number\":" << std::to_string(number_gen())\
			<< ",\n\t\t\"boolean\":" << (number_gen()&1 ? "true" : "false")\
			<< ",\n\t\t\"null\":" << "null\n\t}";
		if (--records > 0)
			fs << ',';
	}
	fs << "\n]";
}

void create_json(const std::string& path, unsigned long records) {
	create_json(path.c_str(), records);
}
