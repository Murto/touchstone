#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>

namespace touchstone {

class json_node;
typedef std::vector<json_node> json_array;
typedef std::string json_string;
typedef std::map<json_string, json_node> json_object;
typedef std::pair<json_string, json_node> json_member;
typedef double json_number;
typedef bool json_bool;

enum class json_type {
	OBJECT,
	ARRAY,
	STRING,
	NUMBER,
	BOOL,
	NONE
};

}
