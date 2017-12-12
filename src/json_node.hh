// Once compiler support for C++17 is
// mainstream on gentoo the data union
// will be replaced with std::variant

#pragma once

#include <map>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace touchstone {

class json_node {
public:

	enum class json_type {
		OBJECT,
		ARRAY,
		STRING,
		NUMBER,
		BOOL,
		NONE
	};

	using array_type = std::vector<json_node>;
	using string_type = std::string;
	using object_type = std::map<string_type, json_node>;
	using number_type = double;
	using bool_type = bool;

	json_node() noexcept = default;
	json_node(const json_node&);
	json_node(json_node&&);
	json_node(const object_type&);
	json_node(object_type&&);
	json_node(const array_type&);
	json_node(array_type&&);
	json_node(const string_type&);
	json_node(string_type&&);
	json_node(const char* const);
	json_node(const number_type);
	json_node(const bool_type);
	~json_node();
	friend std::ostream& operator<<(std::ostream&, const json_node&);
	json_node& operator=(const json_node&);
	json_node& operator=(json_node&&);
	json_node& operator=(const object_type&);
	json_node& operator=(object_type&&);
	json_node& operator=(const array_type&);
	json_node& operator=(array_type&&);
	json_node& operator=(const string_type&);
	json_node& operator=(string_type&&);
	json_node& operator=(const number_type&);
	json_node& operator=(const bool_type&);
	bool is_object() const noexcept;
	bool is_array() const noexcept;
	bool is_string() const noexcept;
	bool is_number() const noexcept;
	bool is_bool() const noexcept;
	bool is_null() const noexcept;
	void nullify() noexcept [[noreturn]];
	object_type& get_object();
	array_type& get_array();
	string_type& get_string();
	number_type& get_number();
	bool_type& get_bool();
	json_node& get_node(const object_type::key_type&);
	json_node& get_node(const array_type::size_type&);
	std::string to_string() const;

private:
	union json_value {
		json_value() noexcept;
		json_value(const object_type&);
		json_value(const array_type&);
		json_value(const string_type&);
		json_value(const number_type&);
		json_value(const bool_type&);
		~json_value();
		object_type obj;
		array_type arr;
		string_type str;
		number_type num;
		bool_type boo;
	} m_value;
	json_type m_type{json_type::NONE};
};

}
