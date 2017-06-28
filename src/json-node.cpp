#include "json-exception.hpp"
#include "json-node.hpp"
#include "parsing.hpp"

#include <sstream>

namespace touchstone {

namespace types {

using namespace parsing;

std::string toString(const JSONObject& obj);
std::string toString(const JSONArray& arr);
std::string toString(const JSONString& str);
std::string toString(const JSONNumber& num);
std::string toString(const JSONBool& boo);

JSONNode::JSONNode() : type{MetaType::NONE} {}

JSONNode::JSONNode(const JSONNode& node) : type{node.type} {
	switch(type) {
		case MetaType::OBJECT:
			value.obj = new JSONObject(*node.value.obj);
			break;
		case MetaType::ARRAY:
			value.arr = new JSONArray(*node.value.arr);
			break;
		case MetaType::STRING:
			value.str = new JSONString(*node.value.str);
			break;
		case MetaType::NUMBER:
			value.num = node.value.num;
			break;
		case MetaType::BOOL:
			value.boo = node.value.boo;
	}
}

JSONNode::JSONNode(const JSONObject& obj) : type{MetaType::OBJECT}, value{.obj = new JSONObject(obj)} {}

JSONNode::JSONNode(const JSONArray& arr) : type{MetaType::ARRAY}, value{.arr = new JSONArray(arr)} {}

JSONNode::JSONNode(const JSONString& str) : type{MetaType::STRING}, value{.str = new JSONString(str)} {}

JSONNode::JSONNode(const JSONNumber& num) : type{MetaType::NUMBER}, value{.num = num} {}

JSONNode::JSONNode(const JSONBool& boo) : type{MetaType::BOOL}, value{.boo = boo} {}

JSONNode::~JSONNode() {
	freeMemory();
}

JSONNode& JSONNode::operator=(const JSONNode& node) {
	type = node.type;
	switch(type) {
		case MetaType::OBJECT:
			value.obj = new JSONObject(*node.value.obj);
			break;
		case MetaType::ARRAY:
			value.arr = new JSONArray(*node.value.arr);
			break;
		case MetaType::STRING:
			value.str = new JSONString(*node.value.str);
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
	freeMemory();
	type = MetaType::OBJECT;
	value.obj = new JSONObject(obj);
	return *this;
}

JSONNode& JSONNode::operator=(const JSONArray& arr) {
	freeMemory();
	type = MetaType::ARRAY;
	value.arr = new JSONArray(arr);
	return *this;
}

JSONNode& JSONNode::operator=(const JSONString& str) {
	freeMemory();
	type = MetaType::STRING;
	value.str = new JSONString(str);
	return *this;
}

JSONNode& JSONNode::operator=(const JSONNumber& num) {
	freeMemory();
	type = MetaType::NUMBER;
	value.num = num;
	return *this;
}

JSONNode& JSONNode::operator=(const JSONBool& boo) {
	freeMemory();
	type = MetaType::BOOL;
	value.boo = boo;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const JSONNode& node) {
	switch(node.type) {
		case MetaType::OBJECT: {
			os << '{';
			auto it = node.value.obj->cbegin();
			while (it != node.value.obj->cend()) {
				os << '\"' << it->first << "\":" << it->second;
				if (++it != node.value.obj->cend()) os << ',';
			}
			return os << '}';
		}
		case MetaType::ARRAY: {
			auto it = node.value.arr->cbegin();
			os << '[';
			while (it != node.value.arr->cend()) {
				os << *it;
				if (++it != node.value.arr->cend()) os << ',';
			}
			return os << ']';
		}
		case MetaType::STRING:
			return os << '\"' << *node.value.str << '\"';
		case MetaType::NUMBER:
			return os << node.value.num;
		case MetaType::BOOL:
			return os << (node.value.boo ? "true" : "false");
	}
	return os << "null";
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
	if (type == MetaType::OBJECT) return *value.obj;
	throw JSONException("Invalid type.");
}

JSONArray& JSONNode::getArray() {
	if (type == MetaType::ARRAY) return *value.arr;
	throw JSONException("Invalid type.");
}

JSONString& JSONNode::getString() {
	if (type == MetaType::STRING) return *value.str;
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
	if (type == MetaType::OBJECT) return value.obj->at(key);
	throw JSONException("Invalid operation.");
}

JSONNode& JSONNode::getNode(const JSONArray::size_type& pos) {
	if (type == MetaType::ARRAY) return value.arr->at(pos);
	throw JSONException("Invalid operation.");
}

void JSONNode::nullify() {
	type = MetaType::NONE;
}

std::string JSONNode::toString() const {
	switch(type) {
		case MetaType::OBJECT:
			return types::toString(*value.obj);
		case MetaType::ARRAY:
			return types::toString(*value.arr);
		case MetaType::STRING:
			return types::toString(*value.str);
		case MetaType::NUMBER:
			return types::toString(value.num);
		case MetaType::BOOL:
			return types::toString(value.boo);
	}
	return "null";
}

std::string toString(const JSONObject& obj) {
	std::stringstream ss;
	ss << '{';
	auto it = obj.cbegin();
	while (it != obj.cend()) {
		ss << '\"' << it->first << '\"' << ':' << it->second.toString();
		if (++it != obj.cend()) ss << ',';
	}
	ss << '}';
	return ss.str();
}

std::string toString(const JSONArray& arr) {
	std::stringstream ss;
	ss << '[';
	auto it = arr.cbegin();
	while (it != arr.cend()) {
		ss << it->toString();
		if (++it != arr.cend()) ss << ',';
	}
	ss << ']';
	return ss.str();
}

std::string toString(const JSONString& str) {
	return "\"" + str + "\"";
}

std::string toString(const JSONNumber& num) {
	return std::to_string(num);
}

std::string toString(const JSONBool& boo) {
	return boo ? "true" : "false";
}

void JSONNode::freeMemory() {
	if (type == MetaType::STRING) delete value.str;
	else if (type == MetaType::OBJECT) delete value.obj;
	else if (type == MetaType::ARRAY) delete value.arr;
}

}

}
