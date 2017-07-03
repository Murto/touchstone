#ifndef TOUCHSTONE_PARSING_HPP
#define TOUCHSTONE_PARSING_HPP

#include "types.hpp"

namespace touchstone {

JSONNode parseJSON(const std::string&);
JSONNode parseJSON(std::istream&);

}

#endif
