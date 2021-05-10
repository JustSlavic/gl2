#include "value.hpp"

#include <assert.h>
#include <string>
#include <vector>


namespace SON {


struct string_storage_t {
    std::string value;
};


struct object_storage_t {
    std::vector<std::string> keys;
    std::vector<Value> values;
};


struct array_storage_t {
    std::vector<Value> values;
};


string_storage_t* dublicate_string_storage(const Value& val) {
    if (!val.is_string() || val.get_storage() == nullptr) return nullptr;

    string_storage_t* holder = (string_storage_t*) val.get_storage();
    string_storage_t* result = new string_storage_t();

    result->value = holder->value;

    return result;
}


bool compare_string_storage(const Value& lhs, const Value& rhs) {
    if (!lhs.is_string() || lhs.get_storage() == nullptr) return false;
    if (!rhs.is_string() || rhs.get_storage() == nullptr) return false;

    string_storage_t* lhs_holder = (string_storage_t*) lhs.get_storage();
    string_storage_t* rhs_holder = (string_storage_t*) rhs.get_storage();

    return lhs_holder->value == rhs_holder->value;
}


object_storage_t* dublicate_object_storage(const Value& val) {
    if (!val.is_object() || val.get_storage() == nullptr) return nullptr;

    object_storage_t* holder = (object_storage_t*) val.get_storage();
    object_storage_t* result = new object_storage_t();

    result->keys = holder->keys;
    result->values = holder->values;

    return result;
}


bool compare_object_storage(const Value& lhs, const Value& rhs) {
    if (!lhs.is_object() || lhs.get_storage() == nullptr) return false;
    if (!rhs.is_object() || rhs.get_storage() == nullptr) return false;

    object_storage_t* lhs_holder = (object_storage_t*) lhs.get_storage();
    object_storage_t* rhs_holder = (object_storage_t*) rhs.get_storage();

    return (lhs_holder->keys == rhs_holder->keys) && (lhs_holder->values == rhs_holder->values);
}


array_storage_t* dublicate_array_storage(const Value& val) {
    if (!val.is_array() || val.get_storage() == nullptr) return nullptr;

    array_storage_t* holder = (array_storage_t*) val.get_storage();
    array_storage_t* result = new array_storage_t();

    result->values = holder->values;

    return result;
}


bool compare_array_storage(const Value& lhs, const Value& rhs) {
    if (!lhs.is_object() || lhs.get_storage() == nullptr) return false;
    if (!rhs.is_object() || rhs.get_storage() == nullptr) return false;

    array_storage_t* lhs_holder = (array_storage_t*) lhs.get_storage();
    array_storage_t* rhs_holder = (array_storage_t*) rhs.get_storage();

    return lhs_holder->values == rhs_holder->values;
}


Value::Value ()
    : type_(type_t::null)
{
    value_.value_storage = nullptr;
}


Value::Value (type_t t)
    : type_(t)
{
    switch (t) {
    case type_t::null:
        value_.value_storage = nullptr;
        break;
    case type_t::boolean:
        value_.boolean = false;
        break;
    case type_t::integer:
        value_.integer = 0;
        break;
    case type_t::floating:
        value_.floating = 0.0;
        break;
    case type_t::string:
        value_.value_storage = new string_storage_t();
        break;
    case type_t::object:
        value_.value_storage = new object_storage_t();
        break;
    case type_t::array:
        value_.value_storage = new array_storage_t();
        break;
    }
}


Value::Value (std::nullptr_t)
    : type_(type_t::null)
{
    value_.value_storage = nullptr;
}


Value::Value (bool val)
    : type_(type_t::boolean)
{
    value_.boolean = val;
}


Value::Value (int32_t val)
    : type_(type_t::integer)
{
    value_.integer = val;
}


Value::Value (uint64_t val)
    : type_(type_t::integer)
{
    value_.integer = val;
}


Value::Value (double val)
    : type_(type_t::floating)
{
    value_.floating = val;
}


Value::Value(const char* c_str)
    : type_(type_t::string)
{
    string_storage_t* holder = new string_storage_t();
    holder->value = std::string(c_str);

    value_.value_storage = holder;
}


Value::Value(std::initializer_list<Value> init)
    : type_(type_t::array)
{
    array_storage_t* holder = new array_storage_t();

    for (auto& val : init) {
        holder->values.push_back(val);
    }

    value_.value_storage = holder;
}


Value::Value (std::initializer_list<std::pair<const char*, Value>> init)
    : type_(type_t::object)
{
    object_storage_t* holder = new object_storage_t();

    for (auto& p : init) {
        holder->keys.push_back(std::string(p.first));
        holder->values.push_back(p.second);
    }

    value_.value_storage = holder;
}


Value::Value (const Value& other)
    : type_(other.type_)
{
    switch (other.type_) {
    case type_t::null:
    case type_t::boolean:
    case type_t::integer:
    case type_t::floating:
        value_ = other.value_;
        break;
    case type_t::string:
        value_.value_storage = dublicate_string_storage(other);
        break;
    case type_t::object:
        value_.value_storage = dublicate_object_storage(other);
        break;
    case type_t::array:
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
    case type_t::null:
    case type_t::boolean:
    case type_t::integer:
    case type_t::floating:
        break;
    case type_t::string:
    case type_t::object:
    case type_t::array:
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


type_t Value::get_type () const {
    return type_;
}


bool Value::is_null () const {
    return type_ == type_t::null;
}


bool Value::is_boolean () const {
    return type_ == type_t::boolean;
}


bool Value::is_integer () const {
    return type_ == type_t::integer;
}


bool Value::is_floating () const {
    return type_ == type_t::floating;
}


bool Value::is_number() const {
    return is_integer() || is_floating();
}


bool Value::is_string () const {
    return type_ == type_t::string;
}


bool Value::is_object () const {
    return type_ == type_t::object;
}


bool Value::is_array () const {
    return type_ == type_t::array;
}


uint64_t Value::get_integer () const {
    if (!is_integer()) return 0;
    return value_.integer;
}


bool Value::get_boolean () const {
    if (!is_boolean()) return false;
    return value_.boolean;
}


double Value::get_floating() const {
    if (!is_floating()) return 0.0;
    return value_.floating;
}


void Value::swap (Value& other) noexcept {
    std::swap(type_, other.type_);
    std::swap(value_, other.value_);
}


bool Value::operator == (const Value& other) const {
    if (get_type() != other.get_type()) return false;

    switch (type_) {
    case type_t::null: return true;
    case type_t::boolean:
        return value_.boolean == other.value_.boolean;
    case type_t::integer:
        return value_.integer == other.value_.integer;
    case type_t::floating:
        return value_.floating == other.value_.floating;
    case type_t::string:
        return compare_string_storage(*this, other);
    case type_t::object:
        return compare_object_storage(*this, other);
    case type_t::array:
        return compare_array_storage(*this, other);
    }
}


bool Value::operator != (const Value& other) const {
    return !(*this == other);
}


Value& Value::operator [] (const char* key) {
    if (is_null()) {
        make_object().swap(*this); // @improve: make function "create<array>();"
    }

    assert(get_type() == type_t::object);

    object_storage_t* holder = (object_storage_t*) value_.value_storage;
    std::string k{ key };

    for (size_t i = 0; i < holder->keys.size(); i++) {
        if (holder->keys[i] == k) return holder->values[i];
    }

    holder->keys.push_back(k);
    holder->values.push_back(Value());

    return holder->values[holder->values.size() - 1];
}


Value& Value::operator [] (int32_t index) {
    assert(is_array());

    array_storage_t* holder = (array_storage_t*) value_.value_storage;
    return holder->values[index];
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


void Value::push (const char* key, const Value& value) {
    if (is_null()) {
        type_ = type_t::object;
        value_.value_storage = new object_storage_t();
    }

    if (type_ != type_t::object || value_.value_storage == nullptr) return;

    object_storage_t* holder = (object_storage_t*) value_.value_storage;
    holder->keys.push_back(std::string(key));
    holder->values.push_back(value);
}


void Value::push (const Value& value) {
    if (!is_array() || value_.value_storage == nullptr) return;

    array_storage_t* holder = (array_storage_t*) value_.value_storage;
    holder->values.push_back(value);
}


void* Value::get_storage () const {
    if (!is_string() && !is_object() && !is_array()) return nullptr;
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
    case type_t::null:
    case type_t::boolean:
    case type_t::integer:
    case type_t::floating:
    case type_t::string:
        return *p_value;
    case type_t::object: {
        auto holder = (object_storage_t*) p_value->get_storage();
        return holder->values[index];
    }
    case type_t::array: {
        auto holder = (array_storage_t*) p_value->get_storage();
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
    case type_t::null:
    case type_t::boolean:
    case type_t::integer:
    case type_t::floating:
    case type_t::string:
        return *p_value;
    case type_t::object: {
        auto holder = (object_storage_t*) p_value->get_storage();
        return holder->values[index];
    }
    case type_t::array: {
        auto holder = (array_storage_t*) p_value->get_storage();
        return holder->values[index];
    }
    }
}


Value::iterator Value::begin () {
    switch (get_type()) {
    case type_t::null: return iterator(this, 0, 0);
    case type_t::boolean:
    case type_t::integer:
    case type_t::floating:
    case type_t::string:
        return iterator(this, 0, 1);
    case type_t::object: {
        if (get_storage() == nullptr) return iterator(nullptr, 0, 0);
        auto holder = (object_storage_t*) value_.value_storage;
        return iterator(this, 0, holder->values.size());
    }
    case type_t::array: {
        if (get_storage() == nullptr) return iterator(nullptr, 0, 0);
        auto holder = (array_storage_t*) value_.value_storage;
        return iterator(this, 0, holder->values.size());
    }
    }
}


Value::iterator Value::end () {
    switch (get_type()) {
    case type_t::null: return iterator(this, 0, 0);
    case type_t::boolean:
    case type_t::integer:
    case type_t::floating:
    case type_t::string:
        return iterator(this, 1, 1);
    case type_t::object: {
        if (get_storage() == nullptr) return iterator(nullptr, 0, 0);
        auto holder = (object_storage_t*) value_.value_storage;
        return iterator(this, holder->values.size(), holder->values.size());
    }
    case type_t::array: {
        if (get_storage() == nullptr) return iterator(nullptr, 0, 0);
        auto holder = (array_storage_t*) value_.value_storage;
        return iterator(this, holder->values.size(), holder->values.size());
    }
    }
}


Value::const_iterator Value::begin () const {
    switch (get_type()) {
    case type_t::null: return const_iterator(this, 0, 0);
    case type_t::boolean:
    case type_t::integer:
    case type_t::floating:
    case type_t::string:
        return const_iterator(this, 0, 1);
    case type_t::object: {
        if (get_storage() == nullptr) return const_iterator(nullptr, 0, 0);
        auto holder = (object_storage_t*) value_.value_storage;
        return const_iterator(this, 0, holder->values.size());
    }
    case type_t::array: {
        if (get_storage() == nullptr) return const_iterator(nullptr, 0, 0);
        auto holder = (array_storage_t*) value_.value_storage;
        return const_iterator(this, 0, holder->values.size());
    }
    }
}


Value::const_iterator Value::end () const {
    switch (get_type()) {
    case type_t::null: return const_iterator(this, 0, 0);
    case type_t::boolean:
    case type_t::integer:
    case type_t::floating:
    case type_t::string:
        return const_iterator(this, 1, 1);
    case type_t::object: {
        if (get_storage() == nullptr) return const_iterator(nullptr, 0, 0);
        auto holder = (object_storage_t*) value_.value_storage;
        return const_iterator(this, holder->values.size(), holder->values.size());
    }
    case type_t::array: {
        if (get_storage() == nullptr) return const_iterator(nullptr, 0, 0);
        auto holder = (array_storage_t*) value_.value_storage;
        return const_iterator(this, holder->values.size(), holder->values.size());
    }
    }
}


Value make_object () {
    return Value(type_t::object);
}


Value make_array () {
    return Value(type_t::array);
}


} // SON
