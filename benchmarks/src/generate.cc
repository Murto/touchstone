#include "random_json.hh"

#include <iostream>

int main(int argc, char* argv[]) {
	create_json("random.json", std::stoul(argv[1]));
}
