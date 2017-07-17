#ifndef TOUCHSTONE_PARSING_HH
#define TOUCHSTONE_PARSING_HH

#include "json_node.hh"
#include "types.hh"

#include <algorithm>
#include <cctype>
#include <cstring>
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
inline void skip_whitespace(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end) {
	while (start != end && whitespace.find(*start) != whitespace.end())
		++start;
}

template<typename input_it_t>
json_node parse(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end) {
	while (whitespace.find(*start) != whitespace.end())	// Can go past end for a whitespace input
		++start;
	
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

	throw std::runtime_error{"[parse] error."};
}

template<typename input_it_t>
json_object parse_object(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end) {
	skip_whitespace(start, end);
	if (*start != '{')
		throw std::runtime_error{"[parse_object] error."};
	
	json_object obj;
	skip_whitespace(start, end);
	if (*start == '}') { // Could be past end
		++start;
		return obj;
	}

	while (++start != end) {
		obj.emplace(parse_member(start, end));
		skip_whitespace(start, end); // Could be past end
		if (*start == '}') {
			++start;
			return obj;
		} else if (*start != ',') {
			throw std::runtime_error{"[parse_object] error."};
		}
	}

	throw std::runtime_error{"[parse_object] error."};
}

template<typename input_it_t>
json_member parse_member(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end) {
	json_string key{parse_string(start, end)};
	skip_whitespace(start, end); //  Could be past end
	if (*start != ':')
		throw std::runtime_error{"[parse_member] error."};
	
	return {key, parse(++start, end)};
}

template<typename input_it_t>
json_array parse_array(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end) {
	skip_whitespace(start, end);
	if (*start != '[')
		throw std::runtime_error{"[parse_array] error."};
	
	std::list<json_node> values;
	skip_whitespace(start, end);
	if (*start == ']') { // Could be past end
		++start;
		return {std::make_move_iterator(values.begin()), std::make_move_iterator(values.end())};
	}

	while (++start != end) {
		values.emplace_back(parse(start, end));
		skip_whitespace(start, end); // Could be past end
		if (*start == ']') {
			++start;
			return {std::make_move_iterator(values.begin()), std::make_move_iterator(values.end())};
		} else if (*start != ',') {
			throw std::runtime_error{"[parse_array] error."};
		}
	}

	throw std::runtime_error{"[parse_array] error."};
}


template<typename input_it_t>
json_string parse_string(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end) {
	skip_whitespace(start, end); // Could be past end
	if (*start != '\"')
		throw std::runtime_error{"[parse_string] error."};

	std::string str;
	while (++start != end) {
		if (*start == '\"') {
			++start;
			return str;
		}

		str += *start;
		if (*start == '\\') // Could be past end
			str += *++start;
	}

	throw std::runtime_error{"[parse_string] error."};
}

template<typename input_it_t>
json_number parse_number(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end) {
	skip_whitespace(start, end);
	std::string num;
	if (*start == '-') {
		num += *start;
		++start;
	}

	if (*start == '0') {
		num += *start;
		++start;
	} else if (isdigit(*start)) {
		do {
			num += *start;
		} while (isdigit(*++start));
	} else {
		throw std::runtime_error{"[parse_number] error."};
	}

	if (*start == '.') {
		num += *start;
		if (!isdigit(*++start))
			throw std::runtime_error{"[parse_number] error."};

		do {
			num += *start;
		} while (isdigit(*++start));
	}

	if (*start == 'e' || *start == 'E') {
		if (*++start == '+' || *start == '-') {
			num += *start;
			++start;
		}

		if (!isdigit(*start))
			throw  std::runtime_error{"[parse_number] error."};

		do {
			num += *start;
		} while (isdigit(*++start));
	}
	return std::stod(num);
}

template<typename input_it_t>
json_bool parse_bool(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end) {
	skip_whitespace(start, end); // Could go past end
	if (*start == 't') {
		char bool_str[4] = {};
		std::copy(start, start + 4, std::begin(bool_str));
		start += 4;
		if (*(int32_t*)(bool_str + 0) == 0x65757274)
			return true;
	} else if (*start == 'f') {
		char bool_str[8] = {};
		std::copy(start, start + 5, std::begin(bool_str));
		start += 5;
		if (*(int64_t*)(bool_str + 0) == 0x65736C6166) // Endianness may be wrong
			return false;
	}

	throw std::runtime_error{"[parse_bool] error."};
}

template<typename input_it_t>
void parse_null(input_it_t&& start, const typename std::remove_reference<input_it_t>::type end) {
	skip_whitespace(start, end); // Could go past end
	if (*start == 'n') {
		char null_str[4] = {};
		std::copy(start, start + 4, std::begin(null_str));
		start += 4;
		if (*(int32_t*)(null_str + 0) == 0x6E756C6C)
			return;
	}

	throw std::runtime_error{"[parse_null] error."};
}

}

#endif
