#ifndef TOUCHSTONE_HPP
#define TOUCHSTONE_HPP

#include "touchstone-types.hpp"

#include <string>

namespace touchstone {
	std::string toString(const JSONObject& obj);
	std::string toString(const JSONArray& arr);
	std::string toString(const JSONString& str);
	std::string toString(const JSONNumber& num);
	std::string toString(const JSONBool& boo);
	JSONNode parseJSON(const std::string& str);
	JSONNode parseJSON(std::string::const_iterator& start, const std::string::const_iterator end);
}

#endif
