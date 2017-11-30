#pragma once

#include "json_node.hh"

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

template<typename t_input_it>
json_node parse(t_input_it&& p_start, const typename std::remove_reference<t_input_it>::type p_end);
template<typename t_input_it>
json_node::object_type parse_object(t_input_it&& p_start, const typename std::remove_reference<t_input_it>::type p_end);
template<typename t_input_it>
json_node::object_type::value_type parse_member(t_input_it&& p_start, const typename std::remove_reference<t_input_it>::type p_end);
template<typename t_input_it>
json_node::array_type parse_array(t_input_it&& p_start, const typename std::remove_reference<t_input_it>::type p_end);
template<typename t_input_it>
json_node::string_type parse_string(t_input_it&& p_start, const typename std::remove_reference<t_input_it>::type p_end);
template<typename t_input_it>
json_node::number_type parse_number(t_input_it&& p_start, const typename std::remove_reference<t_input_it>::type p_end);
template<typename t_input_it>
json_node::bool_type parse_bool(t_input_it&& p_start, const typename std::remove_reference<t_input_it>::type p_end);
template<typename t_input_it>
void parse_null(t_input_it&& p_start, const typename std::remove_reference<t_input_it>::type p_end);

const std::set<char> whitespace{0x20, 0x09, 0x0A, 0x0D};

template<typename t_input_it>
inline bool skip_whitespace(t_input_it&& p_start, const typename std::remove_reference<t_input_it>::type p_end) {
	while (p_start != p_end && whitespace.find(*p_start) != whitespace.end())
		++p_start;

	if (p_start == p_end)
		throw std::runtime_error{"Unexpected end of string."};
}

template<typename t_input_it>
json_node parse(t_input_it&& p_start, const typename std::remove_reference<t_input_it>::type p_end) {
	skip_whitespace(p_start, p_end);
	switch (*p_start) {
		case '{':
			return json_node(parse_object(p_start, p_end));
		case '[':
			return json_node(parse_array(p_start, p_end));
		case '\"':
			return json_node(parse_string(p_start, p_end));
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
			return json_node(parse_number(p_start, p_end));
		case 't':
		case 'f':
			return json_node(parse_bool(p_start, p_end));
		case 'n':
			parse_null(p_start, p_end);
			return json_node();
	}

	throw std::runtime_error(std::string("Expected JSON value token, got '") + *p_start += '\'');
}

template<typename t_input_it>
json_node::object_type parse_object(t_input_it&& p_start, const typename std::remove_reference<t_input_it>::type p_end) {
	skip_whitespace(p_start, p_end);
	if (*p_start != '{')
		throw std::runtime_error{std::string("Expected '{', got '") + *p_start += '\''};
	
	json_node::object_type obj;
	skip_whitespace(p_start, p_end);
	if (*p_start == '}') {
		++p_start;
		return obj;
	}

	while (++p_start != p_end) {
		obj.emplace(parse_member(p_start, p_end));
		skip_whitespace(p_start, p_end);
		if (*p_start == '}') {
			++p_start;
			return obj;
		} else if (*p_start != ',') {
			throw std::runtime_error{std::string("Expected ',', got '") + *p_start += '\''};
		}
	}

	throw std::runtime_error{"Unexpected end of string."};
}

template<typename t_input_it>
inline json_node::object_type::value_type parse_member(t_input_it&& p_start, const typename std::remove_reference<t_input_it>::type p_end) {
	json_node::string_type key{parse_string(p_start, p_end)};
	skip_whitespace(p_start, p_end);
	if (*p_start != ':')
		throw std::runtime_error{std::string("Expected ':', got '") + *p_start += '\''};
	
	if (++p_start == p_end)
		throw std::runtime_error{"Unexpected end of string."};
	
	return {key, parse(p_start, p_end)};
}

template<typename t_input_it>
json_node::array_type parse_array(t_input_it&& p_start, const typename std::remove_reference<t_input_it>::type p_end) {
	skip_whitespace(p_start, p_end);
	if (*p_start != '[')
		throw std::runtime_error{std::string("Expected '[', got '") + *p_start += '\''};
	
	std::list<json_node> values;
	skip_whitespace(p_start, p_end);
	if (*p_start == ']') {
		++p_start;
		return {std::make_move_iterator(values.begin()), std::make_move_iterator(values.end())};
	}

	while (++p_start != p_end) {
		values.emplace_back(parse(p_start, p_end));
		skip_whitespace(p_start, p_end);
		if (*p_start == ']') {
			++p_start;
			return {std::make_move_iterator(values.begin()), std::make_move_iterator(values.end())};
		} else if (*p_start != ',') {
			throw std::runtime_error{std::string("Expected ',', got '") + *p_start += '\''};
		}
	}

	throw std::runtime_error{"Unexpected end of string."};
}

