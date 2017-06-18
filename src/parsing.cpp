#include "json.hpp"
#include "json-exception.hpp"
#include "json-node.hpp"
#include "parsing.hpp"

#include <iterator>
#include <sstream>

namespace touchstone {

namespace parsing {

using namespace types;

std::string toString(const JSONObject& obj) {
	std::stringstream ss;
	ss << '{';
	auto it = obj.cbegin();
	while (it != obj.cend()) {
		ss << '\"' << it->first << '\"' << ':' << it->second.toString();
		if (++it != obj.cend()) ss << ',';
	}
	ss << '}';
	return ss.str();
}

std::string toString(const JSONArray& arr) {
	std::stringstream ss;
	ss << '[';
	auto it = arr.cbegin();
	while (it != arr.cend()) {
		ss << it->toString();
		if (++it != arr.cend()) ss << ',';
	}
	ss << ']';
	return ss.str();
}

std::string toString(const JSONString& str) {
	return "\"" + str + "\"";
}

std::string toString(const JSONNumber& num) {
	return std::to_string(num);
}

std::string toString(const JSONBool& boo) {
	return boo ? "true" : "false";
}

// Parse helper functions
namespace {

JSONObject parseJSONObject(std::string::const_iterator& start, const std::string::const_iterator end);
JSONMember parseJSONMember(std::string::const_iterator& start, const std::string::const_iterator end);
JSONArray parseJSONArray(std::string::const_iterator& start, const std::string::const_iterator end);
JSONString parseJSONString(std::string::const_iterator& start, const std::string::const_iterator end);
JSONNumber parseJSONNumber(std::string::const_iterator& start, const std::string::const_iterator end);
JSONBool parseJSONBool(std::string::const_iterator& start, const std::string::const_iterator end);
void parseJSONNull(std::string::const_iterator& start, const std::string::const_iterator end);

void ignoreWhitespace(std::string::const_iterator& start, const std::string::const_iterator end) {
	while (start != end && json::whitespace.find(*start) != std::string::npos) ++start;
	if (start == end) throw JSONException("Unexpected end of string.");
}

JSONObject parseJSONObject(const std::string& str) {
	auto it = str.cbegin();
	return parseJSONObject(it, str.cend());
}

JSONArray parseJSONArray(const std::string& str) {
	auto it = str.cbegin();
	return parseJSONArray(it, str.cend());
}

JSONString parseJSONString(const std::string& str) {
	auto it = str.cbegin();
	return parseJSONString(it, str.cend());
}

JSONNumber parseJSONNumber(const std::string& str) {
	auto it = str.cbegin();
	return parseJSONNumber(it, str.cend());
}

JSONBool parseJSONBool(const std::string& str) {
	auto it = str.cbegin();
	return parseJSONBool(it, str.cend());
}

void parseJSONNull(const std::string& str) {
	auto it = str.cbegin();
	return parseJSONNull(it, str.cend());
}

JSONObject parseJSONObject(std::string::const_iterator& start, const std::string::const_iterator end) {
	if (start == end) throw JSONException("Unexpected end of string.");
	if (*start != '{') throw JSONException("Unexpected token.");
	JSONObject obj;
	if (++start == end) throw JSONException("Unexpected end of string.");
	while (start != end) {
		ignoreWhitespace(start, end);
		if (*start == '}') {
			++start;
			return obj;
		}
		obj.insert(parseJSONMember(start, end));
		ignoreWhitespace(start, end);
		if (*start == '}') {
			++start;
			return obj;
		}
		if (*start != ',') throw JSONException("Unexpected token.");
		++start;
	}
	throw JSONException("Unexpected end of string.");
}

JSONMember parseJSONMember(std::string::const_iterator& start, const std::string::const_iterator end) {
	if (*start != '\"') throw JSONException("Unexpected token.");
	JSONString key = parseJSONString(start, end);
	ignoreWhitespace(start, end);
	if (*start != ':') throw JSONException("Unexpected token.");
	if (++start == end) throw JSONException("Unexpected end of string.");
	ignoreWhitespace(start, end);
	JSONNode value = parseJSON(start, end);
	return std::make_pair(key, value);
}

JSONArray parseJSONArray(std::string::const_iterator& start, const std::string::const_iterator end) {
	if (start == end) throw JSONException("Unexpected end of string.");
	if (*start != '[') throw JSONException("Unexpected token.");
	JSONArray arr;
	if (++start == end) throw JSONException("Unexpected end of string.");
	while (start != end) {
		ignoreWhitespace(start, end);
		if (*start == ']') {
			++start;
			return arr;
		}
		arr.push_back(parseJSON(start, end));
		ignoreWhitespace(start, end);
		if (*start == ']') {
			++start;
			return arr;
		}
		if (*start != ',') throw JSONException("Unexpected token.");
		++start;
	}
	throw JSONException("Unexpected end of string.");
}

JSONString parseJSONString(std::string::const_iterator& start, const std::string::const_iterator end) {
	if (start == end) throw JSONException("Unexpected end of string.");
	if (*start != '\"') throw JSONException("Unexpected token.");
	auto it = ++start;
	while (start != end) {
		if (*start == '\"') break;
		if (*start == '\\'  && std::distance(start, end) >= 2) std::advance(start, 2);
		++start;
	}
	return std::string(it, start++);
}

JSONNumber parseJSONNumber(std::string::const_iterator& start, const std::string::const_iterator end) {
	if (start == end) throw JSONException("Unexpected end of string.");
	if (*start != '-' && json::digits.find(*start) == std::string::npos) throw JSONException("Unexpected token.");
	auto it = start;
	JSONNumber num;
	bool nFlag{false};
	if (*start == '-') {
		if (++start == end) throw JSONException("Unexpected end of string.");
		nFlag = true;
	}
	if (*start == '0') ++start;
	else while (++start != end && json::digits.find(*start) != std::string::npos);
	if (*start == '.') {
		if (++start == end) throw JSONException("Unexpected end of string.");
		if (json::digits.find(*start) == std::string::npos) throw JSONException("Unexpected token.");
		else while (++start != end && json::digits.find(*start) != std::string::npos);
		if (*start == 'e' || *start == 'E') {
			if (++start == end) throw JSONException("Unexpected end of string.");
			if (*start == '+' || *start == '-')
				if (++start == end) throw JSONException("Unexpected end of string.");
			while (start != end && json::digits.find(*start) != std::string::npos) ++start;
		}
	}
	return stod(std::string(it, start));
}

JSONBool parseJSONBool(std::string::const_iterator& start, const std::string::const_iterator end) {
	if (start == end) throw JSONException("Unexpected end of string.");
	auto it = start;
	if (std::distance(start, end) >= 4) {
		std::advance(start, 4);
		if (std::string(it, start).compare("true") == 0) return true;
		if (std::distance(start, end) >= 1)
			if (std::string(it, ++start).compare("false") == 0) return false;
	}
	throw JSONException("Invalid string.");
}

void parseJSONNull(std::string::const_iterator& start, const std::string::const_iterator end) {
	if (start == end) throw JSONException("Unexpected end of string.");
	auto it = start;
	if (std::distance(start, end) >= 4) {
		std::advance(start, 4);
		if (std::string(it, start).compare("null") == 0) return;
	}
	throw JSONException("Invalid string.");
}

}

JSONNode parseJSON(const std::string& str) {
	auto it = str.cbegin();
	return parseJSON(it, str.cend());
}

JSONNode parseJSON(std::string::const_iterator& start, const std::string::const_iterator end) {
	if (start == end) throw JSONException("Invalid string.");
	if (*start == '{') return JSONNode(parseJSONObject(start, end));
	if (*start == '[') return JSONNode(parseJSONArray(start, end));
	if (*start == '\"') return JSONNode(parseJSONString(start, end));
	if (*start == '-' || json::digits.find(*start) != std::string::npos) return JSONNode(parseJSONNumber(start, end));
	if ((std::distance(start, end) >= 4 && std::string(start, start + 4).compare("true") == 0) || std::distance(start, end) >= 5 && std::string(start, start + 5).compare("false") == 0) return JSONNode(parseJSONBool(start, end));
	if (std::distance(start, end) >= 4 && std::string(start, start + 4).compare("null") == 0) {
		parseJSONNull(start, end);
		return JSONNode();
	}
	throw JSONException("Invalid string.");
}

}

}
