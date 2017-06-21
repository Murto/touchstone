#include "json.hpp"
#include "json-exception.hpp"
#include "json-node.hpp"
#include "parsing-streams.hpp"

#include <ios>
#include <istream>
#include <sstream>

namespace touchstone {

namespace parsing {

using namespace types;

JSONNode parseJSON(const std::string& str) {
	std::istringstream ss(str);
	return parseJSON(ss);
}

JSONObject parseJSONObject(std::istream& ss);
JSONMember parseJSONMember(std::istream& ss);
JSONArray parseJSONArray(std::istream& ss);
JSONString parseJSONString(std::istream& ss);
JSONNumber parseJSONNumber(std::istream& ss);
JSONBool parseJSONBool(std::istream& ss);
void parseJSONNull(std::istream& ss);

JSONNode parseJSON(std::istream& ss) {
	if (ss >> std::ws)
		switch(ss.peek()) {
			case '{':
				return JSONNode(parseJSONObject(ss));
			case '[':
				return JSONNode(parseJSONArray(ss));
			case '\"':
				return JSONNode(parseJSONString(ss));
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
				return JSONNode(parseJSONNumber(ss));
			case 't':
			case 'f':
				return JSONNode(parseJSONBool(ss));
			case 'n':
				parseJSONNull(ss);
				return JSONNode();
		}
	throw JSONException("Invalid string.");
}

JSONObject parseJSONObject(std::istream& ss) {
	char c;
	if (ss.get() == '{') {
		if (!(ss >> std::ws)) throw JSONException("Invalid string.");
		JSONObject obj;
		if (ss.peek() == '}') return obj;
		do {
			obj.insert(parseJSONMember(ss));
			if (!(ss >> std::ws && ss >> c)) throw JSONException("Invalid string.");
			if (c == '}') break;
			if (c != ',') throw JSONException("Unexpected token.");
		} while (ss >> std::ws);
		if (!ss.good()) throw JSONException("Invalid string.");
		return obj;
	}
	throw JSONException("Invalid string.");
}

JSONMember parseJSONMember(std::istream& ss) {
	JSONString str = parseJSONString(ss);
	char c;
	if (!(ss >> std::ws && ss >> c)) throw JSONException("Invalid string.");
	if (!(c == ':' && ss >> std::ws)) throw JSONException("Invalid string.");
	JSONNode node = parseJSON(ss);
	return JSONMember(str, node);
}

JSONArray parseJSONArray(std::istream& ss) {
	char c;
	if (ss.get() == '[') {
		if (!(ss >> std::ws)) throw JSONException("Invalid string.");
		JSONArray arr;
		if (ss.peek() == ']') return arr;
		do {
			arr.push_back(parseJSON(ss));
			if (!(ss >> std::ws && ss >> c)) throw JSONException("Invalid string.");
			if (c == ']') break;
			if (c != ',') throw JSONException("Unexpected token.");
		} while (ss >> std::ws);
		if (!ss.good()) throw JSONException("Invalid string.");
		return arr;
	}
	throw JSONException("Invalid string.");
}

JSONString parseJSONString(std::istream& ss) {
	char c;
	if (ss.get() == '\"') {
		std::ostringstream str;
		bool escFlag = false;
		while (ss >> c && (c != '\"' || escFlag)) {
			escFlag = false;
			str << c;
			if (c == '\\') escFlag = true;
		}
		if (!ss.good()) throw JSONException("Invalid string.");
		return str.str();
	}
	throw JSONException("Invalid string.");
}

JSONNumber parseJSONNumber(std::istream& ss) {
	JSONNumber num;
	if (ss >> num)
		return num;
	throw JSONException("Invalid string.");
}

JSONBool parseJSONBool(std::istream& ss) {
	char c{'\0'};
	if (ss >> c && c == 't') {
		char str[4];
		str[3] = '\0';
		if (ss.read(str, 3)) {
			if (std::string(str) == "rue") return true;
		}
	} else if (c == 'f') {
		char str[5];
		str[4] = '\0';
		if (ss.read(str, 4)) {
			if (std::string(str) == "alse") return false;
		}
	}
	throw JSONException("Invalid string.");
}

void parseJSONNull(std::istream& ss) {
	char c;
	if (ss.get() == 'n') {
		char str[3];
		if (ss.getline(str, 3) && std::string(str) == "ull") return;
	}
	throw JSONException("Invalid string.");
}

}

}
