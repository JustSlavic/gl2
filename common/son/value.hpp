#ifndef SON_VALUE_HPP
#define SON_VALUE_HPP

#include <stdint.h>


namespace SON {


enum Type : uint8_t {
    VALUE_NULL,
    VALUE_BOOLEAN,
    VALUE_INTEGER,
    VALUE_FLOATING,
    VALUE_STRING,
    VALUE_OBJECT,
    VALUE_ARRAY,
    // VALUE_CUSTOM,
};



class Value {
private:
    union ValueHolder {
        bool     boolean;
        uint64_t integer;
        double   floating;
        void*    value_storage;
    } value_;

    Type type_;

public:
    Value (); // Makes NULL value
    explicit Value (Type); // Default value of that type
    explicit Value (bool);
    explicit Value (uint64_t);
    explicit Value (double);

    Value  (const Value&);
    Value  (Value&&);
    ~Value ();

    Value& operator = (const Value&);
    Value& operator = (Value&&);

    Type get_type () const;
    bool is_null () const;
    bool is_bool () const;
    bool is_integer () const;
    bool is_floating () const;
    bool is_string () const;
    bool is_object () const;
    bool is_array () const;

    void swap (Value&);

    bool operator == (const Value&);
    bool operator != (const Value&);

    Value& operator [] (const char* key); // Returns value, associated to the key in object. Works only with object type.
    Value& operator [] (uint64_t index); // Returns value in array. Works only with array type.

    Value& get (const char* key, const Value& default);
    Value& get (uint64_t index, const Value& default);

    // @todo iterators

    // iterator begin ();
    // iterator end ();

    // const_iterator cbegin () const;
    // const_iterator cend () const;
};


} // SON


#endif // SON_VALUE_HPP