template<typename t_input_it>
inline void parse_unicode_sequence(t_input_it&& p_start, const typename std::remove_reference<t_input_it>::type p_end, std::string& p_out) {
	for (int i = 0; i < 4; ++i) {
		if (!isxdigit(*p_start))
			throw std::runtime_error{std::string("Expected hexadecimal, got '") + *p_start += '\''};

		p_out += *p_start;
		++p_start;
	}
}

template<typename t_input_it>
json_node::string_type parse_string(t_input_it&& p_start, const typename std::remove_reference<t_input_it>::type p_end) {
	skip_whitespace(p_start, p_end);
	if (*p_start != '\"')
		throw std::runtime_error{std::string("Expected '\"', got '") + *p_start += '\''};

	std::string str;
	while (++p_start != p_end) {
		if (*p_start == '\"') {
			++p_start;
			return str;
		}

		str += *p_start;
		if (*p_start == '\\' && ++p_start != p_end) {
			str += *p_start;
			if (*p_start == 'u' && std::distance(p_start, p_end) > 4)
				parse_unicode_sequence(++p_start, p_end, str);
		}
	}

	throw std::runtime_error{"Unexpected end of string."};
}

template<typename input_it_t>
json_node::number_type parse_number(input_it_t&& p_start, const typename std::remove_reference<input_it_t>::type p_end) {
	skip_whitespace(p_start, p_end);
	std::string num;
	if (*p_start == '-') {
		num += *p_start;
		++p_start;
	}

	if (p_start == p_end)
		throw std::runtime_error{"Unexpected end of string."};
	
	if (*p_start == '0') {
		num += *p_start;
		++p_start;
	} else if (isdigit(*p_start)) {
		do {
			num += *p_start;
			if (++p_start == p_end)
				return std::stod(num);
		} while (isdigit(*p_start));
	} else {
		throw std::runtime_error{std::string("Expected digit, got '") + *p_start += '\''};
	}

	if (*p_start == '.') {
		num += *p_start;
		if (++p_start == p_end)
			throw std::runtime_error{"Unexpected end of string."};

		if (!isdigit(*p_start))
			throw std::runtime_error{std::string("Expected digit, got '") + *p_start += '\''};

		do {
			num += *p_start;
			if (++p_start == p_end)
				return std::stod(num);
		} while (isdigit(*p_start));
	}

	if (*p_start == 'e' || *p_start == 'E') {
		num += *p_start;
		if (++p_start == p_end)
			throw std::runtime_error{"Unexpected end of string."};

		if (*p_start == '+' || *p_start == '-') {
			num += *p_start;
			if (++p_start == p_end)
				throw std::runtime_error{"Unexpected end of string."};
		}

		if (!isdigit(*p_start))
			throw std::runtime_error{std::string("Expected digit, got '") + *p_start += '\''};

		do {
			num += *p_start;
		} while (++p_start != p_end && isdigit(*p_start));
	}
	return std::stod(num);
}

template<typename t_input_it>
json_node::bool_type parse_bool(t_input_it&& p_start, const typename std::remove_reference<t_input_it>::type p_end) {
	skip_whitespace(p_start, p_end);
	if (*p_start == 't') {
		if (std::distance(p_start, p_end) < 4)
			throw std::runtime_error{"Unexpected end of string."};

		char bool_str[4] = {};
		std::copy(p_start, p_start + 4, std::begin(bool_str));
		p_start += 4;
		if (*(int32_t*)(bool_str + 0) == 0x65757274)
			return true;
	} else if (std::distance(p_start, p_end) >= 5 && *p_start == 'f') {
		if (std::distance(p_start, p_end) < 5)
			throw std::runtime_error{"Unexpected end of string."};

		char bool_str[4] = {};
		++p_start;
		std::copy(p_start, p_start + 4, std::begin(bool_str));
		p_start += 4;
		if (*(int32_t*)(bool_str + 0) == 0x65736C61)
			return false;
	}

	throw std::runtime_error{std::string("Expected 't' or 'f', got '") + *p_start += '\''};
}

template<typename t_input_it>
void parse_null(t_input_it&& p_start, const typename std::remove_reference<t_input_it>::type p_end) {
	skip_whitespace(p_start, p_end);
	if (*p_start == 'n') {
		if (std::distance(p_start, p_end) < 4)
			throw std::runtime_error("Unexpected end of string.");

		char null_str[4] = {};
		std::copy(p_start, p_start + 4, std::begin(null_str));
		p_start += 4;
		if (*(int32_t*)(null_str + 0) == 0x6C6C756E)
			return;
	}

	throw std::runtime_error{std::string("Expected 'n', got '") + *p_start += '\''};
}

}
