#ifndef JSON_EXCEPTION_HPP
#define JSON_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace touchstone {

class JSONException : public std::runtime_error {
public:
	JSONException(const std::string& msg);
	const char* what() const noexcept;
};

}

#endif
