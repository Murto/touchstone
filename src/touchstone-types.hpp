#ifndef TOUCHSTONE_TYPES_HPP
#define TOUCHSTONE_TYPES_HPP

#include <map>
#include <string>
#include <utility>
#include <vector>

namespace touchstone {

class JSONNode;

typedef std::vector<JSONNode> JSONArray;
typedef std::string JSONString;
typedef std::map<JSONString, JSONNode> JSONObject;
typedef std::pair<JSONString, JSONNode> JSONMember;
typedef double JSONNumber;
typedef bool JSONBool;

}

#endif
