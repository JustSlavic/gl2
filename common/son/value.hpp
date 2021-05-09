#ifndef SON_VALUE_HPP
#define SON_VALUE_HPP

#include <stdint.h>

namespace SON {


enum value_type_t : uint8_t {
    VALUE_NULL,
    VALUE_BOOLEAN,
    VALUE_INTEGER,
    VALUE_FLOATING,
    VALUE_STRING,
    VALUE_OBJECT,
    VALUE_ARRAY,
    // @todo: consider making user-defined value types,
    //        along with providing means to accept functions to parse them.
    // VALUE_CUSTOM,
};


class Value {
private:
    union value_holder_t {
        bool     boolean;
        uint64_t integer;
        double   floating;
        void*    value_storage; // Pointer to the string, object or array.
    } value_;

    value_type_t type_;

public:
    Value (); // Makes NULL value.
    explicit Value (value_type_t); // Default value of that type.
    explicit Value (bool);
    explicit Value (int32_t);
    explicit Value (uint64_t);
    explicit Value (double);

    Value  (const Value&);
    Value  (Value&&) noexcept;
    ~Value ();

    Value& operator = (const Value&);
    Value& operator = (Value&&) noexcept;

    value_type_t get_type () const;

    bool is_null () const;
    bool is_boolean () const;
    bool is_integer () const;
    bool is_floating () const;
    bool is_string () const;
    bool is_object () const;
    bool is_array () const;

    uint64_t get_integer () const;
    bool get_boolean () const;
    double get_floating () const;

    void swap (Value&) noexcept;

    bool operator == (const Value&) const;
    bool operator != (const Value&) const;

    Value operator [] (const char* key);
    Value operator [] (int32_t index);

    Value get (const char* key, const Value& default_);
    Value get (int32_t index, const Value& default_);

    template <typename T>
    void push (const char* key, T value) = delete;

    template <typename T>
    void push (T) = delete;

    class iterator {
        friend class Value;

        Value* p_value = nullptr;
        uint64_t index = 0;
        uint64_t size = 0;

        iterator (Value* p_value_, uint64_t index_, uint64_t size_);

    public:
        void operator ++ ();
        void operator -- ();
        bool operator == (const iterator& other) const;
        bool operator != (const iterator& other) const;

        Value& operator * () const;
    };

    class const_iterator {
       friend class Value;

       const Value* p_value = nullptr;
       uint64_t index = 0;
       uint64_t size = 0;

       const_iterator (const Value* p_value_, uint64_t index_, uint64_t size_);

    public:
       void operator ++ ();
       void operator -- ();
       bool operator == (const const_iterator& other) const;
       bool operator != (const const_iterator& other) const;

       const Value& operator * () const;
    };

    iterator begin ();
    iterator end ();

    const_iterator begin () const;
    const_iterator end () const;

    void* get_storage () const;
};


template <> void Value::push (const Value& value);
template <> void Value::push (Value value);
template <> void Value::push (int32_t value);
template <> void Value::push (uint64_t value);

template <> void Value::push (const char* key, const Value& value);
template <> void Value::push (const char* key, Value value);
template <> void Value::push (const char* key, bool value);
template <> void Value::push (const char* key, int32_t value);
template <> void Value::push (const char* key, uint64_t value);
template <> void Value::push (const char* key, double value);


Value make_object ();
Value make_array ();


} // SON


#endif // SON_VALUE_HPP
