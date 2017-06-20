#ifndef TOUCHSTONE_PARSING_STREAMS_HPP
#define TOUCHSTONE_PARSING_STREAMS_HPP

#include "types.hpp"

namespace touchstone {

namespace parsing_streams {

using namespace types;

JSONNode parseJSON_stream(const std::string& str);
JSONNode parseJSON_synced(const std::string& str);

JSONObject parseJSONObject(std::istringstream& ss);
JSONMember parseJSONMember(std::istringstream& ss);
JSONArray parseJSONArray(std::istringstream& ss);
JSONString parseJSONString(std::istringstream& ss);
JSONNumber parseJSONNumber(std::istringstream& ss);
JSONBool parseJSONBool(std::istringstream& ss);
void parseJSONNull(std::istringstream& ss);


}

}

#endif
