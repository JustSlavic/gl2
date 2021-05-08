#include "value.hpp"

namespace SON {


Value::Value ()
    : type_(VALUE_NULL)
{
    value_.value_storage = nullptr;
}


Value::Value (Type t)
    : type_(t)
{
    switch (t) {
    case VALUE_NULL:
    case VALUE_STRING:
    case VALUE_OBJECT:
    case VALUE_LIST:
        value_.value_storage = nullptr;
        break;
    case VALUE_BOOLEAN:
        value_.boolean = false;
        break;
    case VALUE_INTEGER:
        value_.integer = 0;
        break;
    case VALUE_FLOATING:
        value_.floating = 0.0;
        break;
    }
}


Value::Value (bool val)
    : type_(VALUE_BOOLEAN)
{
    value_.boolean = val;
}


Value::Value (uint64_t val)
    : type_(VALUE_INTEGER)
{
    value_.integer = val;
}


Value::Value (double val)
    : type_(VALUE_FLOATING)
{
    value_.floating = val;
}


Value::Value (const Value& other)
    type_(other.type_)
{
    switch (other.type_) {
    case VALUE_NULL:
    case VALUE_BOOLEAN:
    case VALUE_INTEGER:
    case VALUE_FLOATING:
        value_ = other.value_;
        break;
    case VALUE_STRING:
        // @todo: dublicate string
    case VALUE_OBJECT:
        // @todo: dublicate object
    case VALUE_LIST:
        // @todo: dublicate list
        break;
    }
}


Value::Value (Value&& other)
    : Value()
{
    swap(other);
}


Value::~Value () {
    // @todo: release payload if allocated
}


Value& Value::operator = (const Value& other) {
    Value(other).swap(*this);
    return *this;
}


Value& Value::operator = (Value&& other) {
    other.swap(*this);
    return *this;
}


Type Value::get_type () const {
    return type_;
}


bool Value::is_null () const {
    return type_ == VALUE_NULL;
}


bool Value::is_bool () const {
    return type_ == VALUE_BOOLEAN;
}


bool Value::is_integer () const {
    return type_ == VALUE_INTEGER;
}


bool Value::is_floating () const {
    return type_ == VALUE_FLOATING;
}


bool Value::is_string () const {
    return type_ == VALUE_STRING;
}


bool Value::is_object () const {
    return type_ == VALUE_OBJECT;
}


bool Value::is_array () const {
    return type_ == VALUE_ARRAY;
}


void Value::swap (Value& other) {
    std::swap(type_, other.type_);
    std::swap(value_, other.value_);
}


bool Value::operator == (const Value& other) {
    if (type_ != other.type_) return false;

    switch (type_) {
    case VALUE_NULL: return true;
    case VALUE_BOOLEAN:
    case VALUE_INTEGER:
    case VALUE_FLOATING:
        return value_ == other.value_;
    case VALUE_STRING:
        // @todo: compare strings
    case VALUE_OBJECT:
        // @todo: compare objects
    case VALUE_LIST:
        // @todo: compare lists
        return false;
    }
}


bool Value::operator != (const Value& other) {
    return !(*this == other)
}


} // SON
