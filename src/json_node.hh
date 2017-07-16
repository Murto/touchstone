// Once compiler support for C++17 is
// mainstream on gentoo the data union
// will be replaced with std::variant

#ifndef JSON_NODE_HH
#define JSON_NODE_HH

#include "types.hh"

#include <ostream>

namespace touchstone {

class json_node {
public:
	json_node();
	json_node(const json_node&);
	json_node(json_node&&);
	json_node(const json_object&);
	json_node(json_object&&);
	json_node(const json_array&);
	json_node(json_array&&);
	json_node(const json_string&);
	json_node(json_string&&);
	json_node(const char* const);
	json_node(const json_number);
	json_node(const json_bool);
	~json_node();
	friend std::ostream& operator<<(std::ostream&, const json_node&);
	json_node& operator=(const json_node&);
	json_node& operator=(json_node&&);
	json_node& operator=(const json_object&);
	json_node& operator=(json_object&&);
	json_node& operator=(const json_array&);
	json_node& operator=(json_array&&);
	json_node& operator=(const json_string&);
	json_node& operator=(json_string&&);
	json_node& operator=(const json_number&);
	json_node& operator=(const json_bool&);
	bool is_object() const noexcept;
	bool is_array() const noexcept;
	bool is_string() const noexcept;
	bool is_number() const noexcept;
	bool is_bool() const noexcept;
	bool is_null() const noexcept;
	void nullify() noexcept;
	json_object& get_object();
	json_array& get_array();
	json_string& get_string();
	json_number& get_number();
	json_bool& get_bool();
	json_node& get_node(const json_object::key_type&);
	json_node& get_node(const json_array::size_type&);
	std::string to_string() const;

private:
	union json_value {
		json_value();
		json_value(const json_object&);
		json_value(const json_array&);
		json_value(const json_string&);
		json_value(const json_number&);
		json_value(const json_bool&);
		~json_value();
		json_object obj;
		json_array arr;
		json_string str;
		json_number num;
		json_bool boo;
	} value;
	json_type type;
};

}

#endif
