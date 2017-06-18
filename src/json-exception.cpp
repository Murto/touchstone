#include "json-exception.hpp"

namespace touchstone {

namespace types {

JSONException::JSONException(const char* msg) : std::runtime_error(msg) {}

const char* JSONException::what() const noexcept {
	return std::runtime_error::what();
}

}

}
