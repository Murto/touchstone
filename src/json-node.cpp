#include "json-node.hpp"

#include <sstream>

namespace touchstone {

JSONNode::JSONNode() : type{JSONType::NONE} {}

JSONNode::JSONNode(const JSONNode& node) {
	type = node.type;
	switch(type) {
		case JSONType::OBJECT:
			value.obj = new JSONObject(*node.value.obj);
			break;
		case JSONType::ARRAY:
			value.arr = new JSONArray(*node.value.arr);
			break;
		case JSONType::STRING:
			value.str = new JSONString(*node.value.str);
			break;
		case JSONType::NUMBER:
			value.num = node.value.num;
			break;
		case JSONType::BOOL:
			value.boo = node.value.boo;
	}
}

JSONNode::JSONNode(const JSONObject& obj) : type{JSONType::OBJECT} {
	value.obj = new JSONObject(obj);
}

JSONNode::JSONNode(const JSONArray& arr) : type{JSONType::ARRAY} {
	value.arr = new JSONArray(arr);
}

JSONNode::JSONNode(const JSONString& str) : type{JSONType::STRING} {
	value.str = new JSONString(str);
}

JSONNode::JSONNode(const JSONNumber& num) : type{JSONType::NUMBER} {
	value.num = num;
}

JSONNode::JSONNode(const JSONBool& boo) : type{JSONType::BOOL} {
	value.boo = boo;
}

JSONNode::~JSONNode() {
	switch(type) {
		case JSONType::OBJECT:
			delete value.obj;
			break;
		case JSONType::ARRAY:
			delete value.arr;
			break;
		case JSONType::STRING:
			delete value.str;
	}
}

JSONNode& JSONNode::operator=(const JSONNode& node) {
	type = node.type;
	switch(type) {
		case JSONType::OBJECT:
			value.obj = new JSONObject(*node.value.obj);
			break;
		case JSONType::ARRAY:
			value.arr = new JSONArray(*node.value.arr);
			break;
		case JSONType::STRING:
			value.str = new JSONString(*node.value.str);
			break;
		case JSONType::NUMBER:
			value.num = node.value.num;
			break;
		case JSONType::BOOL:
			value.boo = node.value.boo;
	}
	return *this;
}

JSONObject& JSONNode::getObject() {
	if (type == JSONType::OBJECT) return *value.obj;
	throw JSONException("Invalid type.");
}

JSONArray& JSONNode::getArray() {
	if (type == JSONType::ARRAY) return *value.arr;
	throw JSONException("Invalid type.");
}

JSONString& JSONNode::getString() {
	if (type == JSONType::STRING) return *value.str;
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
	if (type == JSONType::OBJECT) return value.obj->at(key);
	throw JSONException("Invalid operation.");
}

JSONNode& JSONNode::getNode(const JSONArray::size_type& pos) {
	if (type == JSONType::ARRAY) return value.arr->at(pos);
	throw JSONException("Invalid operation.");
}

void JSONNode::setValue(const JSONObject& obj) {
	type = JSONType::OBJECT;
	value.obj = new JSONObject(obj);
}

void JSONNode::setValue(const JSONArray& arr) {
	type = JSONType::ARRAY;
	value.arr = new JSONArray(arr);
}

void JSONNode::setValue(const JSONString& str) {
	type = JSONType::STRING;
	value.str = new JSONString(str);
}

void JSONNode::setValue(const JSONNumber& num) {
	type = JSONType::NUMBER;
	value.num = num;
}

void JSONNode::setValue(const JSONBool& boo) {
	type = JSONType::BOOL;
	value.boo = boo;
}

void JSONNode::nullify() {
	type = JSONType::NONE;
}

std::string JSONNode::toString() const {
	switch(type) {
		case JSONType::OBJECT:
			return touchstone::toString(*value.obj);
		case JSONType::ARRAY:
			return touchstone::toString(*value.arr);
		case JSONType::STRING:
			return touchstone::toString(*value.str);
		case JSONType::NUMBER:
			return touchstone::toString(value.num);
		case JSONType::BOOL:
			return touchstone::toString(value.boo);
	}
	return "null";
}

}
