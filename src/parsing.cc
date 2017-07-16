#include "json_node.hh"
#include "parsing.hh"

#include <cstring>
#include <list>
#include <istream>
#include <iterator>
#include <sstream>
#include <stdexcept>

namespace touchstone {

json_node parse_json(const std::string& str) {
	std::istringstream is(str);
	return parse_json(is);
}

json_object parse_json_object(std::istream&);
json_member parse_json_member(std::istream&);
json_array parse_json_array(std::istream&);
json_string parse_json_string(std::istream&);
json_number parse_json_number(std::istream&);
json_bool parse_json_bool(std::istream&);
void parse_json_null(std::istream&);

json_node parse_json(std::istream& is) {
	switch(is.peek()) {
		case '{':
			return {parse_json_object(is)};
		case '[':
			return {parse_json_array(is)};
		case '\"':
			return {parse_json_string(is)};
		case '-':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return {parse_json_number(is)};
		case 't':
		case 'f':
			return {parse_json_bool(is)};
		case 'n':
			parse_json_null(is);
			return {};
	}
	throw std::runtime_error{std::string{"Unexpected character: \'"} + std::string{(char) is.peek(), '\''}};
}

json_object parse_json_object(std::istream& is) {
	if (is.peek() != '{') throw std::runtime_error{std::string{"Expected \'{\', got: \'"} + std::string{(char) is.peek(), '\''}};
	is.ignore();
	json_object obj;
	if ((is >> std::ws).peek() == '}') {
		is.ignore();
		return obj;
	}
	do {
		obj.emplace(parse_json_member(is >> std::ws));
		if ((is >> std::ws).peek() == '}') {
			is.ignore();
			return obj;
		}
		if (is.peek() != ',') throw std::runtime_error{std::string{"Expected \',\', got: \'"} + std::string{(char) is.peek(), '\''}};
		is.ignore();
	} while (is.good());
	throw std::runtime_error{"Unexpected end of input."};
}

json_member parse_json_member(std::istream& is) {
	json_string str{parse_json_string(is)};
	if ((is >> std::ws).peek() != ':') throw std::runtime_error{std::string("Expected \':\', got: \'") + std::string{(char) is.peek(), '\''}};
	return {str, parse_json(is.ignore() >> std::ws)};
}

json_array parse_json_array(std::istream& is) {
	if (is.peek() != '[') throw std::runtime_error{std::string{"Expected \'[\', got: \'"} + std::string{(char) is.peek(), '\''}};
	is.ignore();
	std::list<json_node> items;
	if ((is >> std::ws).peek() == ']') {
		is.ignore();
		return {std::make_move_iterator(std::begin(items)), std::make_move_iterator(std::end(items))};
	}
	do {
		items.emplace_back(parse_json(is >> std::ws));
		if ((is >> std::ws).peek() == ']') {
			is.ignore();
			return {std::make_move_iterator(std::begin(items)), std::make_move_iterator(std::end(items))};
		}
		if (is.peek() != ',') throw std::runtime_error{std::string{"Expected \',\', got: \'"} + std::string{(char) is.peek(), '\''}};
	} while (is.ignore().good());
	throw std::runtime_error{"Unexpected end of input."};
}

json_string parse_json_string(std::istream& is) {
	if (is.peek() != '\"') throw std::runtime_error{std::string{"Expected \'\"\', got: \'"} + std::string{(char) is.peek(), '\''}};
	is.ignore();
	std::string str;
	char c;
	do {
		c = is.get();
		if (c == '\"') return str;
		str += c;
		if (c == '\\') str += is.get();
	} while (is.good());
	throw std::runtime_error{"Unexpected end of input."};
}

json_number parse_json_number(std::istream& is) {
	json_number num;
	if (is >> num)
		return num;
	throw std::runtime_error{"Malformed number."};
}

json_bool parse_json_bool(std::istream& is) {
	if (is.peek() == 't') {
		char str[5] = {};
		if (!is.read(str, 4)) throw std::runtime_error{"Unexpected end of input."};
		if (strcmp(str, "true") != 0) throw std::runtime_error{std::string{"Malformed boolean: \""} + str + '\"'};
		return true;
	} else if (is.peek() == 'f') {
		char str[6] = {};
		if (!is.read(str, 5)) throw std::runtime_error{"Unexpected end of input."};
		if (strcmp(str, "false") != 0) throw std::runtime_error{std::string{"Malformed boolean: \""} + str + '\"'};
		return false;

	}
	throw std::runtime_error{std::string{"Expected 't' or 'f', got: \'"} + std::string{(char) is.peek(), '\''}};
}

void parse_json_null(std::istream& is) {
	if (is.peek() == 'n') {
		char str[5] = {};
		if (!(is.read(str, 4))) throw std::runtime_error{"Unexpected end of input."};
		if (strcmp(str, "null") != 0) throw std::runtime_error{std::string{"Malformed null: \""} + std::string{str, 4} + '\"'};
	} else {
		throw std::runtime_error{std::string{"Expected \'n\', got: \'"} + std::string{(char) is.peek(), '\''}};
	}
}

}
