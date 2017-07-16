#include "json_node.hh"

#include <cstring>
#include <sstream>
#include <stdexcept>

namespace touchstone {

std::string to_string(const json_object&);
std::string to_string(const json_array&);
std::string to_string(const json_string&);
std::string to_string(const json_number&);
std::string to_string(const json_bool&);

json_node::json_node() : type{json_type::NONE} {}

json_node::json_node(const json_node& node) : type{node.type} {
	switch(type) {
		case json_type::OBJECT:
			new (&value.obj) json_object{node.value.obj};
			break;
		case json_type::ARRAY:
			new (&value.arr) json_array{node.value.arr};
			break;
		case json_type::STRING:
			new (&value.str) json_string{node.value.str};
			break;
		case json_type::NUMBER:
			value.num = node.value.num;
			break;
		case json_type::BOOL:
			value.boo = node.value.boo;
	}
}

json_node::json_node(json_node&& node) : type{node.type} {
	switch(type) {
		case json_type::OBJECT:
			new (&value.obj) json_object(std::move(node.value.obj));
			break;
		case json_type::ARRAY:
			new (&value.arr) json_array(std::move(node.value.arr));
			break;
		case json_type::STRING:
			new (&value.str) json_string(std::move(node.value.str));
			break;
		case json_type::NUMBER:
			value.num = node.value.num;
			break;
		case json_type::BOOL:
			value.boo = node.value.boo;
	}
}

json_node::json_node(const json_object& obj) : type{json_type::OBJECT}, value{obj} {}

json_node::json_node(json_object&& obj) : type{json_type::OBJECT} {
	new (&value.obj) json_object(std::move(obj));
}

json_node::json_node(const json_array& arr) : type{json_type::ARRAY}, value{arr} {}

json_node::json_node(json_array&& arr) : type{json_type::ARRAY} {
	new (&value.arr) json_array(std::move(arr));
}

json_node::json_node(const json_string& str) : type{json_type::STRING}, value{str} {}

json_node::json_node(json_string&& str) : type{json_type::STRING} {
	new (&value.str) json_string(std::move(str));
}

json_node::json_node(const char* const str) : type{json_type::STRING}, value{std::string(str)} {}

json_node::json_node(const json_number num) : type{json_type::NUMBER}, value{num} {}

json_node::json_node(const json_bool boo) : type{json_type::BOOL}, value{boo} {}

json_node::~json_node() {
	if (type == json_type::OBJECT) value.obj.~json_object();
	else if (type == json_type::ARRAY) value.arr.~json_array();
	else if (type == json_type::STRING) value.str.~json_string();
}

json_node& json_node::operator=(const json_node& node) {
	switch(node.type) {
		case json_type::OBJECT:
			return *this = node.value.obj;
		case json_type::ARRAY:
			return *this = node.value.arr;
		case json_type::STRING:
			return *this = node.value.str;
		case json_type::NUMBER:
			return *this = node.value.num;
		case json_type::BOOL:
			return *this = node.value.boo;
	}
	type = json_type::NONE;
	return *this;
}

json_node& json_node::operator=(json_node&& node) {
		switch(node.type) {
		case json_type::OBJECT:
			return *this = std::move(node.value.obj);
		case json_type::ARRAY:
			return *this = std::move(node.value.arr);
		case json_type::STRING:
			return *this = std::move(node.value.str);
		case json_type::NUMBER:
			return *this = node.value.num;
		case json_type::BOOL:
			return *this = node.value.boo;
	}
	type = json_type::NONE;
	return *this;
}

json_node& json_node::operator=(const json_object& obj) {
	if (type == json_type::OBJECT) {
		value.obj = obj;
		return *this;
	} else if (type == json_type::ARRAY) {
		value.arr.~json_array();
	} else if(type == json_type::STRING) {
		value.str.~json_string();
	}
	type = json_type::OBJECT;
	new (&value.obj) json_object{obj};
	return *this;
}

json_node& json_node::operator=(json_object&& obj) {
	if (type == json_type::OBJECT) {
		value.obj = std::move(obj);
		return *this;
	} else if (type == json_type::ARRAY) {
		value.arr.~json_array();
	} else if(type == json_type::STRING) {
		value.str.~json_string();
	}
	type = json_type::OBJECT;
	new (&value.obj) json_object(std::move(obj));
	return *this;
}

json_node& json_node::operator=(const json_array& arr) {
	if (type == json_type::ARRAY) {
		value.arr = arr;
		return *this;
	} else if (type == json_type::OBJECT) {
		value.obj.~json_object();
	} else if (type == json_type::STRING) {
		value.str.~json_string();
	}
	type = json_type::ARRAY;
	new (&value.arr) json_array{arr};
	return *this;
}

json_node& json_node::operator=(json_array&& arr) {
	if (type == json_type::ARRAY) {
		value.arr = std::move(arr);
		return *this;
	} else if (type == json_type::OBJECT) {
		value.obj.~json_object();
	} else if (type == json_type::STRING) {
		value.str.~json_string();
	}
	type = json_type::ARRAY;
	new (&value.arr) json_array(std::move(arr));
	return *this;
}

json_node& json_node::operator=(const json_string& str) {
	if (type == json_type::STRING) {
		value.str = str;
		return *this;
	} else if (type == json_type::OBJECT) {
		value.obj.~json_object();
	} else if (type == json_type::ARRAY) {
		value.arr.~json_array();
	}
	type = json_type::STRING;
	new (&value.str) json_string{str};
	return *this;
}

json_node& json_node::operator=(json_string&& str) {
	if (type == json_type::STRING) {
		value.str = std::move(str);
		return *this;
	} else if (type == json_type::OBJECT) {
		value.obj.~json_object();
	} else if (type == json_type::ARRAY) {
		value.arr.~json_array();
	}
	type = json_type::STRING;
	new (&value.str) json_string(std::move(str));
	return *this;
}


json_node& json_node::operator=(const json_number& num) {
	if (type == json_type::OBJECT) value.obj.~json_object();
	else if (type == json_type::ARRAY) value.arr.~json_array();
	else if (type == json_type::STRING) value.str.~json_string();
	type = json_type::NUMBER;
	value.num = num;
	return *this;
}

json_node& json_node::operator=(const json_bool& boo) {
	if (type == json_type::OBJECT) value.obj.~json_object();
	else if (type == json_type::ARRAY) value.arr.~json_array();
	else if (type == json_type::STRING) value.str.~json_string();
	type = json_type::BOOL;
	value.boo = boo;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const json_node& node) {
	switch(node.type) {
		case json_type::OBJECT: {
			os << '{';
			auto it = node.value.obj.cbegin();
			while (it != node.value.obj.cend()) {
				os << '\"' << it->first << "\":" << it->second;
				if (++it != node.value.obj.cend()) os << ',';
			}
			return os << '}';
		}
		case json_type::ARRAY: {
			auto it = node.value.arr.cbegin();
			os << '[';
			while (it != node.value.arr.cend()) {
				os << *it;
				if (++it != node.value.arr.cend()) os << ',';
			}
			return os << ']';
		}
		case json_type::STRING:
			return os << '\"' << node.value.str << '\"';
		case json_type::NUMBER:
			return os << node.value.num;
		case json_type::BOOL:
			return os << (node.value.boo ? "true" : "false");
		case json_type::NONE:
			return os << "null";
	}
}

bool json_node::is_object() const noexcept {
	return type == json_type::OBJECT;
}

bool json_node::is_array() const noexcept {
	return type == json_type::ARRAY;
}

bool json_node::is_string() const noexcept {
	return type == json_type::STRING;
}

bool json_node::is_number() const noexcept {
	return type == json_type::NUMBER;
}

bool json_node::is_bool() const noexcept {
	return type == json_type::BOOL;
}

bool json_node::is_null() const noexcept {
	return type == json_type::NONE;
}

void json_node::nullify() noexcept {
	type = json_type::NONE;
}

json_object& json_node::get_object() {
	if (type == json_type::OBJECT) return value.obj;
	throw std::runtime_error{"Invalid type."};
}

json_array& json_node::get_array() {
	if (type == json_type::ARRAY) return value.arr;
	throw std::runtime_error{"Invalid type."};
}

json_string& json_node::get_string() {
	if (type == json_type::STRING) return value.str;
	throw std::runtime_error{"Invalid type."};
}

json_number& json_node::get_number() {
	if (type == json_type::NUMBER) return value.num;
	throw std::runtime_error{"Invalid type."};
}

json_bool& json_node::get_bool() {
	if (type == json_type::BOOL) return value.boo;
	throw std::runtime_error{"Invalid type."};
}

json_node& json_node::get_node(const json_object::key_type& key) {
	if (type == json_type::OBJECT) return value.obj.at(key);
	throw std::runtime_error{"Invalid operation."};
}

json_node& json_node::get_node(const json_array::size_type& pos) {
	if (type == json_type::ARRAY) return value.arr.at(pos);
	throw std::runtime_error{"Invalid operation."};
}

std::string json_node::to_string() const {
	return static_cast<std::stringstream&>(std::stringstream() << *this).str();
}

json_node::json_value::json_value() {}

json_node::json_value::json_value(const json_object& obj) {
	new(&this->obj) json_object{obj};
}

json_node::json_value::json_value(const json_array& arr) {
	new(&this->arr) json_array{arr};
}

json_node::json_value::json_value(const json_string& str) {
	new(&this->str) json_string{str};
}

json_node::json_value::json_value(const json_number& num) {
	new(&this->num) json_number{num};
}

json_node::json_value::json_value(const json_bool& boo) {
	new (&this->boo) json_bool{boo};
}

json_node::json_value::~json_value() {}

}
