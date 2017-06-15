#ifndef JSON_EXCEPTION_HPP
#define JSON_EXCEPTION_HPP

#include <stdexcept>

namespace touchstone {

class JSONException : public std::runtime_error {
public:
	JSONException(const char* msg);
	const char* what() const noexcept;
};

}

#endif
