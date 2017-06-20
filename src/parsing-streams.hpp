#ifndef TOUCHSTONE_PARSING_STREAMS_HPP
#define TOUCHSTONE_PARSING_STREAMS_HPP

#include "types.hpp"

namespace touchstone {

namespace parsing {

using namespace types;

JSONNode parseJSON_stream(const std::string& str);
JSONNode parseJSON_stream(std::string::const_iterator& start, const std::string::const_iterator end);
JSONNode parseJSON_synced(const std::string& str);
JSONNode parseJSON_synced(std::string::const_iterator& start, const std::string::const_iterator end);

}

}

#endif
