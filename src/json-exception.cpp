#include "json-exception.hpp"

namespace touchstone {

JSONException::JSONException(const char* msg) : std::runtime_error(msg) {}

JSONException::JSONException(const std::string& msg) : std::runtime_error(msg.c_str()) {}

const char* JSONException::what() const noexcept {
	return std::runtime_error::what();
}

}
