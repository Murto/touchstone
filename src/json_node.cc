#include "json_node.hh"

#include <cstring>
#include <sstream>
#include <stdexcept>

namespace touchstone {

std::string to_string(const json_node::object_type&);
std::string to_string(const json_node::array_type&);
std::string to_string(const json_node::string_type&);
std::string to_string(const json_node::number_type&);
std::string to_string(const json_node::bool_type&);

json_node::json_node(const json_node& p_node) : m_type{p_node.m_type} {
	switch(m_type) {
		case json_type::OBJECT:
			new (&m_value.obj) object_type{p_node.m_value.obj};
			break;
		case json_type::ARRAY:
			new (&m_value.arr) array_type{p_node.m_value.arr};
			break;
		case json_type::STRING:
			new (&m_value.str) string_type{p_node.m_value.str};
			break;
		case json_type::NUMBER:
			m_value.num = p_node.m_value.num;
			break;
		case json_type::BOOL:
			m_value.boo = p_node.m_value.boo;
	}
}

json_node::json_node(json_node&& p_node) : m_type{p_node.m_type} {
	switch(m_type) {
		case json_type::OBJECT:
			new (&m_value.obj) object_type(std::move(p_node.m_value.obj));
			break;
		case json_type::ARRAY:
			new (&m_value.arr) array_type(std::move(p_node.m_value.arr));
			break;
		case json_type::STRING:
			new (&m_value.str) string_type(std::move(p_node.m_value.str));
			break;
		case json_type::NUMBER:
			m_value.num = p_node.m_value.num;
			break;
		case json_type::BOOL:
			m_value.boo = p_node.m_value.boo;
	}
}

json_node::json_node(const object_type& p_obj) : m_type{json_type::OBJECT}, m_value{p_obj} {}

json_node::json_node(object_type&& p_obj) : m_type{json_type::OBJECT} {
	new (&m_value.obj) object_type(std::move(p_obj));
}

json_node::json_node(const array_type& p_arr) : m_type{json_type::ARRAY}, m_value{p_arr} {}

json_node::json_node(array_type&& p_arr) : m_type{json_type::ARRAY} {
	new (&m_value.arr) array_type(std::move(p_arr));
}

json_node::json_node(const string_type& p_str) : m_type{json_type::STRING}, m_value{p_str} {}

json_node::json_node(string_type&& p_str) : m_type{json_type::STRING} {
	new (&m_value.str) string_type(std::move(p_str));
}

json_node::json_node(const char* const p_str) : m_type{json_type::STRING}, m_value{std::string(p_str)} {}

json_node::json_node(const number_type p_num) : m_type{json_type::NUMBER}, m_value{p_num} {}

json_node::json_node(const bool_type boo) : m_type{json_type::BOOL}, m_value{boo} {}

json_node::~json_node() {
	if (m_type == json_type::OBJECT) m_value.obj.~object_type();
	else if (m_type == json_type::ARRAY) m_value.arr.~array_type();
	else if (m_type == json_type::STRING) m_value.str.~string_type();
}

json_node& json_node::operator=(const json_node& p_node) {
	switch(p_node.m_type) {
		case json_type::OBJECT:
			return *this = p_node.m_value.obj;
		case json_type::ARRAY:
			return *this = p_node.m_value.arr;
		case json_type::STRING:
			return *this = p_node.m_value.str;
		case json_type::NUMBER:
			return *this = p_node.m_value.num;
		case json_type::BOOL:
			return *this = p_node.m_value.boo;
	}
	m_type = json_type::NONE;
	return *this;
}

json_node& json_node::operator=(json_node&& p_node) {
		switch(p_node.m_type) {
		case json_type::OBJECT:
			return *this = std::move(p_node.m_value.obj);
		case json_type::ARRAY:
			return *this = std::move(p_node.m_value.arr);
		case json_type::STRING:
			return *this = std::move(p_node.m_value.str);
		case json_type::NUMBER:
			return *this = p_node.m_value.num;
		case json_type::BOOL:
			return *this = p_node.m_value.boo;
	}
	m_type = json_type::NONE;
	return *this;
}

json_node& json_node::operator=(const object_type& p_obj) {
	if (m_type == json_type::OBJECT) {
		m_value.obj = p_obj;
		return *this;
	} else if (m_type == json_type::ARRAY) {
		m_value.arr.~array_type();
	} else if(m_type == json_type::STRING) {
		m_value.str.~string_type();
	}
	m_type = json_type::OBJECT;
	new (&m_value.obj) object_type{p_obj};
	return *this;
}

json_node& json_node::operator=(object_type&& p_obj) {
	if (m_type == json_type::OBJECT) {
		m_value.obj = std::move(p_obj);
		return *this;
	} else if (m_type == json_type::ARRAY) {
		m_value.arr.~array_type();
	} else if(m_type == json_type::STRING) {
		m_value.str.~string_type();
	}
	m_type = json_type::OBJECT;
	new (&m_value.obj) object_type(std::move(p_obj));
	return *this;
}

json_node& json_node::operator=(const array_type& p_arr) {
	if (m_type == json_type::ARRAY) {
		m_value.arr = p_arr;
		return *this;
	} else if (m_type == json_type::OBJECT) {
		m_value.obj.~object_type();
	} else if (m_type == json_type::STRING) {
		m_value.str.~string_type();
	}
	m_type = json_type::ARRAY;
	new (&m_value.arr) array_type{p_arr};
	return *this;
}

json_node& json_node::operator=(array_type&& p_arr) {
	if (m_type == json_type::ARRAY) {
		m_value.arr = std::move(p_arr);
		return *this;
	} else if (m_type == json_type::OBJECT) {
		m_value.obj.~object_type();
	} else if (m_type == json_type::STRING) {
		m_value.str.~string_type();
	}
	m_type = json_type::ARRAY;
	new (&m_value.arr) array_type(std::move(p_arr));
	return *this;
}

json_node& json_node::operator=(const string_type& p_str) {
	if (m_type == json_type::STRING) {
		m_value.str = p_str;
		return *this;
	} else if (m_type == json_type::OBJECT) {
		m_value.obj.~object_type();
	} else if (m_type == json_type::ARRAY) {
		m_value.arr.~array_type();
	}
	m_type = json_type::STRING;
	new (&m_value.str) string_type{p_str};
	return *this;
}

json_node& json_node::operator=(string_type&& p_str) {
	if (m_type == json_type::STRING) {
		m_value.str = std::move(p_str);
		return *this;
	} else if (m_type == json_type::OBJECT) {
		m_value.obj.~object_type();
	} else if (m_type == json_type::ARRAY) {
		m_value.arr.~array_type();
	}
	m_type = json_type::STRING;
	new (&m_value.str) string_type(std::move(p_str));
	return *this;
}


json_node& json_node::operator=(const number_type& p_num) {
	if (m_type == json_type::OBJECT) m_value.obj.~object_type();
	else if (m_type == json_type::ARRAY) m_value.arr.~array_type();
	else if (m_type == json_type::STRING) m_value.str.~string_type();
	m_type = json_type::NUMBER;
	m_value.num = p_num;
	return *this;
}

json_node& json_node::operator=(const bool_type& p_boo) {
	if (m_type == json_type::OBJECT) m_value.obj.~object_type();
	else if (m_type == json_type::ARRAY) m_value.arr.~array_type();
	else if (m_type == json_type::STRING) m_value.str.~string_type();
	m_type = json_type::BOOL;
	m_value.boo = p_boo;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const json_node& p_node) {
	using json_type = json_node::json_type;
	switch(p_node.m_type) {
		case json_type::OBJECT: {
			os << '{';
			auto it = p_node.m_value.obj.cbegin();
			while (it != p_node.m_value.obj.cend()) {
				os << '\"' << it->first << "\":" << it->second;
				if (++it != p_node.m_value.obj.cend()) os << ',';
			}
			return os << '}';
		}
		case json_type::ARRAY: {
			auto it = p_node.m_value.arr.cbegin();
			os << '[';
			while (it != p_node.m_value.arr.cend()) {
				os << *it;
				if (++it != p_node.m_value.arr.cend()) os << ',';
			}
			return os << ']';
		}
		case json_type::STRING:
			return os << '\"' << p_node.m_value.str << '\"';
		case json_type::NUMBER:
			return os << p_node.m_value.num;
		case json_type::BOOL:
			return os << (p_node.m_value.boo ? "true" : "false");
		case json_type::NONE:
			return os << "null";
	}
}

bool json_node::is_object() const noexcept {
	return m_type == json_type::OBJECT;
}

bool json_node::is_array() const noexcept {
	return m_type == json_type::ARRAY;
}

bool json_node::is_string() const noexcept {
	return m_type == json_type::STRING;
}

bool json_node::is_number() const noexcept {
	return m_type == json_type::NUMBER;
}

bool json_node::is_bool() const noexcept {
	return m_type == json_type::BOOL;
}

bool json_node::is_null() const noexcept {
	return m_type == json_type::NONE;
}

void json_node::nullify() noexcept {
	m_type = json_type::NONE;
}

json_node::object_type& json_node::get_object() {
	if (m_type == json_type::OBJECT) return m_value.obj;
	throw std::runtime_error{"Invalid type."};
}

json_node::array_type& json_node::get_array() {
	if (m_type == json_type::ARRAY) return m_value.arr;
	throw std::runtime_error{"Invalid type."};
}

json_node::string_type& json_node::get_string() {
	if (m_type == json_type::STRING) return m_value.str;
	throw std::runtime_error{"Invalid type."};
}

json_node::number_type& json_node::get_number() {
	if (m_type == json_type::NUMBER) return m_value.num;
	throw std::runtime_error{"Invalid type."};
}

json_node::bool_type& json_node::get_bool() {
	if (m_type == json_type::BOOL) return m_value.boo;
	throw std::runtime_error{"Invalid type."};
}

json_node& json_node::get_node(const object_type::key_type& p_key) {
	if (m_type == json_type::OBJECT) return m_value.obj.at(p_key);
	throw std::runtime_error{"Invalid operation."};
}

json_node& json_node::get_node(const array_type::size_type& p_pos) {
	if (m_type == json_type::ARRAY) return m_value.arr.at(p_pos);
	throw std::runtime_error{"Invalid operation."};
}

std::string json_node::to_string() const {
	return static_cast<std::stringstream&>(std::stringstream() << *this).str();
}


json_node::json_value::json_value() noexcept {}

json_node::json_value::json_value(const object_type& p_obj) {
	new(&this->obj) object_type{p_obj};
}

json_node::json_value::json_value(const array_type& p_arr) {
	new(&this->arr) array_type{p_arr};
}

json_node::json_value::json_value(const string_type& p_str) {
	new(&this->str) string_type{p_str};
}

json_node::json_value::json_value(const number_type& p_num) {
	new(&this->num) number_type{p_num};
}

json_node::json_value::json_value(const bool_type& p_boo) {
	new (&this->boo) bool_type{p_boo};
}

json_node::json_value::~json_value() noexcept {}

}
