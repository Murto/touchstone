#ifndef TOUCHSTONE_PARSING_HPP
#define TOUCHSTONE_PARSING_HPP

#include "types.hpp"

namespace touchstone {

namespace parsing {

using namespace types;

std::string toString(const JSONObject& obj);
std::string toString(const JSONArray& arr);
std::string toString(const JSONString& str);
std::string toString(const JSONNumber& num);
std::string toString(const JSONBool& boo);
JSONNode parseJSON(const std::string& str);
JSONNode parseJSON(std::string::const_iterator& start, const std::string::const_iterator end);

}

}

#endif
