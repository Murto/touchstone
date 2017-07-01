#include "json.hpp"
#include "json-exception.hpp"
#include "json-node.hpp"
#include "parsing.hpp"

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
	if (is >> std::ws)
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
	throw JSONException("Invalid string.");
}

JSONObject parseJSONObject(std::istream& is) {
	char c;
	if (is.get() == '{') {
		if (!(is >> std::ws)) throw JSONException("Invalid string.");
		JSONObject obj;
		if (is.peek() == '}') return obj;
		do {
			obj.insert(parseJSONMember(is));
			if (!(is >> std::ws && is >> c)) throw JSONException("Invalid string.");
			if (c == '}') break;
			if (c != ',') throw JSONException("Unexpected token.");
		} while (is >> std::ws);
		if (!is.good()) throw JSONException("Invalid string.");
		return obj;
	}
	throw JSONException("Invalid string.");
}

JSONMember parseJSONMember(std::istream& is) {
	JSONString str = parseJSONString(is);
	char c;
	if (!(is >> std::ws && is >> c)) throw JSONException("Invalid string.");
	if (!(c == ':' && is >> std::ws)) throw JSONException("Invalid string.");
	JSONNode node = parseJSON(is);
	return std::make_pair(str, node);
}

JSONArray parseJSONArray(std::istream& is) {
	char c;
	if (is.get() == '[') {
		if (!(is >> std::ws)) throw JSONException("Invalid string.");
		JSONArray arr;
		if (is.peek() == ']') return arr;
		do {
			arr.push_back(parseJSON(is));
			if (!(is >> std::ws && is >> c)) throw JSONException("Invalid string.");
			if (c == ']') break;
			if (c != ',') throw JSONException("Unexpected token.");
		} while (is >> std::ws);
		if (!is.good()) throw JSONException("Invalid string.");
		return arr;
	}
	throw JSONException("Invalid string.");
}

JSONString parseJSONString(std::istream& is) {
	char c;
	if (is.get() == '\"') {
		std::ostringstream ss;
		bool escFlag = false;
		while (is >> c && (c != '\"' || escFlag)) {
			ss << c;
			escFlag = (c == '\\');
		}
		if (!is.good()) throw JSONException("Invalid string.");
		return ss.str();
	}
	throw JSONException("Invalid string.");
}

JSONNumber parseJSONNumber(std::istream& is) {
	JSONNumber num;
	if (is >> num)
		return num;
	throw JSONException("Invalid string.");
}

JSONBool parseJSONBool(std::istream& is) {
	char c{'\0'};
	if (is >> c && c == 't') {
		char str[4];
		str[3] = '\0';
		if (is.read(str, 3)) {
			if (std::string(str) == "rue") return true;
		}
	} else if (c == 'f') {
		char str[5];
		str[4] = '\0';
		if (is.read(str, 4)) {
			if (std::string(str) == "alse") return false;
		}
	}
	throw JSONException("Invalid string.");
}

void parseJSONNull(std::istream& is) {
	char c;
	if (is.get() == 'n') {
		char str[3];
		if (is.getline(str, 3) && std::string(str) == "ull") return;
	}
	throw JSONException("Invalid string.");
}

}
