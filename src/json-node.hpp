// Once compiler support for C++17 is
// mainstream on gentoo the data union
// will be replaced with std::variant

#ifndef JSON_NODE_HPP
#define JSON_NODE_HPP

#include "types.hpp"

#include <ostream>

namespace touchstone {

class JSONNode {
public:
	JSONNode();
	JSONNode(const JSONNode&);
	JSONNode(const JSONObject&);
	JSONNode(const JSONArray&);
	JSONNode(const JSONString&);
	JSONNode(const char* const);
	JSONNode(const JSONNumber&);
	JSONNode(const JSONBool&);
	~JSONNode();
	JSONNode& operator=(const JSONNode&);
	JSONNode& operator=(const JSONObject&);
	JSONNode& operator=(const JSONArray&);
	JSONNode& operator=(const JSONString&);
	JSONNode& operator=(const JSONNumber&);
	JSONNode& operator=(const JSONBool&);
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
	JSONNode& getNode(const JSONObject::key_type&);
	JSONNode& getNode(const JSONArray::size_type&);
	void nullify();
	std::string toString() const;
	friend std::ostream& operator<<(std::ostream&, const JSONNode&);
private:
	JSONType type;
	union Value {
		Value();
		Value(const JSONObject&);
		Value(const JSONArray&);
		Value(const JSONString&);
		Value(const JSONNumber&);
		Value(const JSONBool&);
		~Value();
		JSONObject obj;
		JSONArray arr;
		JSONString str;
		JSONNumber num;
		JSONBool boo;
		
	} value;
};

}

#endif
