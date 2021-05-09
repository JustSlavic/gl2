#include "value.hpp"

#include <assert.h>
#include <string>
#include <vector>


namespace SON {


struct string_holder_t {
    std::string value;
};


struct object_holder_t {
    std::vector<std::string> keys;
    std::vector<Value> values;
};


struct array_holder_t {
    std::vector<Value> values;
};


string_holder_t* dublicate_string_storage(const Value& val) {
    if (val.get_type() != VALUE_STRING || val.get_storage() == nullptr) return nullptr;

    string_holder_t* holder = (string_holder_t*) val.get_storage();
    string_holder_t* result = new string_holder_t();

    result->value = holder->value;

    return result;
}


bool compare_string_storage(const Value& lhs, const Value& rhs) {
    if (lhs.get_type() != VALUE_STRING || lhs.get_storage() == nullptr) return false;
    if (rhs.get_type() != VALUE_STRING || rhs.get_storage() == nullptr) return false;

    string_holder_t* lhs_holder = (string_holder_t*) lhs.get_storage();
    string_holder_t* rhs_holder = (string_holder_t*) rhs.get_storage();

    return lhs_holder->value == rhs_holder->value;
}


object_holder_t* dublicate_object_storage(const Value& val) {
    if (val.get_type() != VALUE_OBJECT || val.get_storage() == nullptr) return nullptr;

    object_holder_t* holder = (object_holder_t*) val.get_storage();
    object_holder_t* result = new object_holder_t();

    result->keys = holder->keys;
    result->values = holder->values;

    return result;
}


bool compare_object_storage(const Value& lhs, const Value& rhs) {
    if (lhs.get_type() != VALUE_OBJECT || lhs.get_storage() == nullptr) return false;
    if (rhs.get_type() != VALUE_OBJECT || rhs.get_storage() == nullptr) return false;

    object_holder_t* lhs_holder = (object_holder_t*) lhs.get_storage();
    object_holder_t* rhs_holder = (object_holder_t*) rhs.get_storage();

    return (lhs_holder->keys == rhs_holder->keys) && (lhs_holder->values == rhs_holder->values);
}


array_holder_t* dublicate_array_storage(const Value& val) {
    if (val.get_type() != VALUE_ARRAY || val.get_storage() == nullptr) return nullptr;

    array_holder_t* holder = (array_holder_t*) val.get_storage();
    array_holder_t* result = new array_holder_t();

    result->values = holder->values;

    return result;
}


bool compare_array_storage(const Value& lhs, const Value& rhs) {
    if (lhs.get_type() != VALUE_OBJECT || lhs.get_storage() == nullptr) return false;
    if (rhs.get_type() != VALUE_OBJECT || rhs.get_storage() == nullptr) return false;

    array_holder_t* lhs_holder = (array_holder_t*) lhs.get_storage();
    array_holder_t* rhs_holder = (array_holder_t*) rhs.get_storage();

    return lhs_holder->values == rhs_holder->values;
}


Value::Value ()
    : type_(VALUE_NULL)
{
    value_.value_storage = nullptr;
}


Value::Value (value_type_t t)
    : type_(t)
{
    switch (t) {
    case VALUE_NULL:
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
    case VALUE_STRING:
        value_.value_storage = new string_holder_t();
        break;
    case VALUE_OBJECT:
        value_.value_storage = new object_holder_t();
        break;
    case VALUE_ARRAY:
        value_.value_storage = new array_holder_t();
        break;
    }
}


Value::Value (bool val)
    : type_(VALUE_BOOLEAN)
{
    value_.boolean = val;
}


Value::Value (int32_t val)
    : type_(VALUE_INTEGER)
{
    value_.integer = val;
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
    : type_(other.type_)
{
    switch (other.type_) {
    case VALUE_NULL:
    case VALUE_BOOLEAN:
    case VALUE_INTEGER:
    case VALUE_FLOATING:
        value_ = other.value_;
        break;
    case VALUE_STRING:
        value_.value_storage = dublicate_string_storage(other);
        break;
    case VALUE_OBJECT:
        value_.value_storage = dublicate_object_storage(other);
        break;
    case VALUE_ARRAY:
        value_.value_storage = dublicate_array_storage(other);
        break;
    }
}


Value::Value (Value&& other) noexcept
    : Value()
{
    swap(other);
}


Value::~Value () {
    switch (type_) {
    case VALUE_NULL:
    case VALUE_BOOLEAN:
    case VALUE_INTEGER:
    case VALUE_FLOATING:
        break;
    case VALUE_STRING:
    case VALUE_OBJECT:
    case VALUE_ARRAY:
        delete value_.value_storage;
    }
}


Value& Value::operator = (const Value& other) {
    Value(other).swap(*this);
    return *this;
}


Value& Value::operator = (Value&& other) noexcept {
    other.swap(*this);
    return *this;
}


value_type_t Value::get_type () const {
    return type_;
}


bool Value::is_null () const {
    return type_ == VALUE_NULL;
}


bool Value::is_boolean () const {
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


uint64_t Value::get_integer () const {
    if (get_type() != VALUE_INTEGER) return 0;
    return value_.integer;
}


bool Value::get_boolean () const {
    if (get_type() != VALUE_BOOLEAN) return false;
    return value_.boolean;
}


double Value::get_floating() const {
    if (get_type() != VALUE_FLOATING) return 0.0;
    return value_.floating;
}


void Value::swap (Value& other) noexcept {
    std::swap(type_, other.type_);
    std::swap(value_, other.value_);
}


bool Value::operator == (const Value& other) const {
    if (type_ != other.type_) return false;

    switch (type_) {
    case VALUE_NULL: return true;
    case VALUE_BOOLEAN:
        return value_.boolean == other.value_.boolean;
    case VALUE_INTEGER:
        return value_.integer == other.value_.integer;
    case VALUE_FLOATING:
        return value_.floating == other.value_.floating;
    case VALUE_STRING:
        return compare_string_storage(*this, other);
    case VALUE_OBJECT:
        return compare_object_storage(*this, other);
    case VALUE_ARRAY:
        return compare_array_storage(*this, other);
    }
}


bool Value::operator != (const Value& other) const {
    return !(*this == other);
}


Value Value::operator [] (const char* key) {
    if (type_ != VALUE_OBJECT || value_.value_storage == nullptr) return Value();

    object_holder_t* holder = (object_holder_t*) value_.value_storage;
    std::string k{ key };

    for (size_t i = 0; i < holder->keys.size(); i++) {
        if (holder->keys[i] == k) return holder->values[i];
    }

    return Value();
}


Value Value::operator [] (int32_t index) {
    if (type_ != VALUE_ARRAY || value_.value_storage == nullptr) return Value();

    array_holder_t* holder = (array_holder_t*) value_.value_storage;

    if (index < holder->values.size()) {
        return holder->values[index];
    }

    return Value();
}


Value Value::get (const char* key, const Value& default_) {
    Value result = (*this)[key];
    if (result.is_null()) return default_;
    return result;
}


Value Value::get (int32_t index, const Value& default_) {
    Value result = (*this)[index];
    if (result.is_null()) return default_;
    return result;
}


template <>
void Value::push (const char* key, const Value& value) {
    if (type_ != VALUE_OBJECT || value_.value_storage == nullptr) return;

    object_holder_t* holder = (object_holder_t*) value_.value_storage;
    holder->keys.push_back(std::string(key));
    holder->values.push_back(value);
}


template <>
void Value::push (const char* key, Value value) {
    if (type_ != VALUE_OBJECT || value_.value_storage == nullptr) return;

    object_holder_t* holder = (object_holder_t*) value_.value_storage;
    holder->keys.push_back(std::string(key));
    holder->values.push_back(std::move(value));
}


template <>
void Value::push(const char* key, bool value) {
    push(key, Value(value));
}


template <>
void Value::push (const char* key, int32_t value) {
    push(key, Value(value));
}


template <>
void Value::push (const char* key, uint64_t value) {
    push(key, Value(value));
}


template <>
void Value::push (const char* key, double value) {
    push(key, Value(value));
}


template <>
void Value::push (const Value& value) {
    if (type_ != VALUE_ARRAY || value_.value_storage == nullptr) return;

    array_holder_t* holder = (array_holder_t*) value_.value_storage;
    holder->values.push_back(value);
}


template <>
void Value::push (Value value) {
    if (type_ != VALUE_ARRAY || value_.value_storage == nullptr) return;

    array_holder_t* holder = (array_holder_t*)value_.value_storage;
    holder->values.push_back(std::move(value));
}


template <>
void Value::push (uint64_t val) {
    push(Value(val));
}


template <> void Value::push (int32_t val) {
    push(Value(val));
}


void* Value::get_storage () const {
    if (type_ != VALUE_STRING && type_ != VALUE_OBJECT && type_ != VALUE_ARRAY) return nullptr;
    return value_.value_storage;
}


Value::iterator::iterator (Value* p_value_, uint64_t index_, uint64_t size_)
    : p_value(p_value_)
    , index(index_)
    , size(size_)
{
}


void Value::iterator::operator ++ () {
    if (index == size) return;
    ++index;
}


void Value::iterator::operator -- () {
    if (index == 0) return;
    --index;
}


bool Value::iterator::operator == (const iterator& other) const {
    return p_value == other.p_value && index == other.index && size == other.size;
}


bool Value::iterator::operator != (const iterator& other) const {
    return !(*this == other);
}


Value& Value::iterator::operator * () const {
    assert(p_value);
    assert(index < size);

    switch (p_value->get_type()) {
    case VALUE_NULL:
    case VALUE_BOOLEAN:
    case VALUE_INTEGER:
    case VALUE_FLOATING:
    case VALUE_STRING:
        return *p_value;
    case VALUE_OBJECT: {
        auto holder = (object_holder_t*) p_value->get_storage();
        return holder->values[index];
    }
    case VALUE_ARRAY: {
        auto holder = (array_holder_t*) p_value->get_storage();
        return holder->values[index];
    }
    }
}


Value::const_iterator::const_iterator (const Value* p_value_, uint64_t index_, uint64_t size_)
    : p_value(p_value_)
    , index(index_)
    , size(size_)
{
}


void Value::const_iterator::operator ++ () {
    if (index == size) return;
    ++index;
}


void Value::const_iterator::operator -- () {
    if (index == 0) return;
    --index;
}


bool Value::const_iterator::operator == (const const_iterator& other) const {
    return p_value == other.p_value && index == other.index && size == other.size;
}


bool Value::const_iterator::operator != (const const_iterator& other) const {
    return !(*this == other);
}


const Value& Value::const_iterator::operator * () const {
    assert(p_value);
    assert(index < size);

    switch (p_value->get_type()) {
    case VALUE_NULL:
    case VALUE_BOOLEAN:
    case VALUE_INTEGER:
    case VALUE_FLOATING:
    case VALUE_STRING:
        return *p_value;
    case VALUE_OBJECT: {
        auto holder = (object_holder_t*) p_value->get_storage();
        return holder->values[index];
    }
    case VALUE_ARRAY: {
        auto holder = (array_holder_t*) p_value->get_storage();
        return holder->values[index];
    }
    }
}


Value::iterator Value::begin () {
    switch (get_type()) {
    case VALUE_NULL: return iterator(this, 0, 0);
    case VALUE_BOOLEAN:
    case VALUE_INTEGER:
    case VALUE_FLOATING:
    case VALUE_STRING:
        return iterator(this, 0, 1);
    case VALUE_OBJECT: {
        if (get_storage() == nullptr) return iterator(nullptr, 0, 0);
        auto holder = (object_holder_t*) value_.value_storage;
        return iterator(this, 0, holder->values.size());
    }
    case VALUE_ARRAY: {
        if (get_storage() == nullptr) return iterator(nullptr, 0, 0);
        auto holder = (array_holder_t*) value_.value_storage;
        return iterator(this, 0, holder->values.size());
    }
    }
}


Value::iterator Value::end () {
    switch (get_type()) {
    case VALUE_NULL: return iterator(this, 0, 0);
    case VALUE_BOOLEAN:
    case VALUE_INTEGER:
    case VALUE_FLOATING:
    case VALUE_STRING:
        return iterator(this, 1, 1);
    case VALUE_OBJECT: {
        if (get_storage() == nullptr) return iterator(nullptr, 0, 0);
        auto holder = (object_holder_t*) value_.value_storage;
        return iterator(this, holder->values.size(), holder->values.size());
    }
    case VALUE_ARRAY: {
        if (get_storage() == nullptr) return iterator(nullptr, 0, 0);
        auto holder = (array_holder_t*) value_.value_storage;
        return iterator(this, holder->values.size(), holder->values.size());
    }
    }
}


Value::const_iterator Value::begin () const {
    switch (get_type()) {
    case VALUE_NULL: return const_iterator(this, 0, 0);
    case VALUE_BOOLEAN:
    case VALUE_INTEGER:
    case VALUE_FLOATING:
    case VALUE_STRING:
        return const_iterator(this, 0, 1);
    case VALUE_OBJECT: {
        if (get_storage() == nullptr) return const_iterator(nullptr, 0, 0);
        auto holder = (object_holder_t*) value_.value_storage;
        return const_iterator(this, 0, holder->values.size());
    }
    case VALUE_ARRAY: {
        if (get_storage() == nullptr) return const_iterator(nullptr, 0, 0);
        auto holder = (array_holder_t*) value_.value_storage;
        return const_iterator(this, 0, holder->values.size());
    }
    }
}


Value::const_iterator Value::end () const {
    switch (get_type()) {
    case VALUE_NULL: return const_iterator(this, 0, 0);
    case VALUE_BOOLEAN:
    case VALUE_INTEGER:
    case VALUE_FLOATING:
    case VALUE_STRING:
        return const_iterator(this, 1, 1);
    case VALUE_OBJECT: {
        if (get_storage() == nullptr) return const_iterator(nullptr, 0, 0);
        auto holder = (object_holder_t*) value_.value_storage;
        return const_iterator(this, holder->values.size(), holder->values.size());
    }
    case VALUE_ARRAY: {
        if (get_storage() == nullptr) return const_iterator(nullptr, 0, 0);
        auto holder = (array_holder_t*) value_.value_storage;
        return const_iterator(this, holder->values.size(), holder->values.size());
    }
    }
}


Value make_object () {
    return Value(VALUE_OBJECT);
}


Value make_array () {
    return Value(VALUE_ARRAY);
}


} // SON
