#ifndef TOUCHSTONE_PARSING_HPP
#define TOUCHSTONE_PARSING_HPP

#include "types.hpp"

namespace touchstone {

namespace parsing {

using namespace types;

JSONNode parseJSON(const std::string& str);
JSONNode parseJSON(std::istream& ss);

}

}

#endif
