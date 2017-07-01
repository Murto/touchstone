#include "json.hpp"
#include "json-exception.hpp"
#include "json-node.hpp"
#include "parsing.hpp"

#include <cstring>
#include <istream>
#include <sstream>

namespace touchstone {

JSONNode parseJSON(const std::string& str) {
	std::istringstream is(str);
	return parseJSON(is);
}

JSONObject parseJSONObject(std::istream& is);
JSONMember parseJSONMember(std::istream& is);
JSONArray parseJSONArray(std::istream& is);
JSONString parseJSONString(std::istream& is);
JSONNumber parseJSONNumber(std::istream& is);
JSONBool parseJSONBool(std::istream& is);
void parseJSONNull(std::istream& is);

JSONNode parseJSON(std::istream& is) {
	switch(is.peek()) {
		case '{':
			return JSONNode(parseJSONObject(is));
		case '[':
			return JSONNode(parseJSONArray(is));
		case '\"':
			return JSONNode(parseJSONString(is));
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
			return JSONNode(parseJSONNumber(is));
		case 't':
		case 'f':
			return JSONNode(parseJSONBool(is));
		case 'n':
			parseJSONNull(is);
			return JSONNode();
	}
	throw JSONException(std::string("Unexpected character: \'") + std::string{(char) is.peek(), '\''});
}

JSONObject parseJSONObject(std::istream& is) {
	if (is.peek() != '{') throw JSONException(std::string("Expected \'{\', got: \'") + std::string{(char) is.peek(), '\''});
	is.ignore();
	JSONObject obj;
	if ((is >> std::ws).peek() == '}') {
		is.ignore();
		return obj;
	}
	do {
		obj.insert(parseJSONMember(is >> std::ws));
		if ((is >> std::ws).peek() == '}') {
			is.ignore();
			return obj;
		}
		if (is.peek() != ',') throw JSONException(std::string("Expected \',\', got: \'") + std::string{(char) is.peek(), '\''});
		is.ignore();
	} while (is.good());
	throw JSONException("Unexpected end of input.");
}

JSONMember parseJSONMember(std::istream& is) {
	JSONString str = parseJSONString(is);
	if ((is >> std::ws).peek() != ':') throw JSONException(std::string("Expected \':\', got: \'") + std::string{(char) is.peek(), '\''});
	is.ignore();
	JSONNode node = parseJSON(is >> std::ws);
	return std::make_pair(str, node);
}

JSONArray parseJSONArray(std::istream& is) {
	if (is.peek() != '[') throw JSONException(std::string("Expected \'[\', got: \'") + std::string{(char) is.peek(), '\''});
	is.ignore();
	JSONArray arr;
	if ((is >> std::ws).peek() == ']') {
		is.ignore();
		return arr;
	}
	do {
		arr.push_back(parseJSON(is >> std::ws));
		if ((is >> std::ws).peek() == ']') {
			is.ignore();
			return arr;
		}
		if (is.peek() != ',') throw JSONException(std::string("Expected \',\', got: \'") + std::string{(char) is.peek(), '\''});
		is.ignore();
	} while (is.good());
	throw JSONException("Unexpected end of input.");
}

JSONString parseJSONString(std::istream& is) {
	if (is.peek() != '\"') throw JSONException(std::string("Expected \'\"\', got: \'") + std::string{(char) is.peek(), '\''});
	is.ignore();
	std::ostringstream ss;
	is >> std::noskipws;
	char c;
	while (is.good()) {
		ss << (c = is.get());
		if (c == '\\') ss << is.get();
		if (c == '\"') return ss.str();
	}
	throw JSONException("Unexpected end of input.");
}

JSONNumber parseJSONNumber(std::istream& is) {
	JSONNumber num;
	if (is >> num)
		return num;
	throw JSONException("Malformed number.");
}

JSONBool parseJSONBool(std::istream& is) {
	if (is.peek() == 't') {
		char str[5] = {};
		if (!is.read(str, 4)) throw JSONException("Unexpected end of input.");
		if (strcmp(str, "true") != 0) throw JSONException(std::string("Malformed boolean: \"") + str + '\"');
		return true;
	} else if (is.peek() == 'f') {
		char str[6] = {};
		if (!is.read(str, 5)) throw JSONException("Unexpected end of input.");
		if (strcmp(str, "false") != 0) throw JSONException(std::string("Malformed boolean: \"") + str + '\"');
		return false;

	}
	throw JSONException(std::string("Expected 't' or 'f', got: \'") + std::string{(char) is.peek(), '\''});
}

void parseJSONNull(std::istream& is) {
	if (is.peek() == 'n') {
		char str[4];
		if (!(is.read(str, 4))) throw JSONException("Unexpected end of input.");
		if (strcmp(str, "null") != 0) throw JSONException(std::string("Malformed null: \"") + std::string(str, 4) + '\"');
	} else throw JSONException(std::string("Expected \'n\', got: \'") + std::string{(char) is.peek(), '\''});
}

}
