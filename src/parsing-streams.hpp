#ifndef TOUCHSTONE_PARSING_STREAMS_HPP
#define TOUCHSTONE_PARSING_STREAMS_HPP

#include "types.hpp"

namespace touchstone {

namespace parsing_streams {

using namespace types;

JSONNode parseJSON_stream(const std::string& str);
JSONNode parseJSON_synced(const std::string& str);
JSONNode parseJSON(std::istringstream& ss);

}

}

#endif
