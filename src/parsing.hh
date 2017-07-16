#ifndef TOUCHSTONE_PARSING_HH
#define TOUCHSTONE_PARSING_HH

#include "types.hh"

namespace touchstone {

json_node parse_json(const std::string&);
json_node parse_json(std::istream&);

}

#endif
