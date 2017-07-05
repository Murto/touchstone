#include "json-exception.hpp"
#include "json-node.hpp"
#include "parsing.hpp"

#include <cstring>
#include <list>
#include <iostream>
#include <istream>
#include <iterator>
#include <sstream>

namespace touchstone {

JSONNode parseJSON(const std::string& str) {
	std::istringstream is(str);
	return parseJSON(is);
}

JSONObject parseJSONObject(std::istream&);
JSONMember parseJSONMember(std::istream&);
JSONArray parseJSONArray(std::istream&);
JSONString parseJSONString(std::istream&);
JSONNumber parseJSONNumber(std::istream&);
JSONBool parseJSONBool(std::istream&);
void parseJSONNull(std::istream&);

JSONNode parseJSON(std::istream& is) {
	switch(is.peek()) {
		case '{':
			return {parseJSONObject(is)};
		case '[':
			return {parseJSONArray(is)};
		case '\"':
			return {parseJSONString(is)};
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
			return {parseJSONNumber(is)};
		case 't':
		case 'f':
			return {parseJSONBool(is)};
		case 'n':
			parseJSONNull(is);
			return {};
	}
	throw JSONException{std::string{"Unexpected character: \'"} + std::string{(char) is.peek(), '\''}};
}

JSONObject parseJSONObject(std::istream& is) {
	if (is.peek() != '{') throw JSONException{std::string{"Expected \'{\', got: \'"} + std::string{(char) is.peek(), '\''}};
	is.ignore();
	JSONObject obj;
	if ((is >> std::ws).peek() == '}') {
		is.ignore();
		return obj;
	}
	do {
		obj.emplace(parseJSONMember(is >> std::ws));
		if ((is >> std::ws).peek() == '}') {
			is.ignore();
			return obj;
		}
		if (is.peek() != ',') throw JSONException{std::string{"Expected \',\', got: \'"} + std::string{(char) is.peek(), '\''}};
		is.ignore();
	} while (is.good());
	throw JSONException{"Unexpected end of input."};
}

JSONMember parseJSONMember(std::istream& is) {
	JSONString str{parseJSONString(is)};
	if ((is >> std::ws).peek() != ':') throw JSONException{std::string("Expected \':\', got: \'") + std::string{(char) is.peek(), '\''}};
	return {str, parseJSON(is.ignore() >> std::ws)};
}

JSONArray parseJSONArray(std::istream& is) {
	if (is.peek() != '[') throw JSONException{std::string{"Expected \'[\', got: \'"} + std::string{(char) is.peek(), '\''}};
	is.ignore();
	std::list<JSONNode> items;
	if ((is >> std::ws).peek() == ']') {
		is.ignore();
		return {std::make_move_iterator(std::begin(items)), std::make_move_iterator(std::end(items))};
	}
	do {
		items.emplace_back(parseJSON(is >> std::ws));
		if ((is >> std::ws).peek() == ']') {
			is.ignore();
			return {std::make_move_iterator(std::begin(items)), std::make_move_iterator(std::end(items))};
		}
		if (is.peek() != ',') throw JSONException{std::string{"Expected \',\', got: \'"} + std::string{(char) is.peek(), '\''}};
	} while (is.ignore().good());
	throw JSONException{"Unexpected end of input."};
}

JSONString parseJSONString(std::istream& is) {
	if (is.peek() != '\"') throw JSONException{std::string{"Expected \'\"\', got: \'"} + std::string{(char) is.peek(), '\''}};
	is.ignore();
	std::string str;
	char c;
	do {
		c = is.get();
		if (c == '\"') return str;
		str += c;
		if (c == '\\') str += is.get();
	} while (is.good());
	throw JSONException{"Unexpected end of input."};
}

JSONNumber parseJSONNumber(std::istream& is) {
	JSONNumber num;
	if (is >> num)
		return num;
	throw JSONException{"Malformed number."};
}

JSONBool parseJSONBool(std::istream& is) {
	if (is.peek() == 't') {
		char str[5] = {};
		if (!is.read(str, 4)) throw JSONException{"Unexpected end of input."};
		if (strcmp(str, "true") != 0) throw JSONException{std::string{"Malformed boolean: \""} + str + '\"'};
		return true;
	} else if (is.peek() == 'f') {
		char str[6] = {};
		if (!is.read(str, 5)) throw JSONException{"Unexpected end of input."};
		if (strcmp(str, "false") != 0) throw JSONException{std::string{"Malformed boolean: \""} + str + '\"'};
		return false;

	}
	throw JSONException{std::string{"Expected 't' or 'f', got: \'"} + std::string{(char) is.peek(), '\''}};
}

void parseJSONNull(std::istream& is) {
	if (is.peek() == 'n') {
		char str[5] = {};
		if (!(is.read(str, 4))) throw JSONException{"Unexpected end of input."};
		if (strcmp(str, "null") != 0) throw JSONException{std::string{"Malformed null: \""} + std::string{str, 4} + '\"'};
	} else {
		throw JSONException{std::string{"Expected \'n\', got: \'"} + std::string{(char) is.peek(), '\''}};
	}
}

}
