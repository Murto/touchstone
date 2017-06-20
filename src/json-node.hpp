// Once compiler support for C++17 is
// mainstream on gentoo the data union
// will be replaced with std::variant

#ifndef JSON_NODE_HPP
#define JSON_NODE_HPP

#include "meta-type.hpp"
#include "types.hpp"

#include <type_traits>

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
	JSONObject& getObject();
	JSONArray& getArray();
	JSONString& getString();
	JSONNumber& getNumber();
	JSONBool& getBool();
	JSONNode& getNode(const JSONObject::key_type& key);
	JSONNode& getNode(const JSONArray::size_type& pos);
	void setValue(const JSONObject& obj);
	void setValue(const JSONArray& arr);
	void setValue(const JSONString& str);
	void setValue(const JSONNumber& num);
	void setValue(const JSONBool& boo);
	void nullify();
	std::string toString() const;
private:
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
