#pragma once

#include "json_node.hh"
#include "types.hh"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <iterator>
#include <list>
#include <set>
#include <string>
#include <type_traits>

namespace touchstone {

template<typename input_it_t>
json_node parse(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end);
template<typename input_it_t>
json_object parse_object(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end);
template<typename input_it_t>
json_member parse_member(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end);
template<typename input_it_t>
json_array parse_array(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end);
template<typename input_it_t>
json_string parse_string(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end);
template<typename input_it_t>
json_number parse_number(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end);
template<typename input_it_t>
json_bool parse_bool(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end);
template<typename input_it_t>
void parse_null(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end);

const std::set<char> whitespace{0x20, 0x09, 0x0A, 0x0D};

template<typename input_it_t>
inline bool skip_whitespace(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end) {
	while (start != end && whitespace.find(*start) != whitespace.end())
		++start;

	if (start == end)
		throw std::runtime_error{"Unexpected end of string."};
}

template<typename input_it_t>
json_node parse(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end) {
	skip_whitespace(start, end);	
	switch (*start) {
		case '{':
			return json_node(parse_object(start, end));
		case '[':
			return json_node(parse_array(start, end));
		case '\"':
			return json_node(parse_string(start, end));
		case '-':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '0':
			return json_node(parse_number(start, end));
		case 't':
		case 'f':
			return json_node(parse_bool(start, end));
		case 'n':
			parse_null(start, end);
			return json_node();
	}

	throw std::runtime_error(std::string("Expected JSON value token, got '") + *start += '\'');
}

template<typename input_it_t>
json_object parse_object(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end) {
	skip_whitespace(start, end);
	if (*start != '{')
		throw std::runtime_error{std::string("Expected '{', got '") + *start += '\''};
	
	json_object obj;
	skip_whitespace(start, end);
	if (*start == '}') {
		++start;
		return obj;
	}

	while (++start != end) {
		obj.emplace(parse_member(start, end));
		skip_whitespace(start, end);
		if (*start == '}') {
			++start;
			return obj;
		} else if (*start != ',') {
			throw std::runtime_error{std::string("Expected ',', got '") + *start += '\''};
		}
	}

	throw std::runtime_error{"Unexpected end of string."};
}

template<typename input_it_t>
inline json_member parse_member(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end) {
	json_string key{parse_string(start, end)};
	skip_whitespace(start, end);
	if (*start != ':')
		throw std::runtime_error{std::string("Expected ':', got '") + *start += '\''};
	
	if (++start == end)
		throw std::runtime_error{"Unexpected end of string."};
	
	return {key, parse(start, end)};
}

template<typename input_it_t>
json_array parse_array(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end) {
	skip_whitespace(start, end);
	if (*start != '[')
		throw std::runtime_error{std::string("Expected '[', got '") + *start += '\''};
	
	std::list<json_node> values;
	skip_whitespace(start, end);
	if (*start == ']') {
		++start;
		return {std::make_move_iterator(values.begin()), std::make_move_iterator(values.end())};
	}

	while (++start != end) {
		values.emplace_back(parse(start, end));
		skip_whitespace(start, end);
		if (*start == ']') {
			++start;
			return {std::make_move_iterator(values.begin()), std::make_move_iterator(values.end())};
		} else if (*start != ',') {
			throw std::runtime_error{std::string("Expected ',', got '") + *start += '\''};
		}
	}

	throw std::runtime_error{"Unexpected end of string."};
}

template<typename input_it_t>
inline void parse_unicode_sequence(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end, std::string& out) {
	for (int i = 0; i < 4; ++i) {
		if (!isxdigit(*start))
			throw std::runtime_error{std::string("Expected hexadecimal, got '") + *start += '\''};

		out += *start;
		++start;
	}
}

template<typename input_it_t>
json_string parse_string(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end) {
	skip_whitespace(start, end);
	if (*start != '\"')
		throw std::runtime_error{std::string("Expected '\"', got '") + *start += '\''};

	std::string str;
	while (++start != end) {
		if (*start == '\"') {
			++start;
			return str;
		}

		str += *start;
		if (*start == '\\' && ++start != end) {
			str += *start;
			if (*start == 'u' && std::distance(start, end) > 4)
				parse_unicode_sequence(++start, end, str);
		}
	}

	throw std::runtime_error{"Unexpected end of string."};
}

template<typename input_it_t>
json_number parse_number(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end) {
	skip_whitespace(start, end);
	std::string num;
	if (*start == '-') {
		num += *start;
		++start;
	}

	if (start == end)
		throw std::runtime_error{"Unexpected end of string."};
	
	if (*start == '0') {
		num += *start;
		++start;
	} else if (isdigit(*start)) {
		do {
			num += *start;
			if (++start == end)
				return std::stod(num);
		} while (isdigit(*start));
	} else {
		throw std::runtime_error{std::string("Expected digit, got '") + *start += '\''};
	}

	if (*start == '.') {
		num += *start;
		if (++start == end)
			throw std::runtime_error{"Unexpected end of string."};

		if (!isdigit(*start))
			throw std::runtime_error{std::string("Expected digit, got '") + *start += '\''};

		do {
			num += *start;
			if (++start == end)
				return std::stod(num);
		} while (isdigit(*start));
	}

	if (*start == 'e' || *start == 'E') {
		num += *start;
		if (++start == end)
			throw std::runtime_error{"Unexpected end of string."};

		if (*start == '+' || *start == '-') {
			num += *start;
			if (++start == end)
				throw std::runtime_error{"Unexpected end of string."};
		}

		if (!isdigit(*start))
			throw std::runtime_error{std::string("Expected digit, got '") + *start += '\''};

		do {
			num += *start;
		} while (++start != end && isdigit(*start));
	}
	return std::stod(num);
}

template<typename input_it_t>
json_bool parse_bool(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end) {
	skip_whitespace(start, end);
	if (*start == 't') {
		if (std::distance(start, end) < 4)
			throw std::runtime_error{"Unexpected end of string."};

		char bool_str[4] = {};
		std::copy(start, start + 4, std::begin(bool_str));
		start += 4;
		if (*(int32_t*)(bool_str + 0) == 0x65757274)
			return true;
	} else if (std::distance(start, end) >= 5 && *start == 'f') {
		if (std::distance(start, end) < 5)
			throw std::runtime_error{"Unexpected end of string."};

		char bool_str[4] = {};
		++start;
		std::copy(start, start + 4, std::begin(bool_str));
		start += 4;
		if (*(int32_t*)(bool_str + 0) == 0x65736C61)
			return false;
	}

	throw std::runtime_error{std::string("Expected 't' or 'f', got '") + *start += '\''};
}

template<typename input_it_t>
void parse_null(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end) {
	skip_whitespace(start, end);
	if (*start == 'n') {
		if (std::distance(start, end) < 4)
			throw std::runtime_error("Unexpected end of string.");

		char null_str[4] = {};
		std::copy(start, start + 4, std::begin(null_str));
		start += 4;
		if (*(int32_t*)(null_str + 0) == 0x6C6C756E)
			return;
	}

	throw std::runtime_error{std::string("Expected 'n', got '") + *start += '\''};
}

}
