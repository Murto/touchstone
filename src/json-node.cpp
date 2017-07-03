#include "json-exception.hpp"
#include "json-node.hpp"

#include <cstring>
#include <sstream>

namespace touchstone {

std::string toString(const JSONObject&);
std::string toString(const JSONArray&);
std::string toString(const JSONString&);
std::string toString(const JSONNumber&);
std::string toString(const JSONBool&);

JSONNode::JSONNode() : type{JSONType::NONE} {}

JSONNode::JSONNode(const JSONNode& node) : type{node.type} {
	switch(type) {
		case JSONType::OBJECT:
			new (&value.obj) JSONObject(node.value.obj);
			break;
		case JSONType::ARRAY:
			new (&value.arr) JSONArray(node.value.arr);
			break;
		case JSONType::STRING:
			new (&value.str) JSONString(node.value.str);
			break;
		case JSONType::NUMBER:
			value.num = node.value.num;
			break;
		case JSONType::BOOL:
			value.boo = node.value.boo;
	}
}

JSONNode::JSONNode(const JSONNode&& node) : type{node.type} {
	switch(type) {
		case JSONType::OBJECT:
			new (&value.obj) JSONObject(std::move(node.value.obj));
			break;
		case JSONType::ARRAY:
			new (&value.arr) JSONArray(std::move(node.value.arr));
			break;
		case JSONType::STRING:
			new (&value.str) JSONString(std::move(node.value.str));
			break;
		case JSONType::NUMBER:
			value.num = node.value.num;
			break;
		case JSONType::BOOL:
			value.boo = node.value.boo;
	}
}

JSONNode::JSONNode(const JSONObject& obj) : type{JSONType::OBJECT}, value{obj} {}

JSONNode::JSONNode(const JSONObject&& obj) : type{JSONType::OBJECT} {
	new (&value.obj) JSONObject(std::move(obj));
}

JSONNode::JSONNode(const JSONArray& arr) : type{JSONType::ARRAY}, value{arr} {}

JSONNode::JSONNode(const JSONArray&& arr) : type{JSONType::ARRAY} {
	new (&value.arr) JSONArray(std::move(arr));
}

JSONNode::JSONNode(const JSONString& str) : type{JSONType::STRING}, value{str} {}

JSONNode::JSONNode(const JSONString&& str) : type{JSONType::STRING} {
	new (&value.str) JSONString(std::move(str));
}

JSONNode::JSONNode(const char* const str) : type{JSONType::STRING}, value{std::string(str)} {}

JSONNode::JSONNode(const JSONNumber num) : type{JSONType::NUMBER}, value{num} {}

JSONNode::JSONNode(const JSONBool boo) : type{JSONType::BOOL}, value{boo} {}

JSONNode::~JSONNode() {
	if (type == JSONType::OBJECT) value.obj.~JSONObject();
	else if (type == JSONType::ARRAY) value.arr.~JSONArray();
	else if (type == JSONType::STRING) value.str.~JSONString();
}

JSONNode& JSONNode::operator=(const JSONNode& node) {
	switch(node.type) {
		case JSONType::OBJECT:
			return *this = node.value.obj;
		case JSONType::ARRAY:
			return *this = node.value.arr;
		case JSONType::STRING:
			return *this = node.value.str;
		case JSONType::NUMBER:
			return *this = node.value.num;
		case JSONType::BOOL:
			return *this = node.value.boo;
	}
	type = JSONType::NONE;
	return *this;
}

JSONNode& JSONNode::operator=(const JSONNode&& node) {
		switch(node.type) {
		case JSONType::OBJECT:
			return *this = std::move(node.value.obj);
		case JSONType::ARRAY:
			return *this = std::move(node.value.arr);
		case JSONType::STRING:
			return *this = std::move(node.value.str);
		case JSONType::NUMBER:
			return *this = node.value.num;
		case JSONType::BOOL:
			return *this = node.value.boo;
	}
	type = JSONType::NONE;
	return *this;
}

JSONNode& JSONNode::operator=(const JSONObject& obj) {
	if (type == JSONType::OBJECT) {
		value.obj = obj;
		return *this;
	} else if (type == JSONType::ARRAY) {
		value.arr.~JSONArray();
	} else if(type == JSONType::STRING) {
		value.str.~JSONString();
	}
	type = JSONType::OBJECT;
	new (&value.obj) JSONObject(obj);
	return *this;
}

JSONNode& JSONNode::operator=(const JSONObject&& obj) {
	if (type == JSONType::OBJECT) {
		value.obj = std::move(obj);
		return *this;
	} else if (type == JSONType::ARRAY) {
		value.arr.~JSONArray();
	} else if(type == JSONType::STRING) {
		value.str.~JSONString();
	}
	type = JSONType::OBJECT;
	new (&value.obj) JSONObject(std::move(obj));
	return *this;
}

JSONNode& JSONNode::operator=(const JSONArray& arr) {
	if (type == JSONType::ARRAY) {
		value.arr = arr;
		return *this;
	} else if (type == JSONType::OBJECT) {
		value.obj.~JSONObject();
	} else if (type == JSONType::STRING) {
		value.str.~JSONString();
	}
	type = JSONType::ARRAY;
	new (&value.arr) JSONArray(arr);
	return *this;
}

JSONNode& JSONNode::operator=(const JSONArray&& arr) {
	if (type == JSONType::ARRAY) {
		value.arr = std::move(arr);
		return *this;
	} else if (type == JSONType::OBJECT) {
		value.obj.~JSONObject();
	} else if (type == JSONType::STRING) {
		value.str.~JSONString();
	}
	type = JSONType::ARRAY;
	new (&value.arr) JSONArray(std::move(arr));
	return *this;
}

JSONNode& JSONNode::operator=(const JSONString& str) {
	if (type == JSONType::STRING) {
		value.str = str;
		return *this;
	} else if (type == JSONType::OBJECT) {
		value.obj.~JSONObject();
	} else if (type == JSONType::ARRAY) {
		value.arr.~JSONArray();
	}
	type = JSONType::STRING;
	new (&value.str) JSONString(str);
	return *this;
}

JSONNode& JSONNode::operator=(const JSONString&& str) {
	if (type == JSONType::STRING) {
		value.str = std::move(str);
		return *this;
	} else if (type == JSONType::OBJECT) {
		value.obj.~JSONObject();
	} else if (type == JSONType::ARRAY) {
		value.arr.~JSONArray();
	}
	type = JSONType::STRING;
	new (&value.str) JSONString(std::move(str));
	return *this;
}


JSONNode& JSONNode::operator=(const JSONNumber& num) {
	if (type == JSONType::OBJECT) value.obj.~JSONObject();
	else if (type == JSONType::ARRAY) value.arr.~JSONArray();
	else if (type == JSONType::STRING) value.str.~JSONString();
	type = JSONType::NUMBER;
	value.num = num;
	return *this;
}

JSONNode& JSONNode::operator=(const JSONBool& boo) {
	if (type == JSONType::OBJECT) value.obj.~JSONObject();
	else if (type == JSONType::ARRAY) value.arr.~JSONArray();
	else if (type == JSONType::STRING) value.str.~JSONString();
	type = JSONType::BOOL;
	value.boo = boo;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const JSONNode& node) {
	switch(node.type) {
		case JSONType::OBJECT: {
			os << '{';
			auto it = node.value.obj.cbegin();
			while (it != node.value.obj.cend()) {
				os << '\"' << it->first << "\":" << it->second;
				if (++it != node.value.obj.cend()) os << ',';
			}
			return os << '}';
		}
		case JSONType::ARRAY: {
			auto it = node.value.arr.cbegin();
			os << '[';
			while (it != node.value.arr.cend()) {
				os << *it;
				if (++it != node.value.arr.cend()) os << ',';
			}
			return os << ']';
		}
		case JSONType::STRING:
			return os << '\"' << node.value.str << '\"';
		case JSONType::NUMBER:
			return os << node.value.num;
		case JSONType::BOOL:
			return os << (node.value.boo ? "true" : "false");
		case JSONType::NONE:
			return os << "null";
	}
}

bool JSONNode::isObject() {
	return type == JSONType::OBJECT;
}

bool JSONNode::isArray() {
	return type == JSONType::ARRAY;
}

bool JSONNode::isString() {
	return type == JSONType::STRING;
}

bool JSONNode::isNumber() {
	return type == JSONType::NUMBER;
}

bool JSONNode::isBool() {
	return type == JSONType::BOOL;
}

bool JSONNode::isNull() {
	return type == JSONType::NONE;
}

JSONObject& JSONNode::getObject() {
	if (type == JSONType::OBJECT) return value.obj;
	throw JSONException("Invalid type.");
}

JSONArray& JSONNode::getArray() {
	if (type == JSONType::ARRAY) return value.arr;
	throw JSONException("Invalid type.");
}

JSONString& JSONNode::getString() {
	if (type == JSONType::STRING) return value.str;
	throw JSONException("Invalid type.");
}

JSONNumber& JSONNode::getNumber() {
	if (type == JSONType::NUMBER) return value.num;
	throw JSONException("Invalid type.");
}

JSONBool& JSONNode::getBool() {
	if (type == JSONType::BOOL) return value.boo;
	throw JSONException("Invalid type.");
}

JSONNode& JSONNode::getNode(const JSONObject::key_type& key) {
	if (type == JSONType::OBJECT) return value.obj.at(key);
	throw JSONException("Invalid operation.");
}

JSONNode& JSONNode::getNode(const JSONArray::size_type& pos) {
	if (type == JSONType::ARRAY) return value.arr.at(pos);
	throw JSONException("Invalid operation.");
}

void JSONNode::nullify() {
	type = JSONType::NONE;
}

std::string JSONNode::toString() const {
	return static_cast<std::stringstream&>(std::stringstream() << *this).str();
}

JSONNode::Value::Value() {}


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
