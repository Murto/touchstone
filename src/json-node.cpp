#include "json-exception.hpp"
#include "json-node.hpp"
#include "parsing.hpp"

#include <cstring>
#include <sstream>

namespace touchstone {

std::string toString(const JSONObject& obj);
std::string toString(const JSONArray& arr);
std::string toString(const JSONString& str);
std::string toString(const JSONNumber& num);
std::string toString(const JSONBool& boo);

JSONNode::JSONNode() : type{MetaType::NONE} {}

JSONNode::JSONNode(const JSONNode& node) : type{node.type} {
	switch(type) {
		case MetaType::OBJECT:
			new (&value.obj) JSONObject(node.value.obj);
			break;
		case MetaType::ARRAY:
			new (&value.arr) JSONArray(node.value.arr);
			break;
		case MetaType::STRING:
			new (&value.str) JSONString(node.value.str);
			break;
		case MetaType::NUMBER:
			value.num = node.value.num;
			break;
		case MetaType::BOOL:
			value.boo = node.value.boo;
	}
}

JSONNode::JSONNode(const JSONObject& obj) : type{MetaType::OBJECT}, value{obj} {}

JSONNode::JSONNode(const JSONArray& arr) : type{MetaType::ARRAY}, value{arr} {}

JSONNode::JSONNode(const JSONString& str) : type{MetaType::STRING}, value{str} {}

JSONNode::JSONNode(const char* const str) : type{MetaType::STRING}, value{std::string(str)} {}

JSONNode::JSONNode(const JSONNumber& num) : type{MetaType::NUMBER}, value{num} {}

JSONNode::JSONNode(const JSONBool& boo) : type{MetaType::BOOL}, value{boo} {}

JSONNode::~JSONNode() {
	if (type == MetaType::OBJECT) value.obj.~JSONObject();
	else if (type == MetaType::ARRAY) value.arr.~JSONArray();
	else if (type == MetaType::STRING) value.str.~JSONString();
}

JSONNode& JSONNode::operator=(const JSONNode& node) {
	type = node.type;
	switch(type) {
		case MetaType::OBJECT:
			value.obj = node.value.obj;
			break;
		case MetaType::ARRAY:
			value.arr = node.value.arr;
			break;
		case MetaType::STRING:
			value.str = node.value.str;
			break;
		case MetaType::NUMBER:
			value.num = node.value.num;
			break;
		case MetaType::BOOL:
			value.boo = node.value.boo;
	}
	return *this;
}

JSONNode& JSONNode::operator=(const JSONObject& obj) {
	if (type == MetaType::OBJECT) {
		value.obj = obj;
		return *this;
	} else if (type == MetaType::ARRAY) {
		value.arr.~JSONArray();
	} else if(type == MetaType::STRING) {
		value.str.~JSONString();
	}
	type = MetaType::OBJECT;
	new (&value.obj) JSONObject(obj);
	return *this;
}

JSONNode& JSONNode::operator=(const JSONArray& arr) {
	if (type == MetaType::ARRAY) {
		value.arr = arr;
		return *this;
	} else if (type == MetaType::OBJECT) {
		value.obj.~JSONObject();
	} else if (type == MetaType::STRING) {
		value.str.~JSONString();
	}
	type = MetaType::ARRAY;
	new (&value.arr) JSONArray(arr);
	return *this;
}

JSONNode& JSONNode::operator=(const JSONString& str) {
	if (type == MetaType::STRING) {
		value.str = str;
		return *this;
	} else if (type == MetaType::OBJECT) {
		value.obj.~JSONObject();
	} else if (type == MetaType::ARRAY) {
		value.arr.~JSONArray();
	}
	type = MetaType::STRING;
	new (&value.str) JSONString(str);
	return *this;
}

JSONNode& JSONNode::operator=(const JSONNumber& num) {
	if (type == MetaType::OBJECT) value.obj.~JSONObject();
	else if (type == MetaType::ARRAY) value.arr.~JSONArray();
	else if (type == MetaType::STRING) value.str.~JSONString();
	type = MetaType::NUMBER;
	value.num = num;
	return *this;
}

JSONNode& JSONNode::operator=(const JSONBool& boo) {
	if (type == MetaType::OBJECT) value.obj.~JSONObject();
	else if (type == MetaType::ARRAY) value.arr.~JSONArray();
	else if (type == MetaType::STRING) value.str.~JSONString();
	type = MetaType::BOOL;
	value.boo = boo;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const JSONNode& node) {
	switch(node.type) {
		case MetaType::OBJECT: {
			os << '{';
			auto it = node.value.obj.cbegin();
			while (it != node.value.obj.cend()) {
				os << '\"' << it->first << "\":" << it->second;
				if (++it != node.value.obj.cend()) os << ',';
			}
			return os << '}';
		}
		case MetaType::ARRAY: {
			auto it = node.value.arr.cbegin();
			os << '[';
			while (it != node.value.arr.cend()) {
				os << *it;
				if (++it != node.value.arr.cend()) os << ',';
			}
			return os << ']';
		}
		case MetaType::STRING:
			return os << '\"' << node.value.str << '\"';
		case MetaType::NUMBER:
			return os << node.value.num;
		case MetaType::BOOL:
			return os << (node.value.boo ? "true" : "false");
		case MetaType::NONE:
			return os << "null";
	}
}

bool JSONNode::isObject() {
	return type == MetaType::OBJECT;
}

bool JSONNode::isArray() {
	return type == MetaType::ARRAY;
}

bool JSONNode::isString() {
	return type == MetaType::STRING;
}

bool JSONNode::isNumber() {
	return type == MetaType::NUMBER;
}

bool JSONNode::isBool() {
	return type == MetaType::BOOL;
}

bool JSONNode::isNull() {
	return type == MetaType::NONE;
}

JSONObject& JSONNode::getObject() {
	if (type == MetaType::OBJECT) return value.obj;
	throw JSONException("Invalid type.");
}

JSONArray& JSONNode::getArray() {
	if (type == MetaType::ARRAY) return value.arr;
	throw JSONException("Invalid type.");
}

JSONString& JSONNode::getString() {
	if (type == MetaType::STRING) return value.str;
	throw JSONException("Invalid type.");
}

JSONNumber& JSONNode::getNumber() {
	if (type == MetaType::NUMBER) return value.num;
	throw JSONException("Invalid type.");
}

JSONBool& JSONNode::getBool() {
	if (type == MetaType::BOOL) return value.boo;
	throw JSONException("Invalid type.");
}

JSONNode& JSONNode::getNode(const JSONObject::key_type& key) {
	if (type == MetaType::OBJECT) return value.obj.at(key);
	throw JSONException("Invalid operation.");
}

JSONNode& JSONNode::getNode(const JSONArray::size_type& pos) {
	if (type == MetaType::ARRAY) return value.arr.at(pos);
	throw JSONException("Invalid operation.");
}

void JSONNode::nullify() {
	type = MetaType::NONE;
}

std::string JSONNode::toString() const {
	return static_cast<std::stringstream&>(std::stringstream() << *this).str();
}

JSONNode::Value::Value() {
	std::memset(this, 0, sizeof(*this));
}


JSONNode::Value::Value(const JSONObject& obj) {
	new(&this->obj) JSONObject(obj);
}

JSONNode::Value::Value(const JSONArray& arr) {
	new(&this->arr) JSONArray(arr);
}

JSONNode::Value::Value(const JSONString& str) {
	new(&this->str) JSONString(str);
}

JSONNode::Value::Value(const JSONNumber& num) {
	new(&this->num) JSONNumber(num);
}

JSONNode::Value::Value(const JSONBool& boo) {
	new (&this->boo) JSONBool(boo);
}

JSONNode::Value::~Value() {}

}
