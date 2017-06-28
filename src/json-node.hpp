// Once compiler support for C++17 is
// mainstream on gentoo the data union
// will be replaced with std::variant

#ifndef JSON_NODE_HPP
#define JSON_NODE_HPP

#include "meta-type.hpp"
#include "types.hpp"

#include <ostream>

namespace touchstone {

namespace types {

class JSONNode {
public:
	JSONNode();
	JSONNode(const JSONNode& node);
	JSONNode(const JSONObject& obj);
	JSONNode(const JSONArray& arr);
	JSONNode(const JSONString& str);
	JSONNode(const JSONNumber& num);
	JSONNode(const JSONBool& boo);
	~JSONNode();
	JSONNode& operator=(const JSONNode& node);
	JSONNode& operator=(const JSONObject& obj);
	JSONNode& operator=(const JSONArray& arr);
	JSONNode& operator=(const JSONString& str);
	JSONNode& operator=(const JSONNumber& num);
	JSONNode& operator=(const JSONBool& num);
	bool isObject();
	bool isArray();
	bool isString();
	bool isNumber();
	bool isBool();
	bool isNull();
	JSONObject& getObject();
	JSONArray& getArray();
	JSONString& getString();
	JSONNumber& getNumber();
	JSONBool& getBool();
	JSONNode& getNode(const JSONObject::key_type& key);
	JSONNode& getNode(const JSONArray::size_type& pos);
	void nullify();
	std::string toString() const;
	friend std::ostream& operator<<(std::ostream& os, const JSONNode& node);
private:
	void freeMemory();

	MetaType type;
	union Value {
		JSONObject* obj;
		JSONArray* arr;
		JSONString* str;
		JSONNumber num;
		JSONBool boo;
	} value;
};

}

}

#endif
